# How to build - Ubuntu 18.04


a) `git clone --recurse-submodules` or untar/unzip

b) `git submodule update --init --recursive` to get submodules l8w8jwt-related if you did not use the full command at step a).

c) run the build script `./build.sh`

d) run `test.sh` to test it all `chmod +x ./test.sh && ./test.sh`


# Manual steps:


## 0. Using



If using cmake for new applications, `cmake .. && make` and in your source code include only the headers:
    #include "include/encode.h"
    #include "include/decode.h"
as required.


## 1. Advanced usage



a) Generate key pairs in ubuntu using
>> ssh-keygen

Note: To avoid the prompt, you can use this to automate the command
    `ssh-keygen -b 4096 -t rsa -m PEM -f ./temp_ssh_key -q -N "" -y`
    `openssl rsa -in temp_ssh_key -pubout -outform PEM -out temp_ssh_key.pub`

Output:
>> Generating public/private rsa key pair.
>> Enter file in which to save the key (`/your_home/.ssh/id_rsa`):

b) Create Token
`make && ./l8test_encode temp_ssh_key ../jwt_test_token_file`

c) Use token and validate
`make && ./l8test_decode temp_ssh_key.pub ../jwt_test_token_file`

Encoder should create the token with some hard-coded values.

Decoder should validate the token and return 0 if successful. If signed by the wrong key, or tampered with, or the payload is different in any way, it will return the cause of the error.


# Comments


ssh-keygen default (https://www.freebsd.org/cgi/man.cgi?query=ssh-keygen&sektion=1&manpath=OpenBSD and again here https://tools.ietf.org/html/rfc7518#section-3.3), SHA256 so corresponding algorithm is in l8w8jwt is:
>> RSASSA-PKCS1-v1_5-SHA256 signing algorithm.
>> #define L8W8JWT_ALG_RS256 3
