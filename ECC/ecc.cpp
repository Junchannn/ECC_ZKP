#include <iostream>
#include <NTL/ZZ.h>
#include "ecc.h"
#include "../common/common.h"

using json = nlohmann::json;

EllipticCurve::EllipticCurve(ZZ p, ZZ a, ZZ b, ZZ order) : p(p), a(a), b(b), order(order) {}


const ZZ& EllipticCurve::getModulus() const{
    return p;
}

const ZZ& EllipticCurve::getA() const{
    return a;
}

const ZZ& EllipticCurve::getB() const{
    return b;
}

const ZZ& EllipticCurve::getOrder() const{
    return order;
}
ostream& operator<<(ostream& out, const EllipticCurve& EC) {
    out << "Elliptic Curve y^2 = x^3 + " << EC.a << "x + " << EC.b << " over " << EC.p << " finite field";
    return out;
}

bool operator==(const EllipticCurve& e1, const EllipticCurve& e2) {
    return ((e1.getA() == e2.getA()) && (e1.getB() == e2.getB()) && (e1.getModulus() == e2.getModulus()));
}

void check_same_curve(const EllipticCurve& e1, const EllipticCurve& e2) {
    if (!(e1 == e2)) {
        perror("Not on the same curve");
        exit(1);
    }
}

ECPoint::ECPoint(const EllipticCurve& EC) : EC(EC), isInfinity(true) {}

ECPoint::ECPoint(const EllipticCurve& EC, ZZ x, ZZ y) : EC(EC), x(x % EC.getModulus()), y(y % EC.getModulus()), isInfinity(false) {}

ECPoint& ECPoint::operator=(const ECPoint& other) {
    if (this == &other) return *this;
    x = other.x;
    y = other.y;
    isInfinity = other.isInfinity;
    return *this;
}

bool ECPoint::isAtInfinity() const {
    return isInfinity;
}

const ZZ& ECPoint::getX() const {
    return x;
}

const ZZ& ECPoint::getY() const {
    return y;
}


ECPoint ECPoint::add(const ECPoint& other) const {
    check_same_curve(EC, other.EC);

    if (this->isInfinity) return other;
    if (other.isInfinity) return *this;

    ZZ p = EC.getModulus();

    if (x == other.x && y != other.y) {
        return ECPoint(EC); // Point at infinity
    }

    ZZ lambda;
    if (x == other.x && y == other.y) {
        ZZ num = MulMod(3 * SqrMod(x, p) + EC.getA(), 1, p);
        ZZ denom = MulMod(2 * y, 1, p); // Ensure denom is reduced modulo p
        InvMod(denom, denom, p);       // Compute modular inverse of denom
        MulMod(lambda, num, denom, p);
    } else {
        ZZ num = SubMod(other.y, y, p);
        ZZ denom = SubMod(other.x, x, p);
        InvMod(denom, denom, p);       // Compute modular inverse of denom
        MulMod(lambda, num, denom, p);
    }

    ZZ xr, yr;
    SqrMod(xr, lambda, p);
    SubMod(xr, xr, x, p);
    SubMod(xr, xr, other.x, p);

    SubMod(yr, x, xr, p);
    MulMod(yr, lambda, yr, p);
    SubMod(yr, yr, y, p);

    return ECPoint(EC, xr, yr);
}

ECPoint ECPoint::multiply(const ZZ& scalar) const {
    if (scalar == 0 || this->isInfinity) {
        return ECPoint(EC); // Return point at infinity
    }

    ECPoint result(EC);
    ECPoint current = *this;

    ZZ k = abs(scalar);
    while (k > 0) {
        if (bit(k, 0)) {
            result = result.add(current);
        }
        current = current.add(current);
        k >>= 1;
    }

    // Handle negative scalars by flipping the y-coordinate
    if (scalar < 0) {
        result.y = SubMod(0, result.y, EC.getModulus()); // Negate y modulo p
    }

    return result;
}

ostream& operator<<(ostream& out, const ECPoint& point) {
    if (point.isAtInfinity()) {
        out << "Point at infinity on ";
    } else {
        out << "Point (" << point.getX() << ", " << point.getY() << ") on ";
    }
    out << point.EC;
    return out;
}

json serialize_ECPoint(const ECPoint& point){
    json p;
    p["x"] = serialize_ZZ(point.getX());
    p["y"] = serialize_ZZ(point.getY());
    return p; // just raw json
}

ECPoint unserialize_ECPoint(const EllipticCurve& curve,const string& data){
    json point = json::parse(data);
    ZZ x = unserialize_ZZ(point["x"]);
    ZZ y = unserialize_ZZ(point["y"]);
    return ECPoint(curve, x, y);
}

json serialize_EC(const EllipticCurve& curve, const ECPoint& generator,bool ispub){
    json ec;
    ec["p"] = serialize_ZZ(curve.getModulus());
    ec["a"] = serialize_ZZ(curve.getA());
    ec["b"] = serialize_ZZ(curve.getB());
    ec["order"] = serialize_ZZ(curve.getOrder());
    if (ispub){ 
        ec["x"] = serialize_ZZ(generator.getX());
        ec["y"] = serialize_ZZ(generator.getY());
    }
    return ec;
}
vector<ZZ> unserialize_EC(const string& data, bool ispub) {
    // Parse the JSON data
    json curve = json::parse(data);

    // Deserialize common curve parameters
    vector<ZZ> temp{
        unserialize_ZZ(curve["p"]), 
        unserialize_ZZ(curve["a"]), 
        unserialize_ZZ(curve["b"]),
        unserialize_ZZ(curve["order"])
    };

    // Deserialize public key components if required
    if (ispub) {
        temp.push_back(unserialize_ZZ(curve["x"]));
        temp.push_back(unserialize_ZZ(curve["y"]));
    }

    return temp;
}

bool operator==(const ECPoint &p1, const ECPoint &p2){
    return (p1.getX() == p2.getX()) && (p1.getY() == p2.getY());
}