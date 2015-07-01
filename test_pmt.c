#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ts_list.h>
#include <tslib.h>
#include <pat.h>
#include <pmt.h>
#include <print_debug.h>
#include <sdt.h>

#define BUFFER_SIZE     (4096)

int main(int argc, char * argv[])
{
   
    FILE *pFile, *pSaveFile;
    unsigned int packetLength = TS_PACKET_SIZE;    

    if((pFile = fopen("ocn_203.ts","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }
    //init_ts_pat_program_list();    

    /*************************************************************************/
#if 0
    TABLE_SECTION_LIST * pat_table = store_psi_si_table(pFile, packetLength, PID_TS_PAT, TABLE_ID_TS_PAT);
    TABLE_SECTION_LIST * pat_index = pat_table;

    P_TS_PAT_TABLE ptsPatTable = (TS_PAT_TABLE *)malloc(sizeof(TS_PAT_TABLE)*(pat_table->last_section_number + 1));
    unsigned int i=0;

    while(NULL != pat_index)
    {
        parse_pat_table(pat_index->pbuffer, ptsPatTable + i);
        show_pat_table_info(ptsPatTable + i);
        i++;
        pat_index = pat_index->next_section;
    }
    free(ptsPatTable);
#endif
    //TS_PAT_TABLE *pat_table_head = parse_pat_table(pFile, packetLength);
    //show_pat_table_info(pat_table_head);
    
    TS_PMT_TABLE *pmt_table_one_program_head = parse_pmt_table_one_program(pFile, packetLength, 147);
    show_pmt_table_info_one_program(pmt_table_one_program_head);
    
//    free_pat_table(pat_table_head);
	
    fclose(pFile);


    return 0;
}


