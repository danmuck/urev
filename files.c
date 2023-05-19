#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "include/structs.h"
#include "include/files.h"
#include "include/generators.h"

    /*
     * Write updated status to the status file or create it if it does not exist.
     */
void update_status_file(status* status) {
    const char* file_name = "status";

    FILE* status_file = fopen(file_name, "w");
    if (status_file == NULL) {
        fprintf(stderr, "Failed to open 'status' file. \n");
    }
    fprintf(status_file, "score:%d\n", status -> score);
    fprintf(status_file, "wrong:%d\n", status -> wrong);
    fprintf(status_file, "wrongtally:%d\n", status -> wrong_tally);
    fprintf(status_file, "total:%d\n", status -> total);
    fprintf(status_file, "session:%d\n", status -> sessions);
    fprintf(status_file, "completion:%d\n", status -> completion);
    fprintf(status_file, "timestamp:%ld\n", status -> timestamp);

    fclose(status_file);

}

    /*
     * Read the status file to the live status struct.
     * If the file does not exist, call update_status_file() and create it.
     */
status* read_status_file(status* status) {
    const char* file_name = "status";
    if (access(file_name, F_OK) != -1) {
        FILE* status_file = fopen(file_name, "r");
        if (status_file == NULL) {
            fprintf(stderr, "Failed to open 'status' file. \n");
        }
        char line_buf[32];
        int line_count = 0;
        while (fgets(line_buf, sizeof(line_buf), status_file)) {
            switch (line_count) {
                case 0:
                    sscanf(line_buf, "score:%d", &status -> score);
                    break;
                case 1:
                    sscanf(line_buf, "wrong:%d", &status -> wrong);
                    break;
                case 2:
                    sscanf(line_buf, "wrongtally:%d", &status -> wrong_tally);
                    break;
                case 3:
                    sscanf(line_buf, "total:%d", &status -> total);
                    break;
                case 4:
                    sscanf(line_buf, "session:%d", &status -> sessions);
                    break;
                case 5:
                    sscanf(line_buf, "completion:%d", &status -> completion);
                    break;
                case 6:
                    sscanf(line_buf, "timestamp:%ld", &status -> timestamp);
                    break;
                default:
                    break;
            }
            line_count++;
        }
        fclose(status_file);

    } else {
        update_status_file(status);
    }
    return status;
}


    /*
     * Append incorrect entries to the errors file; create it if it does not exist.
     * The errors file is printed in the format:
     *      [file mode]:[response]:[starting timestamp]
     */
void append_to_errors(full_problem current, status* stats, int* answers) {
    FILE* error_file = fopen("errors", "a");
    if (error_file == NULL) {
        fprintf(stderr, "Failed to open 'errors' file. \n");
    }
    fprintf(error_file, "%s%s%s:%d%d%d%d:%ld\n",
            current.problems[0].permission_str, current.problems[1].permission_str, current.problems[2].permission_str,
            answers[3], answers[0], answers[1], answers[2],
            stats->timestamp
    );

    fclose(error_file);
}

    /*
     * Append all entries to the problems file; create it if it does not exist.
     * The problems file is printed in the format:
     *      [MD5SUM]:[starting timestamp]:[file mode]:[response]:[ending timestamp]
     *
     * **MD5SUM not currently implemented; using placeholder.
     */
void append_to_problems(full_problem current, status* stats, int* answers) {

    FILE* problems_file = fopen("problems", "a");
    if (problems_file == NULL) {
        fprintf(stderr, "Failed to open 'problems' file. \n");
    }
    fprintf(problems_file, "MD5SUM:");
    fprintf(problems_file, "%ld:", stats -> timestamp);
    fprintf(problems_file, "%s%s%s:", current.problems[0].permission_str, current.problems[1].permission_str, current.problems[2].permission_str);
    fprintf(problems_file, "%d%d%d%d:", answers[3], answers[0], answers[1], answers[2]);
    fprintf(problems_file, "%ld", time(NULL));
    fprintf(problems_file, "\n");

    fclose(problems_file);
}
