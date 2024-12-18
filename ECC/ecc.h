#pragma once

#include <iostream>
#include <NTL/ZZ.h>
#include <nlohmann/json.hpp>
NTL_CLIENT
using json = nlohmann::json;
//secp256k1 parameters


class EllipticCurve {
private:
    ZZ p, a, b, order;

public:
    EllipticCurve(ZZ p, ZZ a, ZZ b, ZZ order);

    const ZZ& getModulus() const;
    const ZZ& getA() const;
    const ZZ& getB() const;
    const ZZ& getOrder() const;
    friend std::ostream& operator<<(std::ostream& out, const EllipticCurve& EC);
};

bool operator==(const EllipticCurve& e1, const EllipticCurve& e2);

void check_same_curve(const EllipticCurve& e1, const EllipticCurve& e2);

class ECPoint {
private:
    ZZ x, y;
    bool isInfinity;

public:
    const EllipticCurve& EC;

    ECPoint(const EllipticCurve& EC);
    ECPoint(const EllipticCurve& EC, ZZ x, ZZ y);
    bool isAtInfinity() const;
    const ZZ& getX() const;
    const ZZ& getY() const;

    ECPoint add(const ECPoint& other) const;
    ECPoint multiply(const ZZ& scalar) const;

    ECPoint& operator=(const ECPoint& other);
    friend std::ostream& operator<<(std::ostream& out, const ECPoint& point);
};

json serialize_ECPoint(const ECPoint& point);
ECPoint unserialize_ECPoint(const EllipticCurve& curve, const string& data);

json serialize_EC(const EllipticCurve& curve, const ECPoint& point, bool ispub);
vector<ZZ> unserialize_EC(const string& data, bool ispub);

bool operator==(const ECPoint& p1, const ECPoint& p2);