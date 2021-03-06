/*********************************************************************
*
* Filename      :   tslib.c
* Description   :   fundamental operation for ts_psi_si table.
* Edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <tslib.h>

//#define DEBUG_DETAIL

/*  
 *  Function    : Parse the header of ts packet header.
 */
int parse_ts_packet_header(TS_PACKET_HEADER *packet_head, unsigned char *buffer)
{
    packet_head->sync_byte                      =   buffer[0];
    packet_head->transport_error_indicator      =   buffer[1] >> 7;
    packet_head->payload_unit_start_indicator   =   buffer[1] >> 6 & 0x01;
    packet_head->transport_priority             =   buffer[1] >> 5 & 0x01;
    packet_head->pid                            =   ((buffer[1] & 0x1F) << 8) | buffer[2];
    packet_head->transport_scrambling_control   =   buffer[3] >> 6;
    packet_head->adaptation_field_control       =   (buffer[3] >> 4) & 0x03;

    packet_head->continuity_counter             =   buffer[3] & 0x0f;
    
    return 0;
}

int show_ts_packet_header(TS_PACKET_HEADER *packet_head)
{
    uprintf("transport_packet()\n");
    uprintf("sync_byte                      :  0x%X\n", packet_head->sync_byte);
    uprintf("transport_error_indicator      :  0x%X\n", packet_head->transport_error_indicator);
    uprintf("payload_unit_start_indicator   :  0x%X\n", packet_head->payload_unit_start_indicator);
    uprintf("transport_priority             :  0x%X\n", packet_head->transport_priority);
    uprintf("pid                            :  0x%X\n", packet_head->pid);
    uprintf("transport_scrambling_control   :  0x%X\n", packet_head->transport_scrambling_control);
    uprintf("adaptation_field_control       :  0x%X\n", packet_head->adaptation_field_control);
    uprintf("continuity_counter             :  0x%X\n", packet_head->continuity_counter);
    
    return 0;
}

/*  
 *  Funciton : locate the ts packet data offset. 
 *             
 *  psi_flag  : to distinguish the PSI/SI packet and PES packet.
 *             0 PSI/SI packet.     1 : PES packet.
 *  
 *  pes_start : meaning the packet_head->payload_unit_start_indicator.
 *              0  : not pes start.
 *              1  : pes start.
 */
unsigned int locate_offset(TS_PACKET_HEADER *packet_head, unsigned char *buffer,
        unsigned int psi_flag, unsigned int * pes_start)
{
    unsigned int offset_length = 0;

    if (0 == psi_flag)
    {
        //Have data
        if ((0x03 == packet_head->adaptation_field_control) || (0x01 == packet_head->adaptation_field_control))
        {
            if( 1 == packet_head->payload_unit_start_indicator )
            {
                if (0x03 == packet_head->adaptation_field_control)
                {
                    //1 + buffer[4] :   point + point_field_length
                    //buffer[4] + 5 : the index of adaptation_field_length;
                    offset_length = TS_BASE_PACKET_HEADER_SIZE + (1 + buffer[4]) + (1 + buffer[buffer[4] + 5]);
                }
                else
                {
                    offset_length = TS_BASE_PACKET_HEADER_SIZE + (1 + buffer[4]);
                }
            }
            else
            {
                if (0x03 == packet_head->adaptation_field_control)//1+buffer[4] : adaptation_field + adaptation_field_length
                {
                    offset_length = TS_BASE_PACKET_HEADER_SIZE + (1 + buffer[4]);
                }
                else
                {
                    offset_length = TS_BASE_PACKET_HEADER_SIZE; 
                }
            }
    
        }
    }
    else //this is pes packet
    {
        *pes_start = 0;
        //Have data

        if ((0x03 == packet_head->adaptation_field_control) || (0x01 == packet_head->adaptation_field_control))
        {
            if( 1 == packet_head->payload_unit_start_indicator )
            {
//                uprintf("This is a PES packet start.\n");
                *pes_start = 1;
            }

            if (0x03 == packet_head->adaptation_field_control)//1+buffer[4] : adaptation_field + adaptation_field_length
            {
                offset_length = TS_BASE_PACKET_HEADER_SIZE + (1 + buffer[4]);
            }
            else
            {
                offset_length = TS_BASE_PACKET_HEADER_SIZE; 
            }
        } 
    
    }
    //uprintf("the value of offset_length is  %d\n",offset_length);

    return offset_length;
}


