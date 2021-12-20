#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "type.h"

unsigned char ADD_ABc(word* C, word A, word B, unsigned char c);
void ADDC(bigint** C, bigint* A, bigint* B);
void ADD(bigint** C, bigint* A, bigint* B);

unsigned char SUB_AbB(word* C, word A, word B, unsigned char b);
void SUBC(bigint** C, bigint* A, bigint* B);
void SUB(bigint** C, bigint* A, bigint* B);

void MUL(bigint** C, bigint* A, bigint* B, int version);
void MULC(bigint** C, bigint* A, bigint* B);
void MUL_AB(bigint** C, word A, word B);
void MULC_K(bigint** C, bigint* A, bigint* B, int flag);

void SQU(bigint** C, bigint* A, int version);
void SQUC(bigint** C, bigint* A);
void SQUC_K(bigint** C, bigint* A, int flag);

void BLD(word* Q, word* A, word B);
void DIVCC(word* Q, bigint** R, bigint* A, bigint* B);
void DIVC(word* Q, bigint** R, bigint* A, bigint* B);
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B);

void DIV_NAIVE(bigint** Q, bigint** R, bigint* A, bigint* B);
void LDA(bigint** Q, bigint** R, bigint* A, bigint* B);

void LtR_MOD_EXP(bigint** C, bigint* A, bigint* n, bigint* M);
void RtL_MOD_EXP(bigint** C, bigint* A, bigint* n, bigint* M);
void MONT_MOD_EXP(bigint** C, bigint* A, bigint* n, bigint* M);
#endif