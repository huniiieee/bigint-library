#ifndef _UTILL_H_
#define _UTILL_H_

#include "type.h"

void array_init(word* arr, int size);
void array_copy(word* dst, word* src, int size);
void array_rand(word* dst, int wordlen);
int resize_memory(word** x, int size);

word LtR_expo(word x, word n);


#endif
