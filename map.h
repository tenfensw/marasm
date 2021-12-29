#include <ctype.h>
#include "marasm.h"

#define MARASM_MAP_DEFAULT_SIZE 256
#define MARASM_MAP_TOKEN_MAX 128
#define MARASM_MAP_TOKEN_HEADER "MARASMV0"

marasm_map_ref marasm_map_init(void);
marasm_map_ref marasm_map_init_from_string(const char* fmt, char** errmp);

marasm_opcode_t marasm_map_get(const marasm_map_ref map, const char* cmd);
bool marasm_map_has(const marasm_map_ref map, const char* cmd);

void marasm_map_dump(const marasm_map_ref map);
void marasm_map_release(marasm_map_ref map);
