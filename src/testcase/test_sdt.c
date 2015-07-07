#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ts_list.h>
#include <tslib.h>
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
    /**************************************************************************/
    TS_SDT_TABLE * sdt_table_head = parse_sdt_table(pFile, packetLength,TABLE_ID_SDT_ACTUAL);

    show_sdt_table_info(sdt_table_head);
    free_sdt_table(sdt_table_head);
    sdt_table_head = NULL;
    /**************************************************************************/
    
    fclose(pFile);

    return 0;
}


