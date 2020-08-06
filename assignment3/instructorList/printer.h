#ifndef _PRINTER_H_
#define _PRINTER_H_


void Printer_init(pthread_mutex_t* CS_mutex);
void Printer_wait();
void Printer_signal();
void Printer_cancel();
void Printer_waitForShutdown();

#endif