/******************************************************
*						Header
*******************************************************/
#include "type.h"
#include "BasicOperation.h"
#include "Operation.h"
#include "utill.h"



/******************************************************
*						Addition
*******************************************************/

/**
* @brief Addition C = A + B + c (c : carry)
* @details word A와 word B, carry c를 더해 word C에 저장한 후 carry값을 반환한다.
* @param word* C 더한 값을 저장할 word 형 변수
* @param word A 더할 word 형 변수
* @param word B 더할 word 형 변수
* @param unsigned char c A와 B를 더해서 얻어지는 carry
* @return carry (0 or 1)
* @throws
*		FAILURE : c가 캐리 범위가 아닌 경우
*/
unsigned char ADD_ABc(word* C, word A, word B, unsigned char c)
{
	unsigned char c2 = 0;

	if ((c != 0) && (c != 1))
	{
		printf("\n********[ ADD_ABc : Input carry error ]********\n");
		return FAILURE;
	}
	*C = A + B;

	if (*C < A)
		c2 = 1;

	*C = *C + (word)c;

	if (*C < c)
		c2 = 1;

	return c2;
}

/**
* @brief Addition core C = A + B (WordLen(A) >= WordLen(B), Sign(A) = Sign(B) = 0)
* @details bigint A와 B의 절댓값을 더해 C에
* @param bigint** C bigint C의 더블포인터
* @param bigint* A bigint A의 포인터
* @param bigint* B bigint B의 포인터
*/
void ADDC(bigint** C, bigint* A, bigint* B)
{
	int n = A->wordlen;
	int m = B->wordlen;
	unsigned char c = 0;
	word zero_B = 0;

	(*C)->sign = A->sign;

	for (int i = 0; i < m; i++)
		c = ADD_ABc(&((*C)->a[i]), A->a[i], B->a[i], c);

	for (int i = m; i < n; i++)
		c = ADD_ABc(&(*C)->a[i], A->a[i], zero_B, c);

	if (c == 1)
		(*C)->a[(*C)->wordlen - 1] = c;
}

/**
* @brief Addition
* @details bigint A와 B를 더해 C에 저장한다. 부호가 같을 경우는 ADDC, 부호가 다를 경우 SUB, 하나 이상이 0일 경우 덧셈 대신 bi_assign 함수를 사용
* @param bigint** C bigint C의 더블포인터
* @param bigint* A bigint A의 포인터
* @param bigint* B bigint B의 포인터
*/
void ADD(bigint** C, bigint* A, bigint* B)
{
	// 둘다 0인 경우는 따로 생각 안해도 될라나 ??

	if (bi_isZero(A))
		bi_assign(C, B);
	else if (bi_isZero(B))
		bi_assign(C, A);
	else
	{
		if ((A->sign == NON_NEGATIVE) && (B->sign == NEGATIVE))
		{
			bi_flip_sig(&B);
			SUB(C, A, B);
		}
		else if ((A->sign == NEGATIVE) && (B->sign == NON_NEGATIVE))
		{
			bi_flip_sig(&A);
			SUB(C, B, A);
		}
		else
		{
			if (A->wordlen >= B->wordlen)
			{
				bi_new(C, A->wordlen + 1);
				ADDC(C, A, B);
				bi_refine(*C);
			}
			else
			{
				bi_new(C, B->wordlen + 1);
				ADDC(C, B, A);
				bi_refine(*C);
			}
		}
	}
}

/******************************************************
*						Subtraction
*******************************************************/

/**
* @brief word A와 word B를 빼 word C에 저장한다.
* @details word A와 word B를 빼 word C에 저장한 후 borrow를 반환
* @param word A 뺄셈을 수행할 word A
* @param word B 뺄셈을 수행할 word B
* @param word* C A와 B를 뺀 값을 저장할 word C의 포인터
* @param unsigned char b 버로우
* @return -1|b2 오류메세지가 출력될 경우 -1, 정상작동했을 경우 b2 반환
*/
unsigned char SUB_AbB(word* C, word A, word B, unsigned char b)
{
	if ((b != 0) && (b != 1))
	{
		printf("\n********[ SUB_AbC : Input borrow error ]********\n");
		return -1;
	}
	unsigned char b2 = 0;

	*C = A - b;

	// A ==0 , b == 1
	if (A < b)
		b2 = 1;

	if (*C < B)
		b2 = b2 + 1;

	*C = *C - B;

	return b2;

}

