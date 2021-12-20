#include "crypto.h"
#include "BasicOperation.h"
#include "Operation.h"
#include "utill.h"

/**
* @brief gcd(A, B) = D (using Euclidean Algorithm)
* @details big integer A�� B�� �ִ� ����� (Greatest Common Divisor, GCD)�� ����Ѵ�.
* @param bigint** D bigint D�� ����������
* @param bigint* A bigint A�� ������
* @param bigint* B bigint B�� ������
*/
void gcd(bigint** D, bigint* A, bigint* B)
{
	bigint* Q = NULL;
	bigint* R = NULL;
	bigint* RET = NULL;

	if ((A == NULL) || (B == NULL))
	{
		printf("Input error\n");
		bi_delete(&Q);
		bi_delete(&R);
		bi_delete(&RET);
		return;
	}

	if (bi_isZero(B))
	{
		bi_assign(D, A);
		bi_delete(&Q);
		bi_delete(&R);
		bi_delete(&RET);
		return;
	}
	else
	{
		DIV(&Q, &R, A, B);
		gcd(&RET, B, R);
		bi_assign(D, RET);
		bi_delete(&Q);
		bi_delete(&R);
		bi_delete(&RET);
		return;
	}
}


/**
 * @brief Find (U, V, D) s.t  A*U + B*V = D (using Extended Euclidean Algorithm)
 * @details EEA������ (U, V, D)���� ����Ѵ�. \n
 *		if D == 1, \n
 *			A*U = 1 (mod B)\n
 *			B*V = 1 (mod A)
 * @param bigint* A bigint A�� ������
 * @param bigint** U bigint U�� ����������
 * @param bigint* B bigint B�� ������
 * @param bigint** V bigint V�� ����������
 * @param bigint** D bigint D�� ����������
 */
void EEA(bigint* A, bigint** U, bigint* B, bigint** V, bigint** D)
{
	if (bi_isZero(B))
	{
		bi_assign(D, A);
		bi_set_one(U);
		bi_set_zero(V);
		return;
	}

	bigint* U0 = NULL;
	bigint* V0 = NULL;

	bigint* AmodB = NULL;
	bigint* AdivB = NULL;

	bigint* tmp = NULL;

	DIV(&AdivB, &AmodB, A, B);
	EEA(B, &U0, AmodB, &V0, D);

	bi_assign(U, V0);
	MUL(&tmp, V0, AdivB, KARATSUBA);
	SUB(V, U0, tmp);

	bi_refine(*U);
	bi_refine(*V);
	bi_refine(*D);

	bi_delete(&U0);
	bi_delete(&V0);
	bi_delete(&AmodB);
	bi_delete(&AdivB);
	bi_delete(&tmp);
}


/**
 * @brief Get l such that a = 2^l * (odd number)
 * @details Miller rabin primality test���� ���Ǵ� �Լ��̴�.\n
 *	�Է� ���� ������ 2�� �������� ��� ��� 2^l * Ȧ���� ���¸� �����ϴ� l�� ����Ѵ�.
 * @param bigint* a bigint a�� ������
 * @return l s.t a = 2^l * (odd number)
 */
int get_l(bigint* a)
{
	int l = 0;
	bigint* tmp = NULL;
	bi_assign(&tmp, a);

	if (tmp == NULL)
	{
		bi_delete(&tmp);
		return -1;
	}

	while (TRUE)
	{
		if ((tmp->a[0] & 1) == 0)
		{
			bi_right_shift(&tmp, 1);
			l++;
		}
		else
		{
			bi_delete(&tmp);
			return l;
		}
	}
}

/**
 * @brief Determine a^{2^{j}*q} is composite (where j in {0, l-1})
 * @details Miller rabin primality test���� ���Ǵ� �Լ��̴�.\n
 *	a^{2^{l-1}*q} �� �ռ������� �Ǵ��Ѵ�.
 * @param bigint* n bigint n�� ������
 * @param bigint* q bigint a�� ������
 * @param int l 2�� �������� ����� l�� int �� ����
 * @param bigint* a bigint a�� ������
 * @return True or False\n
 *		Composite�̸� TRUE, Prime�̸� FALSE
 */
