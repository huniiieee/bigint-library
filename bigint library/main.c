/**
 * @mainpage Main page
 * @author Falcon
 * @date	2021.12.12
 * @section Introduction
 * @subsection Team
 * - Team name : Falcon\n
 * - Team member\n
 *		1. Leader\n
 *         - Kim Hyeong Yeob\n
 *         - E-mail : gudduq159@koomin.ac.kr\n
 *		2. Assistant leader\n
 *		   - Lee Se Hun\n
 *         - E-mail : kyung6805@kookmin.ac.kr\n
 *      3. Intern\n
 *         - Shin Hyeok Su\n
 *         - E-mail : hyeoksu1224@kookmin.ac.kr\n
 * @subsection Feather Library
 * - <b>Purpose</b>\n
 * Big interger operation and Cryptography\n
 * - <b>Main functions in big integer operation</b>\n
 *		#1. Addition (ADD) \n
 *		#2. Subtraction (SUB) \n
 *		#3. Multiplication (MUL) \n
 *		#4. Squaring (SQU)	\n
 *		#5. Division (DIV)	\n
 *		#6. Modular Exponentiation (MODEXP) \n
 * \n
 * - <b>Main functions in cryptography</b>\n
 *
 *		#1. RSA key generation\n
 *		#2. RSA encryption\n
 *		#3. RSA decryption\n
 */

#define _CRT_SECURE_NO_WARNINGS
#include "type.h"
#include "BasicOperation.h"
#include "Operation.h"
#include "utill.h"
#include "crypto.h"
#include "test.h"

clock_t start, finish;
double duration;


int main()
{
	srand((unsigned int)time(NULL));


	//********* Test Program
	Test();

	//********* Memmory check
	//test_mem();
	return 0;
}