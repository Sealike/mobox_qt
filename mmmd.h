#ifndef MMMD_H
#define MMMD_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define R_memset(x, y, z) memset(x, y, z)
#define R_memcpy(x, y, z) memcpy(x, y, z)
#define R_memcmp(x, y, z) memcmp(x, y, z)
typedef unsigned long UINT4;
typedef unsigned char *POINTER;
/* MD5 context. */
typedef struct {
   /* state (ABCD) */
   /*??32bits?,?????????????????????>512bits?,???????512bits?????*/
   UINT4 state[4];
   /* number of bits, modulo 2^64 (lsb first) */
   /*???????bits???,??????bits,??? 2^64 bits,??2^64???64??????*/
   UINT4 count[2];

   /* input buffer */
   /*???????????,512bits*/
   unsigned char buffer[64];
} MD5_CTX;
void MD5Init(MD5_CTX *);
void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
void MD5Final(unsigned char [16], MD5_CTX *);
int	MDDDn(unsigned char *input,unsigned int Degistlen,unsigned char *output);
#endif // MMMD_H
