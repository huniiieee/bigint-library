#include "type.h"
#include "BasicOperation.h"
#include "Operation.h"
#include "utill.h"

/**
* @brief Create Bigint
* @details 구조체 및 구조체 멤버 변수 생성
* @param bigint** x	생성할 bigint형 더블포인터 변수
* @param int wordlen 생성할 구조체 내부의 워드열 길이
*/
void bi_new(bigint** x, int wordlen)
{
	if ((*x != NULL) || (wordlen < 0))
		bi_delete(x);

	*x = (bigint*)malloc(sizeof(bigint));
	if (*x == NULL)
	{
		printf("Memory allocation error\n");
		return;
	}
	(*x)->sign = NON_NEGATIVE;
	(*x)->wordlen = wordlen;
	(*x)->a = (word*)calloc(wordlen, sizeof(word));
}

/**
* @brief Delete Bigint
* @details
*   ZERORIZE가 정의되어 있을 경우 워드열을 모두 0으로 초기화하고 구조체의 메모리를 반환한다.\n
*   ZERORIZE까 정의되어 있지 않을 경우 구조체의 메모리만 반환한다.
* @param bigint** x bigint x의 더블포인터
*/
void bi_delete(bigint** x)
{
	if (*x == NULL)
		return;

#ifdef ZERORIZE
	array_init((*x)->a, (*x)->wordlen);
#endif // ZERORIZE

	if ((*x)->a != NULL)
	{
		free((*x)->a);
		(*x)->a = NULL;
	}

	if (*x != NULL)
	{
		free(*x);
		*x = NULL;
	}

}


/**
* @brief Set Bigint bt array
* @details 입력 받은 워드열로 big integer를 생성한다.
* @param bigint** x bigint x의 더블포인터
* @param int sign bigint의 부호
* @param word* a 워드열
* @param int wordlen 워드열의 길이
*/
void bi_set_by_array(bigint** x, int sign, word* a, int wordlen)
{
	if (((sign != NON_NEGATIVE) && (sign != NEGATIVE)) || a == NULL || wordlen < 0)
		return;

	(*x)->sign = sign;
	(*x)->wordlen = wordlen;

	array_copy((*x)->a, a, wordlen);
}



/**
* @brief Set Bigint by string
* @details 입력 받은 문자열로 big integer를 생성한다.
* @param bigint** x bigint x의 더블포인터
* @param int sign bigint의 부호
* @param char* str bigint 문자열
*/
int bi_set_by_string_hex(bigint** x, int sign, char* str)
{
	if (((sign != NON_NEGATIVE) && (sign != NEGATIVE)) || str == NULL)
		return FAILURE;

	for (unsigned int i = 0; i < strlen(str); i++)
	{
		if ((str[i] < '0') || ((str[i] > '9') && (str[i] < 'A')) || ((str[i] > 'F') && (str[i] < 'a')) || (str[i] > 'f'))
			return FAILURE;

	}

	int n = sizeof(word) * 2;
	int r = strlen(str) % n;
	int size = (int)(strlen(str) / n);
	if (r)
		size++;
	bi_new(x, size);
	(*x)->sign = sign;

	for (unsigned int i = 0; i < strlen(str); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'F')                // str값이 'A'(65)~'F'(70) 일때
			str[i] = str[i] - 'A' + 11;
		else if (str[i] >= 'a' && str[i] <= 'f')                // str값이 'a'(97)~'f'(102) 일때
			str[i] = str[i] - 'a' + 11;
		else if (str[i] >= '0' && str[i] <= '9')            // str값이 '0'(48)~'9'(57) 일때
			str[i] = str[i] - '0' + 1;
	}

	word tmp = 0;
	int cnt = 0;
	if (r)
	{
		for (int i = 0; i < r; i++)
			tmp = tmp * 16 + str[i] - 1;

		(*x)->a[size - 1] = tmp;

		for (int i = size - 2; i >= 0; i--)
		{
			tmp = 0;
			for (int j = 0; j < n; j++)
			{
				tmp = tmp * 16 + str[r + cnt] - 1;
				cnt++;
			}
			(*x)->a[i] = tmp;
		}
	}
	else
	{
		for (int i = size - 1; i >= 0; i--)
		{
			tmp = 0;
			for (int j = 0; j < n; j++)
			{
				tmp = tmp * 16 + str[cnt] - 1;
				cnt++;
			}
			(*x)->a[i] = tmp;
		}
	}

	bi_refine(*x);
	return SUCCESS;
}


