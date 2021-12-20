#include "type.h"
#include "BasicOperation.h"
#include "Operation.h"
#include "utill.h"

/**
* @brief Create Bigint
* @details ����ü �� ����ü ��� ���� ����
* @param bigint** x	������ bigint�� ���������� ����
* @param int wordlen ������ ����ü ������ ���忭 ����
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
*   ZERORIZE�� ���ǵǾ� ���� ��� ���忭�� ��� 0���� �ʱ�ȭ�ϰ� ����ü�� �޸𸮸� ��ȯ�Ѵ�.\n
*   ZERORIZE�� ���ǵǾ� ���� ���� ��� ����ü�� �޸𸮸� ��ȯ�Ѵ�.
* @param bigint** x bigint x�� ����������
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
* @details �Է� ���� ���忭�� big integer�� �����Ѵ�.
* @param bigint** x bigint x�� ����������
* @param int sign bigint�� ��ȣ
* @param word* a ���忭
* @param int wordlen ���忭�� ����
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
* @details �Է� ���� ���ڿ��� big integer�� �����Ѵ�.
* @param bigint** x bigint x�� ����������
* @param int sign bigint�� ��ȣ
* @param char* str bigint ���ڿ�
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
		if (str[i] >= 'A' && str[i] <= 'F')                // str���� 'A'(65)~'F'(70) �϶�
			str[i] = str[i] - 'A' + 11;
		else if (str[i] >= 'a' && str[i] <= 'f')                // str���� 'a'(97)~'f'(102) �϶�
			str[i] = str[i] - 'a' + 11;
		else if (str[i] >= '0' && str[i] <= '9')            // str���� '0'(48)~'9'(57) �϶�
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
*	sign�� NEGATIVE�� -�� ������ش�. \n
*	WORDLEN�� 8�̸� 2�ڸ� 16����(1����Ʈ)��, WORDLEN�� 32�� 8�ڸ� 16����(4����Ʈ)�� , WORDLEN�� 64�� 16�ڸ� 16����(8����Ʈ)�� ����Ѵ�
* @param bigint* x bigint x�� ������
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
* @details Big integer x�� fp�� �Է� ���� ���Ͽ� ����.
* @param bigint* x ���Ͽ� �Է��� Big integer �� ������
* @param FILE* fp Big integer�� �Է��� ���� ������
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
* @details ���� ���忭�� 0�� ��� �ǹ̰� ���� ������ �������ְ� ���忭 ���� �缳�����ش�
* @param bigint* x Refine�� bigint�� ������
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
* @details �Է� ���� X�� Y�� �Ҵ��Ѵ�.
* @param bigint** y �Ҵ�� ���� bigint �� ����������
* @param bigint* x �Ҵ��� ���� bigint �� ������
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
* @details �Է� ���� ũ��, ��ȣ�� ���� ������ big interger�� �����Ѵ�.
* @param bigint** x ������ bigint�� ����� ���� x�� ����������
* @param int sign ������ bigint�� ��ȣ
* @param int wordlen ������ bigint�� ���忭�� ����
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
* @details �Է� ���� big integer�� ���忭 ���̸� ��ȯ�Ѵ�.
* @param bigint* x bigint x�� ������
* @return x�� ���忭�� ����
*/
int bi_get_word_len(bigint* x)
{
	return x->wordlen;
}

/**
* @brief Get bit length of big integer
* @details �Է� ���� big integer�� ��Ʈ�� ���̸� ��ȯ�Ѵ�.
* @param bigint* x bigint x�� ������
* @return x�� �Ҵ�� bit������ �޸��� ũ��
*/
int bi_get_bit_len(bigint* x)
{
	return x->wordlen * sizeof(word) * 8;
	//return x->wordlen * WORDLEN;
}

/**
* @brief Get j th bit of big integer
* @details bigint x�� j��° ��Ʈ ���� ��ȯ�Ѵ�.
* @param bigint* x bigint x�� ������
* @param int j �о�� ��ġ
* @return bigint x�� j��° ��Ʈ ��
* @throws
*		FAILURE	j�� ������ ���
*		FAILURE	j�� ���忭 ũ�⺸�� ū ���
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

	q = (j - 1) / WORDLEN;	// �� ��° ��������

	if (q > x->wordlen)
	{
		printf("\n�̷��� ������� �Ѿ��\n");
		return FAILURE;
	}

	r = (j - 1) % WORDLEN;	// �ش� ������ ���° ��Ʈ����

	return ((x->a[q]) >> r) & 0x01;
}



/**
* @brief Get sign of big integer
* @details �Է��� bigint�� ��ȣ�� ��ȯ�Ѵ�.
* @param bigint* x �Է��� bigint x�� ������
* @return bigint x�� ��ȣ
*/
int bi_get_sign(bigint* x)
{
	return x->sign;
}

/**
* @brief Flip sign of big integer
* @details �Է��� bigint�� ��ȣ�� �ٲ۴�
* @param bigint** x �Է��� bigint x�� ����������
*/
void bi_flip_sig(bigint** x)
{
	(*x)->sign = (*x)->sign ? NON_NEGATIVE : NEGATIVE;
}

/**
* @brief Set big integer to 1
* @details �Է��� bigint x�� 1�� �����Ѵ�.
* @param bigint** x �Է��� bigint x�� ����������
*/
void bi_set_one(bigint** x)
{
	bi_new(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x1;
}

/**
* @brief Set big integer to 0
* @details �Է��� bigint x�� 0���� �����Ѵ�.
* @param bigint** x �Է��� bigint x�� ����������
*/
void bi_set_zero(bigint** x)
{
	bi_new(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x0;
}

/**
* @brief Determine big integer is 0
* @details �Է��� bigint x�� 0���� Ȯ���Ѵ�.
* @param bigint* x �Է��� bigint x�� ������
* @return TRUE or FALSE \n
*	0�̸� TRUE, �ƴϸ� FALSE
*/
int bi_isZero(bigint* x)
{

	if ((x->sign == NEGATIVE) || (x->wordlen > 1))
		return FALSE;

	return (x->a[0] == 0);
}

/**
* @brief Determine big integer is 1
* @details �Է��� bigint x�� 1���� Ȯ���Ѵ�.
* @param bigint* x �Է��� bigint x�� ������
* @return TRUE or FALSE \n
*	0�̸� TRUE, �ƴϸ� FALSE
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
*		big integer A�� B�� ũ�⸦ ���Ѵ�.\n
*		A > B : 1 \n
*		A = B : 0 \n
*		A < B : -1
* @param bigint* A �Է��� bigint A�� ������
* @param bigint* B �Է��� bigint B�� ������
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
*		big integer A�� B�� ũ�⸦ ���Ѵ�.\n
*		A > B : 1 \n
*		A = B : 0 \n
*		A < B : -1 \n
* @param bigint* A �Է��� bigint A
* @param bigint* B �Է��� bigint B
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
* @details bigint A�� shamt��ŭ �������� ��Ʈ ����Ʈ ������ �Ѵ�.
* @param bigint** A bigint A�� ���� ������
* @param int shamt ����Ʈ �� ��Ʈ�� ũ��
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
* @details bigint A�� shamt��ŭ ���������� ��Ʈ ����Ʈ ������ �Ѵ�.
* @param bigint** A bigint A�� ���� ������
* @param int shamt ����Ʈ �� ��Ʈ�� ũ��
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
*	x�� 2^redamt�� ���� �������� ����Ѵ�.
* @param bigint** x bigint x�� ���� ������
* @param int redamt mod �� ��
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