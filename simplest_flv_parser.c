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

   TAG_HEADER tagheader;
   uint previoustagsize,previoustagsize_z = 0;
   uint ts=0,ts_new=0;

   ifh = fopen(url, "rb+");
   if( ifh == NULL)
   {
       printf("Failed to open files!\r\n");
       return -1;
   }
  
   //FLV file header
    fread((char *)&flv, 1, sizeof(FLV_HEADER), ifh);

    fprintf(myout,"============== FLV Header ==============\n");
	fprintf(myout,"Signature:  0x %c %c %c\n",flv.Signature[0],flv.Signature[1],flv.Signature[2]);
	fprintf(myout,"Version:    0x %X\n",flv.Version);
	fprintf(myout,"Flags  :    0x %X\n",flv.Flags);
	fprintf(myout,"HeaderSize: 0x %X\n",reverse_bytes((byte *)&flv.DataOffset, sizeof(flv.DataOffset)));
	fprintf(myout,"========================================\n");

    fseek(ifh, reverse_bytes((byte *)&flv.DataOffset, sizeof(flv.DataOffset)), SEEK_SET);

    //process each tag
    do 
    {
        previoustagsize = _getw(ifh);
        fread((void)&taghrader, sizeof(TAG_HEADER), 1, ifh);

        int tagheader_datasize = tagheader.DataSize[0]*65536  + tagheader.DataSize[1]*256  + tagheader.DataSize[2];
        int tagheader_timestamp= tagheader.Timestamp[0]*65536 + tagheader.Timestamp[1]*256 + tagheader.Timestamp[2];

        char tagtype_str[10];
        switch(tagheaer.TagType)
        {
            case TAG_TYPE_AUDIO:
                sprintf(tagtype_str,"AUDIO");
            break;

		    case TAG_TYPE_VIDEO:
                sprintf(tagtype_str,"VIDEO");
            break;

		    case TAG_TYPE_SCRIPT:
                sprintf(tagtype_str,"SCRIPT");
            break;
            
		    default:
                sprintf(tagtype_str,"UNKNOWN");
            break;
        }

        fprintf(myout, "[%6s ] %d %d |,", tagtype_str, tagheader_datasize, tagheader_timestamp);

        if(feof(ifh))
            break;

        switch(tagheader.TagType)
        {
            

        }

    }
}

int main(int argc,char *argv[])
{
    
    return 0;
}
