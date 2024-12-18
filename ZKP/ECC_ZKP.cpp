#include "ECC_ZKP.h"
#include "../ECC/ecc.h"
#include "../common/common.h"


ECPoint Prover_ecc::gen_param(){
    this->r = generate_random_number(conv<ZZ>(2), this->order);
    return generator.multiply(this->r);
}

ZZ Prover_ecc::gen_prove(ZZ e){
    ZZ z = this->r + e * this->w;
    rem(z, z, this->order);
    return z;
}

ZZ Verifier_ecc::gen_challenge(){
    this->e = generate_random_number(conv<ZZ>(2), this->order);
    return this->e; 
}

bool Verifier_ecc::verify(ZZ z){
    ECPoint b1 = this->generator.multiply(z);
    ECPoint b2 = this->A.add(this->Y.multiply(this->e));
    return b1 == b2;
}