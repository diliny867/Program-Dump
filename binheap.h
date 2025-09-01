#pragma once

#include <stdlib.h>

#define __swap(a, b, type) \
    do{ \
        type tmp = *(a); \
        *(a) = *(b); \
        *(b) = tmp; \
    }while(0);

#define __binheaph_push_up(h, index, cmp) \
    do{  \
        size_t parent; \
        size_t i = (index); \
        while(i > 0){ \
            parent = (i - 1) >> 1; \
            if(!cmp((h) + parent, (h) + i)) \
                break; \
            __swap((h) + parent, (h) + i, typeof(*(h))); \
            i = parent; \
        } \
    }while(0);


#define binheap_add(h, len, elem, cmp) \
    do{ \
        (h)[len] = (elem); \
        __binheaph_push_up(h, len, cmp); \
    }while(0);

#define binheap_heapify(h, len, cmp) \
    do{ \
        for(size_t i = (len) - 1; i > 0; i--){ \
            size_t parent = (i - 1) >> 1; \
            if(cmp((h) + parent, (h) + i)) \
                __swap((h) + parent, (h) + i, typeof(*(h))); \
        } \
    }while(0);


// #include <stdint.h>
// 
// typedef int64_t bh_value_t;
//
// typedef int (*bh_cmp_t) (bh_value_t* a, bh_value_t* b);
//
// void bh_add(bh_value_t* h, size_t len, bh_value_t elem, bh_cmp_t cmp);
// void bh_heapify(bh_value_t* h, size_t len, bh_cmp_t cmp);
//
// int bh_cmp_default(bh_value_t* a, bh_value_t* b);
//
//
// #ifdef BINARYHEAP_IMPLEMENTATION
//
// static void swap(bh_value_t* a, bh_value_t* b){
//     bh_value_t tmp = *a;
//     *a = *b;
//     *b = tmp;
// }
//
// int bh_cmp_default(bh_value_t* a, bh_value_t* b){
//     return *a <= *b;
// }
//
// static int _bh_swap(bh_value_t* a, bh_value_t* b, bh_cmp_t cmp){
//     if(cmp(a, b)) return 0;
//     swap(a, b);
//     return 1;
// }
//
// static void _bh_push_up(bh_value_t* h, size_t index, bh_cmp_t cmp){
//     size_t parent;
//     while(index > 0){
//         parent = (index - 1) >> 1;
//         if(!_bh_swap(h + parent, h + index, cmp))
//             break;
//         index = parent;
//     }
// }
//
// void bh_add(bh_value_t* h, size_t len, bh_value_t elem, bh_cmp_t cmp){
//     h[len] = elem;
//     _bh_push_up(h, len, cmp);
// }
//
// void bh_heapify(bh_value_t* h, size_t len, bh_cmp_t cmp){
//     for(size_t i = len - 1; i > 0; i--){
//         size_t parent = (i - 1) >> 1;
//         _bh_swap(h + parent, h + i, cmp);
//     }
// }
//
// #endif
