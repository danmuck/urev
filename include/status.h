#ifndef UREV_STATUS_H
#define UREV_STATUS_H

#include "structs.h"
#include <ncurses.h>

void print_status(status input);
void n_print_status(status *stats);
void check_wrongtally(status *stats);
void update_session(status *stats);
void update_status(status *stats);

#endif //UREV_STATUS_H
