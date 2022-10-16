#pragma once

#include <unistd.h>
#include <stdbool.h>

typedef struct String {
	char *data;
	size_t len;
} String;

String string_new(char *data);
char string_char_at(String *self, size_t idx);
bool string_is_null(String *self);
size_t string_len(String *self);
