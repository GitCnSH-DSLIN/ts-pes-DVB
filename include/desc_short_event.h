/*
 * desc_short_event.h
 *
 *  Created on: Jun 24, 2011
 *      Author: orion
 */

#ifndef _DESC_SHORT_EVENT_H_
#define _DESC_SHORT_EVENT_H_

typedef struct short_event_desc{
	unsigned char descriptor_tag;
	unsigned char descriptor_length;
	unsigned int ISO_639_language_code;
	unsigned char event_name_length;
	char* event_name;
	unsigned char text_length;
	char* text;
	void* next_desc;
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

int decode_short_evt_desc(unsigned char* byteptr, int this_section_length,
        SHORT_EVENT_DESC* desc_short_event);
void free_short_evt_desc(SHORT_EVENT_DESC* head);

#endif /* _DESC_SHORT_EVENT_H_ */
