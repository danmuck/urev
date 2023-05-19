#ifndef UREV_GENERATORS_H
#define UREV_GENERATORS_H

#include "structs.h"

problem generate_permissions();
session_status* generate_session();
status* generate_status();
char generate_file_type();
full_problem generate_full_problem(int sticky_bit_include);

#endif //UREV_GENERATORS_H