/**
* @brief A >= B > 0인 bigint A와 bigint B를 빼 bigint C에 저장한다.
* @details A >= B > 0인 bigint A와 bigint B를 word 단위로 빼며 C에 저장
* @param bigint** C 결과값을 저장할 bigint C의 더블포인터
* @param bigint* A 뺄셈을 수행할 bigint A의 포인터
* @param bigint* B 뺄셈을 수행할 bigint B의 포인터
*/
void SUBC(bigint** C, bigint* A, bigint* B)
{
	int i = 0;
	int n = A->wordlen;
	int m = B->wordlen;

	unsigned char b = 0;
	word zero_B = 0;

	for (i = 0; i < m; i++)
		b = SUB_AbB(&(*C)->a[i], A->a[i], B->a[i], b);

	for (i; i < n; i++)
		b = SUB_AbB(&(*C)->a[i], A->a[i], zero_B, b);

}

/**
* @brief bigint A와 bigint B를 빼 bigint C에 저장한다.
* @details A와 B가 둘 다 0이거나 둘 중 하나가 0인 경우에 대해 E셈연산을 사용하지 않고 처리해주고 그 후로는 A와 B의 부호와 크기를 비교하며 case별로 E셈 or 덧셈 진행
* @param bigint** C 뺄셈의 결과값을 저장할 bigint C의 더블포인터
* @param bigint* A 뺄셈을 수행할 bigint A의 포인터
* @param bigint* B 뺄셈을 수행할 bigint B의 포인터
*/
void SUB(bigint** C, bigint* A, bigint* B)
{
	if (bi_compare(A, B) == 0)
	{
		bi_set_zero(C);
		return;
	}

	if (bi_isZero(A))
	{
		bi_flip_sig(&B);
		bi_assign(C, B);
		return;
	}

	if (bi_isZero(B))
	{
		bi_assign(C, A);
		return;
	}

	if ((bi_compare(A, B) >= 0) && (A->sign == NON_NEGATIVE && B->sign == NON_NEGATIVE))
	{
		bi_new(C, A->wordlen);
		SUBC(C, A, B);
		bi_refine(*C);
		return;
	}
	else if ((bi_compare(A, B) < 0) && (A->sign == NON_NEGATIVE && B->sign == NON_NEGATIVE))
	{
		bi_new(C, B->wordlen);
		SUBC(C, B, A);
		bi_flip_sig(C);
		bi_refine(*C);
		return;
	}

	if ((bi_compare(A, B) >= 0) && (A->sign == NEGATIVE && B->sign == NEGATIVE))
	{
		bi_new(C, B->wordlen);
		bi_flip_sig(&A);
		bi_flip_sig(&B);
		SUBC(C, B, A);
		bi_refine(*C);
		return;
	}
	else if ((bi_compare(A, B) < 0) && (A->sign == NEGATIVE && B->sign == NEGATIVE))
	{
		bi_new(C, A->wordlen);
		bi_flip_sig(&A);
		bi_flip_sig(&B);
		SUBC(C, A, B);
		bi_flip_sig(C);
		bi_refine(*C);
		return;
	}

	if ((A->sign == NON_NEGATIVE) && (B->sign == NEGATIVE))
	{
		bi_flip_sig(&B);
		ADD(C, A, B);
		return;
	}
	else if ((A->sign == NEGATIVE) && (B->sign == NON_NEGATIVE))
	{
		bi_flip_sig(&A);
		ADD(C, A, B);
		bi_flip_sig(C);
		return;
	}
}

/******************************************************
*					Multiplication
*******************************************************/

