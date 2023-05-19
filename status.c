#include <string.h>
#include "include/status.h"
#include "include/files.h"
#include "include/generators.h"

    /*
     * Print status to standard window for debugging
     */
void print_status(status input) {
    printf(" [Session: %3d] [Score: %3d] [Total: %3d / Wrong: %3d] \n [Wrong Tally: %3d] [Completion: %3d] \n [Timestamp: %ld] \n",
           input.sessions, input.score, input.total, input.wrong, input.wrong_tally, input.completion, input.timestamp);
}

    /*
    * Print Status to ncurses window
    */
void n_print_status(status *stats) {
    int iteration = 192;
    mvwprintw(stdscr, 0, 0, " [Session: %3d] [Set: %3d] [Iteration: %3d / %3d] \n [Score: %3d] [ # correct: %3d / # incorrect: %d ] [ %s ] \n",
              stats -> sessions, stats -> sessions, stats -> total, iteration,
              stats -> score, stats -> score, stats -> wrong, stats -> session_info -> wrongtally);
    refresh();
}

    /*
     * Wrongtally implementation, not complete yet
     */
void check_wrongtally(status *stats) {
    if (stats -> wrong_tally > 4) {
        // hit max wrong_tally
        stats -> wrong_tally = 0;
        stats -> session_info -> streak = 0;
        stats -> session_info -> wrongtally = "";
    }
    else if (stats -> session_info -> streak > 8 && stats -> wrong_tally != 0) {
        // hit streak to counter wrong_tally
        // idk what to do here???
    }
    int i = 0;
    while (i < stats -> wrong_tally) {
        strcat(stats -> session_info -> wrongtally, " X");
    }
    i = 0;
    while (i < stats -> session_info -> streak) {
        if (stats->session_info->streak < 5) {
            strcat(stats -> session_info -> streak_prog, "-");
        } else {
            strcat(stats -> session_info -> streak_prog, "=");
        }
    }
}

    /*
     * Generate a new session and apply it to user status.
     */
void update_session(status *stats) {
    stats -> session_info = generate_session();
    stats -> session_info -> session = stats -> sessions;
}

    /*
     * Update status to account for session/wrongtally
     */
void update_status(status *stats) {
    // check session status
    if (stats -> total % 12 == 0) {
        stats -> sessions++;
        update_session(stats);
    }
}