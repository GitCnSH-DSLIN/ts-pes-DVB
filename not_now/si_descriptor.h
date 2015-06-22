/*********************************************************************
*
* Filename      :   si_descriptor.h
* Description   :   given the fundamental struction of si descriptor of DVB
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef  _MPEG_SI_DESCRITPOR_H_
#define  _MPEG_SI_DESCRIPTOR_H_

struct CA_identifier_descriptor{
    unsigned char descriptor_tag;
    unsigned char descriptor_length;
    unsigned short * CA_system_id;
    void * next_desc;
};


#define NETWORKNAME_DESC_TAG(b)			(b[0])
#define NETWORKNAME_DESC_LENGTH(b)		(b[1])

int decode_network_name_desc(byte* byteptr, int this_section_length,NetworkNameDesc* desc_network_name);
void free_network_name_desc(NetworkNameDesc* head);


#endif
