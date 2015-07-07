#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ts_list.h>
#include <tslib.h>
#include <print_debug.h>
#include <tdt.h>

#define BUFFER_SIZE     (4096)

int main(int argc, char * argv[])
{
   
    FILE *pFile;
    unsigned int packetLength = TS_PACKET_SIZE;    

    if((pFile = fopen("ocn_203.ts","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }
    /**************************************************************************/
    TS_TDT_TABLE * tdt_table_head = parse_tdt_table(pFile, packetLength);

    show_tdt_table_info(tdt_table_head);
    free_tdt_table(tdt_table_head);
    tdt_table_head = NULL;
    /**************************************************************************/
    
    fclose(pFile);

    return 0;
}


