#pragma once

#include <stdio.h>

#include "xorshift.h"


uint32_t xorshift32_n(xorshift32_state* state, uint32_t n) {
	return xorshift32(state) % n;
}
uint32_t xorshift32_range(xorshift32_state* state, uint32_t from, uint32_t to) {
	return from + xorshift32(state) % (to - from);
}
double xorshift32_frange(xorshift32_state* state, double from, double to) {
	return from + ((double)xorshift32(state) / (double)UINT32_MAX) * (to - from);
}

uint64_t xorshift64_n(xorshift64_state* state, uint64_t n){
	return xorshift64(state) % n;
}
uint64_t xorshift64_range(xorshift64_state* state, uint64_t from, uint64_t to){
	return from + xorshift64(state) % (to - from);
}
uint64_t xorshift64_frange(xorshift64_state* state, double from, double to){
	return from + ((double)xorshift64(state) / (double)UINT64_MAX) * (to - from);
}

uint32_t xorshift32_128_n(xorshift32_128_state* state, uint32_t n){
	return xorshift32_128(state) % n;
}
uint32_t xorshift32_128_range(xorshift32_128_state* state, uint32_t from, uint32_t to){
	return from + xorshift32_128(state) % (to - from);
}
double xorshift32_128_frange(xorshift32_128_state* state, double from, double to) {
	return from + ((double)xorshift32_128(state) / (double)UINT64_MAX) * (to - from);
}


int xorshift32_01(xorshift32_state* state, double probability) {
	return xorshift32(state) <= (uint32_t)((XORSHIFT32_MAX) * probability);
}
int xorshift64_01(xorshift64_state* state, double probability) {
	return xorshift64(state) <= (uint64_t)((XORSHIFT64_MAX) * probability);
}

