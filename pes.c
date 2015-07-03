/*********************************************************************
*
* Filename      :   pes.c
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tslib.h>
#include <pmt.h>
#include <pat.h>
#include <ts_list.h>
#include <print_debug.h>

int store_pes_stream(FILE *pFile, FILE *storeFile,
        unsigned int ts_packet_length, unsigned int mElementaryPid)
{
    TS_PACKET_HEADER mtsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &mtsPacketHeader;

    unsigned char * tmpBuffer = (unsigned char *)malloc(ts_packet_length);
    unsigned char * pFreetmpBuffer = tmpBuffer;
    unsigned int pes_start;
    unsigned int offsetLength = 0;
        
    while((fread(tmpBuffer, ts_packet_length, 1, pFile) == 1))
    {

        parse_ts_packet_header(&mtsPacketHeader, tmpBuffer);
       
        if (mElementaryPid == ptsPacketHeader->pid)
        {
            offsetLength = locate_offset(ptsPacketHeader, tmpBuffer, PES_PACKET_FLAG, &pes_start);
#ifdef DEBUG_DETAIL
            uprintf("the offsetLength is %d\n",offsetLength);
            show_ts_packet_header(ptsPacketHeader);
            show_packet_memory(tmpBuffer,ts_packet_length);
#endif
            if(1 != fwrite(tmpBuffer + offsetLength, ts_packet_length - offsetLength, 1, storeFile))
            {
                uprintf("fwrite failed \n");
                return 1;
            }
        }
    }

    free(pFreetmpBuffer);
    return 0;
}














