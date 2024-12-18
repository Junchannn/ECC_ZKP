# Use Ubuntu 20.04 as the base image
FROM ubuntu:20.04

# Set non-interactive frontend to suppress prompts during installation
ENV DEBIAN_FRONTEND=noninteractive
ARG NTL_VERSION=11.4.1

# Install required dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++-10 \
    wget \
    make \
    libgmp-dev \
    libc6-dev \
    linux-libc-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

# Set GCC and G++ to version 10
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10

# Download and install NTL library with GMP support
RUN wget --no-check-certificate -O ntl.tar.gz "https://www.shoup.net/ntl/ntl-$NTL_VERSION.tar.gz" && \
    mkdir -p /usr/src/ntl && \
    tar -xzf ntl.tar.gz -C /usr/src/ntl --strip-components=1 && \
    rm ntl.tar.gz && \
    cd /usr/src/ntl/src && \
    ./configure NTL_GMP_LIP=on CXXFLAGS="-g -O2 -fPIC -march=native -pthread -std=c++20" && \
    make && \
    make install && \
    rm -rf /usr/src/ntl

# Create directory structure for the project
RUN mkdir -p /ZKP_server/server /ZKP_server/ZKP /ZKP_server/thread_pool /ZKP_server/ECC /ZKP_server/common

# Copy source code into the container
COPY server/* /ZKP_server/server/
COPY ZKP/* /ZKP_server/ZKP/
COPY thread_pool/* /ZKP_server/thread_pool/
COPY ECC/* /ZKP_server/ECC/
COPY common/* /ZKP_server/common/

# Compile the verifier_server binary
RUN cd /ZKP_server && \
    g++ -std=c++20 -I/user/include/nlohmann -o verifier_server \
    common/common.cpp server/*.cpp ZKP/ECC_ZKP.cpp thread_pool/*.cpp ECC/*.cpp \
    -lntl -lgmp -pthread

# Expose port 1337 for the server
EXPOSE 1337

# Set the default command to run the server
CMD ["/ZKP_server/verifier_server"]
