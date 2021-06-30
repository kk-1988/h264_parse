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
    {
        printf("GetAnnexbNALU：Could not allocate Buf memory\n");
        nalu->startcodeprefix_len = 3;

        
    }

}

