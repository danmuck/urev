#ifndef UREV_FILES_H
#define UREV_FILES_H

status* read_status_file(status* status);
void update_status_file(status* status);
void append_to_errors(full_problem current, status* stats, int* answers);
void append_to_problems(full_problem current, status* stats, int* answers);

#endif //UREV_FILES_H
