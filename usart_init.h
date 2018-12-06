#ifndef USART_INIT_H
#define USART_INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>

 int OpenDev(char *dev, int speed,int databits,int stopbits,int parity);

#endif // USART_INIT_H
