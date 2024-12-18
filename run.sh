#!/bin/bash

g++ -std=c++20 -I/user/include/nlohmann -o verifier_server common/common.cpp server/*.cpp ZKP/ECC_ZKP.cpp thread_pool/*.cpp ECC/*.cpp -lntl -lgmp -pthread
g++ -std=c++20 -I/user/include/nlohmann -o prover_client common/common.cpp client/*.cpp ZKP/ECC_ZKP.cpp ECC/*.cpp -lntl -lgmp -pthread


