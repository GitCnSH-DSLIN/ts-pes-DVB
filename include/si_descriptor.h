/*********************************************************************
*
* Filename      :   si_descriptor.h
* Description   :   given the fundamental struction of si descriptor of DVB
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef  _MPEG_SI_DESCRITPOR_H_
#define  _MPEG_SI_DESCRIPTOR_H_

typedef struct CA_identifier_descriptor{
    unsigned char descriptor_tag;
    unsigned char descriptor_length;
    unsigned short * CA_system_id;
    void * next_desc;
}CA_IDENTIFIER_DESC, *P_CA_IDENTIFIER_DESC;


int decode_CA_identifier_desc(byte* byteptr, int this_section_length,
        P_CA_IDENTIFIER_DESC * pCaIdentifier_desc);

void free_CA_identifier_desc(pCaIdentifier_desc * head);


#endif
