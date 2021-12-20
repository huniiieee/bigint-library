#ifndef _TYPE_H_
#define _TYPE_H_
/******************************************************
*					 Header file
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

/******************************************************
*						Option
*******************************************************/
//#define SAGE_CODE
#define ZERORIZE



/**
 * @brief ���ϴ� bit ���� ����\n
 * - <b>8-bit</b>\n
 *	 WORDLEN 8
 * - <b>32-bit</b>\n
 *	 WORDLEN 32
 * - <b>64-bit</b>\n
 *	 WORDLEN 64
 */
#define WORDLEN 8
 //#define WORDLEN 32		
 //#define WORDLEN 64	

 /**
  * @brief WORDELN ������ ���� word ����\n
  * - <b>WORDLEN 8</b>        \n
  *   word == unsigned char	  \n
  * \n
  * - <b>WORDLEN 32</b>		  \n
	  word == unsigned int	  \n
  * \n
  * - <b>WORDLEN 64</b>		  \n
  *   word == unsigned long long	  \n
  *
  */
#if WORDLEN == 8
typedef unsigned char	word;
#elif WORDLEN == 32
typedef unsigned int	word;
#elif WORDLEN == 64
typedef unsigned long long	word;
#endif


/******************************************************
*						Macro
*******************************************************/
#define i_bit(x, i) (((x) >> (i)) & 0x1)
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))


#define TRUE			1
#define FALSE			0

#define SUCCESS			1
#define FAILURE			0

#define NEGATIVE		1
#define NON_NEGATIVE	0

#define MAX_WORD_LEN	1024
#define CHAR			8
#define INT				32
#define LONGLONG		64


typedef unsigned char byte;

/******************************************************
*				 	Structure
*******************************************************/

/**
* @struct bigint
* @brief ū ������ ��� ���� ����ü
*/
typedef struct {
	int sign;		///< Sign : (NON_NEGATIVE, NEGATIVE) = (0, 1)
	int wordlen;	///< ���忭�� ����
	word* a;		///< ���忭�� �����ּ�
}bigint;


/**
* @struct RSA_PUBKEY
* @brief RSA ����Ű �Ķ���͸� ��� ���� ����ü
*/
typedef struct {
	bigint* N;		///< ����Ű N
	bigint* e;		///< ����Ű e
}RSA_PUBKEY;


/**
* @struct RSA_PRIVKEY
* @brief RSA ����Ű �Ķ���͸� ��� ���� ����ü
*/
typedef struct {
	bigint* N;		///< �����ǵ� �Ǵ� ����, ����� ���� ����
	bigint* p;		///< ����Ű �Ҽ� p
	bigint* q;		///< ����Ű �Ҽ� q
	bigint* d;		///< ����Ű d
}RSA_PRIVKEY;


/**
* @brief Multiplication version list
*/
enum MUL_TYPE {
	NAIVE = 0x01,
	KARATSUBA
};

/**
* @brief Division version list
*/
enum DIV_TYPE {
	MPL_TYPE = 0x02,
	BLD_TYPE = 0x03
};

/**
* @brief Modular exponentiation version list
*/
enum MOD_EXPO_TYPE {
	LtR = 0x01,
	RtL,
	MONT
};

/**
* @brief Primality test version list
*/
enum PRIMALITY_TYPE {
	MILLER_RABIN = 0x02,
};


/**
* @brief Test mode version list
*/
enum MODE_TYPE {
	CALCULATION = 0x31,
	CRYPTOGRAPHY
};

/**
* @brief Cryptography algorithm version list
*/
enum CRYPTO_TYPE {
	RSA = 0x31
};


/**
* @brief Biginteger operation version list
*/
enum OPERATION_TYPE {
	ADDITION = 0x31,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	MODULAR_EXPONENTIATION
};

enum TEST_TYPE {
	RANDOM_NUMBER = 0x31,
	USER_INPUT,
};
#endif