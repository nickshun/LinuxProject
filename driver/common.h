/****
 * common.h
 *
 * Contains generic common global defines.
 * Serguei Mokhov
 *
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <linux/ioctl.h>

/* All these are to be treated as blanks or delimiters */
#define BLANK_CHARS       " \n\t\r'"

#define MAJOR_NUM       248

#define IOCTL_RESET _IO(MAJOR_NUM, 0) 
#define IOCTL_ZERO _IOW(MAJOR_NUM, 1, int) 
#define IOCTL_VALIDATE _IOR(MAJOR_NUM, 2, int) 

#endif
