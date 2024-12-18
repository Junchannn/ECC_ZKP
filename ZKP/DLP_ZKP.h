#pragma once

#include <NTL/ZZ.h>     

NTL_CLIENT

class Prover_dlp {
    private:
        ZZ r, w, p, order, g;
    public:
        ZZ y, n;

        // Constructor
        Prover_dlp(ZZ w, ZZ p, ZZ g);
        // Generates a parameter for the protocol
        ZZ gen_param();
        // Sets the state based on the challenge e
        ZZ gen_prove(ZZ e);
};

class Verifier_dlp {
    public:
        ZZ p, order, e, a, y;
        ZZ g = ZZ(2);
        // Generates a public key and sets p and order
        void gen_public_key();
        // Generates a challenge e
        void gen_challenge();
        // Verifies the response z from the prover
        bool verify(ZZ z);
};