/**
* @brief  0보다 작지 않은 word A와 word B를 곱해 bigint C에 저장한다
* @details word를 반으로 쪼개서 곱셈 진행
* @param bigint** C 곱셈을 수행한 결과값을 저장할 bigint C의 더블포인터
* @param word A 곱셈을 수행할 word A
* @param word B 곱셈을 수행할 word B
*/
void MUL_AB(bigint** C, word A, word B)
{

	bi_new(C, 2);
	word A1 = A >> (sizeof(word) * 4);
	word A0 = A ^ (A1 << (sizeof(word) * 4));

	word B1 = B >> (sizeof(word) * 4);
	word B0 = B ^ (B1 << (sizeof(word) * 4));

	word T1 = A1 * B0;
	word T0 = A0 * B1;

	T0 = T1 + T0;
	T1 = (T0 < T1);

	(*C)->a[0] = A0 * B0;
	(*C)->a[1] = A1 * B1;
	word T = (*C)->a[0];

	(*C)->a[0] = (*C)->a[0] + (T0 << (sizeof(word) * 4));
	(*C)->a[1] = (*C)->a[1] + (T1 << (sizeof(word) * 4)) + (T0 >> (sizeof(word) * 4)) + (((*C)->a[0]) < T);
	bi_refine(*C);
}

/**
* @brief 0보다 작지 않은 bigint A와 bigint B를 곱해 bigint C에 저장한다
* @details word단위로 곱하고 곱한 값들을 더하는 방식으로 곱셈진행
* @param bigint** C 곱셈을 수행한 결과값을 저장할 bigint C의 더블포인터
* @param bigint* A 곱셈을 수행할 bigint A의 포인터
* @param bigint* B 곱셈을 수행할 bigint B의 포인터
*/
void MULC(bigint** C, bigint* A, bigint* B)
{
	int n = A->wordlen;
	int m = B->wordlen;
	bigint* T1 = NULL;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			MUL_AB(&T1, A->a[i], B->a[j]);
			bi_left_shift(&T1, WORDLEN * (i + j));
			ADDC(C, *C, T1);
		}
	}
	bi_delete(&T1);
}

/**
* @brief bigint A,B의 곱셈을 수행한다
* @details A와 B 둘 중 하나 이상이 0일 경우 C에 0을 저장하고, A와 B의 절댓값이 1인지 확인 후 곱셈연산을 사용하지않고 처리하고 A와 B가 같으면 제곱연산 수행. version에 따라 NAIVE or KARATSUBA곱셈 진행
* @param bigint** C 곱셈을 수행한 결과값을 저장할 bigint C의 더블포인터
* @param bigint* A 곱셈을 수행할 bigint A의 포인터
* @param bigint* B 곱셈을 수행할 bigitn B의 포인터
* @param int version NAIVE 또는 KARATSUBA방법으로 연산을 수행하게 하는 변수
*/
void MUL(bigint** C, bigint* A, bigint* B, int version)
{
	int A_sign = 0;
	int B_sign = 0;

	if (bi_isZero(A) || bi_isZero(B))
	{
		bi_set_zero(C);
		return;
	}

	if (bi_isOne(A))
	{
		bi_assign(C, B);
		return;
	}
	bi_flip_sig(&A);

	if (bi_isOne(A))
	{
		bi_assign(C, B);
		bi_flip_sig(C);
		return;
	}
	bi_flip_sig(&A);

	if (bi_isOne(B))
	{
		bi_assign(C, A);
		return;
	}
	bi_flip_sig(&B);

	if (bi_isOne(B))
	{
		bi_assign(C, A);
		bi_flip_sig(C);
		return;
	}
	bi_flip_sig(&B);

	if (bi_compare(A, B) == 0)
	{
		SQU(C, A, KARATSUBA);
		return;
	}

	A_sign = A->sign;
	B_sign = B->sign;
	A->sign = NON_NEGATIVE;
	B->sign = NON_NEGATIVE;
	switch (version)
	{
	case NAIVE:
		bi_new(C, A->wordlen + B->wordlen);
		MULC(C, A, B);
		break;
	case KARATSUBA:
		MULC_K(C, A, B, 4);
		break;
	default:
		bi_new(C, A->wordlen + B->wordlen);
		MULC(C, A, B);
		break;
	}
	A->sign = A_sign;
	B->sign = B_sign;
	(*C)->sign = A->sign ^ B->sign;
	bi_refine(*C);
}

