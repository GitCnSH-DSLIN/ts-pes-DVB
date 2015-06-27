
void (*do_show_descriptors_info[192])(SDT_DESCRIPTOR_COMMON *ptmp);

SDT_DESCRIPTOR_COMMON * (*do_decode_desc[192])(unsigned char * byteptr, int this_section_length);

SDT_DESCRIPTOR_COMMON * decode_default(unsigned char * byteptr, int this_section_length);

SDT_DESCRIPTOR_COMMON * decode_desc(unsigned char * byteptr, int this_section_length);

void free_desc(void* phead);

void show_desc_default(SDT_DESCRIPTOR_COMMON *ptmp);
