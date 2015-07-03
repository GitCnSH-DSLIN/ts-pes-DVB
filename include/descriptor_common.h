/*********************************************************************
*
* Filename      :   sdt_descriptor.h
* Description   :   given the fundamental struction for descriptors of sdt service.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef  _MPEG_TS_SI_DESCRIPTOR_COMMON_H_
#define  _MPEG_TS_SI_DESCRIPTOR_COMMON_H_


typedef struct descriptor_common{
    unsigned char descriptor_tag;
    unsigned char descriptor_length;
    struct descriptor_common * next_desc;
}DESCRIPTOR_COMMON, *P_DESCRIPTOR_COMMON;

void (*do_show_descriptors_info[64])(DESCRIPTOR_COMMON *ptmp);

DESCRIPTOR_COMMON * (*do_decode_desc[64])(unsigned char * byteptr, int this_section_length);

void (*do_free_descriptors[64])(DESCRIPTOR_COMMON *ptmp);



DESCRIPTOR_COMMON * decode_default(unsigned char * byteptr, int this_section_length);

DESCRIPTOR_COMMON * decode_desc(unsigned char * byteptr, int this_section_length);


void free_desc(DESCRIPTOR_COMMON *phead);

void free_desc_default(DESCRIPTOR_COMMON* head);

void show_desc(DESCRIPTOR_COMMON *phead);
void show_desc_default(DESCRIPTOR_COMMON *ptmp);

#endif
