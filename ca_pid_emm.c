/*********************************************************************
*
* Filename      :   CA_PID_EMM.c
* Description   :   parse the CA_PID_EMM data.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ca_pid_emm.h>
#include <tslib.h>
#include <ts_list.h>
#include <print_debug.h>

CA_PID_EMM * alloc_init_capid_emm(unsigned char * buffer)
{
    CA_PID_EMM * ptmp = (CA_PID_EMM *)malloc(sizeof(CA_PID_EMM));
    unsigned char * pData = (unsigned char *)malloc(sizeof(char)*(CA_PID_EMM_SECTION_LENGTH(buffer)-8));
    
    ptmp->table_id          =   CA_PID_EMM_TABLE_ID(buffer);
    ptmp->section_length    =   CA_PID_EMM_SECTION_LENGTH(buffer);
    ptmp->pdata             =   pData;
    ptmp->next_section      =   NULL;
    ptmp->cur_pdata_length  =   0;

//    uprintf("table_id           :   0x%x\n",ptmp->table_id);
//    uprintf("section_length     :   0x%x\n",ptmp->section_length);

    return ptmp;
}


CA_PID_EMM * ca_pid_list_lastnode(CA_PID_EMM * Header)
{
    CA_PID_EMM * tmp = Header;
    CA_PID_EMM * cur = tmp;
    while(tmp != NULL)
    {
        cur = tmp;
        tmp = tmp->next_section;
    }
    return cur;
}


CA_PID_EMM *ca_pid_add_list(CA_PID_EMM *Header, CA_PID_EMM *newNode)
{
    CA_PID_EMM * lastNode = ca_pid_list_lastnode(Header);

                 lastNode->next_section = newNode;
}


void show_section_info(CA_PID_EMM * pcaPidEmm)
{

    uprintf("CA_PID_EMM->table_id           :   0x%x\n",pcaPidEmm->table_id);
    uprintf("CA_PID_EMM->section_length     :   0x%x\n",pcaPidEmm->section_length);
    //uprintf("CA_PID_EMM->cur_pdata_length   :   0x%x\n\n",pcaPidEmm->cur_pdata_length);

    unsigned int looplength = pcaPidEmm->section_length - SECTION_INFO_AFTER_SIZE;
    unsigned char * pData = pcaPidEmm->pdata;
    int i;

    for(i = 0; i < looplength; i++)
    {
        uprintf("%02X ", pData[i]);
        if((i + 1)%16 == 0)
            uprintf("\n");
    }
    uprintf("\n\n\n\n");

}

void show_ca_pid_emm_list_info(CA_PID_EMM *pcaPidEmmHeader)
{
    CA_PID_EMM * tmp = pcaPidEmmHeader;
    while(NULL != tmp)
    {
        show_section_info(tmp); 
        tmp = tmp->next_section;
    }
}

void free_ca_pid_emm_list(CA_PID_EMM *pcaPidEmmHeader)
{
    CA_PID_EMM * tmp = pcaPidEmmHeader;
    CA_PID_EMM * cur = tmp;
    while(NULL != tmp )
    {
        free(tmp->pdata);
        tmp->pdata = NULL;
        
        cur = tmp;
        tmp = tmp->next_section;
        free(cur);
    }
}



/*ret = 1 meaning reach the end. 0 : not end */