/**
* @brief Show big integer base hex
* @details
*	sign이 NEGATIVE면 -를 출력해준다. \n
*	WORDLEN이 8이면 2자리 16진법(1바이트)을, WORDLEN이 32면 8자리 16진법(4바이트)을 , WORDLEN이 64면 16자리 16진법(8바이트)을 출력한다
* @param bigint* x bigint x의 포인터
*/
void bi_show(bigint* x)
{
	int i = 0;

	if (x->sign == NEGATIVE)
		printf("-");
	printf("0x");
	for (i = x->wordlen - 1; i >= 0; i--)
	{
#if WORDLEN == 8
		printf("%02x", x->a[i]);
#elif WORDLEN == 32
		printf("%08x", x->a[i]);
#elif WORDLEN == 64
		printf("%016llx", x->a[i]);
#endif
	}
	printf("\n");
}

/**
* @brief Show big integer for sage
* @details Big integer x를 fp로 입력 받은 파일에 쓴다.
* @param bigint* x 파일에 입력할 Big integer 형 포인터
* @param FILE* fp Big integer를 입력할 파일 포인터
*/
void bi_show_sage_syntax(bigint* x, FILE* fp)
{
	int i = 0;
	if (x->sign == NEGATIVE)
		fputs("-", fp);
	fputs("0x", fp);
	for (i = x->wordlen - 1; i >= 0; i--)
	{
#if WORDLEN == 8
		fprintf(fp, "%02x", x->a[i]);
#elif WORDLEN == 32
		fprintf(fp, "%08x", x->a[i]);
#elif WORDLEN == 64
		fprintf(fp, "%016llx", x->a[i]);
#endif
	}
	fprintf(fp, "\n");
}

/**
* @brief Refine Big integer (Remove Last zero words)
* @details 상위 워드열이 0인 경우 의미가 없기 때문에 제거해주고 워드열 길이 재설정해준다
* @param bigint* x Refine할 bigint형 포인터
*/
void bi_refine(bigint* x)
{
	if (x == NULL)
		return;

	int new_wordlen = x->wordlen;
	while (new_wordlen > 1)
	{
		if (x->a[new_wordlen - 1] != 0)
			break;
		new_wordlen--;
	}

	if (x->wordlen != new_wordlen)
	{
		x->wordlen = new_wordlen;
		resize_memory(&(x->a), sizeof(word) * new_wordlen);
	}

	if ((x->wordlen == 1) && (x->a[0] == 0x00))
		x->sign = NON_NEGATIVE;
}

/**
* @brief Assign big integer
* @details 입력 받은 X를 Y에 할당한다.
* @param bigint** y 할당될 값의 bigint 형 더블포인터
* @param bigint* x 할당할 값의 bigint 형 포인터
*/
void bi_assign(bigint** y, bigint* x)
{
	if (*y != NULL)
		bi_delete(y);

	bi_new(y, x->wordlen);
	(*y)->sign = x->sign;
	array_copy((*y)->a, x->a, x->wordlen);
}

/**
* @brief Generation random big interger
* @details 입력 받은 크기, 부호를 갖는 임의의 big interger를 생성한다.
* @param bigint** x 생성된 bigint가 저장될 공간 x의 더블포인터
* @param int sign 생성된 bigint의 부호
* @param int wordlen 생성된 bigint의 워드열의 길이
*/
void bi_gen_rand(bigint** x, int sign, int wordlen)
{
	if (*x != NULL)
		bi_delete(x);

	bi_new(x, wordlen);
	(*x)->sign = sign;
	array_rand((*x)->a, wordlen);

	bi_refine(*x);
}

/**
* @brief Get word length of big integer
* @details 입력 받은 big integer의 워드열 길이를 반환한다.
* @param bigint* x bigint x의 포인터
* @return x의 워드열의 길이
*/
int bi_get_word_len(bigint* x)
{
	return x->wordlen;
}

/**
* @brief Get bit length of big integer
* @details 입력 받은 big integer의 비트열 길이를 반환한다.
* @param bigint* x bigint x의 포인터
* @return x가 할당된 bit단위의 메모리의 크기
*/
int bi_get_bit_len(bigint* x)
{
	return x->wordlen * sizeof(word) * 8;
	//return x->wordlen * WORDLEN;
}

