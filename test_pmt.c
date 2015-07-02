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

    /*************************************************************************/
    init_ts_pat_program_list();    
    
    TS_PAT_TABLE *pat_table_head = parse_pat_table(pFile, packetLength);
    show_pat_table_info(pat_table_head);
    show_pat_program_info();
    
    init_ts_pmt_stream_list();
        
    TS_PMT_TABLE *pmt_table_one_program_head = parse_pmt_table_one_program(pFile, packetLength, 130);
    show_pmt_table_info_one_program(pmt_table_one_program_head);
    show_pmt_stream_info();
    
//    free_pat_table(pat_table_head);
	
    fclose(pFile);


    return 0;
}


