#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ts_list.h>
#include <tslib.h>
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

    if((pFile = fopen("ocn_203.ts","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }

    /*************************************************************************/
    init_ts_pat_program_list();    
    
    TS_PAT_TABLE *pat_table_head = parse_pat_table(pFile, packetLength);
    show_pat_table_info(pat_table_head);
   
#if 0
    init_ts_pmt_stream_list();
    unsigned int programPID = search_given_program_info(102);    
    TS_PMT_TABLE *pmt_table_one_program_head = parse_pmt_table_one_program(pFile, packetLength,102, programPID);
    show_pmt_table_info_one_program(pmt_table_one_program_head);
    show_pmt_stream_info();
#endif
  
    init_ts_pmt_stream_list();
    setup_pmt_stream_list(pFile, packetLength);
    show_pmt_stream_info();

    unsigned int searchProgramId = 0; 
    printf("\nInput your service id(program_number): ");
    if(1 != scanf("%d",&searchProgramId))
    {
        uprintf("scanf data failed");
    }

    unsigned int programUserPid = search_given_program_info(searchProgramId);
    TS_PMT_TABLE *pmt_table_one_program_head = parse_pmt_table_one_program(pFile, packetLength,
            searchProgramId, programUserPid);

    show_pmt_table_info_one_program(pmt_table_one_program_head);
    printf("\nInput the elementary_PID to store :");
    scanf("%s",storeIdStream);

    if((pSaveFile = fopen(storeIdStream,"wb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }

    int ret = store_pes_stream(pFile,pSaveFile,packetLength,
            atoi(storeIdStream));
    
    free_pat_table(pat_table_head);

    fclose(pSaveFile);
    fclose(pFile);


    return 0;
}


