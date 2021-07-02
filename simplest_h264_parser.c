#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    NALU_TYPE_SLICE     = 1,
    NALU_TYPE_DPA       = 2,
    NALU_TYPE_DPB       = 3,
    NALU_TYPE_DPC       = 4,
    NALU_TYPE_IDR       = 5,
    NALU_TYPE_SEI       = 6,
    NALU_TYPE_SPS       = 7,
    NALU_TYPE_PPS       = 8,
    NALU_TYPE_AUD       = 9,
    NALU_TYPE_EOSEQ     = 10,
    NALU_TYPE_EOSTREAM  = 11,
    NALU_TYPE_FILL      = 12,
} NaluType;

typedef enum {
    NALU_PRIORITY_DISOSABLE = 0,
    NALU_PRIRITY_LOW    = 1,
    NALU_PRIORITY_HIGH  = 2,
    NALU_PRIORITY_HIGHEST   = 3
};

typedef struct
{
    int startcodeprefix_len;
    unsigned len;
    unsigned max_size;
    int forbidden_bit;
    int nal_reference_idc;
    int nal_unit_type;
    char *buf;
}NALU_t;

FILE *h264bitstream = NULL;

int info2 = 0,info3 = 0;

static int FindStartCode2(unsigned char *Buf)
{
    if(Buf[0]!=0 || Buf[1]!=0 || Buf[2] != 1) return 0;
    else return 1;
}

int GetAnnexbNALU(NALU_t *nalu)
{
    int pos = 0;
    int StartCodeFound, rewind;
    unsigned char *Buf;

    if((Buf = (unsigned char *)calloc(nalu->max_size, sizeof(char))) == NULL)
        printf("GetAnnexbNALU：Could not allocate Buf memory\n");


        nalu->startcodeprefix_len = 3;

        if (3 != fread(Buf, 1, 3, h264bitstream)){
            free(Buf);
            return 0;
        }

        info2 = FindStartCode2(Buf);
        if(info2 != 1)
        {
            if(1 != fread(Buf+3,1,1, h264bitstream))
            {
                free(Buf);
                return 0;
            }
            info3 = FindStartCode3(Buf);
            if(info3 != 1)
            {
                free(Buf);
                return -1;
            }
            else
            {
                pos = 4;
                nalu->startcodeprefix_len = 4;
            }
        }
        else
        {
            nalu->startcodeprefix_len = 3;
            pos = 3;
        }

        StartCodeFound = 0;
        info2 = 0;
        info3 = 0;

        while(!StartCodeFound)
        {
            if(feof(h264bitstream))
            {
                nalu->len = (pos - 1)-nalu->startcodeprefix_len;
                memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
                nalu->forbidden_bit = nalu->buf[0] & 0x80;  //1bit
                nalu->nal_reference_idc = nalu->buf[0] & 0x60;  //2bit
                nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;    //5bit
                free(Buf);
                return pos - 1;
            }    
            Buf[pos++] = fgetc(h264bitstream);
            info3 = FindStartCodes(&Buf[pos - 4]);
            if(info3 != 1)
                info2 = FindStartCode2(&Buf[pos - 3]);
            StartCodeFound = (info2 == 1 || info3 == 1);
        }

        rewind = (info3 == 1)?-4:-3;

        if(0 != fseek(h264bitstream, rewind, SEEK_CUR)){
            free(Buf);
            printf("GetAnnexbNALU:Candnot fseek in the bit stream file");
        }

        nalu->len = (pos + rewind) - nalu->startcodeprefix_len;
        memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
        nalu->forbidden_bit = nalu->buf[0] & 0x80;      //1bit
        nalu->nal_reference_idc = nalu->buf[0] & 0x60;  //2bit
        nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;    //5bit
        free(Buf);

        return (pos + rewind);
}

