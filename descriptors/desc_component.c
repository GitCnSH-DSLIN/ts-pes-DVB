/*********************************************************************
*
* Filename      :   desc_component.c
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sdt.h>
#include <descriptor_common.h>
#include <desc_component.h>
#include <print_debug.h>

DESCRIPTOR_COMMON * decode_component_desc(unsigned char * byteptr, int this_section_length)
{
	unsigned char* b = byteptr;

	COMPONENT_DESC * desc_component = (COMPONENT_DESC *)malloc(sizeof(COMPONENT_DESC));
	memset(desc_component, 0, sizeof(COMPONENT_DESC));
    
	desc_component->descriptor_tag          = COMPONENT_DESC_TAG(b);
	desc_component->descriptor_length       = COMPONENT_DESC_LENGTH(b);
	desc_component->stream_content          = COMPONENT_DESC_STREAM_CONTENT(b);
	desc_component->component_type          = COMPONENT_DESC_COMPONENT_TYPE(b);
	desc_component->ISO_639_language_code   = COMPONENT_DESC_ISO_639_LANGUAGE_CODE(b);

	desc_component->text_char = (char*)calloc(desc_component->descriptor_length - 6 + 1, sizeof(char));
	strncpy(desc_component->text_char, (char*)&b[8], desc_component->descriptor_length - 6);
	
    return (DESCRIPTOR_COMMON *)desc_component; 
}

void free_component_desc(DESCRIPTOR_COMMON* head)
{
	COMPONENT_DESC * phead = (COMPONENT_DESC *)head;
    free(phead->text_char);
	phead->text_char = NULL;
	
    free_desc(phead->next_desc);
    phead->next_desc = NULL;
	
    free(phead);
	phead = NULL;
    head  = NULL;
}

void show_component_descriptor(DESCRIPTOR_COMMON *ptmp)
{
    COMPONENT_DESC * tmp = (COMPONENT_DESC*)ptmp;
	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   network_name_descriptor\n");
	uprintf("\t\tdescriptor_length  :   0x%x\n",tmp->descriptor_length);
	uprintf("\t\tstream_content     :   0x%x(%d)\n",tmp->stream_content, tmp->stream_content);
	uprintf("\t\tcomponent_type     :   0x%x(%d)\n",tmp->component_type, tmp->component_type);
	uprintf("\t\tISO_639_langua_code:   0x%x(%d)\n",tmp->ISO_639_language_code, tmp->ISO_639_language_code);
	uprintf("\t\ttext_char          :   %s\n",tmp->text_char);
}




