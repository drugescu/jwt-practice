#!/bin/sh

# Switch to build directory
mkdir build && cd build && cmake .. && make
cd build && make

# Generate RSA key with 4096 bits, PEM format, SHA256 basic hash signature
ssh-keygen -b 4096 -t rsa -m PEM -f ./temp_ssh_key -q -N "" -y
openssl rsa -in temp_ssh_key -pubout -outform PEM -out temp_ssh_key.pub
echo "Generated RSA private and public keys."

# Make encoder and test
./l8test_encode temp_ssh_key ../jwt_test_token_file

#Make decoder and test
./l8test_decode temp_ssh_key.pub ../jwt_test_token_file