/**
* @brief 0보다 작지 않은 bigint A,B를 KARATSUBA 방식을 이용해 곱셈한다
* @details KARATSUBA(분할 정복 방식)을 사용해 NAIVE보다 빠르게 연산한다.
* @param bigint** C 곱셈의 결과값을 저장할 bigint C의 더블포인터
* @param bigint* A 곱셈을 수행할 bigint A의 포인터
* @param bigint* B 곱셈을 수행할 bigint B의 포인터
* @param int flag 워드열이 일정 크기보다 작아지면 NAIVE한 곱셈을 이용하는 것이 빠르다. 그 일정크기를 flag값으로 설정
*/
void MULC_K(bigint** C, bigint* A, bigint* B, int flag)
{
	int S0_sign = 0;
	int S1_sign = 0;
	int n = A->wordlen;
	int m = B->wordlen;
	if (flag >= min(n, m))
	{
		MUL(C, A, B, NAIVE);
		return;
	}

	int l = (max(n, m) + 1) >> 1;
	bigint* A0 = NULL;
	bigint* A1 = NULL;
	bigint* B0 = NULL;
	bigint* B1 = NULL;
	bigint* T0 = NULL;
	bigint* T1 = NULL;
	bigint* R = NULL;
	bigint* S0 = NULL;
	bigint* S1 = NULL;
	bigint* S = NULL;
	bigint* T = NULL;	// real temp
	bi_assign(&A0, A);
	bi_reduction(&A0, l * WORDLEN);
	bi_assign(&A1, A);
	bi_right_shift(&A1, l * WORDLEN);

	bi_assign(&B0, B);
	bi_reduction(&B0, l * WORDLEN);

	bi_assign(&B1, B);
	bi_right_shift(&B1, l * WORDLEN);

	MULC_K(&T0, A0, B0, flag);
	MULC_K(&T1, A1, B1, flag);
	bi_left_shift(&T1, 2 * l * WORDLEN);
	ADD(C, T1, T0);
	bi_right_shift(&T1, 2 * l * WORDLEN);
	bi_assign(&R, *C);



	SUB(&S0, B1, B0);
	SUB(&S1, A0, A1);
	S0_sign = S0->sign;
	S1_sign = S1->sign;
	if (S0->sign == NEGATIVE)
		bi_flip_sig(&S0);
	if (S1->sign == NEGATIVE)
		bi_flip_sig(&S1);
	MULC_K(&S, S1, S0, flag);
	S->sign = S0_sign ^ S1_sign;
	ADD(&T, S, T1);
	ADD(&S, T, T0);
	bi_left_shift(&S, l * WORDLEN);
	ADD(C, R, S);
	bi_delete(&A0);
	bi_delete(&A1);
	bi_delete(&B0);
	bi_delete(&B1);
	bi_delete(&T0);
	bi_delete(&T1);
	bi_delete(&R);
	bi_delete(&S0);
	bi_delete(&S1);
	bi_delete(&S);
	bi_delete(&T);
}

/******************************************************
*					   Squaring
*******************************************************/

