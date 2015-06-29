/*********************************************************************
*
* Filename      :   sdt_descriptor.h
* Description   :   given the fundamental struction for descriptors of sdt service.
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/


extern void (*do_show_descriptors_info[64])(SDT_DESCRIPTOR_COMMON *ptmp);

extern SDT_DESCRIPTOR_COMMON * (*do_decode_desc[64])(unsigned char * byteptr, int this_section_length);

extern void (*do_free_descriptors[64])(SDT_DESCRIPTOR_COMMON *ptmp);



extern SDT_DESCRIPTOR_COMMON * decode_default(unsigned char * byteptr, int this_section_length);

extern SDT_DESCRIPTOR_COMMON * decode_desc(unsigned char * byteptr, int this_section_length);


extern void free_desc(SDT_DESCRIPTOR_COMMON *phead);

extern void free_desc_default(SDT_DESCRIPTOR_COMMON* head);


extern void show_sdt_service_descriptors_info(SDT_SERVICE * sdtService);

extern void show_desc_default(SDT_DESCRIPTOR_COMMON *ptmp);
