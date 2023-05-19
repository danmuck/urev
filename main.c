#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "include/structs.h"
#include "include/files.h"
#include "include/generators.h"
#include "include/status.h"

//#include <openssl/md5.h>

//void free_problems(problem *old_problem) {
//    for (int i = 0; i < 3; i++) {
//        free(old_problem[i].permission_str);
//        old_problem[i].solution = 0;
//    }
//}

    /*
     * Check for valid user input of octal values only.
     */
void check_valid_input(int *answers, int index, WINDOW* window) {
    switch (answers[index]) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            break;
        default:
            switch (index) {
                case 0:
                    wmove(window, 3, 7);
                    wprintw(window, "     ");
                    mvwscanw(window, 3, 7,  "%d", &answers[index]);
                    break;
                case 1:
                    wmove(window, 3, 11);
                    wprintw(window, "     ");
                    mvwscanw(window, 3, 11,  "%d", &answers[index]);
                    break;
                case 2:
                    wmove(window, 3, 15);
                    wprintw(window, "     ");
                    mvwscanw(window, 3, 15,  "%d", &answers[index]);
                    break;
                case 3:
                    wmove(window, 3, 3);
                    wprintw(window, "     ");
                    mvwscanw(window, 3, 3,  "%d", &answers[index]);
                    break;
                default:
                    break;
            }
            check_valid_input(answers, index, window);
            break;
    }
}
    /*
     * Check user entries for a given problem and update status.
     */
void check_answers(full_problem current, status *stats, WINDOW* window, int sticky_bit_include) {
    stats -> timestamp = time(NULL);
    int answers[4];
    if (sticky_bit_include == TRUE) {
        mvwscanw(window, 3, 3,  "%d", &answers[3]);
        check_valid_input(answers, 3, window);
    } else answers[3] = 0;
    mvwscanw(window, 3, 7,  "%d", &answers[0]);
    check_valid_input(answers, 0, window);

    mvwscanw(window, 3, 11,  "%d", &answers[1]);
    check_valid_input(answers, 1, window);

    mvwscanw(window, 3, 15,  "%d", &answers[2]);
    check_valid_input(answers, 2, window);

    append_to_problems(current, stats, answers);

    int correctness = TRUE;
    for (int i = 0; i < 3; i++) {
        if (current.problems[i].solution == answers[i]) {

        } else {
            correctness = FALSE;
            break;
        }
    }
    if (sticky_bit_include == TRUE) {
        if (current.sticky_bit != answers[3]) {
            correctness = FALSE;
        }
    }
    if (correctness == TRUE) {
        stats->score += 1;
    } else {
        stats -> wrong += 1;
        stats -> wrong_tally += 1;
        append_to_errors(current, stats, answers);
    }
    stats -> total += 1;
}
    /*
     * Print a problem to the ncurses window;
     * take a process user entries and write files.
     */
void print_problem(status *stats) {
    initscr(); // init ncurses
    int height, width, start_y, start_x;
    height = 5;
    width = 20;
    start_y = start_x = 5;
    WINDOW *win = newwin(height, width, start_y, start_x);
    refresh();
    wrefresh(win);

    // print stats and generate problem
    n_print_status(stats);
    full_problem out_prob;
    int sticky_bit = FALSE;
    if (stats -> total >= 96) sticky_bit = TRUE;
    out_prob = generate_full_problem(sticky_bit);
    mvwprintw(win, 1, 1, "      u   g   o\n");
    mvwprintw(win, 2, 1, "  %c  %s %s %s\n", generate_file_type(), out_prob.problems[0].permission_str, out_prob.problems[1].permission_str, out_prob.problems[2].permission_str);

    // check answers and update to stdscr
    check_answers(out_prob, stats, win, TRUE);
    n_print_status(stats);

    wrefresh(win);

    endwin(); // end ncurses
}

int main() {

    srand(time(NULL) + clock());

    printf("TEST STATUS =========================================\n");
    status* stats = generate_status();
    stats = read_status_file(stats);

    int session_start = stats->sessions;
    while (stats->sessions < session_start + 1) {
        print_problem(stats);
        update_status(stats);
        update_status_file(stats);
    }

    print_status(*stats);

//    check_md5sum("urev");
    return 0;
}




//    full_problem out_prob;
//    out_prob = generate_full_problem(FALSE);
//    printf(":     u   g   o\n");
//    printf(": %c  %s %s %s\n", generate_file_type(), out_prob.problems[0].permission_str, out_prob.problems[1].permission_str, out_prob.problems[2].permission_str);
//    printf(": %d   %d   %d   %d\n", out_prob.sticky_bit, out_prob.problems[0].solution, out_prob.problems[1].solution, out_prob.problems[2].solution);
//    printf("WITH STICKY BIT ON ==================================\n");
//    out_prob = generate_full_problem(TRUE);
//    printf(":     u   g   o\n");
//    printf(": %c  %s %s %s\n", generate_file_type(), out_prob.problems[0].permission_str, out_prob.problems[1].permission_str, out_prob.problems[2].permission_str);
//    printf(": %d   %d   %d   %d\n", out_prob.sticky_bit, out_prob.problems[0].solution, out_prob.problems[1].solution, out_prob.problems[2].solution);

////////////////////////////////////

//int check_md5sum(char* path) {
//    FILE* file;
//    MD5_CTX md5Context;
//    unsigned char digest[MD5_DIGEST_LENGTH];
//    unsigned char buffer[1024];
//    int bytesRead;
//
//    // Open the binary file in read mode
//    file = fopen(path, "rb");
//    if (file == NULL) {
//        printf("Failed to open the file. \n");
//        return 1;
//    }
//
//    // Initialize the MD5 context
//    MD5_Init(&md5Context);
//
//    // Read the file in chunks and update the MD5 context
//    while ((bytesRead = fread(buffer, 1, 1024, file)) != 0) {
//        MD5_Update(&md5Context, buffer, bytesRead);
//    }
//
//    // Finalize the MD5 computation and obtain the digest
//    MD5_Final(digest, &md5Context);
//
//    // Close the file
//    fclose(file);
//
//    // Print the MD5 checksum in hexadecimal format
//    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
//        printf("%02x", digest[i]);
//    }
//    printf("\n");
//
//    return 0;
//}