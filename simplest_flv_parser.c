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
            case TAG_TYPE_AUDIO:{
                CHAR audiotag_str[100] = {0};
                strcat(audiotag_str, "| ");
                char tagdata_first_byte;
                tagdata_first_byte = fgetc(ifh);
                int x = tagdata_first_byte&0xF0;
                x = x >> 4;
                switch(x)
                {
                    case 0:strcat(audiotag_str, "Linear PCM, platform endian");break;
                    case 1:strcat(audiotag_str, "ADPCM");break;
                    case 2:strcat(audiotag_str, "MP3");break;
                    case 3:strcat(audiotag_str, "Linear PCM, little endian");break;
                    case 4:strcat(audiotag_str, "Nellymoser 16-kHz mono");break;
                    case 5:strcat(audiotag_str, "Nellymoser 8-kHz mono");break;
                    case 6:strcat(audiotag_str, "Nellymoser");break;
                    case 7:strcat(audiotag_str, "G.711 A-law logarithmic PCM");break;
                    case 8:strcat(audiotag_str, "G.711 mu-law logarithmic PCM");break;
                    case 9:strcat(audiotag_str, "reserved");break;
                    case 10:strcat(audiotag_str, "AAC");break;
                    case 11:strcat(audiotag_str, "Speex");break;
                    case 14:strcat(audiotag_str, "MP3 8-Khz");break;
                    case 15:strcat(audiotag_str, "Device-specific sound");break;
                    default:strcat(audiotag_str, "UNKNOWN");break;
                }
                strcat(audiotag_str,"| ");
                x = tagdata_first_byte&0x0C;
                x = x >> 2;
                switch(x)
                {
                    case 0:strcat(audiotag_str,"5.5-kHz");break;
                    case 1:strcat(audiotag_str,"1-kHz");break;
                    case 2:strcat(audiotag_str,"22-kHz");break;
                    case 3:strcat(audiotag_str,"44-kHz");break;
                    default:strcat(audiotag_str,"UNKNOWN");break;
                }
                strcat(audiotag_str, "| ");
                x = tagdata_first_byte&0x02;
                x = x >> 1;
                switch(x)
                {
                    case 0:strcat(audiotag_str, "8Bit");break;
                    case 1:strcat(audiotag_str, "16Bit");break;
                    default:strcat(audiotag_str, "UNKNOW");break;
                }
                strcat(audiotag_str, "| ");
                x=tagdata_first_byte&0x01;
                switch(x)
                {
                    case 0:strcat(audiotag_str, "Mono");break;
                    case 1:strcat(audiotag_str, "Stereo");break;
                    default:strcat(audiotag_str, "UNKNOW);break;
                }
                fprintf(myout, "%s", audiotag_str);

                if(output_a != 0 && afg == NULL){
                    afg = fopen("output.mp3", "wb");
                }
            }

        }

    }
}

int main(int argc,char *argv[])
{
    
    return 0;
}
