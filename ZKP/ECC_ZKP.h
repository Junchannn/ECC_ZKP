#pragma once

#include <NTL/ZZ.h>
#include "../ECC/ecc.h"

class Prover_ecc {
private:
    ZZ r, w;

public:
    EllipticCurve pub_curve;
    ECPoint generator;
    ECPoint Y;
    ZZ order;

    // Constructor
    Prover_ecc(ZZ w, EllipticCurve pub_curve, ZZ order, ECPoint generator)
        : w(w), pub_curve(pub_curve), generator(generator), order(order), Y(pub_curve) {    
            this->Y = generator.multiply(w);
    }

    ECPoint gen_param();
    ZZ gen_prove(ZZ e);
};

class Verifier_ecc {
public:
    // secp256k1 parameters
    const ZZ p = conv<ZZ>("115792089237316195423570985008687907853269984665640564039457584007908834671663");
    const ZZ a = conv<ZZ>("0");
    const ZZ b = conv<ZZ>("7");
    const ZZ gx = conv<ZZ>("55066263022277343669578718895168534326250603453777594175500187360389116729240");
    const ZZ gy = conv<ZZ>("32670510020758816978083085130507043184471273380659243275938904335757337482424");
    const ZZ order = conv<ZZ>("115792089237316195423570985008687907852837564279074904382605163141518161494337");

    EllipticCurve curve;
    ECPoint generator, A, Y;
    ZZ e;

    // Constructor
    Verifier_ecc()
        : curve(p, a, b, order),
          generator(curve, gx, gy),
          A(curve),
          Y(curve) {}

    ZZ gen_challenge();
    bool verify(ZZ z);
};
