#include "map.h"
#include "bytecode.h"

struct marasm_bytecode_s {
	// what to accept
	marasm_bytecode_state_t state;

	// what to flush
	marasm_opcode_t* values;

	// array specs
	size_t size;
	size_t allocated;
};

marasm_bytecode_ref marasm_bytecode_init() {
	marasm_bytecode_ref bc = malloc(sizeof(struct marasm_bytecode_s));
	bzero(bc, sizeof(struct marasm_bytecode_s));

	bc->allocated = MARASM_BYTECODE_ALLOC_MIN;
	bc->values = calloc(bc->allocated, sizeof(marasm_opcode_t));

	return bc;
}

marasm_bytecode_state_t marasm_bytecode_get_state(const marasm_bytecode_ref bc) {
	return bc->state;
}

void marasm_bytecode_reallocate_if_needed(marasm_bytecode_ref bc) {
	if (bc && (bc->size + 1) >= bc->allocated) {
		bc->allocated += MARASM_BYTECODE_ALLOC_MIN;
		bc->values = realloc(bc->values, sizeof(marasm_opcode_t) * bc->allocated);
	}
}

bool marasm_bytecode_push_command(marasm_bytecode_ref bc, const char* cmd, const marasm_map_ref cmdmap) {
	if (!bc || !cmd || !cmdmap || bc->state != MARASM_BYTECODE_STATE_COMMAND || !marasm_map_has(cmdmap, cmd))
		return false;

	marasm_bytecode_reallocate_if_needed(bc);

	bc->values[bc->size++] = marasm_map_get(cmdmap, cmd);
	bc->state = MARASM_BYTECODE_STATE_REGISTER;
	return true;
}

bool marasm_bytecode_push_register(marasm_bytecode_ref bc, const char* str) {
	if (!bc || !str || strlen(str) < 2 || str[0] != 'r' || bc->state != MARASM_BYTECODE_STATE_REGISTER)
		return false;

	const char* numst = str + 1;
	bc->values[bc->size++] = (marasm_opcode_t)(atoi(numst));
	bc->state = MARASM_BYTECODE_STATE_INTERMEDIATE;
	return true;
}

bool marasm_bytecode_push_intermediate(marasm_bytecode_ref bc, const char* vl) {
	if (!bc || !vl || bc->state != MARASM_BYTECODE_STATE_INTERMEDIATE)
		return false;

	marasm_bytecode_reallocate_if_needed(bc);

	// TODO: format check
	marasm_opcode_t vlv = (marasm_opcode_t)(atoi(vl));
	bc->values[bc->size++] = vlv;

	bc->state = MARASM_BYTECODE_STATE_REGISTER;
	return true;
}

bool marasm_bytecode_save(const marasm_bytecode_ref bc, const char* where) {
	if (!bc)
		return false;

	FILE* iov = fopen(where, "w+");
	if (!iov)
	        return false;

	fwrite(bc->values, sizeof(marasm_opcode_t), bc->size, iov);
	fflush(iov);
	fclose(iov);
	return true;
}

void marasm_bytecode_release(marasm_bytecode_ref bc) {
	if (!bc)
		return;

	free(bc->values);
	free(bc);
}
