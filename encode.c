#include <stdio.h>
#include <string.h>
#include "include/encode.h"

int main(int argc, char **argv)
{
    char* jwt;
    size_t jwt_length;
    
    FILE *priv_key, *jwt_file;
    char buff_priv[4096], buff_jwt[1024], buff_priv_final[4096];

    // Must take arguments
    if (argc <= 1) {
      printf("[USAGE]: ./encode <RSA_private_key_file> <JWT_output_token_file>\n");
      return 0;
    }
    else {
      // Open and read private key
      priv_key = fopen(argv[1], "r");
      fread(buff_priv, 1, 4096, (FILE*)priv_key);
      printf("\n ------------------------ ENCODER ------------------------------ \n");
      printf("\n[Read RSA Private Key]: %s\n", buff_priv);
      
      fclose(priv_key);
    }


    struct l8w8jwt_claim header_claims[] =
    {
        {
            .key = "kid",
            .key_length = 3,
            .value = "guuid-ley-001",
            .value_length = strlen("guuid-ley-001"),
            .type = L8W8JWT_CLAIM_TYPE_STRING
        }
    };

    // The actual payload of information we transmit
    struct l8w8jwt_claim payload_claims[] =
    {
        {
            .key = "ctx",
            .key_length = 3,
            .value = "Title of Token",
            .value_length = strlen("Title of Token"),
            .type = L8W8JWT_CLAIM_TYPE_STRING
        },
        {
            .key = "age",
            .key_length = 3,
            .value = "32",
            .value_length = strlen("32"),
            .type = L8W8JWT_CLAIM_TYPE_INTEGER
        },
        {
            .key = "height",
            .key_length = strlen("height"),
            .value = "1.77",
            .value_length = strlen("1.77"),
            .type = L8W8JWT_CLAIM_TYPE_NUMBER
        },
        {
            .key = "adult",
            .key_length = strlen("adult"),
            .value = "true",
            .value_length = strlen("true"),
            .type = L8W8JWT_CLAIM_TYPE_BOOLEAN
        },
        {
            .key = "nullkey",
            .key_length = strlen("nullkey"),
            .value = "null",
            .value_length = strlen("null"),
            .type = L8W8JWT_CLAIM_TYPE_NULL
        }
    };

    struct l8w8jwt_encoding_params params;
    l8w8jwt_encoding_params_init(&params);

    params.alg = L8W8JWT_ALG_RS256;

    // sub: The subject of the token
    params.sub = "Subject of Token";
    params.sub_length = strlen("Subject of Token");

    // iss: The issuer of the token
    params.iss = "Issuer of Token";
    params.iss_length = strlen("Issuer of Token");

    // aud: The audience of the token
    params.aud = "Audience of Token";
    params.aud_length = strlen("Audience of Token");

    // exp: This will probably be the registered claim most often used. This will define the expiration in NumericDate value. The expiration MUST be after the current date/time.
    params.iat = time(NULL);
    params.exp = time(NULL) + 600; // Set to expire after 10 minutes (600 seconds).

    params.additional_header_claims = header_claims;
    params.additional_header_claims_count = sizeof(header_claims) / sizeof(struct l8w8jwt_claim);

    params.additional_payload_claims = payload_claims;
    params.additional_payload_claims_count = sizeof(payload_claims) / sizeof(struct l8w8jwt_claim);

    // The RSA key used to encode
    params.secret_key = (unsigned char*) buff_priv;
    params.secret_key_length = strlen(buff_priv);

    params.out = &jwt;
    params.out_length = &jwt_length;

    int r = l8w8jwt_encode(&params);
    
    printf("\nl8w8jwt_encode_rs256 function returned %s (code %d).\n\nCreated token: \n%s\n", r == L8W8JWT_SUCCESS ? "successfully" : "", r, jwt);

    if (r == L8W8JWT_SUCCESS) {
      jwt_file = fopen(argv[2], "w");
      fwrite(jwt, 1, strlen(jwt), (FILE*)jwt_file);
      fclose (jwt_file);
    }
    else {
      // printf("Error %d is %s", r, errs[r]);
      // To implement
    }

    free(jwt);
    return 0;
}
