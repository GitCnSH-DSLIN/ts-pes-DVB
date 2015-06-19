#include <stdio.h>
#include <stdlib.h>
#include <ts_psi.h>
#include <ts_list.h>
#include <print_debug.h>

int main(int argc, char * argv[])
{
   
    FILE *pFile, *pSaveFile;
    TS_PACKET_HEADER mtsPacketHeader;
    TS_PAT_TABLE mtsPatTable;
    TS_PMT_TABLE mtsPmtTable;
    unsigned char tmpPacketBuffer[188] = {'\0'};
    unsigned int packetLength = TS_PACKET_SIZE;
    unsigned int searchProgramId, programUserPid;
    unsigned char storeIdStream[20]={'\0'};
    int ret;

    if((pFile = fopen("ocn_203.ts","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }
    init_ts_psi_list();    

    ret = find_given_table(pFile, tmpPacketBuffer, packetLength,
            &mtsPacketHeader, PID_TS_PAT);
    if(-1 == ret)
    {
        uprintf("Can't find given table\n");
        return -1;
    }

    parse_pat_table(tmpPacketBuffer, &mtsPacketHeader, &mtsPatTable);
    setup_pmt_stream_list(pFile, packetLength);

    /**************************************************************************/
    printf("\nInput your service id(program_number): ");
    if(1 != scanf("%d",&searchProgramId))
    {
        uprintf("scanf data failed");
    }

    programUserPid = search_given_program_info(searchProgramId);
    
    ret = find_given_table(pFile, tmpPacketBuffer, packetLength,
            &mtsPacketHeader, programUserPid);
    
    if(-1 == ret)
    {
        uprintf("Can't find the PMT table\n");
        return -1;
    }

    parse_pmt_table(tmpPacketBuffer,searchProgramId, &mtsPacketHeader, &mtsPmtTable);

    /**************************************************************************/
    printf("\nInput the elementary_PID to store :");
    scanf("%s",storeIdStream);

    if((pSaveFile = fopen(storeIdStream,"wb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }

    ret = store_pes_stream(pFile,pSaveFile,packetLength,
            atoi(storeIdStream));

	fclose(pFile);
    fclose(pSaveFile);

    return 0;
}

