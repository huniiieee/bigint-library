#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include "type.h"

// Cryptography
int get_l(bigint* a);
int is_composite(bigint* n, bigint* q, int l, bigint* a);
int Miller_Rabin_test(bigint* n, int k);
int is_prime(bigint* N, int version);

void gcd(bigint** D, bigint* A, bigint* B);
void EEA(bigint* A, bigint** U, bigint* B, bigint** V, bigint** D);

void MOD_MUL(bigint** C, bigint* A, bigint* B, bigint* M);

void RSA_gen_key(RSA_PUBKEY* pub, RSA_PRIVKEY* priv, int n);
void RSA_encryption(bigint** CT, bigint* PT, RSA_PUBKEY* KEY);
void RSA_decryption(bigint** PT, bigint* CT, RSA_PRIVKEY* KEY);
#endif