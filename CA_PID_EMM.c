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
#include <ts_psi.h>
#include <ts_list.h>
#include <print_debug.h>

CA_PID_EMM * alloc_init_capid_emm(unsigned char * buffer)
{
    CA_PID_EMM * ptmp = (CA_PID_EMM *)malloc(sizeof(CA_PID_EMM));
    unsigned char * pData = (unsigned char *)malloc(sizeof(char)*CA_PID_EMM_SECTION_LENGTH(buffer));
    
    ptmp->table_id          =   CA_PID_EMM_TABLE_ID(buffer);
    ptmp->section_length    =   CA_PID_EMM_SECTION_LENGTH(buffer);
    ptmp->pdata             =   pData;
    ptmp->next_section      =   NULL;

    uprintf("table_id           :   0x%x\n",ptmp->table_id);
    uprintf("section_length     :   0x%x\n",ptmp->section_length);

    return ptmp;
}

CA_PID_EMM * ca_pid_list_last(CA_PID_EMM * Header)
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
    CA_PID_EMM * lastNode = ca_pid_list_last(Header);

                 lastNode->next_section = newNode;
}



/*ret = 1 meaning reach the end. 0 : not end */

int store_one_packet_data(CA_PID_EMM * caPidEmmHeader, unsigned char *tmpbuffer,
        P_TS_PACKET_HEADER ptsPacketHeader)
{
    unsigned int offsetLength = 0;
    //unsigned int continuity_counter;
    unsigned int ret = 0;
    
    unsigned int pes_start;
    offsetLength = locate_offset(ptsPacketHeader, tmpbuffer, 1, &pes_start);
    
    //continuity_counter = ptsPacketHeader->continuity_counter;
    //unsigned int section_length = ((tmpbuffer[offsetLength + 2] & 0x0f)<<8) | (tmpbuffer[offsetLength + 3]);
    //we just write the simple situation. 
    //Now we ignore the adaption_field_control situation
    unsigned int point_field_length = 0;
    if (1 == ptsPacketHeader->payload_unit_start_indicator)
            point_field_length = tmpbuffer[4];

    //find the last_caPidEmmHeader
    CA_PID_EMM * last_caPidEmm = ca_pid_list_last(caPidEmmHeader);

    //is caPidEmmHeader  first packet
    unsigned char * b = tmpbuffer + offsetLength;
    unsigned int packet_remain;
    
    if(last_caPidEmm == caPidEmmHeader)
    {
        unsigned char * pData = (unsigned char *)malloc(sizeof(char)*CA_PID_EMM_SECTION_LENGTH(b));
    
        caPidEmmHeader->table_id          =   CA_PID_EMM_TABLE_ID(b);
        caPidEmmHeader->section_length    =   CA_PID_EMM_SECTION_LENGTH(b);
        caPidEmmHeader->pdata             =   pData;
        caPidEmmHeader->next_section      =   NULL;

        memcpy(caPidEmmHeader->pdata, tmpbuffer + offsetLength + 11,caPidEmmHeader->section_length);
        
        packet_remain = TS_PACKET_LENGTH - offsetLength - 11 - caPidEmmHeader->section_length;
        b = tmpbuffer + offsetLength + 11 + caPidEmmHeader->section_length;
    }
    else
    {
        //finish prev section copy
        if(0 != point_field_length)
        {
            int copyIndex = last_caPidEmm->section_length - point_field_length;
            memcpy(last_caPidEmm->pdata + copyIndex, tmpbuffer + 4, tmpbuffer[4]);
            
            packet_remain = TS_PACKET_LENGTH - offsetLength - 11;
            b = tmpbuffer + offsetLength + 11;
        }
    }

    
    while (packet_remain > 0)
    {    
        //allocate a new caPidEmm
        CA_PID_EMM * new_capidEmm = alloc_init_capid_emm(b);
        ca_pid_add_list(caPidEmmHeader, new_capidEmm);

        if(packet_remain < new_capidEmm->section_length)
        {
            memcpy(new_capidEmm->pdata, b, packet_remain);
            break;
        }
        else
            memcpy(new_capidEmm->pdata, b, new_capidEmm->section_length);
        
        packet_remain -= new_capidEmm->section_length;
        b += new_capidEmm->section_length;

        if(1 == CA_PID_IS_END(b))
        {
            ret = 1;
            break;
        }

        //following the section end is another section.
        //11byte section_information. and the section is in (byte[2]&0xf)<<4 | byte[3]
        if (packet_remain > 11)
        {
            packet_remain -= 11;
            b += 11;
        }
        else
            break;
    }

    return ret;
}



int load_ca_pid_emm(FILE *pFile, unsigned int CA_PID,
        CA_PID_EMM * caPidEmmHeader, unsigned int packetLength)
{
    unsigned char *tmpbuffer = (unsigned char *)malloc(packetLength);
    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;

    while((fread(tmpbuffer, packetLength, 1, pFile) == 1))
    {
        parse_ts_packet_header(ptsPacketHeader, tmpbuffer);

        if (CA_PID == ptsPacketHeader->pid)
        {
            if(1 == store_one_packet_data(caPidEmmHeader, tmpbuffer, ptsPacketHeader))
            {
                uprintf("We reach the end\n");
                break;
            }
        }
    }
    return 0;
}
