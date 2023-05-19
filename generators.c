#include <stdlib.h>
#include "include/generators.h"

    /*
     * Generate a single problem permission string.
     *      examples: ---, rwx, r-x, -wx
     *
     * **NOTE: 3 permission strings are required for a problem.
     *         This is handled by generate_full_problem(), using this function.
     */
problem generate_permissions() {
    problem new;
    char* permissions = (char*)malloc(4 * sizeof(char));

    // generate solution
    int solution = rand() % 8;

    // assign the appropriate permission characters based on the solution
    permissions[0] = (solution & 4) ? 'r' : '-';
    permissions[1] = (solution & 2) ? 'w' : '-';
    permissions[2] = (solution & 1) ? 'x' : '-';
    permissions[3] = '\0';

    new.permission_str = permissions;
    new.solution = solution;

    return new;
}

    /*
     * Generate a new session with all streaks/wrongtally reset.
     */
session_status* generate_session() {
    session_status *new_session = malloc(sizeof(session_status));

    new_session -> session = 0;
    new_session -> correct = 0;
    new_session -> incorrect = 0;
    new_session -> streak = 0;
    new_session -> wrongtally = "";
    new_session -> streak_prog = "";

    return new_session;
}

    /*
     * Generate a new status.
     *
     * **NOTE: This should only be called on the first session as the
     *         status will persist throughout the life of the activity.
     *         (via the status file)
     */
status* generate_status() {
    status *new = malloc(sizeof(status));

    new -> score = 0;
    new -> wrong = 0;
    new -> wrong_tally = 0;
    new -> total = 0;
    new -> sessions = 0;
    new -> completion = 0;
    new -> timestamp = time(NULL);
    new -> session_info = generate_session();

    return new;
}

    /*
     * Generate a psuedo-random file type to appear before a problem.
     *
     *  UNIX filetypes:
     *      '-' Regular files contain data in a plain text or binary format.
     *          They can be read and written by users.
     *
     *      'd' Directories are used to organize files into a hierarchical structure.
     *          They contain entries that point to other files or directories.
     *
     *      'b' Block devices represent devices that provide block-level access to data,
     *          such as hard drives or SSDs. They allow random access to data in fixed-size blocks.
     *
     *      'c' Character devices represent devices that provide character-level access to data,
     *          such as terminals, serial ports, or printers. They handle data as a stream of characters.
     *
     *      'l' Symbolic links, also known as soft links, are special files that act as
     *          pointers to other files or directories. They provide a convenient way to
     *          reference files or directories located elsewhere in the file system.
     *
     *      'p' Named pipes, also known as FIFOs (First In First Out), are special files that enable
     *          interprocess communication by allowing one process to write data into the pipe and
     *          another process to read it.
     *
     *      's' Sockets are special files used for interprocess communication on the same machine or
     *          over a network. They enable processes to communicate by sending and receiving data.
     */
char generate_file_type() {
    int random = rand() % 7;
    switch (random) {
        case 1:
            random = 'p';
            break;
        case 2:
            random = 's';
            break;
        case 3:
            random = 'l';
            break;
        case 4:
            random = 'b';
            break;
        case 5:
            random = 'c';
            break;
        case 6:
            random = 'd';
            break;
        default:
            random = '-';
    }
    return (char)random;
}

    /*
     * Generate a full_problem (an actual problem to be presented to the user)
     * If the sticky_bit is set to TRUE, a 4-tier octal permission will be added
     * to the problem and the generated permissions will be modified to account for it.
     */
full_problem generate_full_problem(int sticky_bit_include) {
    full_problem output;
    output.sticky_bit = 0;

    for (int i = 0; i < 3; i++) {
        // generate 3 problems and assign them to the full_problem with their solutions
        // default sticky bit = 0
        problem new = generate_permissions();

        output.problems[i] = new;
    }

    if (sticky_bit_include == TRUE) {
        // generate sticky bit and change the problems to account for it
        char* sticky_permissions = (char*)malloc(4 * sizeof(char));
        int sticky_bit = rand() % 8;
        sticky_permissions[0] = (sticky_bit & 4) ? 'S' : '-';
        sticky_permissions[1] = (sticky_bit & 2) ? 'S' : '-';
        sticky_permissions[2] = (sticky_bit & 1) ? 'T' : '-';
        sticky_permissions[3] = '\0';

        output.sticky_bit = sticky_bit;

        for (int i = 0; i < 3; i++) {
            if (sticky_permissions[i] != '-') {
                if (output.problems[i].permission_str[2] == '-') {
                    output.problems[i].permission_str[2] = sticky_permissions[i];
                }
                else {
                    switch (sticky_permissions[i]) {
                        case 'S':
                            output.problems[i].permission_str[2] = 's';
                            break;
                        case 'T':
                            output.problems[i].permission_str[2] = 't';
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    return output;
}