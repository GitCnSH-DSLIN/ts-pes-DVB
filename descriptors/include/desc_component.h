/*********************************************************************
*
* Filename      :   desc_component.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/


#ifndef  _MPEG_SI_DESCRITPOR_H_
#define  _MPEG_SI_DESCRIPTOR_H_

#include <sdt.h>

typedef struct component_descriptor{
    unsigned char descriptor_tag;
    unsigned char descriptor_length;
    struct descriptor_common * next_desc;//add by myself
    unsigned reserved_future_use : 4;
    unsigned stream_content : 4;
    unsigned component_type : 8;
    unsigned component_tag  : 8;
    unsigned int ISO_639_language_code; //24 bit
    unsigned char * text_char;
}COMPONENT_DESC, *P_COMPONENT_DESC;


#define COMPONENT_DESC_TAG(b)               (b[0])
#define COMPONENT_DESC_LENGTH(b)            (b[1])
#define COMPONENT_DESC_STREAM_CONTENT(b)    (b[2] & 0x0f)
#define COMPONENT_DESC_COMPONENT_TYPE(b)    (b[3])
#define COMPONENT_DESC_COMPONENT_TAG(b)     (b[4])
#define COMPONENT_DESC_ISO_639_LANGUAGE_CODE(b) ((b[5] << 16) | (b[6] << 8) | b[7])






DESCRIPTOR_COMMON * decode_component_desc(unsigned char* byteptr, int this_section_length);

void free_component_desc(DESCRIPTOR_COMMON * head);

void show_component_descriptor(DESCRIPTOR_COMMON *ptmp);

#endif

