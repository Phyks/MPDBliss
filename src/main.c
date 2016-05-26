#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>

#include "analysis.h"
#include "constants.h"
#include "utilities.h"

// TODO: Handle deletions from db


int main(int argc, char** argv) {
	if (argc < 3) {
		printf("Usage: %s basepath [relative_filenames].\n", argv[0]);
		return EXIT_SUCCESS;
	}

    // Get data directory, init db file
	char blissify_data_folder[DEFAULT_STRING_LENGTH + 1] = "";
	char blissify_data_db[DEFAULT_STRING_LENGTH + 1] = "";
	if (0 != _init_db(blissify_data_folder, blissify_data_db)) {
		exit(EXIT_FAILURE);
	}

	char base_path[DEFAULT_STRING_LENGTH + 1] = "";
    strncat(base_path, argv[1], DEFAULT_STRING_LENGTH);
    strip_trailing_slash(base_path);
    strncat(base_path, "/", 1 - strlen(base_path));

    // Connect to SQLite db
    sqlite3 *dbh;
    if (0 != sqlite3_open(blissify_data_db, &dbh)) {
        fprintf(stderr, "Unable to open SQLite db.\n");
		exit(EXIT_FAILURE);
    }
    int dberr = sqlite3_exec(dbh, "PRAGMA foreign_keys = ON", NULL, NULL, NULL);
    if (SQLITE_OK != dberr) {
        fprintf(stderr, "Unable to open SQLite db.\n");
        sqlite3_close(dbh);
		exit(EXIT_FAILURE);
    }

	for (int i = 2; i < argc; ++i) {
		_parse_music_helper(dbh, base_path, argv[i]);
	}

    // Close SQLite connection
    sqlite3_close(dbh);

    printf("Done! :)\n");

    return EXIT_SUCCESS;
}