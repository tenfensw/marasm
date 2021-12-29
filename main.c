#include "marasm.h"
#include "map.h"

int main(const int argc, const char** argv) {
	marasm_arguments args = marasm_parse_args(argc, argv);

	// check args validity
	if (marasm_not_valid_path(args.mapfile)) {
		fprintf(stderr, "error! please specify a valid path to 'dvm.map' file via -P flag\n");
		return 1;
	} else if (marasm_not_valid_path(args.input)) {
		fprintf(stderr, "error! please specify input assembly\n");
		return 2;
	}

	// for certain error handling
	char* logline = NULL;

	// process command map (dvm.map)
	char* mapcraw = marasm_read_path(args.mapfile, NULL);
	marasm_map_ref mapctnt = marasm_map_init_from_string(mapcraw, &logline);

	free(mapcraw);
	marasm_map_dump(mapctnt);

	return 0;
}
