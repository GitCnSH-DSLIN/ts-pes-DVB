/*********************************************************************
*
* Filename      :   desc_short_event.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef _DESC_SHORT_EVENT_H_
#define _DESC_SHORT_EVENT_H_

#include <sdt.h>

typedef struct short_event_desc{
	unsigned char descriptor_tag;
	unsigned char descriptor_length;
	void* next_desc;
	unsigned int ISO_639_language_code;
	unsigned char event_name_length;
	char* event_name;
	unsigned char text_length;
	char* text;
}SHORT_EVENT_DESC,*P_SHORT_EVENT_DESC;


#define SHORT_EVT_DESC_TAG(b)			(b[0])
#define SHORT_EVT_DESC_LENGTH(b)		(b[1])
#define SHORT_EVT_DESC_LANG(b)			((b[2] << 16) | (b[3] << 8) | b[4])
#define SHORT_EVT_DESC_NAME_LENGTH(b)	(b[5])
#define	SHORT_EVT_DESC_TEXT_LENGTH(b)	({	\
											int n = SHORT_EVT_DESC_NAME_LENGTH(b); \
											int base = n + 6; \
											b[base]; \
										})

DESCRIPTOR_COMMON * decode_short_event_desc(unsigned char* byteptr, int this_section_length);
void free_short_event_desc(DESCRIPTOR_COMMON* head);

void show_short_event_descriptor(DESCRIPTOR_COMMON *ptmp);

#endif /* _DESC_SHORT_EVENT_H_ */
