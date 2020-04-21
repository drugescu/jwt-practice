README
------


How to build - Ubuntu 18.04
---------------------------


a) git clone or untar/unzip
b) create a build directory inside with "mkdir build"
c) change to the build directory "cd build"
d) run cmake "cmake .."
e) run make to build the apps (encoder and decoder)
f) run make install to install the libraries in /usr/local/lib
  In the future, this step will also add the app binaries to /usr/bin
  Note: run with sudo to allow changes, if not root! It's a system dir!
g) run test.sh to test it all "chmod +x ./test.sh && ./test.sh"


Manual steps:
-------------


0. Installing
-------------


I did all these, generated the required libraries and put them in the archive/repo to avodi all these steps. 
In the interest of documenting things, I have added the steps here.

You will have to git clone the l8w8jwt library from https://github.com/GlitchedPolygons/l8w8jwt
Do the same for the dependencies:

a) cmocka-1.1.15 - the latest version is not available in git, so whatever you find in git and in l8w8jwt will no longer compile in ubuntu!
Instead download it from https://git.cryptomilk.org/projects/cmocka.git/snapshot/cmocka-1.1.5.tar.gz
Un-tar and repeat the mkdir build && cd build && cmake .. && make
b) git clone the mbed-tls repo, do the same procedures
Shove the cmocka there by force since the variant it has will no longer build

Build the libl8w8jwt.a

Get the other header-only libraries and start using them.

If using cmake for new applications, cmake .. && make and in your source code include only the headers:
  #include "include/encode.h"
  #include "include/decode.h"
as required.


1. Using
--------


a) Generate key pairs in ubuntu using
>> ssh-keygen

Note: To avoid the prompt, you can use this to automate the command
>> ssh-keygen -b 4096 -t rsa -m PEM -f ./temp_ssh_key -q -N "" -y
>> openssl rsa -in temp_ssh_key -pubout -outform PEM -out temp_ssh_key.pub

Output:
>> Generating public/private rsa key pair.
>> Enter file in which to save the key (/your_home/.ssh/id_rsa):

b) Create Token
make && ./l8test_encode temp_ssh_key ../jwt_test_token_file

c) Use token and validate
make && ./l8test_decode temp_ssh_key.pub ../jwt_test_token_file

Encoder should create the token with some hard-coded values.

Decoder should validate the token and return 0 if successful. If signed by the wrong key, or tampered with, or the payload is different in any way, it will return the cause of the error.


COMMENTS
--------

ssh-keygen default (https://www.freebsd.org/cgi/man.cgi?query=ssh-keygen&sektion=1&manpath=OpenBSD and again here https://tools.ietf.org/html/rfc7518#section-3.3), SHA256 so corresponding algorithm is in l8w8jwt is:
>> RSASSA-PKCS1-v1_5-SHA256 signing algorithm.
>> #define L8W8JWT_ALG_RS256 3
