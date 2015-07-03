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
#include <nit.h>

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
    /**************************************************************************/
    TS_NIT_TABLE * nit_table_head = parse_nit_table(pFile, packetLength);

    show_nit_table_info(nit_table_head);
    //free_sdt_table(sdt_table_head);
    //sdt_table_head = NULL;
    /**************************************************************************/
    
    fclose(pFile);

    return 0;
}


