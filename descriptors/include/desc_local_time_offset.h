/*********************************************************************
*
* Filename      :   desc_local_time_offset.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef _DESC_LOCAL_TIME_OFFSET_H_
#define _DESC_LOCAL_TIME_OFFSET_H_
#include <sdt.h>

typedef struct local_time_offset_item{
	unsigned country_code : 24;
    unsigned country_regin_id : 6;
    unsigned reserved : 1;
    unsigned local_time_offset_polarity : 1;
    unsigned local_time_offset : 16;
    unsigned long time_of_change : 40;
    unsigned short next_time_offset;//16bit

	struct local_time_offset_item * next_item;
}LOCAL_TIME_OFFSET_ITEM, *P_LOCAL_TIME_OFFSET_ITEM;


typedef struct local_time_offset_desc{
	unsigned char descriptor_tag;
	unsigned char descriptor_length;

    struct sdt_descriptor_common * next_desc;
    struct local_time_offset_item * first_item;
}LOCAL_TIME_OFFSET_DESC,*P_LOCAL_TIME_OFFSET_DESC;


#define LOCAL_TIME_OFFSET_DESC_TAG(b)                   (b[0])
#define LOCAL_TIME_OFFSET_DESC_LENGTH(b)				(b[1])


#define LOCAL_TIME_OFFSET_ITEM_COUNTRY_CODE(b)			((b[0] << 16) | b[1] << 8 | b[2])
#define LOCAL_TIME_OFFSET_ITEM_COUNTRY_REGIN_ID(b)		(b[3] & 0xfc)
#define LOCAL_TIME_OFFSET_ITEM_POLARITY(b)		        (b[3] & 0x1)
#define LOCAL_TIME_OFFSET_ITEM_TIME_OFFSET(b)		    ((b[4] << 8) | b[5])
#define LOCAL_TIME_OFFSET_ITEM_TIME_OF_CHANGE(b)		((b[6] << 32) | (b[7] << 24) | (b[8] << 16) | \
                                                            (b[9] << 8) | b[10])
#define LOCAL_TIME_OFFSET_ITEM_NEXT_TIME_OFFSET(b)		((b[11] << 8) | b[12])


LOCAL_TIME_OFFSET_ITEM * insert_localtime_offset_item_node(LOCAL_TIME_OFFSET_ITEM * Header, LOCAL_TIME_OFFSET_ITEM * node);

void decode_localtime_offset_item(unsigned char* byteptr, LOCAL_TIME_OFFSET_ITEM * item_localtime);

SDT_DESCRIPTOR_COMMON * decode_local_time_offset_desc(unsigned char* byteptr, int this_section_length);


void free_localtime_offset_item(LOCAL_TIME_OFFSET_ITEM *pheader);

void free_local_time_offset_desc(SDT_DESCRIPTOR_COMMON *head);


void show_localtime_offset_item(LOCAL_TIME_OFFSET_ITEM *head);

void show_local_time_offset_descriptor(SDT_DESCRIPTOR_COMMON *ptmp);

#endif /* _DESC_LOCAL_TIME_OFFSET_H_ */