/**
* @brief Get j th bit of big integer
* @details bigint x의 j번째 비트 값을 반환한다.
* @param bigint* x bigint x의 포인터
* @param int j 읽어올 위치
* @return bigint x의 j번째 비트 값
* @throws
*		FAILURE	j가 음수인 경우
*		FAILURE	j가 워드열 크기보다 큰 경우
*/
int bi_get_jth_bit(bigint* x, int j)
{
	if (j <= 0)
	{
		printf("choose j>0\n");
		return FAILURE;
	}

	int q = 0;
	int r = 0;

	q = (j - 1) / WORDLEN;	// 몇 번째 워드인지

	if (q > x->wordlen)
	{
		printf("\n이러면 워드길이 넘어감요\n");
		return FAILURE;
	}

	r = (j - 1) % WORDLEN;	// 해당 워드의 몇번째 비트인지

	return ((x->a[q]) >> r) & 0x01;
}



/**
* @brief Get sign of big integer
* @details 입력한 bigint의 부호를 반환한다.
* @param bigint* x 입력한 bigint x의 포인터
* @return bigint x의 부호
*/
int bi_get_sign(bigint* x)
{
	return x->sign;
}

/**
* @brief Flip sign of big integer
* @details 입력한 bigint의 부호를 바꾼다
* @param bigint** x 입력한 bigint x의 더블포인터
*/
void bi_flip_sig(bigint** x)
{
	(*x)->sign = (*x)->sign ? NON_NEGATIVE : NEGATIVE;
}

