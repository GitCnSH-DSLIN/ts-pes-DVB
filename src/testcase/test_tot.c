#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ts_list.h>
#include <tslib.h>
#include <print_debug.h>
#include <tot.h>

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
    TS_TOT_TABLE * tot_table_head = parse_tot_table(pFile, packetLength);

    show_tot_table_info(tot_table_head);
    free_tot_table(tot_table_head);
    tot_table_head = NULL;
    /**************************************************************************/
    
    fclose(pFile);

    return 0;
}