/*  
 *  Funciton    : store one section data of ts_psi_si table. 
 *  
 *  Note        : used for ts_psi_table which have the section_number and last_section_number.
 *                like PAT,PMT,SDT,NIT and etc.
 */
int store_one_section_data(FILE *pFile, unsigned int mPacketLength, TABLE_SECTION_LIST * p_table_section )
{
    
    int ret = -1;
    unsigned int pid, table_id, section_number, buffer_size, section_size;
    unsigned char * ptsbuffer = (unsigned char *)malloc(mPacketLength); 
    unsigned int offset_length = 0;
    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;

    pid             = p_table_section->pid;
    table_id        = p_table_section->table_id;
    section_number  = p_table_section->section_number;

    fseek(pFile, 0, SEEK_SET);
    
    while(fread(ptsbuffer, mPacketLength, 1, pFile) == 1)
    {
        parse_ts_packet_header(ptsPacketHeader, ptsbuffer);

        offset_length = locate_offset(ptsPacketHeader, ptsbuffer, PSI_SI_PACKET_FLAG, 0);

        //the payload_unit_start == 1 meaning the start of a section.
        if ((pid == ptsPacketHeader->pid) && (table_id == TS_PSI_SI_TABLE_ID(ptsbuffer,offset_length))
                && (section_number == TS_PSI_SI_TABLE_SECTION_NUM(ptsbuffer,offset_length)) 
                && (1 == TS_PACKET_HEADER_PAYLOAD_UNIT_START_INDICATOR(ptsbuffer)))
        {
            ret = 0;
//            uprintf("the section length is %d\n",TS_PSI_SI_TABLE_SECTION_LENGTH(ptsbuffer, offset_length));
            unsigned int continuity_counter = ptsPacketHeader->continuity_counter;            
            p_table_section->buffer_size    = TS_PSI_SI_TABLE_SECTION_LENGTH(ptsbuffer, offset_length) + SECTION_INFO_HEADER_SIZE + offset_length;
//            uprintf("the value of buffer_size if %d\n", p_table_section->buffer_size);
            buffer_size                     = p_table_section->buffer_size;
            section_size                    = buffer_size - SECTION_INFO_HEADER_SIZE - offset_length;
            p_table_section->pbuffer        = (unsigned char *)malloc(buffer_size);
            if(NULL == p_table_section->pbuffer)
            {
                uprintf("malloc %d byte failed, try again\n",buffer_size);
                p_table_section->pbuffer        = (unsigned char *)malloc(buffer_size);
            }
            unsigned char *pbuffer_index    = p_table_section->pbuffer;
            //for the payload_unit_start, we directory copy from the ts start byte----sync.(other ts packet don't have the
            //table id and so on

            unsigned int copy_loop_total = (section_size + SECTION_INFO_HEADER_SIZE + offset_length) / mPacketLength;
            unsigned int copy_count = 0;
            if(0 == copy_loop_total)
                memcpy(pbuffer_index, ptsbuffer, buffer_size);
            else
            {
                unsigned int first_copy_flag = 1;
                while(buffer_size > 0)
                {
                    if(1 == first_copy_flag)
                    {
                        first_copy_flag = 0;
                        memcpy(pbuffer_index, ptsbuffer, mPacketLength);
                    #ifdef DEBUG_DETAIL
                        uprintf("the offset_length is %d\n",offset_length);
                        uprintf("We copy %d byte and buffer_reserved  is %d\n",mPacketLength, buffer_size - mPacketLength);
                        show_packet_memory(ptsbuffer, mPacketLength);
                        show_packet_memory(pbuffer_index,mPacketLength);
                    #endif
                        pbuffer_index += mPacketLength;
                        buffer_size   -= mPacketLength;
                        copy_count++;
                    }
                    else
                    {
                        if(fread(ptsbuffer, mPacketLength, 1, pFile) !=1 )
                        {
                            fseek(pFile, 0, SEEK_SET);
                            fread(ptsbuffer, mPacketLength, 1, pFile);
                        } 
                        
                        parse_ts_packet_header(ptsPacketHeader, ptsbuffer);
                        offset_length = locate_offset(ptsPacketHeader, ptsbuffer, PSI_SI_PACKET_FLAG, 0);

                        if(pid == ptsPacketHeader->pid
                                && (ptsPacketHeader->continuity_counter == (continuity_counter + copy_count)%16)) 
                        {
                            if(buffer_size + SECTION_INFO_HEADER_SIZE + offset_length < mPacketLength)
                            {
                                memcpy(pbuffer_index, ptsbuffer + offset_length, buffer_size);
                            
                            #ifdef DEBUG_DETAIL
                                uprintf("We copy %d byte and buffer_reserved  is 0\n",buffer_size);
                                uprintf("the offset_length is %d",offset_length);
                                show_packet_memory(ptsbuffer, buffer_size);
                                uprintf("Copy Finished\n");
                            #endif
                                pbuffer_index += buffer_size;
                                buffer_size -=buffer_size;
                                break; 
                            }
                            else
                            {
                                memcpy(pbuffer_index, ptsbuffer + offset_length, mPacketLength-offset_length);

                            #ifdef DEBUG_DETAIL
                                uprintf("the offset_length is %d\n",offset_length);
                                uprintf("We copy %d byte and buffer_reserved  is %d\n",mPacketLength-offset_length,buffer_size-(mPacketLength-offset_length));
                                show_packet_memory(ptsbuffer, mPacketLength);
                            #endif
                                pbuffer_index += mPacketLength - offset_length;
                                buffer_size   -= (mPacketLength - offset_length);
                                copy_count++;
                            }

                        }
                    }
                
                }

            }//end of if ... else
        
            break;
        }//end of if

    }//end of while


    if(ret == -1)
        uprintf("Can't find the given table\n");
    
    //return to the SEEK_SET position of pFile.
    fseek(pFile, 0, SEEK_SET);
    free(ptsbuffer);

    return ret;
}



