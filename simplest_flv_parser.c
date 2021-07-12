#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)


#define TAG_TYPE_SCRIPT 18
#define TAG_TYPE_AUDIO  8
#define TAG_TYPE_VIDEO  9

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct
{
    byte Signature[3];
    byte Version;
    byte Flags;
    uint DataOffset;
}FLV_HEADER;

typedef struct{
    byte TagType;
    byte DataSize[3];
    byte Timestamp[3];
    uint Reserved;
}TAG_HEADER;

uint reverse_bytes(byte *p, char c){
    int r = 0;
    int i;
    for(i = 0;i < c;i++)
        r |= ( *(p+i) << (((c - 1) * 8) - 8*i));

    return r;
}

int simplest_flv_parser(char *url)
{
   int output_a = 1;
   int output_v = 1;

   FILE*ifh = NULL,*vfg = NULL, *afg = NULL;

   FILE *myout = stdout;

   FLV_HEADER flv;



}

int main(int argc,char *argv[])
{
    
    return 0;
}