/**
* @brief 제곱연산을 수행한다
* @details 곱셈연산과 거의 동일하게 수행되지만 제곱연산에서는 값이 겹치는 부분이 있기 때문에 그 부분은 한번 연산후 2배처리 해줌
* @param bigint** C 제곱연산을 수행한 후 결과값을 저장할 bigint C의 더블포인터
* @param bigint* A 제곱연산을 수행할 bigint A의 포인터
*/
void SQUC(bigint** C, bigint* A)
{
	bigint* C1 = NULL;
	bigint* C2 = NULL;
	bigint* T1 = NULL;
	bigint* T3 = NULL;

	int n = A->wordlen;

	bi_new(&C1, 2 * n);
	bi_new(&C2, 2 * n);
	for (int i = 0; i < n; i++)
	{
		MUL_AB(&T1, A->a[i], A->a[i]);
		bi_left_shift(&T1, 2 * i * WORDLEN);
		ADDC(&C1, C1, T1);

		for (int j = i + 1; j < n; j++)
		{

			MUL_AB(&T3, A->a[i], A->a[j]);
			bi_left_shift(&T3, WORDLEN * (i + j));
			ADDC(&C2, C2, T3);
		}
	}
	bi_left_shift(&C2, 1);
	ADD(C, C1, C2);
	bi_delete(&C1);
	bi_delete(&C2);
	bi_delete(&T1);
	bi_delete(&T3);
}

/**
* @brief 제곱연산을 수행한다.
* @details bigint A가 0 또는 1일 경우 C에 A를 저장한다. -1일 경우엔 1을 저장한다. 곱셈과 마찬가지로 NAIVE or KARATSUBA중 한개 수행
* @param bigint** C 제곱연산의 결과값을 저장할 bigint C의 더블포인터
* @param bigint* A 제곱연산을 수행할 bigint A의 포인터
* @param int version NAIVE or KARATSUBA 선택
*/
void SQU(bigint** C, bigint* A, int version)
{
	if (bi_isZero(A) || bi_isOne(A))
	{
		bi_assign(C, A);
		return;
	}
	bi_flip_sig(&A);
	if (bi_isOne(A))
	{
		bi_assign(C, A);
		return;
	}
	bi_flip_sig(&A);
	switch (version)
	{
	case NAIVE:
		//bi_new(C, A->wordlen * 2);
		SQUC(C, A);
		break;
	case KARATSUBA:
		//bi_new(C, A->wordlen * 2);
		SQUC_K(C, A, 3);
		break;
	default:
		//bi_new(C, A->wordlen * 2);
		SQUC(C, A);
		break;
	}
}

/**
* @brief bigint A를 KARATSUBA 방식을 이용해 제곱한다
* @details KARATSUBA(분할 정복 방식)을 사용해 NAIVE보다 빠르게 연산한다
* @param bigint** C 제곱의 결과값을 저장할 bigint C의 더블포인터
* @param bigint* A 제곱을 수행할 bigint A의 포인터
* @param int flag 워드열이 일정 크기보다 작아지면 NAIVE한 제곱을 이용하는 것이 빠르다. 그 일정크기를 flag값으로 설정
*/
void SQUC_K(bigint** C, bigint* A, int flag)
{
	int n = A->wordlen;
	if (flag >= n)
	{
		SQU(C, A, NAIVE);
		return;
	}

	int l = (n + 1) >> 1;
	if (A->sign == NEGATIVE)
		bi_flip_sig(&A);

	bigint* A0 = NULL;
	bigint* A1 = NULL;
	bigint* T0 = NULL;
	bigint* T1 = NULL;
	bigint* S = NULL;
	bigint* R = NULL;

	bi_assign(&A0, A);
	bi_reduction(&A0, l * WORDLEN);

	bi_assign(&A1, A);
	bi_right_shift(&A1, l * WORDLEN);

	SQUC_K(&T0, A0, flag);
	SQUC_K(&T1, A1, flag);

	bi_left_shift(&T1, 2 * l * WORDLEN);
	ADD(&R, T1, T0);
	MULC_K(&S, A1, A0, flag);
	bi_left_shift(&S, l * WORDLEN + 1);
	ADD(C, R, S);
	bi_refine(*C);

	bi_delete(&A0);
	bi_delete(&A1);
	bi_delete(&T0);
	bi_delete(&T1);
	bi_delete(&S);
	bi_delete(&R);
}


/******************************************************
*					  Division
*******************************************************/

