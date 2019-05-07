#ifndef TTY_H
#define TTY_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

typedef unsigned char uchar;

int tty_init();
int tty_end();
int tty_read(uchar *buf,int nbytes);
int tty_write(uchar *buf,int nbytes);
int tty_writecmd(uchar *buf,int nbytes);
void tty_fflush();


#endif // TTY_H