int is_composite(bigint* n, bigint* q, int l, bigint* a)
{
	bigint* a_mod_n = NULL;
	bigint* tmp = NULL;
	bigint* n_1 = NULL;
	bigint* one = NULL;
	bigint* two = NULL;
	bigint* loc_a = NULL;

	bi_assign(&loc_a, a);

	// a = a^q%n
	bi_assign(&tmp, loc_a);
	LtR_MOD_EXP(&loc_a, tmp, q, n);

	// a_mod_n = a%n
	DIV(&tmp, &a_mod_n, loc_a, n);
	if (bi_isOne(a_mod_n))
	{
		bi_delete(&a_mod_n);
		bi_delete(&tmp);
		bi_delete(&n_1);
		bi_delete(&one);
		bi_delete(&two);
		bi_delete(&loc_a);
		return FALSE;
	}

	// n_1 = n - 1
	bi_set_one(&one);
	SUB(&n_1, n, one); //	n = n-1

	ADD(&two, one, one);

	for (int i = 0; i < l; i++)
	{
		DIV(&tmp, &a_mod_n, loc_a, n);
		SUB(&n_1, n, one); //	n = n-1
		if (bi_compare(a_mod_n, n_1) == 0)
		{
			bi_delete(&a_mod_n);
			bi_delete(&tmp);
			bi_delete(&n_1);
			bi_delete(&one);
			bi_delete(&two);
			bi_delete(&loc_a);
			return FALSE;
		}
		bi_assign(&tmp, loc_a);
		LtR_MOD_EXP(&loc_a, tmp, two, n);
	}

	bi_delete(&a_mod_n);
	bi_delete(&tmp);
	bi_delete(&n_1);
	bi_delete(&one);
	bi_delete(&two);
	bi_delete(&loc_a);
	return TRUE;
}

/**
 * @brief Determine big integer n is prime (using Miller-Rabin)
 * @details �־��� ���� �Ҽ����� �Ǵ��Ѵ�.
 * @param bigint* n bigint n�� �����ͷ� �Ҽ����� Ȯ���� ���
 * @param int k Miller-rabin �׽�Ʈ�� �ݺ��� Ƚ���� ���� int�� ����
 * @return True or False\n
 *		Prime �̸� TRUE, Composite�̸� FALSE
 */
int Miller_Rabin_test(bigint* n, int k)
{
	int l = 0;
	int ret = 0;
	bigint* a = NULL;
	bigint* q = NULL;
	// Constant 
	bigint* one = NULL;
	bi_set_one(&one);

	bigint* n_1 = NULL;
	SUB(&n_1, n, one);

	l = get_l(n_1);	// n-1�� ���� l ���
	bi_assign(&q, n_1);
	bi_right_shift(&q, l);
	while (k > 0)
	{
		bi_gen_rand(&a, NON_NEGATIVE, n->wordlen);	// a�� ���� �̽�..

		ret = is_composite(n, q, l, a);
		if (ret == TRUE)
		{
			bi_delete(&a);
			bi_delete(&q);
			bi_delete(&one);
			bi_delete(&n_1);
			return FALSE;
		}
		k--;
	}
	bi_delete(&a);
	bi_delete(&q);
	bi_delete(&one);
	bi_delete(&n_1);
	return TRUE;
}

/**
 * @brief Determine big integer n is prime
 * @details �־��� ���� �Ҽ����� �Ǵ��Ѵ�.
 * @param bigint* N bigint N�� �����ͷ� �Ҽ����� Ȯ���� ���
 * @param int version � ����� Primality test�� ���������� �����ϴ� int�� ����
 * @return True or False\n
 *		Prime �̸� TRUE, Composite�̸� FALSE
 */
int is_prime(bigint* N, int version)
{
	switch (version)
	{
	case NAIVE:
		return 0;
		break;
	case MILLER_RABIN:
		return Miller_Rabin_test(N, 5);
	default:
		return 0;
		break;
	}
}

/**
 * @brief Generation i-bit random big integer
 * @details �־��� ��Ʈ���̸� ���� ������ ���� �����Ѵ�.
 * @param bigint** P ������ ���� ����� bigint ���� ���������� ����
 * @param int bitLen �� ��Ʈ�� ������ ���� �������� ����� int�� ����
 */
void bi_get_i_bit_rand(bigint** P, int bitLen)
{
	int q = bitLen / WORDLEN;
	int r = bitLen % WORDLEN;

	word MASK = 0x01;
	word MSB = 1;

	if (r == 0)
	{
		MSB = MSB << (WORDLEN - 1);
	}
	else
	{
		q++;
		MSB = MSB << (r - 1);
	}

	while (TRUE)
	{
		bi_gen_rand(P, NON_NEGATIVE, q);
		if ((*P)->wordlen == q)
			break;
	}

	MASK = (MASK << (r + 1)) - 1;

	if (r != 0)
		(*P)->a[(*P)->wordlen - 1] = (*P)->a[(*P)->wordlen - 1] & MASK;

	(*P)->a[(*P)->wordlen - 1] = (*P)->a[(*P)->wordlen - 1] | MSB;
	(*P)->a[0] |= 0x01;
	bi_refine(*P);
}

/**
 * @brief Modular multiplication (C = A*B mod M)
 * @details A * B mod M�� ����Ѵ�.
 * @param bigint** C ������� ������ bigint�� ���������� ����
 * @param bigint* A �Է� A�� ��� bigint�� ���������� ����
 * @param bigint* B �Է� B�� ��� bigint�� ���������� ����
 * @param bigint* M �Է� M�� ��� bigint�� ���������� ����
 */
