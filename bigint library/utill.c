#define _CRT_SECURE_NO_WARNINGS
#include "type.h"

/**
* @brief Initialize words
* @details ���忭 arr�� 0���� �ʱ�ȭ�Ѵ�.
* @param word* arr ���忭 arr�� ���� �ּ�
* @param int size ���忭 arr�� ����
*/
void array_init(word* arr, int size)
{
	memset(arr, 0, size * (WORDLEN >> 3));
}


/**
* @brief Generation random number to words
* @details ���Ƿ� ������ ���� 1 byte�� maksing�Ͽ� ���忭�� ����Ʈ ������ �Ҵ��Ѵ�.
* @param word* dst �������� ����Ʈ�� �Ҵ��� ���忭�� ���� �ּ�
* @param int wordlen ���忭�� ����
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
* @details dst�� src�� �ִ� ���� �����Ѵ�
* @param word* dst ������ �迭�� ������ ���忭 dst�� �����ּ�
* @param word* src ������ ������ �ִ� ���忭 src�� �����ּ�
* @param int size dst�� src�� ũ��
*/
void array_copy(word* dst, word* src, int size)
{
	memcpy(dst, src, size * (WORDLEN >> 3));
}

/**
* @brief Resize memory
* @details �޸𸮸� sizeof(word) * size ����Ʈ ��ŭ ���Ҵ��Ѵ�.
* @param word** x ���忭�� ����������
* @param int size �ٽ� �����Ҵ� �� ���忭�� ����
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
* @details left to Right������� x^n �� ����Ѵ�.
* @param word x ��
* @param word n ����
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
