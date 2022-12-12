#ifndef _EXCEPTION_CREATOR_H_
#define _EXCEPTION_CREATOR_H_

void create_prefetch_abort();
void create_supervisor_call();
void create_undefined_instruction();
void create_data_abort();
void wait_for_int();

#endif