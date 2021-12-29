#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

// size_t
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <crtdefs.h>
#endif

/// opcode type (32-bit)
typedef uint32_t marasm_opcode_t;

#define MARASM_ERROR_MAX 1024

#define SIZE_T(num) ((size_t)( (num >= 0) ? (num) : ((-1) * (num)) ))
#define SET_IF_AVAILABLE(ptr, vl) { \
					if (ptr) \
						(*ptr) = vl; \
				  }
#define SET_ERROR(ptr, vl) SET_IF_AVAILABLE(ptr, strdup(vl))
#define SET_SPRINTF(ptr, ...) { \
					if (ptr) \
						(*ptr) = calloc(MARASM_ERROR_MAX, sizeof(char)); \
						sprintf(*ptr, __VA_ARGS__); \
					} \
			      }

typedef struct {
	// own path
	const char* me;

	// explicit map file path
	const char* mapfile;

	// input file contents/stdin contents
	const char* input;
	// output file path
	const char* output;

	// be big endian (true by default)
	bool be;
} marasm_arguments;

/// string hash map
typedef struct marasm_map_s* marasm_map_ref;

/// bytecode storage object
typedef struct marasm_bytecode_s* marasm_bytecode_ref;

/// preprocess CLI args into a structure
marasm_arguments marasm_parse_args(const int argc, const char** argv);
/// checks if the file exists
bool marasm_not_valid_path(const char* path);
/// reads file into memory
char* marasm_read_path(const char* path, size_t* rsizep);

marasm_bytecode_ref marasm_parse(const char* input, char** logp);
