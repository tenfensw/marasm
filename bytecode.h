#pragma once

#include "marasm.h"

#define MARASM_BYTECODE_ALLOC_MIN 128

typedef enum {
	MARASM_BYTECODE_STATE_COMMAND,
	MARASM_BYTECODE_STATE_REGISTER,
	MARASM_BYTECODE_STATE_INTERMEDIATE // operand
} marasm_bytecode_state_t;

marasm_bytecode_ref marasm_bytecode_init(void);

marasm_bytecode_state_t marasm_bytecode_get_state(const marasm_bytecode_ref bc);

bool marasm_bytecode_push_command(marasm_bytecode_ref bc, const char* cmd, const marasm_map_ref cmdmap);
bool marasm_bytecode_push_register(marasm_bytecode_ref bc, const char* str);
bool marasm_bytecode_push_intermediate(marasm_bytecode_ref bc, const char* vl);

bool marasm_bytecode_save(const marasm_bytecode_ref bc, const char* where);
void marasm_bytecode_dump(const marasm_bytecode_ref bc);

void marasm_bytecode_release(marasm_bytecode_ref bc);