void MOD_MUL(bigint** C, bigint* A, bigint* B, bigint* M)
{
	bigint* tmp = NULL;
	bigint* R = NULL;
	MUL(C, A, B, KARATSUBA);
	DIV(&tmp, &R, *C, M);
	bi_assign(C, R);

	bi_delete(&tmp);
	bi_delete(&R);
}


/**
 * @brief Generation key fair for RSA
 * @details RSA���� ���� (����Ű, ����Ű)���� �����Ѵ�.
 * @param RSA_PUBKEY* pub Public key�� ���� RSA_PUBKEY�� ������ ����
 * @param RSA_PRIVKEY* priv Private key�� ���� RSA_PRIVKEY�� ������ ����
 * @param int n RSA Ű�� ��Ʈ ���̸� ���� int�� ����
 */
void RSA_gen_key(RSA_PUBKEY* pub, RSA_PRIVKEY* priv, int n)
{
	if (n < 2)
	{
		printf("n must ve greater than 2\n");
		return;
	}

	bigint* tmp = NULL;
	bigint* one = NULL;
	bigint* phi = NULL;
	bigint* p_1 = NULL;
	bigint* q_1 = NULL;
	bigint* d = NULL;

	bi_set_one(&one);

	pub->N = NULL;
	pub->e = NULL;

	priv->d = NULL;
	priv->N = NULL;
	priv->p = NULL;
	priv->q = NULL;

	ADD(&tmp, one, one);
	ADD(&pub->e, tmp, one);

	/**
	* Gen rand two prime p, q
	 */
	printf("\n\t Let's find p \t\t[");
	while (TRUE)
	{
		printf(".");
		bi_get_i_bit_rand(&priv->p, n >> 1);
		if (is_prime(priv->p, MILLER_RABIN))
			break;

	}

	printf("]\n\t Let's find q \t\t[");
	while (TRUE)
	{
		printf(".");
		bi_get_i_bit_rand(&priv->q, n >> 1);

		if (is_prime(priv->q, MILLER_RABIN))
			break;
	}

	// Calcuate : PHI
	SUB(&p_1, priv->p, one);
	SUB(&q_1, priv->q, one);
	MUL(&phi, p_1, q_1, KARATSUBA);
	MUL(&priv->N, priv->p, priv->q, KARATSUBA);
	bi_assign(&pub->N, priv->N);

	if (phi == NULL)
	{
		bi_delete(&tmp);
		bi_delete(&one);
		bi_delete(&phi);
		bi_delete(&p_1);
		bi_delete(&q_1);
		bi_delete(&d);
		return;
	}

	printf("]\n\t Let's find e, d \t[");
	while (TRUE)
	{
		// e,d ���� ����
		printf(".");
		bi_gen_rand(&pub->e, NON_NEGATIVE, phi->wordlen);
		gcd(&d, pub->e, phi);
		if (bi_isOne(d))
		{
			EEA(pub->e, &priv->d, phi, &tmp, &d);
			if (bi_isOne(d) && (bi_compare(pub->e, priv->d) != 0))
			{
				if (priv->d->sign == NEGATIVE)
				{
					bi_assign(&tmp, priv->d);
					ADD(&priv->d, tmp, phi);
				}
				break;
			}
		}
	}
	printf("]\n");
	bi_delete(&tmp);
	bi_delete(&one);
	bi_delete(&phi);
	bi_delete(&p_1);
	bi_delete(&q_1);
	bi_delete(&d);
}


/**
 * @brief RSA encryption
 * @details RSA ��ȣȭ�� �����Ѵ�.
 * @param bigint** CT ��ȣ���� ������ bigint�� ���������� ����
 * @param bigint** PT ���� ������ bigint�� ���������� ����
 * @param RSA_PUBKEY* KEY ��ȣȭ�� ���� ����Ű�� ����� RSA_PUBKEY�� ������ ����
 */
void RSA_encryption(bigint** CT, bigint* PT, RSA_PUBKEY* KEY)
{
	if (bi_compare(PT, KEY->N) != -1)
	{
		printf("The message is too large.");
	}
	LtR_MOD_EXP(CT, PT, KEY->e, KEY->N);
}
/**
 * @brief RSA decryption
 * @details RSA ��ȣȭ�� �����Ѵ�.
 * @param bigint** PT ��ȣȭ�� ���� ������ bigint�� ���������� ����
 * @param bigint** CT ��ȣȭ�� ��ȣ���� ����� bigint�� ���������� ����
 * @param RSA_PRIVKEY* KEY ��ȣȭ�� ���� ����Ű�� ����� RSA_PRIVKEY�� ������ ����
 */
void RSA_decryption(bigint** PT, bigint* CT, RSA_PRIVKEY* KEY)
{
	if (bi_compare(CT, KEY->N) != -1)
	{
		printf("The message is too large.");
	}
	LtR_MOD_EXP(PT, CT, KEY->d, KEY->N);
}