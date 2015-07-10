#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tslib.h>
#include <print_debug.h>
#include <user_channel.h>
#include <nit_user_channel.h>

int main(int argc, char * argv[])
{
   
    FILE *pFile;
    unsigned int packetLength = TS_PACKET_SIZE;    

    if((pFile = fopen("/home/jensen/workspace/TsFile/ocn_203.ts","rb")) == NULL)
    {
        uprintf("Open file failed\n");
        return -1;
    }

    /*************************************************************************/
    setup_user_channel_list_from_nit(pFile,packetLength, 0x2030000);
    show_user_channel_info_list();
    

    fclose(pFile);
    return 0;
}




