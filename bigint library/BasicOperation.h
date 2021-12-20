#ifndef _BASICOPERATION_H_
#define _BASICOPERATION_H_

#include "type.h"

void bi_delete(bigint** x);
void bi_new(bigint** x, int wordlen);

void bi_set_by_array(bigint** x, int sign, word* a, int wordlen);
int bi_set_by_string_hex(bigint** x, int sign, char* str);

void bi_show(bigint* x);		// 일단 진법 관계 없이 워드열을 출력 (Hex)
void bi_show_sage_syntax(bigint* x, FILE* fp);
void bi_show_hex(bigint* x);
void bi_show_dec(bigint* x);
void bi_show_bin(bigint* x);
void bi_refine(bigint* x);
void bi_assign(bigint** y, bigint* x);

void bi_get_i_bit_rand(bigint** P, int bitLen);
void bi_gen_rand(bigint** x, int sign, int wordlen);
int bi_get_word_len(bigint* x);
int bi_get_bit_len(bigint* x);
int bi_get_jth_bit(bigint* x, int j);
int bi_get_sign(bigint* x);

void bi_flip_sig(bigint** x);
void bi_set_one(bigint** x);
void bi_set_zero(bigint** x);
int bi_isZero(bigint* x);
int bi_isOne(bigint* x);

int bi_compareABS(bigint* A, bigint* B);
int bi_compare(bigint* A, bigint* B);


void bi_left_shift(bigint** A, int shamt);
void bi_right_shift(bigint** B, int shamt);
void bi_reduction(bigint** x, int reamt);

#endif