/**
* @brief 2word binary long division
* @details A는 2 word, B는 1 word이며 Q는 A를 B로 나눈 몫이고 이 함수는 MULTI PRECISION LONG DIVISON에 사용됨
* @param word* Q A를 B로 나눈 몫의 포인터
* @param word* 2 word A(나눠지는 수)의 포인터
* @param word B 1 word B(나누는 수)
*/
void BLD(word* Q, word* A, word B)
{
	word R = A[1];
	word T;
	*Q = 0;
	for (int i = WORDLEN - 1; i >= 0; i--)
	{
		if (R >= LtR_expo(2, WORDLEN - 1))
		{
			T = R;
			*Q += LtR_expo(2, i);
			R = R + i_bit(A[0], i);
			R = R - (B - T);
		}
		else
		{
			R = 2 * R + i_bit(A[0], i);
			if (R >= B)
			{
				*Q += LtR_expo(2, i);
				R = R - B;
			}
		}
	}
}


/**
* @brief bigint A를 B로 나누는 나눗셈
* @details A를 B로 나누는데 몫의 근사치를 구하고 실제 몫을 찾아가는 함수. 이 함수는 MULTI PRECISION LONG DIVISON에 사용됨
* @param word* Q A를 B로 나눈 몫
* @param bigint** R A를 B로 나눈 나머지의 더블 포인터
* @param bigint* A 나눠지는 bigint의 포인터
* @param bigint* B 나누는 bigint의 포인터
*/
void DIVCC(word* Q, bigint** R, bigint* A, bigint* B)
{
	int n = A->wordlen;
	int m = B->wordlen;
	word A2[2] = { 0, };
	A2[1] = A->a[A->wordlen - 1];
	A2[0] = A->a[A->wordlen - 2];

	if (n == m)
		*Q = A->a[n - 1] / B->a[n - 1];

	if (n == (m + 1))
	{
		if (A->a[n - 1] == B->a[m - 1])
			*Q = -1;
		else
			BLD(Q, A2, B->a[m - 1]);
	}
	bigint* T1 = NULL;
	bi_new(&T1, 1);

	T1->a[0] = *Q;
	bigint* T2 = NULL;
	MUL(&T2, B, T1, NAIVE);

	bigint* T3 = NULL;
	bi_assign(&T3, A);
	SUB(R, T3, T2);

	bi_assign(&T3, *R);
	while ((*R)->sign == NEGATIVE)
	{
		*Q -= 1;
		ADD(R, T3, B);
		bi_assign(&T3, *R);
	}
	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&T3);
}

/**
* @brief bigint A를 B로 나누는 나눗셈
* @details DIVCC를 쓸 수 있게 초기 조건을 맞춰주는 함수. 그리고 DIVCC이용해 몫과 나머지 구함. 이 함수는 MULTI PRECISION LONG DIVISON에 사용됨
* @param word* Q A를 B로 나눈 몫
* @param bigint** R A를 B로 나눈 나머지의 더블 포인터
* @param bigint* A 나눠지는 bigint의 포인터
* @param bigint* B 나누는 bigint의 포인터
*/
void DIVC(word* Q, bigint** R, bigint* A, bigint* B)
{

	if ((bi_compare(B, A)) == 1)
	{
		*Q = 0;
		return;
	}
	int k = 0;
	word T = B->a[B->wordlen - 1];
	while (i_bit(T, WORDLEN - 1) != 1)
	{
		T = T << 1;
		k++;
	}
	bi_left_shift(&A, k);
	bi_left_shift(&B, k);
	DIVCC(Q, R, A, B);
	bi_right_shift(R, k);
	bi_right_shift(&B, k);
}

