# ECC_ZKP

A C++ implementation of an elliptic-curve-based Zero-Knowledge Proof (ZKP) protocol using the secp256k1 curve.

The project demonstrates a client-server authentication flow where a prover proves knowledge of a secret scalar without revealing it.

## Overview

This project implements a basic Schnorr-style Zero-Knowledge Proof over elliptic curves.

The prover holds a private witness `w` and publishes:

```text
Y = wG
```

where `G` is the elliptic-curve generator.

The protocol follows three main steps:

### 1. Commitment

```text
A = rG
```

The prover samples a random nonce `r` and sends the commitment `A` to the verifier.

### 2. Challenge

```text
e <- random
```

The verifier sends a random challenge `e`.

### 3. Response

```text
z = r + e * w mod q
```

The prover responds with `z`.

The verifier accepts if:

```text
zG = A + eY
```

This proves that the prover knows `w` without revealing it.

## Project Structure

```text
ECC_ZKP/
├── ECC/                 # Elliptic curve and EC point operations
│   ├── ecc.cpp
│   └── ecc.h
│
├── ZKP/                 # ZKP protocol logic
│   ├── ECC_ZKP.cpp
│   └── ECC_ZKP.h
│
├── client/              # Prover-side TCP client
│   ├── ClientHandler.cpp
│   ├── TCPClient.cpp
│   └── main.cpp
│
├── server/              # Verifier-side TCP server
│   ├── ConnectionHandler.cpp
│   ├── TCPServer.cpp
│   └── main.cpp
│
├── common/              # Shared utilities
├── thread_pool/         # Thread pool used by the server
├── data/                # Output/log data
├── docker-compose.yaml  # Docker Compose setup
└── run.sh               # Native build script
```

## Features

- Elliptic curve arithmetic over finite fields
- secp256k1 curve parameters
- Schnorr-style ECC zero-knowledge proof
- TCP-based prover-verifier communication
- Multi-client prover simulation
- Thread-pool-based verifier server
- Docker Compose support
- Native C++ build support

## Dependencies

For native build:

- C++20 compiler
- NTL
- GMP
- pthread
- nlohmann/json

On Ubuntu/Debian, install the main dependencies with:

```bash
sudo apt update
sudo apt install -y g++ libntl-dev libgmp-dev nlohmann-json3-dev
```

## Build

Build the server and client using:

```bash
chmod +x run.sh
./run.sh
```

This produces two executables:

```text
verifier_server
prover_client
```

The build script compiles the verifier server and prover client with C++20, NTL, GMP, and pthread.

If your `nlohmann/json` header is installed system-wide, you may remove or adjust this include path if needed:

```bash
-I/user/include/nlohmann
```

## Run Locally

Start the verifier server first:

```bash
./verifier_server
```

In another terminal, run the prover client:

```bash
./prover_client <num_clients>
```

Example:

```bash
./prover_client 10
```

This launches 10 prover clients. Each client connects to the verifier server and runs the ECC ZKP protocol.

## Run with Docker Compose

You can also run the project using Docker Compose:

```bash
docker compose up --build
```

The server exposes port:

```text
1337
```

The client writes timing/output data to:

```text
data/client_output.txt
```

## Protocol Flow

```text
Verifier Server                         Prover Client
---------------                         -------------

Send curve parameters  ----------------> Receive curve parameters

                                         Choose private witness w
                                         Compute public key Y = wG
                                         Choose random r
                                         Compute A = rG

Receive A, Y           <---------------- Send A, Y

Generate challenge e   ----------------> Receive e

                                         Compute z = r + e*w mod q

Receive z              <---------------- Send z

Verify:
    zG == A + eY

Return:
    Authorized / Malicious
```

## Main Components

### `ECC/`

Implements elliptic curve operations:

- finite-field curve representation
- elliptic-curve point representation
- point addition
- scalar multiplication
- serialization/deserialization of curve points

### `ZKP/`

Implements the ECC zero-knowledge proof logic:

- prover commitment generation
- verifier challenge generation
- prover response generation
- verifier proof checking

### `server/`

Implements the verifier side:

- listens on TCP port `1337`
- sends public curve parameters
- receives prover commitment and public key
- sends random challenge
- verifies the proof
- supports concurrent connections through a thread pool

### `client/`

Implements the prover side:

- spawns multiple client processes
- connects to the verifier server
- receives curve parameters
- generates witness, commitment, and proof
- records waiting time/output data

## Example Output

A successful proof results in:

```text
Authorized
```

If verification fails, the server returns:

```text
Malicious
```

## Notes

This project is mainly for educational and experimental purposes.

It demonstrates how a zero-knowledge proof can be built over elliptic curves and integrated into a simple networked prover-verifier system.

The current implementation is not intended for production cryptographic use without further security review, input validation, constant-time implementation checks, and protocol hardening.

## License

No license has been specified yet. 

