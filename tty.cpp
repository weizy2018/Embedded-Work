#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tty.h"

//#define BAUDRATE B9600
#define BAUDRATE B38400
#define COM2 "/dev/ttySAC2"
//#define COM2 "/dev/tts/2"

static int fd;
static struct termios oldtio,newtio;

//==============================================================
int tty_end()
{
    tcsetattr(fd,TCSANOW,&oldtio);	 	/* restore old modem setings */
    close(fd);
    return 0;
}

//==============================================================
int tty_read(uchar *buf,int nbytes)
{
    return read(fd,buf,nbytes);
}
//==============================================================
int tty_write(uchar *buf,int nbytes)
{
    int i;
    for(i=0; i<nbytes; i++) {
        write(fd,&buf[i],1);
        //usleep(100);
    }
    return tcdrain(fd);
}

//==============================================================
int tty_writecmd(uchar *buf,int nbytes)
{
    int i;
    for(i=0; i<nbytes; i++) {
        write(fd,&buf[i],1);
        usleep(100);
    }
    write(fd,"\r",1);
    usleep(300000);
    return tcdrain(fd);
}

//==============================================================
int tty_init()
{
    fd = open(COM2, O_RDWR ); //| O_NONBLOCK);//
    if (fd <0) {
        perror(COM2);
        printf("int tty_init() *******************tty init fail**********************");
        return -1;
    }

    tcgetattr(fd,&oldtio); /* save current modem settings */
    bzero(&newtio, sizeof(newtio));

//	newtio.c_cflag = oldstdtio.c_cflag;
//	newtio.c_iflag = oldstdtio.c_iflag;
//	newtio.c_oflag = oldstdtio.c_oflag;
// 	newtio.c_lflag = oldstdtio.c_lflag;

    newtio.c_cflag = BAUDRATE | /*CRTSCTS |*/ CS8 | CLOCAL | CREAD ;
    newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;

    newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
    newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    newtio.c_cc[VERASE]   = 0;     /* del */
    newtio.c_cc[VKILL]    = 0;     /* @ */
    newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
    newtio.c_cc[VTIME]    = 0;     /* ²»Ê¹ÓÃ·Ö¸î×ÖÔª×éµÄ¼ÆÊ±Æ÷ */
    newtio.c_cc[VMIN]     = 1;     /* ÔÚ¶ÁÈ¡µ½ 1 ¸ö×ÖÔªÇ°ÏÈÍ£Ö¹ */
    newtio.c_cc[VSWTC]    = 0;     /* '\0' */
    newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
    newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    newtio.c_cc[VEOL]     = 0;     /* '\0' */
    newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    newtio.c_cc[VEOL2]    = 0;     /* '\0' */


//	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
//	newtio.c_oflag  &= ~OPOST;   /*Output*/
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);/*set attrib	  */

#if 0
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
    newtio.c_iflag = IGNPAR; /*input flag*/
    newtio.c_oflag = 0;		/*output flag*/
    newtio.c_lflag = 0;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
//#else
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;/*ctrol flag*/
//	newtio.c_iflag = IGNPAR; /*input flag*/
    newtio.c_iflag &= ~(ICRNL|IGNCR | INLCR |IGNBRK |BRKINT ); //input flag
    newtio.c_oflag &= ~OPOST;		/*output flag*/
    newtio.c_lflag = ECHO |NOFLSH;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
#endif


/* now clean the modem line and activate the settings for modem */
// 	tcflush(fd, TCIFLUSH);
//	tcsetattr(fd,TCSANOW,&newtio);/*set attrib	  */
//	signal(SIGTERM,do_exit);
//	signal(SIGQUIT,do_exit);
//	signal(SIGINT,do_exit);
    return 0;
}

void tty_fflush(){
    tcflush(fd,TCIFLUSH);//Ë¢ÐÂÊäÈëÊä³ö»º³å
}
