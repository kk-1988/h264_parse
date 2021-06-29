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
    

}NALU_t;