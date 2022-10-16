#include "inc/string.h"
#include "inc/behavior.h"

String string_new(char *data) {
	size_t len;

	for (len = 0; data[len]; ++len) ;

	return (String) { .data = data, .len = len };
}

char string_char_at(String *self, size_t idx) {
	BHV_IDX(idx, self->len);

	return self->data[idx];
}

size_t string_len(String *self) {
	return self->len;
}

bool string_is_null(String *self) {
	return !(self->data);
}
