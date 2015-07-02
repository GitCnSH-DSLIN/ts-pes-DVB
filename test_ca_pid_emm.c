#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ts_list.h>
#include <tslib.h>
#include <ca_pid_emm.h>
#include <pat.h>
#include <pmt.h>
#include <pes.h>
#include <print_debug.h>
#include <sdt.h>

#define BUFFER_SIZE     (4096)

int main(int argc, char * argv[])
{
   
    FILE *pFile, *pSaveFile;
    unsigned int packetLength = TS_PACKET_SIZE;    
    unsigned char storeIdStream[20]={'\0'};

    //if((pFile = fopen("ocn_203.ts","rb")) == NULL)
    if((pFile = fopen("EMMStressSoakTest_V3.11.9.mpg","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }

    unsigned int caPid = 0x500;
    unsigned int ts_packet_length = TS_PACKET_LENGTH;
    CA_PID_EMM * pcaPidEmmHeader = (P_CA_PID_EMM)malloc(sizeof(CA_PID_EMM));
    pcaPidEmmHeader->next_section = NULL;
    load_ca_pid_emm(pFile, caPid, pcaPidEmmHeader, ts_packet_length);
    
    show_ca_pid_emm_list_info(pcaPidEmmHeader);
    free_ca_pid_emm_list(pcaPidEmmHeader);
    
    fclose(pFile);

    return 0;
}


