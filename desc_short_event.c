/*
 * desc_short_event.c
 *
 *  Created on: Jun 24, 2011
 *      Author: orion
 */
#include <stdio.h>
#include <desc_short_event.h>
#include <print_debug.h>

int decode_short_evt_desc(unsigned char* byteptr, int this_section_length,
        SHORT_EVENT_DESC* desc_short_event){
	unsigned char* b = byteptr;

	desc_short_event->descriptor_tag = SHORT_EVT_DESC_TAG(b);
	desc_short_event->descriptor_length = SHORT_EVT_DESC_LENGTH(b);
	desc_short_event->ISO_639_language_code = SHORT_EVT_DESC_LANG(b);
	desc_short_event->event_name_length = SHORT_EVT_DESC_NAME_LENGTH(b);
	desc_short_event->event_name = (char *)calloc(desc_short_event->event_name_length + 1,sizeof(char));
	strncpy(desc_short_event->event_name,(char*)&b[6],desc_short_event->event_name_length);
	desc_short_event->text_length = SHORT_EVT_DESC_TEXT_LENGTH(b);
	desc_short_event->text = (char *)calloc(desc_short_event->text_length + 1,sizeof(char));
	strncpy(desc_short_event->text,(char*)&b[desc_short_event->event_name_length + 7],desc_short_event->text_length);

	uprintf("desc_short_event->descriptor_tag:0x%x\n",desc_short_event->descriptor_tag);
	uprintf("desc_short_event->descriptor_length:0x%x\n",desc_short_event->descriptor_length);
	uprintf("desc_short_event->ISO_639_language_code:0x%x\n",desc_short_event->ISO_639_language_code);
	uprintf("desc_short_event->event_name_length:0x%x\n",desc_short_event->event_name_length);
	uprintf("desc_short_event->event_name:%s\n",desc_short_event->event_name);
	uprintf("desc_short_event->text_length:0x%x\n",desc_short_event->text_length);
	uprintf("desc_short_event->text:%s\n",desc_short_event->text);

	return (desc_short_event->descriptor_length + 2);
}

void free_short_evt_desc(SHORT_EVENT_DESC* head){
	free(head->event_name);
	free(head->text);
	head->event_name = NULL;
	head->text = NULL;
	free_desc(head->next_desc);
	head->next_desc = NULL;
	free(head);
	head = NULL;
}