TABLE_SECTION_LIST * alloc_and_init_table_section(unsigned char * tmp_ts_buffer, 
        unsigned int section_num, unsigned int offset_length)
{
    
    TABLE_SECTION_LIST * p_table_section = (TABLE_SECTION_LIST *)malloc(sizeof(TABLE_SECTION_LIST));
    memset(p_table_section, 0, sizeof(TABLE_SECTION_LIST));

    p_table_section->pid                = TS_PACKET_HEADER_PID(tmp_ts_buffer);
    p_table_section->table_id           = TS_PSI_SI_TABLE_ID(tmp_ts_buffer, offset_length);
    p_table_section->section_number     = section_num;
    
    p_table_section->last_section_number= TS_PSI_SI_TABLE_LAST_SECTION_NUM(tmp_ts_buffer, offset_length);
    p_table_section->buffer_size        = 0;
    p_table_section->pbuffer            = NULL;
    p_table_section->next_section       = NULL;


    return p_table_section;
}


TABLE_SECTION_LIST * table_section_list_add(TABLE_SECTION_LIST *head, TABLE_SECTION_LIST *node)
{
    TABLE_SECTION_LIST * ptmp = head;
    TABLE_SECTION_LIST * pcur = NULL;

    while (ptmp != NULL)
    {
        pcur = ptmp;
        ptmp = ptmp->next_section;
    }

    pcur->next_section = node;

    return head;
}

/*  
 *  Funciton    : store all section data of ts_psi_si table. 
 *  
 *  Note        : used for ts_psi_table which have the section_number and last_section_number.
 *                like PAT,PMT,SDT,NIT and etc.
 */
 TABLE_SECTION_LIST * store_psi_si_table(FILE *pFile, unsigned int mPacketLength, 
        unsigned int mUserPid,unsigned int tableId)
{
    unsigned int offset_length = 0;
    unsigned int last_section_number = 0;
    unsigned char * tmp_ts_buffer = (unsigned char *)malloc(mPacketLength);

    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;

    TABLE_SECTION_LIST * p_section_head = NULL;
    TABLE_SECTION_LIST * p_section_node = NULL;

    fseek(pFile, 0, SEEK_SET);

    while((fread(tmp_ts_buffer, mPacketLength, 1, pFile) == 1))
    {
        parse_ts_packet_header(ptsPacketHeader, tmp_ts_buffer);

        offset_length = locate_offset(ptsPacketHeader, tmp_ts_buffer, PSI_SI_PACKET_FLAG, 0);
        
        //the payload_unit_start == 1 meaning the start of a section.
        if (mUserPid == ptsPacketHeader->pid && tableId == TS_PSI_SI_TABLE_ID(tmp_ts_buffer,offset_length))
        {
            last_section_number = TS_PSI_SI_TABLE_LAST_SECTION_NUM(tmp_ts_buffer,offset_length);
        #ifdef DEBUG_DETAIL
            uprintf("Last_Section_Number is %d\n",last_section_number);
        #endif
            int i = 0;
            for(i = 0; i <= last_section_number; i++)
            {
                if(0 == i)
                {
                    p_section_head = alloc_and_init_table_section(tmp_ts_buffer, i, offset_length);
                    store_one_section_data(pFile, mPacketLength, p_section_head);

                    //show_packet_memory(p_section_head->pbuffer,p_section_head->buffer_size);
                }
                else
                {
                    p_section_node = alloc_and_init_table_section(tmp_ts_buffer, i, offset_length);
                    store_one_section_data(pFile, mPacketLength, p_section_node);
                    
                    //show_packet_memory(p_section_node->pbuffer,p_section_node->buffer_size);
                    
                    table_section_list_add(p_section_head, p_section_node);
                }
            
            
            }
            break;
        }
    }//end of while


    free(tmp_ts_buffer);
    if (NULL == p_section_head)
    {
        uprintf("Can't find given table\n");
        exit(-1);
    }
    
    return p_section_head;
}       