int store_one_packet_data(CA_PID_EMM * caPidEmmHeader, unsigned char *tmpbuffer,
        P_TS_PACKET_HEADER ptsPacketHeader)
{
    unsigned int offsetLength = 0;
    unsigned int ret = 0;
    
    unsigned int pes_start;
    offsetLength = locate_offset(ptsPacketHeader, tmpbuffer, 0, &pes_start);
    
    //we just write the simple situation. 
    //Now we ignore the adaption_field_control situation
    // the point_field area data follow and belong to the prev section. 
    unsigned int point_field_length = 0;
    if (1 == ptsPacketHeader->payload_unit_start_indicator)
            point_field_length = tmpbuffer[4];

    //find the last_caPidEmmHeader
    CA_PID_EMM * last_caPidEmm = ca_pid_list_lastnode(caPidEmmHeader);

    //is caPidEmmHeader  first packet
    unsigned char * b = tmpbuffer + offsetLength;
    unsigned int packet_remain;
    
    if(last_caPidEmm == caPidEmmHeader)
    {
        unsigned char * pData = (unsigned char *)malloc(sizeof(char)*(CA_PID_EMM_DATA_LENGTH(b)));
    
        caPidEmmHeader->table_id          =   CA_PID_EMM_TABLE_ID(b);
        caPidEmmHeader->section_length    =   CA_PID_EMM_SECTION_LENGTH(b);
        caPidEmmHeader->pdata             =   pData;
        caPidEmmHeader->next_section      =   NULL;
        // there are total 11 byte from table_id and related info. and have 8 byte after section_length. 
        memcpy(caPidEmmHeader->pdata, tmpbuffer + offsetLength + SECTION_INFO_SIZE,
                caPidEmmHeader->section_length - SECTION_DATA_LESS);

        packet_remain = TS_PACKET_LENGTH - offsetLength - SECTION_INFO_BEFORE_SIZE - caPidEmmHeader->section_length;
        b = tmpbuffer + offsetLength + SECTION_INFO_BEFORE_SIZE + caPidEmmHeader->section_length;
    }

    else
    {
        //to finish prev section copy
        if(0 != point_field_length)
        {
            //int copyIndex = last_caPidEmm->section_length - point_field_length - 8;
            memcpy(last_caPidEmm->pdata + last_caPidEmm->cur_pdata_length, tmpbuffer + 4 + 1, tmpbuffer[4]);
            packet_remain = TS_PACKET_LENGTH - offsetLength;
            b = tmpbuffer + offsetLength;
        }
        else
        {
            unsigned int copy_length =  last_caPidEmm->section_length - SECTION_INFO_AFTER_SIZE - last_caPidEmm->cur_pdata_length;
            memcpy(last_caPidEmm->pdata + last_caPidEmm->cur_pdata_length, tmpbuffer + 4, copy_length);
            packet_remain = TS_PACKET_LENGTH - offsetLength - copy_length;
            b = tmpbuffer + 4 + copy_length;
        }
    }

    
    while (packet_remain > 0)
    {    
        if(1 == CA_PID_EMM_IS_END(b))
        {
            ret = 1;
            break;
        }
        //allocate a new caPidEmm
        CA_PID_EMM * new_capidEmm = alloc_init_capid_emm(b);
        ca_pid_add_list(caPidEmmHeader, new_capidEmm);

        //now the packet_remain include 11 byte secion_info
        if(packet_remain < new_capidEmm->section_length + SECTION_INFO_BEFORE_SIZE)
        {
            //the 11 is the section info
            memcpy(new_capidEmm->pdata, b + SECTION_INFO_SIZE, packet_remain - SECTION_INFO_SIZE);
            new_capidEmm->cur_pdata_length = packet_remain - SECTION_INFO_SIZE;
            break;
        }
        else
        {
            memcpy(new_capidEmm->pdata, b + SECTION_INFO_SIZE, new_capidEmm->section_length - SECTION_INFO_AFTER_SIZE);
            new_capidEmm->cur_pdata_length = new_capidEmm->section_length - SECTION_INFO_AFTER_SIZE;
        }

        packet_remain -= (new_capidEmm->section_length + SECTION_INFO_BEFORE_SIZE);
        b += (new_capidEmm->section_length + SECTION_INFO_BEFORE_SIZE);

    }

    return ret;
}



int load_ca_pid_emm(FILE *pFile, unsigned int CA_PID,
        CA_PID_EMM * caPidEmmHeader, unsigned int packetLength)
{
    unsigned char *tmpbuffer = (unsigned char *)malloc(packetLength);
    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;
    int CountNum = 0;

    while((fread(tmpbuffer, packetLength, 1, pFile) == 1))
    {
        parse_ts_packet_header(ptsPacketHeader, tmpbuffer);

        if (CA_PID == ptsPacketHeader->pid)
        {
            printf("\n\n\tCount :   %d\n\n", CountNum);
            CountNum++;
            if(1 == store_one_packet_data(caPidEmmHeader, tmpbuffer, ptsPacketHeader))
            {
                uprintf("We reach the end\n\n");
                break;
            }
        }
    }
    return 0;
}