/**
* @brief bigint A를 B로 나누는 나눗셈
* @details A와 B는 모두 0보다 작지 않은 bigint이며 이 함수가 MULTI PRECISON LONG DIVISION이다
* @param bigint** Q A를 B로 나눈 몫의 더블 포인터
* @param bigint** R A를 B로 나눈 나머지의 더블 포인터
* @param bigint* A 나눠지는 bigint의 포인터
* @param bigint* B 나누는 bigint의 포인터
*/
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	if ((bi_compare(B, A)) == 1)
	{
		bi_set_zero(Q);
		bi_assign(R, A);
		return;
	}

	if (bi_isZero(B))
	{
		printf("Zero divisor \n");
		return;
	}
	int n = A->wordlen;
	int m = B->wordlen;

	bi_new(Q, n);
	bi_new(R, m);

	bigint* T1 = NULL;
	bi_new(&T1, 1);

	word Q1;

	for (int i = n - 1; i >= 0; i--)
	{
		T1->a[0] = A->a[i];
		bi_left_shift(R, WORDLEN);
		ADDC(R, *R, T1);
		DIVC(&Q1, R, *R, B);
		(*Q)->a[i] = Q1;
	}
	bi_refine(*Q);
	bi_refine(*R);
	bi_delete(&T1);
}


/**
* @brief bigint A를 B로 나누는 나눗셈
* @details A와 B는 모두 0보다 작지 않은 bigint이며 B보다 작은 값이 되도록 A에서 계속 B를 A다
* @param bigint** Q A를 B로 나눈 몫의 더블 포인터
* @param bigint** R A를 B로 나눈 나머지의 더블 포인터
* @param bigint* A 나눠지는 bigint의 포인터
* @param bigint* B 나누는 bigint의 포인터
*/
void DIV_NAIVE(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	if ((bi_compare(B, A)) == 1)
	{
		bi_set_zero(Q);
		bi_assign(R, A);
		return;
	}
	if (bi_isZero(B))
	{
		printf("Zero divisor \n");
		return;
	}

	if (bi_isOne(B))
	{
		bi_assign(Q, A);
		bi_set_zero(R);
		return;
	}

	bi_set_zero(Q);
	bi_assign(R, A);

	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* T3 = NULL;
	bi_set_one(&T1);
	bi_set_zero(&T2);
	bi_assign(&T3, A);
	while (bi_compare(*R, B) >= 0)
	{
		ADD(Q, T2, T1);
		bi_assign(&T2, *Q);
		SUB(R, T3, B);
		bi_assign(&T3, *R);
	}

	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&T3);
}

/**
* @brief bigint A를 B로 나누는 나눗셈
* @details A와 B는 모두 0보다 작지 않은 bigint이며 BINARY LONG DIVISON방식으로 나눗셈 진행
* @param bigint** Q A를 B로 나눈 몫의 더블 포인터
* @param bigint** R A를 B로 나눈 나머지의 더블 포인터
* @param bigint* A 나눠지는 bigint의 포인터
* @param bigint* B 나누는 bigint의 포인터
*/
void LDA(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	if ((bi_compare(B, A)) == 1)
	{
		bi_set_zero(Q);
		bi_assign(R, A);
		return;
	}
	if (bi_isZero(B))
	{
		printf("Zero divisor \n");
		return;
	}
	if (bi_isOne(B))
	{
		bi_assign(Q, A);
		bi_set_zero(R);
		return;
	}
	int n = A->wordlen;
	bi_set_zero(Q);
	bi_set_zero(R);
	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* T3 = NULL;
	bi_set_one(&T2);
	bi_set_zero(&T3);
	for (int i = n - 1; i >= 0; i--)
	{
		for (int j = WORDLEN - 1; j >= 0; j--)
		{
			bi_left_shift(R, 1);
			bi_assign(&T1, *R);
			if (i_bit(A->a[i], j))
			{
				ADD(R, T1, T2);
				bi_assign(&T1, *R);
			}
			if (bi_compare(*R, B) >= 0)
			{
				bi_left_shift(&T2, i * WORDLEN + j);
				ADD(Q, T3, T2);
				bi_assign(&T3, *Q);
				bi_right_shift(&T2, i * WORDLEN + j);
				SUB(R, T1, B);
				bi_assign(&T1, *R);
			}
		}
	}
	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&T3);
}


/******************************************************
*				Modular exponentiation
*******************************************************/

