/*
   Copyright 2020 Raphael Beck

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <string.h>
#include "include/decode.h"
#include "include/l8w8jwt/claim.h"

int main(int argc, char **argv)
{
    FILE *pub_key, *jwt_file;
    char buff_pub[1024], buff_jwt[1024];
    
    // Must take arguments
    if (argc <= 1) {
      printf("[USAGE]: ./decode <RSA_public_key_file> <JWT_token_file>\n");
      return 0;
    }
    else {
      pub_key = fopen(argv[1], "r");
      fread(buff_pub, 1, 1024, (FILE*)pub_key);
      printf("\n ------------------------ DECODER ------------------------------ \n");
      printf("\n[Read RSA Public Key]:\n%s\n\n", buff_pub);
      
      jwt_file = fopen(argv[2], "r");
      fscanf(jwt_file, "%s", buff_jwt);
      printf("[Read JWT Token]:\n%s\n\n", buff_jwt);
      
      fclose(pub_key); fclose(jwt_file);
    }
    
    struct l8w8jwt_decoding_params params;
    l8w8jwt_decoding_params_init(&params);

    params.alg = L8W8JWT_ALG_RS256;

    params.jwt = (char*) buff_jwt;
    params.jwt_length = strlen(buff_jwt);
    
    params.verification_key = (unsigned char*)buff_pub;
    params.verification_key_length = strlen(buff_pub);

    // This will generate validation result +1 if different
    params.validate_iss = "Black Mesa";
    params.validate_iss_length = strlen(params.validate_iss);

    // This will generate validation result +2 if different
    params.validate_sub = "Gordon Freeman";
    params.validate_sub_length = strlen(params.validate_sub);

    params.validate_exp = true;
    params.exp_tolerance_seconds = 60;

    params.validate_iat = true;
    params.iat_tolerance_seconds = 60;

    enum l8w8jwt_validation_result validation_result;
    struct l8w8jwt_claim *out_claims;
    size_t claims_length;
    
    // Decode
    int r = l8w8jwt_decode(&params, &validation_result, &out_claims, &claims_length);
    
    // Decode claims portion
    //l8w8jwt_write_claims(chillbuff* stringbuilder, struct l8w8jwt_claim* claims, const size_t claims_count);

    printf("\nl8w8jwt_decode_ps512 function returned %s (code %d).\n\nValidation result: \n%d\n", r == L8W8JWT_SUCCESS ? "successfully" : "", r, validation_result);
    
    // Primary Token validation
    if (validation_result) {
      // Token tampered with, signature could not be verified when decoding using public key
      if (validation_result & L8W8JWT_SIGNATURE_VERIFICATION_FAILURE) {
        printf("Certificate Validation Failed: Signature did not match - certificate could have been tampered with.\n");
        return 0;
      }
      
      // Verify issuer - VM2?
      if (validation_result & L8W8JWT_VALID) {
        printf("Certificate Validation Failed: Issuer did not match.\n");
      }
      
      // Verify subject - VM3?
      if (validation_result & L8W8JWT_SUB_FAILURE) {
        printf("Certificate Validation Failed: Subject did not match.\n");
      }
      
      // Verify audience - admin vm3?
      if (validation_result & L8W8JWT_AUD_FAILURE) {
        printf("Certificate Validation Failed: Audience did not match.\n");
      }
    }
    
    // Get out_claims
    printf("\nFound %d claims.\n", (int) claims_length);
    
    for (int i = 0; i < claims_length; i++) {
      printf("  [%d] - Claim key = '%s'\n", i, out_claims[i].key);
      printf("         Claim value = '%s'\n", out_claims[i].value);
      printf("         Claim type = '%s'\n", claim_types[out_claims[i].type]);
    }
    
    printf("\nExpected payload was:\n  Issuer: %s\n  Subject: %s\n", params.validate_iss, params.validate_sub);
    
    // Custom Token validation
    // Use claims here and validate them

    // Free stuff
    l8w8jwt_free_claims(out_claims, claims_length);
    
    return 0;
}
