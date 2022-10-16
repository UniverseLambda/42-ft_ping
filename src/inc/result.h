#pragma once

#include <unistd.h>
#include <stdbool.h>

#define RES_OK(X) result_ok(X, sizeof(X))
#define RES_ERR(E) result_err(E)
#define UNWRAP(T, R) *((T *)(result_unwrap(R)))
#define UNWRAP_ERR(R) result_unwrap_err(R)

typedef struct Result {
	bool is_err;

	unsigned int magic_start;
	union {
		char data[128];
		int error_code;
	} payload;
	unsigned int magic_end;
} Result;

Result result_err(int error_code);
Result result_ok(void *data, size_t len);

bool result_is_ok(Result *self);
bool result_is_err(Result *self);

void *result_unwrap(Result *self);
int result_unwrap_err(Result *self);