/*  
 *  Funciton    : store one section data of ts_psi_si table. 
 *  
 *  Note        : used for ts_psi_table which don't have the section_number and last_section_number.
 *                like TOT,TDT and etc.
 */
int store_one_section_data_reserved(FILE *pFile, unsigned int mPacketLength, TABLE_SECTION_LIST * p_table_section)
{
    
    int ret = -1;
    unsigned int pid, table_id, section_number, buffer_size, section_size;
    unsigned char * ptsbuffer = (unsigned char *)malloc(mPacketLength); 
    unsigned int offset_length = 0;
    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;

    pid             = p_table_section->pid;
    table_id        = p_table_section->table_id;
    //section_number  = p_table_section->section_number;

    fseek(pFile, 0, SEEK_SET);
    
    while(fread(ptsbuffer, mPacketLength, 1, pFile) == 1)
    {
        parse_ts_packet_header(ptsPacketHeader, ptsbuffer);

        offset_length = locate_offset(ptsPacketHeader, ptsbuffer, PSI_SI_PACKET_FLAG, 0);

        //the payload_unit_start == 1 meaning the start of a section.
        if ((pid == ptsPacketHeader->pid) && (table_id == TS_PSI_SI_TABLE_ID(ptsbuffer,offset_length))
      //          && (section_number == TS_PSI_SI_TABLE_SECTION_NUM(ptsbuffer,offset_length)) 
                && (1 == TS_PACKET_HEADER_PAYLOAD_UNIT_START_INDICATOR(ptsbuffer)))
        {
            ret = 0;
            //uprintf("the section length is %d\n",TS_PSI_SI_TABLE_SECTION_LENGTH(ptsbuffer, offset_length));
            unsigned int continuity_counter = ptsPacketHeader->continuity_counter;            
            p_table_section->buffer_size    = TS_PSI_SI_TABLE_SECTION_LENGTH(ptsbuffer, offset_length) + SECTION_INFO_HEADER_SIZE + offset_length;
            //uprintf("the value of buffer_size if %d\n", p_table_section->buffer_size);
            buffer_size                     = p_table_section->buffer_size;
            section_size                    = buffer_size - SECTION_INFO_HEADER_SIZE - offset_length;
            p_table_section->pbuffer        = (unsigned char *)malloc(buffer_size);
            if(NULL == p_table_section->pbuffer)
            {
                uprintf("malloc %d byte failed, try again\n",buffer_size);
                p_table_section->pbuffer        = (unsigned char *)malloc(buffer_size);
            }
            unsigned char *pbuffer_index    = p_table_section->pbuffer;
            //for the payload_unit_start, we directory copy from the ts start byte----sync.(other ts packet don't have the
            //table id and so on
    
            unsigned int copy_loop_total = (section_size + SECTION_INFO_HEADER_SIZE + offset_length) / mPacketLength;
            unsigned int copy_count = 0;
            if(0 == copy_loop_total)
                memcpy(pbuffer_index, ptsbuffer, buffer_size);
            else
            {
                unsigned int first_copy_flag = 1;
                while(buffer_size > 0)
                {
                    if(1 == first_copy_flag)
                    {
                        first_copy_flag = 0;
                        memcpy(pbuffer_index, ptsbuffer, mPacketLength);
                    #ifdef DEBUG_DETAIL
                        uprintf("the offset_length is %d\n",offset_length);
                        uprintf("We copy %d byte and buffer_reserved  is %d\n",mPacketLength, buffer_size - mPacketLength);
                        show_packet_memory(ptsbuffer, mPacketLength);
                        show_packet_memory(pbuffer_index,mPacketLength);
                    #endif
                        pbuffer_index += mPacketLength;
                        buffer_size   -= mPacketLength;
                        copy_count++;
                    }
                    else
                    {
                        if(fread(ptsbuffer, mPacketLength, 1, pFile) !=1 )
                        {
                            fseek(pFile, 0, SEEK_SET);
                            fread(ptsbuffer, mPacketLength, 1, pFile);
                        } 
                        
                        parse_ts_packet_header(ptsPacketHeader, ptsbuffer);
                        offset_length = locate_offset(ptsPacketHeader, ptsbuffer, PSI_SI_PACKET_FLAG, 0);

                        if(pid == ptsPacketHeader->pid
                                && (ptsPacketHeader->continuity_counter == (continuity_counter + copy_count)%16)) 
                        {
                            if(buffer_size + SECTION_INFO_HEADER_SIZE + offset_length < mPacketLength)
                            {
                                memcpy(pbuffer_index, ptsbuffer + offset_length, buffer_size);
                            
                            #ifdef DEBUG_DETAIL
                                uprintf("We copy %d byte and buffer_reserved  is 0\n",buffer_size);
                                uprintf("the offset_length is %d",offset_length);
                                show_packet_memory(ptsbuffer, buffer_size);
                                uprintf("Copy Finished\n");
                            #endif
                                pbuffer_index += buffer_size;
                                buffer_size -=buffer_size;
                                break; 
                            }
                            else
                            {
                                memcpy(pbuffer_index, ptsbuffer + offset_length, mPacketLength-offset_length);

                            #ifdef DEBUG_DETAIL
                                uprintf("the offset_length is %d\n",offset_length);
                                uprintf("We copy %d byte and buffer_reserved  is %d\n",mPacketLength-offset_length,buffer_size-(mPacketLength-offset_length));
                                show_packet_memory(ptsbuffer, mPacketLength);
                            #endif
                                pbuffer_index += mPacketLength - offset_length;
                                buffer_size   -= (mPacketLength - offset_length);
                                copy_count++;
                            }

                        }
                    }
                
                }

            }//end of if ... else
        
            break;
        }//end of if

    }//end of while


    if(ret == -1)
        uprintf("Can't find the given table\n");
    
    //return to the SEEK_SET position of pFile.
    fseek(pFile, 0, SEEK_SET);
    free(ptsbuffer);

    return ret;
}

