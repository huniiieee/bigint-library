#define _CRT_SECURE_NO_WARNINGS
#include "test.h"
#include "BasicOperation.h"
#include "Operation.h"
#include "type.h"
#include "utill.h"
#include "crypto.h"



/**
 * @brief Test rsa
 * @details RSA 키 생성, 암호호, 복호화 테스트를 수행한다.\n
 * RSA 키 길이, 테스트 횟수를 입력 받아서 sage형식의 테스트 코드를 보여준다.
 */
void test_rsa()
{
	RSA_PUBKEY PUB;
	RSA_PRIVKEY PRIV;
	bigint* PT = NULL;
	bigint* CT = NULL;
	bigint* RE_PT = NULL;

	bigint* ED = NULL;
	bigint* p = NULL;
	bigint* phi = NULL;
	bigint* p_1 = NULL;
	bigint* q_1 = NULL;
	bigint* one = NULL;
	bigint* tmp = NULL;
	bi_set_one(&one);
	int ver = 0;
	int n = 0;
	int numOftest = 0;
	/**
	* Gen key test
	*/

	while (TRUE)
	{
		printf("\n\t* Recommended key length : { 1024, 2048, 3072, 4096 }\n\n");
		printf("\t  Valid input range (0, 4096]\n\n");
		printf("\t  Input key length : ");
		n = scanf(" %d", &ver);
		if ((ver > 4096) || (ver <= 0))
		{
			printf("\n\t Invalid input, please try again.\n\n");
			printf("\t-------------------------------------------------------\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	printf("\t-------------------------------------------------------\n");
	printf("\t-------------------------------------------------------\n");

	while (getchar() != '\n');

	while (TRUE)
	{
		printf("\n\t* Recommended the number of test : { 1024, 2048, 3072, 4096 }\n\n");
		printf("\t* Valid input range (0, 1000]\n\n");
		printf("\t  Input number of test : ");
		n = scanf(" %d", &numOftest);
		if ((numOftest <= 0) || (numOftest > 1000))
		{
			printf("\n\t Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');

	printf("\t-------------------------------------------------------\n");
	printf("\t-------------------------------------------------------\n");

	printf("\n\n\n");

	printf("\n\t|-------------------------------------------------------|\n");
	printf("\t|                       RSA %-4d                        |\n", ver);
	printf("\t|-------------------------------------------------------|\n\n");

	printf("\t\t\t[STEP 1] Key generation\n\n");
	RSA_gen_key(&PUB, &PRIV, ver);	// Error 발생시 종료 하도록 처리

	SUB(&p_1, PRIV.p, one);
	SUB(&q_1, PRIV.q, one);
	MUL(&phi, p_1, q_1, KARATSUBA);


	MOD_MUL(&ED, PUB.e, PRIV.d, phi);
	printf("\n\n\t\t\t[STEP 2] Key validation\n");

	printf("\t\t   ----------------------------------\n");
	printf("\t\t  |       ed = 1 (mod phi(N))        |\n");
	printf("\t\t   ----------------------------------\n\n");
	printf("\t e = "); bi_show(PUB.e);
	printf("\t d = "); bi_show(PRIV.d);
	printf("\t N = "); bi_show(PRIV.N);
	printf("\t ed = "); bi_show(ED);
	printf("\n\n\n");
	/**
	* Enc, Dec test
	*/
	printf("\t\t\t[Step 3] Encryption & Decryption\n\n");
	for (int i = 0; i < numOftest; i++)
	{
		bi_gen_rand(&PT, NON_NEGATIVE, PUB.N->wordlen);
		if (bi_compare(PUB.N, PT) == -1)
		{
			DIV(&one, &tmp, PT, PUB.N);
			bi_assign(&PT, tmp);
		}

		printf("\tPT = "); bi_show(PT);
		RSA_encryption(&CT, PT, &PUB);
		printf("\tCT = "); bi_show(CT);

		RSA_decryption(&RE_PT, CT, &PRIV);
		printf("\tRecovered_PT = "); bi_show(RE_PT);

		printf("\tprint(pow(PT, e, N) == CT)\n");
		printf("\tprint(pow(CT, d, N) == Recovered_PT)\n");
		printf("\n\n");
	}
	printf("\t* Copy and paste the results into the link below.\n");
	printf("\t https://sagecell.sagemath.org/\n");
	printf("\n\n\t* Exit RSA test\n");
	n = getchar();
	printf("--------------------------------------------------------------------------------");
	printf("\n\n\n");
}


/**
 * @brief Test ADD
 * @details ADD 함수의 테스트를 수행한다.\n
 * 값을 어떻게 받을지, 테스트 수행 횟수를 입력 받아서 sage형식의 테스트 코트를 출력한다.\n
 * 사용자 입력의 경우 sign은 0 or 1, 워드열은 hex로 입력해야한다.\n
 */
void test_add()
{
	int numOftest = 0;
	int n = 0;
	unsigned char input = 0;

	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;

	char a[MAX_WORD_LEN] = { 0, };
	char b[MAX_WORD_LEN] = { 0, };
	// test 1 : 같은 길이  :  O
	// test 2 : 다른 길이
	int wLen1 = 0;
	int wLen2 = 0;
	int i = 0;
	int sign1 = 0;
	int sign2 = 0;



	printf("\n\t|-------------------------------------------------------|\n");
	printf("\t|                       Addition                        |\n");
	printf("\t|-------------------------------------------------------|\n\n");



	// 입력을 랜덤하게 받을지, 직접 입력할지 선택
	while (TRUE)
	{
		printf("\t* Choose a way to enter the value.\n\n");
		printf("\t  [1] Random number\n");
		printf("\t  [2] User input\n");
		printf(">>>  ");
		n = scanf(" %c", &input);
		if ((input != RANDOM_NUMBER) && (input != USER_INPUT))
		{
			printf("\n\t  Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");


	// 몇 번 테스트 할지 선택
	while (TRUE)
	{
		printf("\t* Valid input range (0, 1000]\n\n");
		printf("\t  Number of test : ");
		n = scanf(" %d", &numOftest);
		if ((numOftest <= 0) || (numOftest > 1000))
		{
			printf("\n\t Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");



	for (i = 0; i < numOftest; i++)
	{
		if (input == RANDOM_NUMBER)
		{
			wLen1 = rand() % 64 + 1;
			wLen2 = rand() % 64 + 1;

			sign1 = rand() % 2;
			sign2 = rand() % 2;
			bi_gen_rand(&A, sign1, wLen1);
			bi_gen_rand(&B, sign2, wLen2);
			printf("\n\n\n");

		}
		else
		{

			printf("\t\t   ----------------------------------\n");
			printf("\t\t  |          Set sign of A           |\n");
			printf("\t\t   ----------------------------------\n");
			// A의 부호 선택
			while (TRUE)
			{
				printf("\t* Valid input (0 or 1)]\n\n");
				printf("\t\t0 -> Non-negative\n");
				printf("\t\t1 -> Negative\n");
				printf("\n\t  Sign of A : ");
				n = scanf(" %d", &sign1);
				if ((sign1 != 0) && (sign1 != 1))
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}

			while (getchar() != '\n');
			printf("\n\n\n\n\n");

			printf("\t\t   ----------------------------------\n");
			printf("\t\t  |          Set sign of B           |\n");
			printf("\t\t   ----------------------------------\n");

			// B의 부호 선택
			while (TRUE)
			{
				printf("\t* Valid input (0 or 1)]\n\n");
				printf("\t\t0 -> Non-negative\n");
				printf("\t\t1 -> Negative\n");
				printf("\n\t  Sign of B : ");
				n = scanf(" %d", &sign2);
				if ((sign2 != 0) && (sign2 != 1))
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}

			while (getchar() != '\n');
			printf("\n\n\n");

			// A의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input A : 0x");
				n = scanf(" %s", a);
				printf("%c", sign1);
				printf("%d", sign1);
				n = bi_set_by_string_hex(&A, sign1, a);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");

			// B의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input B : 0x");
				n = scanf(" %s", b);
				n = bi_set_by_string_hex(&B, sign2, b);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");
		}

		printf("\tA = "); bi_show(A);
		printf("\tB = "); bi_show(B);
		ADD(&C, A, B);
		printf("\tC = "); bi_show(C);
		printf("\tprint((A + B) == C)\n");
	}
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);

	printf("\t* Copy and paste the results into the link below.\n");
	printf("\t https://sagecell.sagemath.org/\n");
	printf("\n\n\t----------- Exit Addition test -----------\n");

	n = getchar();
	printf("--------------------------------------------------------------------------------");
	printf("\n\n\n");
}

/**
 * @brief Test SUB
 * @details SUB 함수의 테스트를 수행한다.\n
 * 값을 어떻게 받을지, 테스트 수행 횟수를 입력 받아서 sage형식의 테스트 코트를 출력한다.\n
 * 사용자 입력의 경우 sign은 0 or 1, 워드열은 hex로 입력해야한다.\n
 */
void test_sub()
{
	int numOftest = 0;
	int n = 0;
	unsigned char input = 0;

	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;

	char a[MAX_WORD_LEN] = { 0, };
	char b[MAX_WORD_LEN] = { 0, };

	int wLen1 = 0;
	int wLen2 = 0;
	int i = 0;
	int sign1 = 0;
	int sign2 = 0;

	printf("\n\t|-------------------------------------------------------|\n");
	printf("\t|                      Subtraction                      |\n");
	printf("\t|-------------------------------------------------------|\n\n");

	// 입력을 랜덤하게 받을지, 직접 입력할지 선택
	while (TRUE)
	{
		printf("\t* Choose a way to enter the value.\n\n");
		printf("\t  [1] Random number\n");
		printf("\t  [2] User input\n");
		printf(">>>  ");
		n = scanf(" %c", &input);
		if ((input != RANDOM_NUMBER) && (input != USER_INPUT))
		{
			printf("\n\t  Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");


	while (TRUE)
	{
		printf("\t* Valid input range (0, 1000]\n\n");
		printf("\t  Number of test : ");
		n = scanf(" %d", &numOftest);
		if ((numOftest <= 0) || (numOftest > 1000))
		{
			printf("\n\t Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');



	for (i = 0; i < numOftest; i++)
	{
		if (input == RANDOM_NUMBER)
		{
			wLen1 = rand() % 64 + 1;
			wLen2 = rand() % 64 + 1;

			sign1 = rand() % 2;
			sign2 = rand() % 2;
			bi_gen_rand(&A, sign1, wLen1);
			bi_gen_rand(&B, sign2, wLen2);
			printf("\n\n\n");

		}
		else
		{

			printf("\t\t   ----------------------------------\n");
			printf("\t\t  |          Set sign of A           |\n");
			printf("\t\t   ----------------------------------\n");


			// A의 부호 선택
			while (TRUE)
			{
				printf("\t* Valid input (0 or 1)]\n\n");
				printf("\t\t0 -> Non-negative\n");
				printf("\t\t1 -> Negative\n");
				printf("\n\t  Sign of A : ");
				n = scanf(" %d", &sign1);
				if ((sign1 != 0) && (sign1 != 1))
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n\n\n");

			printf("\t\t   ----------------------------------\n");
			printf("\t\t  |          Set sign of B           |\n");
			printf("\t\t   ----------------------------------\n");

			// B의 부호 선택
			while (TRUE)
			{
				printf("\t* Valid input (0 or 1)]\n\n");
				printf("\t\t0 -> Non-negative\n");
				printf("\t\t1 -> Negative\n");
				printf("\n\t  Sign of B : ");
				n = scanf(" %d", &sign2);
				if ((sign2 != 0) && (sign2 != 1))
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");

			// A의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input A : 0x");
				n = scanf(" %s", a);
				n = bi_set_by_string_hex(&A, sign1, a);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");

			// B의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input B : 0x");
				n = scanf(" %s", b);
				n = bi_set_by_string_hex(&B, sign2, b);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");
		}

		printf("\tA = "); bi_show(A);
		printf("\tB = "); bi_show(B);
		SUB(&C, A, B);
		printf("\tC = "); bi_show(C);
		printf("\tprint((A - B) == C)\n");
	}

	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);

	printf("\t* Copy and paste the results into the link below.\n");
	printf("\t https://sagecell.sagemath.org/\n");
	printf("\n\n\t* Exit Subtraction test\n");
	n = getchar();
	printf("--------------------------------------------------------------------------------");
	printf("\n\n\n");
}


/**
 * @brief Test MUL
 * @details MUL 함수의 테스트를 수행한다.\n
 * 값을 어떻게 받을지, 어떤 버전의 곱셈을 수행할지, 테스트 수행 횟수를 입력 받아서 sage형식의 테스트 코트를 출력한다.\n
 * 사용자 입력의 경우 sign은 0 or 1, 워드열은 hex로 입력해야한다.\n
 */
void test_mul()
{
	int numOftest = 0;
	int n = 0;
	unsigned char input = 0;
	int type = 0;

	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;

	char a[MAX_WORD_LEN] = { 0, };
	char b[MAX_WORD_LEN] = { 0, };

	int wLen1 = 0;
	int wLen2 = 0;
	int i = 0;
	int sign1 = 0;
	int sign2 = 0;

	printf("\n\t|-------------------------------------------------------|\n");
	printf("\t|                     Multiplication                    |\n");
	printf("\t|-------------------------------------------------------|\n\n");

	// 입력을 랜덤하게 받을지, 직접 입력할지 선택
	while (TRUE)
	{
		printf("\t* Choose a way to enter the value.\n\n");
		printf("\t  [1] Random number\n");
		printf("\t  [2] User input\n");
		printf(">>>  ");
		n = scanf(" %c", &input);
		if ((input != RANDOM_NUMBER) && (input != USER_INPUT))
		{
			printf("\n\t  Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");

	// 입력을 랜덤하게 받을지, 직접 입력할지 선택
	while (TRUE)
	{
		printf("\t* Choose a version of multiplication.\n\n");
		printf("\t  [1] Naive \n");
		printf("\t  [2] Karatsuba\n");
		printf(">>>  ");
		n = scanf(" %d", &type);

		if ((type != NAIVE) && (type != KARATSUBA))
		{
			printf("\n\t  Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");

	while (TRUE)
	{
		printf("\t* Valid input range (0, 1000]\n\n");
		printf("\t  Number of test : ");
		n = scanf(" %d", &numOftest);
		if ((numOftest <= 0) || (numOftest > 1000))
		{
			printf("\n\t Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');


	for (i = 0; i < numOftest; i++)
	{
		if (input == RANDOM_NUMBER)
		{
			wLen1 = rand() % 64 + 1;
			wLen2 = rand() % 64 + 1;

			sign1 = rand() % 2;
			sign2 = rand() % 2;
			bi_gen_rand(&A, sign1, wLen1);
			bi_gen_rand(&B, sign2, wLen2);
			printf("\n\n\n");

		}
		else
		{

			printf("\t\t   ----------------------------------\n");
			printf("\t\t  |          Set sign of A           |\n");
			printf("\t\t   ----------------------------------\n");


			// A의 부호 선택
			while (TRUE)
			{
				printf("\t* Valid input (0 or 1)]\n\n");
				printf("\t\t0 -> Non-negative\n");
				printf("\t\t1 -> Negative\n");
				printf("\n\t  Sign of A : ");
				n = scanf(" %d", &sign1);
				if ((sign1 != 0) && (sign1 != 1))
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n\n\n");

			printf("\t\t   ----------------------------------\n");
			printf("\t\t  |          Set sign of B           |\n");
			printf("\t\t   ----------------------------------\n");

			// B의 부호 선택
			while (TRUE)
			{
				printf("\t* Valid input (0 or 1)]\n\n");
				printf("\t\t0 -> Non-negative\n");
				printf("\t\t1 -> Negative\n");
				printf("\n\t  Sign of B : ");
				n = scanf(" %d", &sign2);
				if ((sign1 != 0) && (sign1 != 1))
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}

			while (getchar() != '\n');
			printf("\n\n\n");

			// A의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input A : 0x");
				n = scanf(" %s", a);
				n = bi_set_by_string_hex(&A, sign1, a);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");

			// B의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input B : 0x");
				n = scanf(" %s", b);
				n = bi_set_by_string_hex(&B, sign2, b);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");
		}


		printf("\tA = "); bi_show(A);
		printf("\tB = "); bi_show(B);

		if (type == NAIVE)
			MUL(&C, A, B, NAIVE);
		else
			MUL(&C, A, B, KARATSUBA);

		printf("\tC = "); bi_show(C);
		printf("\tprint((A * B) == C)\n");
	}
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);

	printf("\t* Copy and paste the results into the link below.\n");
	printf("\t https://sagecell.sagemath.org/\n");
	printf("\n\n\t* Exit Multiplication test\n");
	n = getchar();
	printf("--------------------------------------------------------------------------------");
	printf("\n\n\n");
}




/**
 * @brief Test DIV
 * @details DIV 함수의 테스트를 수행한다.\n
 * 값을 어떻게 받을지, 어떤 버전의 나눗셈을 수행할지, 테스트 수행 횟수를 입력 받아서 sage형식의 테스트 코트를 출력한다.\n
 * 워드열은 hex로 입력해야한다.\n
 */
void test_div()
{
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* Q = NULL;
	bigint* R = NULL;


	int numOftest = 0;
	int n = 0;
	unsigned char input = 0;
	int type = 0;

	char a[MAX_WORD_LEN] = { 0, };
	char b[MAX_WORD_LEN] = { 0, };

	int wLen1 = 0;
	int wLen2 = 0;
	int i = 0;
	int sign1 = 0;
	int sign2 = 0;

	printf("\n\t|-------------------------------------------------------|\n");
	printf("\t|                        Division                       |\n");
	printf("\t|-------------------------------------------------------|\n\n");

	while (TRUE)
	{
		printf("\t* Choose a way to enter the value.\n\n");
		printf("\t  [1] Random number\n");
		printf("\t  [2] User input\n");
		printf(">>>  ");
		n = scanf(" %c", &input);
		if ((input != RANDOM_NUMBER) && (input != USER_INPUT))
		{
			printf("\n\t  Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");

	// 입력을 랜덤하게 받을지, 직접 입력할지 선택
	while (TRUE)
	{
		printf("\t* Choose a version of division.\n\n");
		printf("\t  [1] Naive \n");
		printf("\t  [2] Multi-precision long division\n");
		printf("\t  [3] Binary long division\n");
		printf(">>>  ");
		n = scanf(" %d", &type);

		if ((type != NAIVE) && (type != MPL_TYPE) && (type != BLD_TYPE))
		{
			printf("\n\t  Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");


	while (TRUE)
	{
		printf("\t* Valid input range (0, 1000]\n\n");
		printf("\t  Number of test : ");
		n = scanf(" %d", &numOftest);
		if ((numOftest <= 0) || (numOftest > 1000))
		{
			printf("\n\t Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');



	for (i = 0; i < numOftest; i++)
	{
		if (input == RANDOM_NUMBER)
		{
			wLen1 = rand() % 64 + 1;
			wLen2 = rand() % 64 + 1;

			sign1 = rand() % 2;
			sign2 = rand() % 2;
			bi_gen_rand(&A, sign1, wLen1);
			bi_gen_rand(&B, sign2, wLen2);
			printf("\n\n\n");

		}
		else
		{
			// A의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input A : 0x");
				n = scanf(" %s", a);
				n = bi_set_by_string_hex(&A, sign1, a);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");

			// B의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input B : 0x");
				n = scanf(" %s", b);
				n = bi_set_by_string_hex(&B, sign2, b);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");
		}


		printf("\tA = "); bi_show(A);
		printf("\tB = "); bi_show(B);

		if (type == NAIVE)
			DIV_NAIVE(&Q, &R, A, B);
		else if (type == MPL_TYPE)
			DIV(&Q, &R, A, B);
		else
			LDA(&Q, &R, A, B);

		printf("\tQ = "); bi_show(Q);
		printf("\tR = "); bi_show(R);

		printf("\tprint((A // B) == Q)");
		printf("\tprint((A %% B) == R)");
		printf("\n\n");
	}


	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&Q);
	bi_delete(&R);

	printf("\t* Copy and paste the results into the link below.\n");
	printf("\t https://sagecell.sagemath.org/\n");
	printf("\n\n\t* Exit Division test\n");
	n = getchar();
	printf("--------------------------------------------------------------------------------");
	printf("\n\n\n");
}





/**
 * @brief Test MDOEXP
 * @details MDOEXP 함수의 테스트를 수행한다.\n
 * 값을 어떻게 받을지, 어떤 버전의 모듈러 지수승을 수행할지, 테스트 수행 횟수를 입력 받아서 sage형식의 테스트 코트를 출력한다.\n
 * 워드열은 hex로 입력해야한다.\n
 */
void test_modexp()
{
	int numOftest = 0;
	int n = 0;
	unsigned char input = 0;
	int type = 0;

	char a[MAX_WORD_LEN] = { 0, };
	char b[MAX_WORD_LEN] = { 0, };
	char c[MAX_WORD_LEN] = { 0, };

	int i = 0;

	bigint* A = NULL;
	bigint* N = NULL;
	bigint* M = NULL;
	bigint* C = NULL;

	int wLen1 = 0;
	int wLen2 = 0;
	int wLen3 = 0;

	printf("\n\t|-------------------------------------------------------|\n");
	printf("\t|                 Modular exponentiation                |\n");
	printf("\t|-------------------------------------------------------|\n\n");


	// 입력을 랜덤하게 받을지, 직접 입력할지 선택
	while (TRUE)
	{
		printf("\t* Choose a way to enter the value.\n\n");
		printf("\t  [1] Random number\n");
		printf("\t  [2] User input\n");
		printf(">>>  ");
		n = scanf(" %c", &input);
		if ((input != RANDOM_NUMBER) && (input != USER_INPUT))
		{
			printf("\n\t  Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");


	// 입력을 랜덤하게 받을지, 직접 입력할지 선택
	while (TRUE)
	{
		printf("\t* Choose a version of multiplication.\n\n");
		printf("\t  [1] Left to Right \n");
		printf("\t  [2] Right to Left\n");
		printf("\t  [3] Montgomery ladder\n");
		printf(">>>  ");
		n = scanf(" %d", &type);

		if ((type != LtR) && (type != RtL) && (type != MONT))
		{
			printf("\n\t  Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");


	// 몇 번 테스트 할지 선택
	while (TRUE)
	{
		printf("\t* Valid input range (0, 100]\n\n");
		printf("\t  Number of test : ");
		n = scanf(" %d", &numOftest);
		if ((numOftest <= 0) || (numOftest > 100))
		{
			printf("\n\t Invalid input, please try again.\n\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}
	while (getchar() != '\n');
	printf("\n\n\n");


	for (i = 0; i < numOftest; i++)
	{
		if (input == RANDOM_NUMBER)
		{
			wLen1 = rand() % 64 + 1;
			wLen2 = rand() % 32 + 1;
			wLen3 = rand() % 20 + 1;
			bi_gen_rand(&A, NON_NEGATIVE, wLen1);
			bi_gen_rand(&N, NON_NEGATIVE, wLen2);
			bi_gen_rand(&M, NON_NEGATIVE, wLen2);
			N->a[0] |= 1;
			M->a[0] |= 1;

			printf("\n\n\n");
		}
		else
		{
			// A의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input A : 0x");
				n = scanf(" %s", a);
				n = bi_set_by_string_hex(&A, NON_NEGATIVE, a);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");


			// N의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input N : 0x");
				n = scanf(" %s", a);
				n = bi_set_by_string_hex(&N, NON_NEGATIVE, a);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");

			// M의 워드열 입력
			while (TRUE)
			{
				printf("\t *Input M : 0x");
				n = scanf(" %s", a);
				n = bi_set_by_string_hex(&M, NON_NEGATIVE, a);
				if (n == FAILURE)
				{
					printf("\n\t Invalid input, please try again.\n\n");
					while (getchar() != '\n');
					continue;
				}
				break;
			}
			while (getchar() != '\n');
			printf("\n\n\n");
		}


		printf("\tA = "); bi_show(A);
		printf("\tM = "); bi_show(M);
		printf("\tn = "); bi_show(N);

		if (type == LtR)
			LtR_MOD_EXP(&C, A, N, M);
		else if (type == RtL)
			RtL_MOD_EXP(&C, A, N, M);
		else
			MONT_MOD_EXP(&C, A, N, M);

		printf("\tC = "); bi_show(C);
		printf("\tprint(pow(A, n, M) == C)");
		printf("\n\n");
	}

	bi_delete(&A);
	bi_delete(&M);
	bi_delete(&N);
	bi_delete(&C);

	printf("\t* Copy and paste the results into the link below.\n");
	printf("\t https://sagecell.sagemath.org/\n");
	printf("\n\n\t* Exit Modular exponentiation test\n");
	n = getchar();
	printf("--------------------------------------------------------------------------------");
	printf("\n\n\n");
}

/*****************************************************************************************
*
******************************************************************************************/


/**
 * @brief Test Feather library
 * @details Feather 라이브러리에서 제공하는 함수를 테스트한다.\n
 * 계산기와 암호 모드를 선택하고, 각 모드에서 제공하는 함수를 선택하여 테스트를 진행한다.\n
 */
void Test()
{
	int flag = 0;
	unsigned char test_num = 0;
	int tmp = 0;
	printf("\n    *****************************************************************\n");
	printf("   *     _____          _   _                 _            _         *\n");
	printf("   *    |  ___|__  __ _| |_| |__   ___ _ __  | |_ ___  ___| |_       *\n");
	printf("   *    | |_ / _ \\/ _` | __| '_ \\ / _ \\ '__| | __/ _ \\/ __| __|      *\n");
	printf("   *    |  _|  __/ (_| | |_| | | |  __/ |    | ||  __/\\__ \\ |_       *\n");
	printf("   *    |_|  \\___|\\__,_|\\__|_| |_|\\___|_|     \\__\\___||___/\\__|      *\n");
	printf("   *                                                                 *\n");
	printf("   *                                ____ ____ _    ____ ____ __ _    *\n");
	printf("   *                              - |--- |--| |___ |___ [__] | \\|    *\n");
	printf("    *****************************************************************\n");
	printf("\n\n\n");
	while (TRUE)
	{
		printf("\n  |-----------------------------------------------------------------|\n");
		printf("  |              Big number library _ Feather                       |\n");
		printf("  | Choose the mode you want. (if q or Q -> exit)                   |\n");
		printf("  |                                                                 |\n");
		printf("  | [1] Calculation                                                 |\n");
		printf("  | [2] Cryptography                                                |\n");
		printf("  |-----------------------------------------------------------------|\n");
		printf(">>>  ");


		tmp = scanf(" %c", &test_num);

		while (getchar() != '\n')
			flag = 1;


		if ((test_num == 'q') || (test_num == 'Q'))
		{
			printf("Exit big number library feather test\n");
			printf("--------------------------------------------------------------------------------");
			printf("\n\n\n");
			break;
		}


		switch (test_num)
		{
		case CALCULATION:
			Test_calculation();
			break;
		case CRYPTOGRAPHY:
			Test_cryptography();
			break;
		default:
			printf(" > Enter 1 or 2!\n");
			break;
		}
		printf("\n");
	}

}


/**
 * @brief Test cryptography
 * @details Feather 라이브러리에서 제공하는 Cryptography 함수를 테스트한다.\n
 * cryptography 연산들 중에서 정의된 알고리즘을 선택한다.\n
 */
void Test_cryptography()
{
	unsigned char test_num = 0;
	int tmp = 0;
	int flag = 0;

	while (TRUE)
	{
		printf("\n\t|-------------------------------------------------------|\n");
		printf("\t|                   Cryptograpy test.                   |\n");
		printf("\t| Choose the algorithm you want. (if q or Q -> exit)    |\n");
		printf("\t| [1] RSA (Textbook)                                    |\n");
		printf("\t|-------------------------------------------------------|\n");
		printf(">>>  ");

		tmp = scanf(" %c", &test_num);

		while (getchar() != '\n')
			flag = 1;

		if ((test_num == 'q') || (test_num == 'Q'))
		{
			printf("\t* Exit cryptography test\n");
			printf("--------------------------------------------------------------------------------");
			printf("\n\n\n");
			break;
		}
		if (flag == 0)
		{
			switch (test_num)
			{
			case CALCULATION:
				test_rsa();
				break;
			default:
				printf("Invalid input. Enter a number 1.\n");
				break;
			}
		}
		if (flag != 0)
			printf("Invalid input. Enter a number 1.\n");
		flag = 0;
		printf("\n");
	}
}

/**
 * @brief Test calculation
 * @details Feather 라이브러리에서 제공하는 calculation 함수를 테스트한다.\n
 * calculation 연산들 중에서 정의된 연산을 선택한다.\n
 */
void Test_calculation()
{
	unsigned char test_num = 0;
	int tmp = 0;
	int flag = 0;
	while (TRUE)
	{
		printf("\n\t|-------------------------------------------------------|\n");
		printf("\t|                   Calculation test.                   |\n");
		printf("\t| Choose the operation you want. (if q or Q -> exit)    |\n");
		printf("\t|                                                       |\n");
		printf("\t| [1] Addition                                          |\n");
		printf("\t| [2] Subtraction                                       |\n");
		printf("\t| [3] Multiplication                                    |\n");
		printf("\t| [4] Division                                          |\n");
		printf("\t| [5] Modular exponentiation                            |\n");
		printf("\t|-------------------------------------------------------|\n");
		printf(">>>  ");


		tmp = scanf(" %c", &test_num);

		while (getchar() != '\n')
			flag = 1;

		if ((test_num == 'q') || (test_num == 'Q'))
		{
			printf("\t* Exit calculation test\n");
			printf("--------------------------------------------------------------------------------");
			printf("\n\n\n");
			break;
		}

		if (flag == 0)
		{
			switch (test_num)
			{
			case ADDITION:
				test_add();
				break;
			case SUBTRACTION:
				test_sub();
				break;
			case MULTIPLICATION:
				test_mul();
				break;
			case DIVISION:
				test_div();
				break;
			case MODULAR_EXPONENTIATION:
				test_modexp();
				break;

			default:
				printf("Invalid input. Enter a number from 0 to 5.\n");
				break;
			}
		}

		if (flag != 0)
			printf("Invalid input. Enter a number from 0 to 5.\n");
		flag = 0;
		printf("\n");
	}
}

/******************************************************************************************/
/**
 * @brief Check memory leak
 * @details Memory leak을 확인하기 위해, 구현된 main function들을 모두 호출한다.\n
 */
void test_mem()
{
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;

	bigint* Q = NULL;
	bigint* R = NULL;

	bigint* N = NULL;
	bigint* M = NULL;

	bigint* one = NULL;
	bigint* tmp = NULL;
	bi_set_one(&one);

	RSA_PUBKEY PUB;
	RSA_PRIVKEY PRIV;

	int sign1 = 0;
	int sign2 = 0;
	int sign3 = 0;

	int wLen1 = 0;
	int wLen2 = 0;
	int wLen3 = 0;

	int testLoop = 1;
	int n = 64;
	int i = 0;
	// test 1 = 8-bit
	// add 
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 128 + 1;
		wLen2 = rand() % 128 + 1;

		sign1 = rand() % 2;
		sign2 = rand() % 2;

		bi_gen_rand(&A, sign1, wLen1);
		bi_gen_rand(&B, sign2, wLen2);
		ADD(&C, A, B);
	}
	printf("End test ADD\n");


	// sub
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 128 + 1;
		wLen2 = rand() % 128 + 1;

		sign1 = rand() % 2;
		sign2 = rand() % 2;

		bi_gen_rand(&A, sign1, wLen1);
		bi_gen_rand(&B, sign2, wLen2);
		SUB(&C, A, B);
	}
	printf("End test SUB\n");


	// mul - naive
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 128 + 1;
		wLen2 = rand() % 128 + 1;

		sign1 = rand() % 2;
		sign2 = rand() % 2;

		bi_gen_rand(&A, sign1, wLen1);
		bi_gen_rand(&B, sign2, wLen2);
		MUL(&C, A, B, NAIVE);
	}
	printf("End test MUL-NAIVE\n");


	// mul - kara
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 128 + 1;
		wLen2 = rand() % 128 + 1;

		sign1 = rand() % 2;
		sign2 = rand() % 2;

		bi_gen_rand(&A, sign1, wLen1);
		bi_gen_rand(&B, sign2, wLen2);
		MUL(&C, A, B, KARATSUBA);
	}
	printf("End test MUL-KARATSUBA\n");


	// squ - naive
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 128 + 1;

		sign1 = rand() % 2;

		bi_gen_rand(&A, sign1, wLen1);
		SQU(&C, A, NAIVE);
	}
	printf("End test SQU-NAIVE\n");


	// squ - kara
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 128 + 1;

		sign1 = rand() % 2;

		bi_gen_rand(&A, sign1, wLen1);

		SQU(&C, A, KARATSUBA);
	}
	printf("End test SQU-KARATUSBA\n");


	// div - naive
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = 128;
		wLen2 = 128;

		bi_gen_rand(&A, NON_NEGATIVE, wLen1);
		bi_gen_rand(&B, NON_NEGATIVE, wLen2);

		DIV_NAIVE(&Q, &R, A, B);
	}
	printf("End test DIV-NAIVE\n");


	// div - multi
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 128 + 1;
		wLen2 = rand() % 128 + 1;

		bi_gen_rand(&A, NON_NEGATIVE, wLen1);
		bi_gen_rand(&B, NON_NEGATIVE, wLen2);

		DIV(&Q, &R, A, B);
	}
	printf("End test DIV-MULTI\n");

	// div - bin
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 128 + 1;
		wLen2 = rand() % 128 + 1;

		bi_gen_rand(&A, NON_NEGATIVE, wLen1);
		bi_gen_rand(&B, NON_NEGATIVE, wLen2);

		LDA(&Q, &R, A, B);
	}
	printf("End test DIV-BINARY\n");


	// LtR
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 64 + 1;
		wLen2 = rand() % 64 + 1;
		wLen3 = rand() % 32 + 1;

		bi_gen_rand(&A, NON_NEGATIVE, wLen1);
		bi_gen_rand(&N, NON_NEGATIVE, wLen2);
		bi_gen_rand(&M, NON_NEGATIVE, wLen3);

		LtR_MOD_EXP(&C, A, N, M);
	}
	printf("End test LtR\n");

	// RtL
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 64 + 1;
		wLen2 = rand() % 64 + 1;
		wLen3 = rand() % 32 + 1;


		bi_gen_rand(&A, NON_NEGATIVE, wLen1);
		bi_gen_rand(&N, NON_NEGATIVE, wLen2);

		bi_gen_rand(&M, NON_NEGATIVE, wLen3);

		RtL_MOD_EXP(&C, A, N, M);
	}
	printf("End test RtL\n");


	// Mont
	for (i = 0; i < testLoop; i++)
	{
		wLen1 = rand() % 64 + 1;
		wLen2 = rand() % 64 + 1;
		wLen3 = rand() % 32 + 1;

		bi_gen_rand(&A, NON_NEGATIVE, wLen1);
		bi_gen_rand(&N, NON_NEGATIVE, wLen2);
		bi_gen_rand(&M, NON_NEGATIVE, wLen3);

		MONT_MOD_EXP(&C, A, N, M);
	}
	printf("End test Mont\n");


	for (i = 0; i < testLoop; i++)
	{
		RSA_gen_key(&PUB, &PRIV, n);

		bi_gen_rand(&A, NON_NEGATIVE, PUB.N->wordlen);

		if (bi_compare(PUB.N, A) == -1)
		{
			DIV(&one, &tmp, A, PUB.N);
			bi_assign(&A, tmp);
		}

		RSA_encryption(&C, A, &PUB);
		RSA_decryption(&B, C, &PRIV);
	}
	printf("End test RSA\n");


	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);
	bi_delete(&N);
	bi_delete(&M);
	bi_delete(&Q);
	bi_delete(&R);
	bi_delete(&one);
	bi_delete(&tmp);
	bi_delete(&PUB.N);
	bi_delete(&PUB.e);
	bi_delete(&PRIV.N);
	bi_delete(&PRIV.p);
	bi_delete(&PRIV.q);
	bi_delete(&PRIV.d);
}