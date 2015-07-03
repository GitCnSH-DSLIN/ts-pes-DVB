#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ts_psi.h>
#include <ts_list.h>
#include <ca_pid_emm.h>
#include <print_debug.h>
#include <sdt.h>

#define BUFFER_SIZE     (4096)

int main(int argc, char * argv[])
{
   
    FILE *pFile, *pSaveFile;
    //TS_PACKET_HEADER mtsPacketHeader;
    TS_PAT_TABLE mtsPatTable;
    TS_PMT_TABLE mtsPmtTable;
    //unsigned char tmpPacketBuffer[188] = {'\0'};
    unsigned int packetLength = TS_PACKET_SIZE;
    unsigned int searchProgramId, programUserPid;
    unsigned char storeIdStream[20]={'\0'};
    int ret;
    int iCount = 0;
    
    unsigned char * tmpPacketBuffer = (unsigned char *)malloc(BUFFER_SIZE);
    memset(tmpPacketBuffer,'\0', BUFFER_SIZE);

    if((pFile = fopen("ocn_203.ts","rb")) == NULL)
    //if((pFile = fopen("EMMStressSoakTest_V3.11.9.mpg","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }
    init_ts_psi_list();    

    /*************************************************************************/
//#if 0
    ret = find_given_table(pFile, tmpPacketBuffer, packetLength, PID_TS_PAT);
    if(-1 == ret)
    {
        uprintf("Can't find given table\n");
        return -1;
    }

    for(iCount = 0; iCount < ret; iCount++)
    {
        parse_pat_table(tmpPacketBuffer + packetLength*iCount, &mtsPatTable);
    }
    
    setup_pmt_stream_list(pFile, packetLength);
//#endif
    /**************************************************************************/
//#if 0
    printf("\nInput your service id(program_number): ");
    if(1 != scanf("%d",&searchProgramId))
    {
        uprintf("scanf data failed");
    }

    programUserPid = search_given_program_info(searchProgramId);
    
    ret = find_given_table(pFile, tmpPacketBuffer, packetLength, programUserPid);
    
    if(-1 == ret)
    {
        uprintf("Can't find the PMT table\n");
        return -1;
    }


    for(iCount = 0; iCount < ret; iCount++)
    {
        parse_pmt_table(tmpPacketBuffer + packetLength*iCount, searchProgramId, &mtsPmtTable);
    }

//#endif
    /**************************************************************************/
    TS_SDT_TABLE mtsSdtTable;

    //ret = find_given_table_more(pFile, tmpPacketBuffer, packetLength, PID_TS_SI_SDT, TABLE_ID_SDT_OTHER);
    ret = find_given_table_more(pFile, tmpPacketBuffer, packetLength, PID_TS_SI_SDT, TABLE_ID_SDT_ACTUAL);

    if(-1 == ret)
    {
        uprintf("Can't find the SDT table\n");
        return -1;
    }

    for(iCount = 0; iCount < ret; iCount++)
    {
        parse_sdt_table(tmpPacketBuffer + packetLength*iCount,packetLength, &mtsSdtTable);
    }

    free_sdt(&mtsSdtTable);

    /**************************************************************************/
#if 0
    printf("\nInput the elementary_PID to store :");
    scanf("%s",storeIdStream);

    if((pSaveFile = fopen(storeIdStream,"wb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }

    ret = store_pes_stream(pFile,pSaveFile,packetLength,
            atoi(storeIdStream));
    //fclose(pSaveFile);
#endif
    /**************************************************************************/
#ifdef DEFINE CA_PID_EMM
    unsigned int caPid = 0x500;
    unsigned int ts_packet_length = TS_PACKET_LENGTH;
    CA_PID_EMM * pcaPidEmmHeader = (P_CA_PID_EMM)malloc(sizeof(CA_PID_EMM));
    pcaPidEmmHeader->next_section = NULL;
    load_ca_pid_emm(pFile, caPid, pcaPidEmmHeader, ts_packet_length);
    
    show_ca_pid_emm_list_info(pcaPidEmmHeader);
    free_ca_pid_emm_list(pcaPidEmmHeader);
#endif
    /**************************************************************************/

	fclose(pFile);

    free(tmpPacketBuffer);

    return 0;
}

