#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tslib.h>
#include <print_debug.h>
#include <user_channel.h>

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
    setup_user_channel_list(pFile, packetLength);
    show_user_channel_info_list();
    
    //init_ts_pmt_stream_list();
    //setup_pmt_stream_list(pFile, packetLength);
    
    //show_pmt_stream_info();


    fclose(pFile);


    return 0;
}



