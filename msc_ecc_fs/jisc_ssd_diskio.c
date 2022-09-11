#include "./src/pff3a/diskio.h"

extern void read_fs(unsigned char* buff, uint32_t lba, unsigned int offset, unsigned count);

DSTATUS disk_initialize(void)
{
    //マウント処理はないので常に成功する
    return RES_OK;
}

DRESULT disk_readp(
    BYTE* buff,
    DWORD lba,
    UINT offset,
    UINT count
){
    read_fs(buff, lba, offset, count);
    return RES_OK;
}

/*
DRESULT disk_writep(
    const BYTE* buff,
    DWORD lba_or_buffsize
){
    if(!buff){
        if(lba){
            // start_transaction
        }else{
            // end_transaction
        }
    }else{
        //buffer write
    }

    return RES_OK;
}
*/