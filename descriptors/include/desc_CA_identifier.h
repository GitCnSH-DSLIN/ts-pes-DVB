/*********************************************************************
*
* Filename      :   desc_CA_identifier.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/


#ifndef  _MPEG_SI_DESCRITPOR_H_
#define  _MPEG_SI_DESCRIPTOR_H_
#include <sdt.h>

typedef struct CA_identifier_descriptor{
    unsigned char descriptor_tag;
    unsigned char descriptor_length;
    struct sdt_descriptor_common * next_desc;
    unsigned short * CA_system_id;
}CA_IDENTIFIER_DESC, *P_CA_IDENTIFIER_DESC;


SDT_DESCRIPTOR_COMMON * decode_CA_identifier_desc(unsigned char* byteptr, int this_section_length);

void free_CA_identifier_desc(SDT_DESCRIPTOR_COMMON * head);

void show_CA_identifier_descriptor(SDT_DESCRIPTOR_COMMON *ptmp);

#endif
