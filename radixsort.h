#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>


static void fill_indices(uint8_t* ptr, size_t count, int stride, int size, size_t* indices, char xmask){
    uint8_t digit;
    for(size_t i = 0; i < count; i++){
        digit = *(ptr + stride) ^ xmask;
        indices[digit] += size; // directly add size now so we can advance to next element directly by it in collect_indices
        ptr += size; // go to next element
    }
    size_t total = 0, curr = 0;
    for(int i = 0; i < 256; i++){ // convert local counts of elements with current digit to global indexes for that digit
        curr = indices[i];
        indices[i] = total;
        total += curr;
    }
}
static void collect_indices(uint8_t* ptr, uint8_t* out, size_t count, int stride, int size, size_t* indices, char xmask){
    size_t index;
    uint8_t digit;
    for(size_t i = 0; i < count; i++){
        digit = *(ptr + stride) ^ xmask; // digit of current element
        index = indices[digit];
        for(int j = 0; j < size; j++){ //copy element bytes to output
            out[index + j] = ptr[j];
        }
        // memcpy(out + index, ptr, size);
        ptr += size;
        indices[digit] += size; // increase index to next element
    }
}
static inline void swapptrs(uint8_t** a, uint8_t** b){
    uint8_t* tmp = *a;
    *a = *b;
    *b = tmp;
}
void radixsort8(void* ptr, size_t count, int elem_radix_count, int has_negatives){ // elem_radix_count == sizeof(mytype)
    if(ptr == NULL || elem_radix_count <= 0) return;
    size_t indices[256];
    size_t size = count * elem_radix_count;
    uint8_t* out = (uint8_t*)malloc(size);
    uint8_t* cptr = (uint8_t*)ptr;
    char xmask = has_negatives ? 0x80 : 0; // invert msb mask for negatives (it wont matter applying it for each byte)
    for(int i = 0; i < elem_radix_count; i++){
        memset(indices, 0, 256 * sizeof(size_t));
        fill_indices(cptr, count, i, elem_radix_count, indices, xmask);
        collect_indices(cptr, out, count, i, elem_radix_count, indices, xmask);
        swapptrs(&cptr, &out);
    }
    if(elem_radix_count & 1){ // if operation count is odd, the end data is located in temporary array
        out = cptr; // out right now is in cptr
        memcpy(ptr, out, size); // so copy it
    }
    free(out);
}

