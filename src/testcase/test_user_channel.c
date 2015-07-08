#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ts_list.h>
#include <tslib.h>
#include <pat.h>
#include <pmt.h>
#include <print_debug.h>
#include <sdt.h>


int main(int argc, char * argv[])
{
   
    FILE *pFile;
    unsigned int packetLength = TS_PACKET_SIZE;    

    if((pFile = fopen("ocn_203.ts","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }

    /*************************************************************************/
    init_ts_pat_program_list();    
    
    TS_PAT_TABLE *pat_table_head = parse_pat_table(pFile, packetLength);
    //show_pat_table_info(pat_table_head);
    show_pat_program_info();
   
    
    //init_ts_pmt_stream_list();
    //setup_pmt_stream_list(pFile, packetLength);
    
    //show_pmt_stream_info();


    free_pat_table(pat_table_head);
    fclose(pFile);


    return 0;
}



