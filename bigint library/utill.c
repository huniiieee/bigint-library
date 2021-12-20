#define _CRT_SECURE_NO_WARNINGS
#include "type.h"

/**
* @brief Initialize words
* @details 워드열 arr을 0으로 초기화한다.
* @param word* arr 워드열 arr의 시작 주소
* @param int size 워드열 arr의 길이
*/
void array_init(word* arr, int size)
{
	memset(arr, 0, size * (WORDLEN >> 3));
}


/**
* @brief Generation random number to words
* @details 임의로 생성된 값을 1 byte씩 maksing하여 워드열에 바이트 단위로 할당한다.
* @param word* dst 랜덤으로 바이트를 할당한 워드열의 시작 주소
* @param int wordlen 워드열의 길이
*/
void array_rand(word* dst, int wordlen)
{
	byte* p = (byte*)dst;
	int cnt = wordlen * sizeof(word);

	while (cnt > 0)
	{
		*p = rand() & 0xff;
		p++;
		cnt--;
	}
}


/**
* @brief  Copy array
* @details dst에 src에 있는 값을 복사한다
* @param word* dst 복사한 배열을 저장할 워드열 dst의 시작주소
* @param word* src 복사할 정보가 있는 워드열 src의 시작주소
* @param int size dst와 src의 크기
*/
void array_copy(word* dst, word* src, int size)
{
	memcpy(dst, src, size * (WORDLEN >> 3));
}

/**
* @brief Resize memory
* @details 메모리를 sizeof(word) * size 바이트 만큼 재할당한다.
* @param word** x 워드열의 더블포인터
* @param int size 다시 동적할당 할 워드열의 길이
* @return SUCCESS or FAILURE
*/
int resize_memory(word** x, int size)
{
	//word* tmp = (word*)realloc(*x, size);
	word* tmp = (word*)realloc(*x, sizeof(word) * size);
	if (tmp == NULL)
		return FAILURE;

	*x = tmp;
	return SUCCESS;
}

/**
* @brief Exponentiation
* @details left to Right방식으로 x^n 을 계산한다.
* @param word x 밑
* @param word n 지수
* @return x^n
*/
word LtR_expo(word x, word n)
{
	word tmp = 0;
	int i = 0;
	int l = 0;

	tmp = n;
	while (tmp != 0)
	{
		tmp = tmp >> 1;
		l++;
	}

	tmp = 1;
	for (i = l - 1; i >= 0; i--)
	{
		tmp = tmp * tmp;
		if (i_bit(n, i))
			tmp = tmp * x;
	}

	return tmp;
}
