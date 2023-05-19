#ifndef UREV_STRUCTS_H
#define UREV_STRUCTS_H

#include <time.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct {
    int session;
    int correct;
    int incorrect;
    int streak;
    char* wrongtally;
    char* streak_prog;
} session_status;

typedef struct {
    int score;
    int wrong;
    int wrong_tally;
    int total;
    int sessions;
    int completion;
    time_t timestamp;
    session_status *session_info;
} status;

typedef struct {
    char* permission_str;
    int solution;
} problem;

typedef struct {
    problem problems[3];
    int sticky_bit;
} full_problem;

#endif //UREV_STRUCTS_H