/**
* @brief Set big integer to 1
* @details 입력한 bigint x를 1로 설정한다.
* @param bigint** x 입력한 bigint x의 더블포인터
*/
void bi_set_one(bigint** x)
{
	bi_new(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x1;
}

/**
* @brief Set big integer to 0
* @details 입력한 bigint x를 0으로 설정한다.
* @param bigint** x 입력한 bigint x의 더블포인터
*/
void bi_set_zero(bigint** x)
{
	bi_new(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x0;
}

/**
* @brief Determine big integer is 0
* @details 입력한 bigint x가 0인지 확인한다.
* @param bigint* x 입력한 bigint x의 포인터
* @return TRUE or FALSE \n
*	0이면 TRUE, 아니면 FALSE
*/
int bi_isZero(bigint* x)
{

	if ((x->sign == NEGATIVE) || (x->wordlen > 1))
		return FALSE;

	return (x->a[0] == 0);
}

/**
* @brief Determine big integer is 1
* @details 입력한 bigint x가 1인지 확인한다.
* @param bigint* x 입력한 bigint x의 포인터
* @return TRUE or FALSE \n
*	0이면 TRUE, 아니면 FALSE
*/
int bi_isOne(bigint* x)
{
	if ((x->sign == NEGATIVE) || (x->wordlen > 1))
		return FALSE;

	return (x->a[0] == 1);
}

/**
* @brief Compare absolute value of big integer A, B
* @details
*		big integer A와 B의 크기를 비교한다.\n
*		A > B : 1 \n
*		A = B : 0 \n
*		A < B : -1
* @param bigint* A 입력한 bigint A의 포인터
* @param bigint* B 입력한 bigint B의 포인터
* @return 1 or 0 or -1
*/
int bi_compareABS(bigint* A, bigint* B)
{
	int i = 0;
	int n = A->wordlen;
	int m = B->wordlen;

	// case 1 : wordlen(A) > wordlen(B)
	if (n > m)
		return 1;

	// case 2 : wordlen(A) < wordlen(B)
	if (n < m)
		return -1;

	// case 3 : wordlen(A) = wordlen(B)
	for (i = (n - 1); i >= 0; i--)
	{
		if (A->a[i] > B->a[i])
			return 1;

		if (A->a[i] < B->a[i])
			return -1;
	}

	return 0;
}

/**
* @brief Compare value of big integer A, B
* @details
*		big integer A와 B의 크기를 비교한다.\n
*		A > B : 1 \n
*		A = B : 0 \n
*		A < B : -1 \n
* @param bigint* A 입력한 bigint A
* @param bigint* B 입력한 bigint B
* @return 1 or 0 or -1
*/
int bi_compare(bigint* A, bigint* B)
{
	int ret = 0;

	// case1 : A, B are same sign
	if ((A->sign == NON_NEGATIVE) && (B->sign == NEGATIVE))
		return 1;

	if ((A->sign == NEGATIVE) && (B->sign == NON_NEGATIVE))
		return -1;

	// case2 : A, B are different sign
	ret = bi_compareABS(A, B);

	if (A->sign == NON_NEGATIVE)
		return ret;
	else
		return ret * -1;
}

/**
* @brief Left shift
* @details bigint A를 shamt만큼 왼쪽으로 비트 쉬프트 연산을 한다.
* @param bigint** A bigint A의 더블 포인터
* @param int shamt 쉬프트 할 비트의 크기
*/
void bi_left_shift(bigint** A, int shamt)
{
	// l : shift amount
	// w : word bit length
	// n : word length
	// k : l = w*k
	// r : l = w*k + r
	int w = sizeof(word) * 8;
	int k = shamt / w;
	int r = shamt % w;
	int new_wordlen = (*A)->wordlen + k;


	if (shamt == 0)
		return;


	word* tmp = NULL;
	tmp = (word*)calloc((*A)->wordlen, sizeof(word));
	if (tmp == NULL)
	{
		printf("memory allocation failure");
		return;
	}

	array_copy(tmp, (*A)->a, (*A)->wordlen);

	if (shamt < 0)
	{
		printf("\n********[ Shift amount is non-nagative ]********\n");
		return;
	}

	if (r == 0)
	{
		resize_memory(&(*A)->a, new_wordlen);
		array_init((*A)->a, new_wordlen);
		array_copy((*A)->a + k, tmp, (*A)->wordlen);
		(*A)->wordlen = new_wordlen;
	}
	else
	{
		new_wordlen++;
		resize_memory(&(*A)->a, new_wordlen);
		array_init((*A)->a, new_wordlen);

		(*A)->a[k] = tmp[0] << r;
		for (int i = 1; i < (*A)->wordlen; i++)
			(*A)->a[i + k] = (tmp[i] << r) | (tmp[i - 1] >> (w - r));

		(*A)->a[new_wordlen - 1] = tmp[(*A)->wordlen - 1] >> (w - r);
		(*A)->wordlen = new_wordlen;
	}

	bi_refine(*A);
	if (tmp != NULL)
		free(tmp);
	tmp = NULL;
}


/**
* @brief Right shift
* @details bigint A를 shamt만큼 오른쪽으로 비트 쉬프트 연산을 한다.
* @param bigint** A bigint A의 더블 포인터
* @param int shamt 쉬프트 할 비트의 크기
*/
void bi_right_shift(bigint** A, int shamt)
{

	int w = sizeof(word) * 8;
	int k = shamt / w;
	int r = shamt % w;
	int new_wordlen = (*A)->wordlen - k;

	if (shamt == 0)
		return;


	word* tmp = NULL;
	tmp = (word*)calloc((*A)->wordlen, sizeof(word));
	if (tmp == NULL)
	{
		printf("memory allocation failure");
		return;
	}
	array_copy(tmp, (*A)->a, (*A)->wordlen);

	if (shamt < 0)
	{
		printf("\n********[ Shift amount is non-nagative ]********\n");
		return;
	}

	if (shamt >= (w * (*A)->wordlen))
		bi_set_zero(A);
	else if (r == 0)
	{
		resize_memory(&(*A)->a, new_wordlen);
		array_init((*A)->a, new_wordlen);
		array_copy((*A)->a, tmp + k, new_wordlen);
		(*A)->wordlen = new_wordlen;
	}
	else
	{
		resize_memory(&(*A)->a, new_wordlen);
		array_init((*A)->a, new_wordlen);

		for (int i = 0; i < new_wordlen - 1; i++)
			(*A)->a[i] = (tmp[k + i + 1] << (w - r)) | (tmp[k + i] >> r);

		(*A)->a[new_wordlen - 1] = tmp[k + new_wordlen - 1] >> r;
		(*A)->wordlen = new_wordlen;
	}

	bi_refine(*A);
	if (tmp != NULL)
		free(tmp);
	tmp = NULL;
}

/**
* @brief x mod 2^redamt
* @details
*	x를 2^redamt로 나눈 나머지를 계산한다.
* @param bigint** x bigint x의 더블 포인터
* @param int redamt mod 할 값
*/
void bi_reduction(bigint** x, int redamt)
{
	int i = 0;

	int w = sizeof(word) * 8;
	int k = redamt / w;
	int r = redamt % w;

	if (redamt < 0)
	{
		printf("\n********[redamt>=0 ]********\n");
		return;
	}

	if (redamt >= (w * (*x)->wordlen))
		return;

	if (redamt == 0)
	{
		bi_set_zero(x);
		return;
	}

	if (r == 0)
	{
		resize_memory(&(*x)->a, k);
		(*x)->wordlen = k;
	}
	else
	{
		resize_memory(&(*x)->a, k + 1);
		(*x)->wordlen = k + 1;
		(*x)->a[k] = (*x)->a[k] & (LtR_expo(2, r) - 1);
	}
	bi_refine(*x);
}