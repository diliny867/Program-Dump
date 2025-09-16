#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void _qsort(char* ptr, size_t count, int elem_size, int (*cmp) (void*, void*)){
    if(count <= 1) return;
    size_t pos = 0;
    size_t it = 1;
#ifdef _MSC_VER // how much ive come to hate msvc
    char* tmp = (char*)_alloca(elem_size);
#else
    char tmp[elem_size];
#endif
    char *a, *b, *next;
    while(it < count){
        a = ptr + pos * elem_size; // pivot
        b = ptr + it * elem_size; // iterators to search for elements
        if(cmp(a, b) > 0){ // if larger move larger to where new lesser was found, lesser to pivot, pivot to next
            next = a + elem_size; // next after pivot
            memcpy(tmp, b, elem_size); // save at it
            memcpy(b, next, elem_size); // copy to it next past a
            memcpy(next, a, elem_size); // copy a to next
            memcpy(a, tmp, elem_size); // copy it to a
            pos++;
        }
        it++;
    }

    _qsort(ptr, pos, elem_size, cmp); // left
    _qsort(ptr + (pos + 1) * elem_size, count - pos - 1, elem_size, cmp); //right
}
void my_qsort(void* ptr, size_t count, int elem_size, int (*cmp) (void*, void*)){
    if(ptr == NULL || count == 0 || elem_size <= 0 || cmp == NULL) return;
    _qsort((char*)ptr, count, elem_size, cmp);
}

