#include "map.h"

struct marasm_map_s {
	char** names;
	marasm_opcode_t* values;

	size_t allocated;
};

bool marasm_map_has_with_index(const marasm_map_ref map, const char* cmd, size_t* indexp) {
	if (!map || !cmd)
		return false;

	size_t fin = 0;
	while (fin < map->allocated) {
		// TODO: handle uppercase notations
		if (!map->names[fin])
			break;
		else if (strcmp(map->names[fin], cmd) == 0) {
			SET_IF_AVAILABLE(indexp, fin);
			return true;
		}
	}

	return false;
}

marasm_map_ref marasm_map_init() {
	marasm_map_ref map = malloc(sizeof(struct marasm_map_s));
	map->allocated = MARASM_MAP_DEFAULT_SIZE;

	// init arrays
	map->names = calloc(map->allocated, sizeof(char*));
	map->values = calloc(map->allocated, sizeof(marasm_opcode_t));

	return map;
}

marasm_map_ref marasm_map_init_from_string(const char* fmt, char** errmp) {
	if (!fmt) {
		SET_ERROR(errmp, "NULL input")
		return NULL;
	}

	marasm_map_ref map = marasm_map_init();

	// each line/space token
	char* token = NULL;
	// met header yet?
	bool begun = false;
	// last command name
	char* name = NULL;
	// append command index
	size_t aindex = 0;

	for (size_t index = 0; index < strlen(fmt); index++) {
		if (!token)
			token = calloc(MARASM_MAP_TOKEN_MAX, sizeof(char));

		const char current = fmt[index];

		// add character to token unless we hit tab, space or '\n'
		if (isspace(current) == 0 && current != ':')
			token[strlen(token)] = current;
		else {
			// handle the token
			if (strcmp(token, MARASM_MAP_TOKEN_HEADER) == 0)
				begun = true;
			else if (begun) {
				if (current == ':')
					name = strdup(token); // this was the command name
				else {
					if (!name) {
						// TODO: fix memory leaks

						SET_ERROR(errmp, "Stray value")
						return NULL;
					}

					map->names[aindex] = name;
					map->values[aindex++] = atoi(token); // TODO: verify values

					name = NULL;
				}

			}

			free(token);
			token = NULL;

		}
	}

	// clean up
	free(token);
	free(name);

	return map;
}

marasm_opcode_t marasm_map_get(const marasm_map_ref map, const char* cmd) {
	size_t where = 0;

	if (marasm_map_has_with_index(map, cmd, &where))
		return map->values[where];

	return 0;
}

bool marasm_map_has(const marasm_map_ref map, const char* cmd) {
	return marasm_map_has_with_index(map, cmd, NULL);
}

void marasm_map_dump(const marasm_map_ref map) {
	if (!map)
		fprintf(stderr, "(null)\n");
	else {
		size_t index = 0;

		fprintf(stderr, "{ \n");
		while (index < map->allocated && map->names[index]) {
			fprintf(stderr, "  \"%s\" => %u\n", map->names[index], map->values[index]);
			index += 1;
		}
		fprintf(stderr, "}\n");
	}
}

void marasm_map_release(marasm_map_ref map) {
	if (!map)
		return;

	for (size_t index = 0; index < map->allocated; index++)
		free(map->names[index]);

	free(map->values);
	free(map->names);
	free(map);
}

