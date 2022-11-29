#ifndef _REGCHECK_H_
#define _REGCHECK_H_

#include <config.h>
#include <lib/utils.h> // Platzahlter für die Headerdatei, die euer kprintf enthält

void _check_return_jump(void);
void _check_registers(void);
void _check_cpsr(void);
void _check_spsr(void);

void register_checker(void);

#endif //_REGCHECK_H_
