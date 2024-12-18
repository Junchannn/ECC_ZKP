#include "DLP_ZKP.h"
#include "../common/common.h"

// Prover_dlp class constructor
Prover_dlp::Prover_dlp(ZZ w, ZZ p, ZZ g) : w(w), p(p), g(g) {
    this->order = p - 1;
    this->y = PowerMod(g, w, p);
}

// Prover_dlp methods
ZZ Prover_dlp::gen_param() {
    this->r = generate_random_number(g, this->order);
    return PowerMod(this->g, this->r, this->p);
}

ZZ Prover_dlp::gen_prove(ZZ e) {
    ZZ z = this->r + e * this->w;
    rem(z, z, this->order);
    return z;
}

// Verifier_dlp class constructor

void Verifier_dlp::gen_public_key() {
    GenPrime(this->p, 3072);
    this->order = this->p - 1;
}

void Verifier_dlp::gen_challenge() {
    this->e = generate_random_number(this->g, this->order);
}

bool Verifier_dlp::verify(ZZ z) {
    ZZ b1 = PowerMod(this->g, z, this->p);
    ZZ b2 = (this->a * PowerMod(this->y, this->e, this->p)) % this->p;
    return b1 == b2;
}
