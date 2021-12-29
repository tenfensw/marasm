#include "bytecode.h"
#include "marasm.h"

marasm_arguments marasm_parse_args(const int argc, const char** argv) {
	marasm_arguments result;
	memset(&result, 0, sizeof(marasm_arguments));

	// absolute path already known
	result.me = argv[0];

	// go through the args
	for (size_t index = 1; index < SIZE_T(argc); index++) {
		const char* arg = argv[index];

		if (strlen(arg) >= 2 && arg[0] == '-') {
			const char* operand = arg + 2;

			// valid arg to process
			switch (arg[1]) {
				case 'P': {
					// map file page
					result.mapfile = operand;
					break;
				}
				case 'o': {
					// output file
					result.output = operand;
					break;
				}
				case 'm': {
					// little or big endian
					result.be = !(strcmp(operand, "le") == 0);
					break;
				}
				default: {
					fprintf(stderr, "warning! ignored unknown flag - '-%c'\n", arg[1]);
					break;
				}
			}
		} else if (!result.input)
			result.input = arg; // path
		else
			fprintf(stderr, "warning! ignored unknown parameter - '%s'\n", arg);
	}

	return result;
}

bool marasm_not_valid_path(const char* path) {
	if (!path)
		return true;

	return (access(path, F_OK) != 0);
}

char* marasm_read_path(const char* path, size_t* rsizep) {
	if (marasm_not_valid_path(path))
		return NULL;

	FILE* rdr = fopen(path, "r");
	if (!rdr)
		return NULL;

	// get file size
	fseek(rdr, 0L, SEEK_END);
	size_t rsize = ftell(rdr);
	SET_IF_AVAILABLE(rsizep, rsize)

	rewind(rdr);

	// read it
	char* ctnt = calloc(rsize + 1, sizeof(char));
	fread(ctnt, sizeof(char), rsize, rdr);

	fclose(rdr);
	return ctnt;
}

marasm_bytecode_ref marasm_parse(const char* input, char** logp) {
	if (!input) {
		SET_ERROR(logp, "NULL input")
		return NULL;
	}

	marasm_bytecode_ref result = marasm_bytecode_init();

	// token
	char* token = NULL;
	// current "line number"
	size_t linen = 1;

	for (size_t index = 0; index < strlen(input); index++) {
		const char current = input[index];

		if (current == '\n') {
			linen += 1;

			if (marasm_bytecode_get_state(result) == MARASM_BYTECODE_STATE_INTERMEDIATE)
				marasm_bytecode_push_intermediate(result, token); // TODO: format checks
			else if (!token)
				continue;
			else {
				// TODO: memory leaks

				// TODO: finish
			}

		}
	}

	return result;
}
