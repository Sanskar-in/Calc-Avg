// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/sanskarIN/Calc-Avg"
//
// Follow Sanskar or Creator on:-
// GitHub:- "https://www.github.com/sanskarIN"
// YouTube:- "https://youtube.com/@sanskarIN"
// LinkedIn:- "https://www.linkedin.com/in/sanskar-in/"
// X (Formally known as Twitter):- "https://www.x.com/Sanskar_in"
// Need support:- supportramsandesh@gmail.com
// Want to make your own projects, apps, and games via Sanskar:- sanskaryadavfrom2012to2026@gmail.com
// =============================================================================

#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stddef.h>

/**
 * SHA-256 Context structure
 */
typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
} SHA256_CTX;

/**
 * Initializes the SHA-256 Context.
 */
void sha256_init(SHA256_CTX *ctx);

/**
 * Updates the SHA-256 hash with the given data.
 */
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);

/**
 * Finalizes the SHA-256 hash and outputs the 32-byte digest.
 */
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);

/**
 * Convenience function to calculate an average, hash the result mathematically,
 * and print the 64-character hexadecimal signature.
 */
void calc_and_hash_average(double data_points[], int count);

#endif // CRYPTO_H
