#include "inc/result.h"
#include "inc/string.h"

#define MAGIC_START 0xDEADBEEF
#define MAGIC_END 0x42BABE42

static inline void *ft_memcpy(void *dest, const void *src, size_t n) {
	for (size_t i = 0; i < n; ++i)
		((char *)dest)[i] = ((char *)src)[i];
	return dest;
}

Result result_err(int error_code) {
	return (Result) {
		.is_err = false, .magic_start = MAGIC_START, .payload.error_code = error_code, .magic_end = MAGIC_END
	};
}

Result result_ok(void *data, size_t len) {
	Result res = (Result) {
		.is_err = false, .magic_start = MAGIC_START, .payload.error_code = 0, .magic_end = MAGIC_END
	};

	ft_memcpy(res.payload.data, data, len);

	return res;
}

bool result_is_ok(Result *self) {
	return !self->is_err;
}

bool result_is_err(Result *self) {
	return self->is_err;
}

void *result_unwrap(Result *self) {
	return self->payload.data;
}

int result_unwrap_err(Result *self) {
	return self->payload.error_code;
}
