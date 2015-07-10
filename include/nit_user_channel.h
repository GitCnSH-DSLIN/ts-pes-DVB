/*********************************************************************
*
* Filename      :   nit_user_channel.h
* Description   :   fundamental struction of PAT table
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef  _MPEG_TS_SI_NIT_USER_CHANNEL_H_
#define  _MPEG_TS_SI_NIT_USER_CHANNEL_H_

typedef struct nit_freq_info{

    unsigned int frequency;
    struct list_head list;

}NIT_FREQ_INFO,*P_NIT_FREQ_INFO;

NIT_FREQ_INFO  __ts_nit_freq_list;

void init_ts_nit_freq_list(void);

void find_user_channel_from_given_freq(FILE *pFile, unsigned int packetLength, unsigned int given_freq);

void search_freq_list_from_nit(TS_NIT_TABLE * nit_table_head);

int setup_user_channel_list_from_nit(FILE *pFile, unsigned int packetLength, unsigned int default_freq);

int write_file_user_channel_info();
int read_user_channel_from_storefile();

void free_nit_freq_list_info();

int show_nit_freq_list_info(void);

#endif