/**
* @brief A^n mod M 수행
* @details A와 B는 모두 0보다 작지 않은 bigint이며 A^n mod M 연산을 수행하는데 left to right 방식으로 수행
* @param bigint** C A^n mod M 수행 결과 bigint의 더블 포인터
* @param bigint* A 밑
* @param bigint* n 지수
* @param bigint* M 나누는 수
*/
void LtR_MOD_EXP(bigint** C, bigint* A, bigint* n, bigint* M)
{
	bigint* tmp = NULL;
	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* Q = NULL;

	int l = 0;
	bi_assign(&tmp, n);
	while (!bi_isZero(tmp))
	{
		bi_right_shift(&tmp, 1);
		l++;
	}
	bi_set_one(&T1);

	for (int i = l; i > 0; i--)
	{
		SQU(&T2, T1, KARATSUBA);
		DIV(&Q, &T1, T2, M);
		if (bi_get_jth_bit(n, i))
		{
			MUL(C, A, T1, KARATSUBA);
			DIV(&Q, &T1, *C, M);
		}
	}


	bi_assign(C, T1);
	bi_refine(*C);
	bi_delete(&tmp);
	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&Q);
}

/**
* @brief A^n mod M 수행
* @details A와 B는 모두 0보다 작지 않은 bigint이며 A^n mod M 연산을 수행하는데 right to left 방식으로 수행
* @param bigint** C A^n mod M 수행 결과 bigint의 더블 포인터
* @param bigint* A 밑
* @param bigint* n 지수
* @param bigint* M 나누는 수
*/
void RtL_MOD_EXP(bigint** C, bigint* A, bigint* n, bigint* M)
{
	bigint* tmp = NULL;
	bigint* T0 = NULL;
	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* Q = NULL;
	int l = 0;

	bi_assign(&tmp, n);
	while (!bi_isZero(tmp))
	{
		bi_right_shift(&tmp, 1);
		l++;
	}

	bi_assign(&T1, A);
	bi_set_one(&T0);
	for (int i = 1; i <= l; i++)
	{
		if (bi_get_jth_bit(n, i))
		{
			MUL(&T2, T1, T0, KARATSUBA);
			DIV(&Q, &T0, T2, M);
		}
		SQU(&T2, T1, KARATSUBA);
		DIV(&Q, &T1, T2, M);
	}

	bi_assign(C, T0);
	bi_refine(*C);

	bi_delete(&tmp);
	bi_delete(&T0);
	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&Q);
}

/**
* @brief A^n mod M 수행
* @details A와 B는 모두 0보다 작지 않은 bigint이며 A^n mod M 연산을 수행하는데 montgomery 방식으로 수행
* @param bigint** C A^n mod M 수행 결과 bigint의 더블 포인터
* @param bigint* A 밑
* @param bigint* n 지수
* @param bigint* M 나누는 수
*/
void MONT_MOD_EXP(bigint** C, bigint* A, bigint* n, bigint* M)
{
	bigint* tmp = NULL;
	bigint* T0 = NULL;
	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* Q = NULL;
	bigint* R = NULL;
	int l = 0;

	bi_assign(&tmp, n);
	while (!bi_isZero(tmp))
	{
		bi_right_shift(&tmp, 1);
		l++;
	}

	bi_set_one(&T0);
	bi_assign(&T1, A);
	for (int i = l; i > 0; i--)
	{
		if (bi_get_jth_bit(n, i))
		{
			MUL(&T2, T1, T0, KARATSUBA);
			DIV(&Q, &T0, T2, M);
			SQU(&T2, T1, KARATSUBA);
			DIV(&Q, &T1, T2, M);
		}
		else
		{
			MUL(&T2, T1, T0, KARATSUBA);
			DIV(&Q, &T1, T2, M);
			SQU(&T2, T0, KARATSUBA);
			DIV(&Q, &T0, T2, M);
		}
	}
	bi_assign(C, T0);
	bi_refine(*C);

	bi_delete(&tmp);
	bi_delete(&T0);
	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&Q);
	bi_delete(&R);
}