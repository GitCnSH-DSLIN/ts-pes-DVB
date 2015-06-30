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
    *pes_start = 0;

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
 *  Function    : Find the given table on the basis mUserPid.
 *  Description : 1. when find, copy the given table to storeBuffer.
 *                2. store the given table header to ptsPacketHeader.
 *  Note        : because some table use some sections to store it.
 *                So we need store a total table. 
 *                This function can't use to find pes data.
 *
 *  Return      : section_list_header.
 *                For SDT table . with more sections situation. Need to deal with.
 */

 TABLE_SECTION_LIST * store_psi_si_table(FILE *pFile, unsigned int mPacketLength, 
        unsigned int mUserPid,unsigned int tableId)
{
    unsigned int offset_length = 0;
    unsigned int section_num = 0, last_section_num = 0;
    unsigned char * tmp_ts_buffer = (unsigned char *)malloc(mPacketLength);

    TS_PACKET_HEADER tsPacketHeader;
    P_TS_PACKET_HEADER ptsPacketHeader = &tsPacketHeader;

    while((fread(tmp_ts_buffer, mPacketLength, 1, pFile) == 1))
    {
        parse_ts_packet_header(ptsPacketHeader, tmp_ts_buffer);

        offset_length = locate_offset(ptsPacketHeader, tmp_ts_buffer, PSI_SI_PACKET_FLAG, 0);
        
        //the payload_unit_start == 1 meaning the start of a section.
        if (mUserPid == ptsPacketHeader->pid && tableId == TS_PSI_SI_TABLE_ID(tmp_ts_buffer,offset_length))
        {
            last_section_number = TS_PSI_SI_TABLE_LAST_SECTION_NUM(tmp_ts_buffer,offset_length);

            for(i = 0; i <= last_section_number; i++)
            {
                if(0 == i)
                {
                    TABLE_SECTION_LIST * p_section_head = (TABLE_SECTION_LIST *)malloc(sizeof(TABLE_SECTION_LIST));
                    memset(p_section_head, 0, sizeof(TABLE_SECTION_LIST));
                    store_one_section_data();
                }
                else
                {
                    TABLE_SECTION_LIST *p_section_node = (TABLE_SECTION_LIST *)malloc(sizeof(TABLE_SECTION_LIST));
                    memset(p_section_node, 0, sizeof(TABLE_SECTION_LIST));
                }
            
            
            
            }

        }
            
            
            
            unsigned int copy_loop_total = 0;
            unsigned int copy_count = 0;
            unsigned int continuity_counter;
            unsigned int section_length;
            ret = 1;

            continuity_counter = ptsPacketHeader->continuity_counter;
            section_length = ((tmpbuffer[offsetLength + 2] & 0x0f)<<8) | (tmpbuffer[offsetLength + 3]);
            //3 meaning before section_length bytes.    
            copy_loop_total = (section_length + 3 + offsetLength) / mPacketLength;

            if(0 == copy_loop_total)
                memcpy(storeBuffer, tmpbuffer, mPacketLength);
            else
            {
                for (; copy_count <= copy_loop_total;)
                {
                    if(0 == copy_count)
                    {
                        memcpy(pstoreBuffer, tmpbuffer, mPacketLength);
                        copy_count++;
                        pstoreBuffer += mPacketLength;
                    }
                    else
                    {
                        if(fread(tmpbuffer, mPacketLength, 1, pFile) !=1 )
                            fseek(pFile, 0, SEEK_SET);
                        parse_ts_packet_header(ptsPacketHeader, tmpbuffer);
                        offsetLength = locate_offset(ptsPacketHeader, tmpbuffer, 1, &pes_start);

                        if(mUserPid == ptsPacketHeader->pid && ptsPacketHeader->continuity_counter == continuity_counter + copy_count) 
                        {
                            memcpy(pstoreBuffer,tmpbuffer+offsetLength, mPacketLength-offsetLength);
                            copy_count++;
                            pstoreBuffer += mPacketLength-offsetLength;
                        }
                    }
                }
            }
            break;
        }
    }


    
    if(ret == -1)
    {
        uprintf("Can't find the given table\n");
    }
    //return to the SEEK_SET position of pFile.
    fseek(pFile, 0, SEEK_SET);
    free(tmpbuffer);

    return ret;
}