/*  
 *  Funciton    : store all section data of ts_psi_si table. 
 *  
 *  Note        : used for ts_psi_table which don't have the section_number and last_section_number.
 *                like TOT,TDT and etc.
 */
 TABLE_SECTION_LIST * store_psi_si_table_reserved(FILE *pFile, unsigned int mPacketLength, 
        unsigned int mUserPid,unsigned int tableId)
{
    unsigned int offset_length = 0;
    unsigned char * tmp_ts_buffer = (unsigned char *)malloc(mPacketLength);

    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;

    TABLE_SECTION_LIST * p_section_head = NULL;

    fseek(pFile, 0, SEEK_SET);

    while((fread(tmp_ts_buffer, mPacketLength, 1, pFile) == 1))
    {
        parse_ts_packet_header(ptsPacketHeader, tmp_ts_buffer);

        offset_length = locate_offset(ptsPacketHeader, tmp_ts_buffer, PSI_SI_PACKET_FLAG, 0);
        
        //the payload_unit_start == 1 meaning the start of a section.
        if (mUserPid == ptsPacketHeader->pid && tableId == TS_PSI_SI_TABLE_ID(tmp_ts_buffer,offset_length))
        {
            //last_section_number = TS_PSI_SI_TABLE_LAST_SECTION_NUM(tmp_ts_buffer,offset_length);
            p_section_head = alloc_and_init_table_section(tmp_ts_buffer, 0, offset_length);
            store_one_section_data_reserved(pFile, mPacketLength, p_section_head);
        #ifdef DEBUG_DETAIL
            show_packet_memory(p_section_head->pbuffer,p_section_head->buffer_size);
        #endif
            break;
        }
    }//end of while


    free(tmp_ts_buffer);
    if (NULL == p_section_head)
    {
        uprintf("Can't find given table\n");
        exit(-1);
    }
    
    return p_section_head;
}       
            


void show_packet_memory(unsigned char * buffer, unsigned int packetLength)
{
    int index = 0;
    printf("\nPrepare to show the  memory info\n");
    for(; index < packetLength; index++)
    {
            uprintf("%02X ",buffer[index]);
            if((index+1)%16 == 0)
                uprintf("\n");
    }
    uprintf("\n");
}

