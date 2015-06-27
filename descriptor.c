#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <print_debug.h>
#include <sdt.h>
#include <ts_psi.h>
#include <desc_short_event.h>
#include <desc_network_name.h>
#include <desc_service.h>
#include <desc_service_list.h>

SDT_DESCRIPTOR_COMMON * decode_default(unsigned char * byteptr, int this_section_length)
{
    unsigned char * b = byteptr;

    SDT_DESCRIPTOR_COMMON *desc_default = (SDT_DESCRIPTOR_COMMON *)malloc(sizeof(SDT_DESCRIPTOR_COMMON));
    memset(desc_default, 0, sizeof(SDT_DESCRIPTOR_COMMON));
    desc_default->descriptor_tag = b[0];
    desc_default->descriptor_length = b[1];
    
    return desc_default;
}

SDT_DESCRIPTOR_COMMON * (*do_decode_desc[192])(unsigned char * byteptr, int this_section_length) = {
    /*decode for each descriptors*/
    decode_network_name_desc,                   //0x40
    decode_service_list_desc,                   //0x41
    decode_default,                               //0x42
    decode_default,                               //0x43
    decode_default,                                       //0x44
    decode_default,                                       //0x45
    decode_default,                                       //0x46
    decode_default,                                       //0x47
    decode_service_desc,                        //0x48
    decode_default,                                       //0x49
    decode_default,                                       //0x4a
    decode_default,                                       //0x4b
    decode_default,                                       //0x4c
    decode_short_event_desc,                    //0x4d
    decode_default,                                       //0x4e
    decode_default,                                       //0x4f
    decode_default,                                       //0x50
    decode_default,                                       //0x51
    decode_default,                                       //0x52
    decode_default,                                       //0x53
    decode_default,                                       //0x54
    decode_default,                                       //0x55
    decode_default,                                       //0x56
    decode_default,                                       //0x57
    decode_default,                                       //0x58
    decode_default,                                       //0x59
    decode_default,                                       //0x5a
    decode_default,                                       //0x5b
    decode_default,                                       //0x5c
    decode_default,                                       //0x5d
    decode_default,                                       //0x5e
    decode_default,                                       //0x5f
    decode_default,                                       //0x60
    decode_default,                                       //0x61
    decode_default,                                       //0x62
    decode_default,                                       //0x63
    decode_default,                                       //0x64
    decode_default,                                       //0x65
    decode_default,                                       //0x66
    decode_default,                                       //0x67
    decode_default,                                       //0x68
    decode_default,                                       //0x69
    decode_default,                                       //0x6a
    decode_default,                                       //0x6b
    decode_default,                                       //0x6c
    decode_default,                                       //0x6d
    decode_default,                                       //0x6e
    decode_default,                                       //0x6f
    decode_default,                                       //0x70
    decode_default,                                       //0x71
    decode_default,                                       //0x72
    decode_default,                                       //0x73
    decode_default,                                       //0x74
    decode_default,                                       //0x75
    decode_default,                                       //0x76
    decode_default,                                       //0x77
    decode_default,                                       //0x78
    decode_default,                                       //0x79
    decode_default,                                       //0x7a
    decode_default,                                       //0x7b
    decode_default,                                       //0x7c
    decode_default,                                       //0x7d
    decode_default,                                       //0x7e
    decode_default,                                       //0x7f
    decode_default,                                       //0x80
    decode_default,                                       //0x81
    decode_default,                                       //0x82
    decode_default,                                       //0x83
    decode_default,                                       //0x84
    decode_default,                                       //0x85
    decode_default,                                       //0x86
    decode_default,                                       //0x87
    decode_default,                                       //0x88
    decode_default,                                       //0x89
    decode_default,                                       //0x8a
    decode_default,                                       //0x8b
    decode_default,                                       //0x8c
    decode_default,                                       //0x8d
    decode_default,                                       //0x8e
    decode_default,                                       //0x8f
    decode_default,                                       //0x90
    decode_default,                                       //0x91
    decode_default,                                       //0x92
    decode_default,                                       //0x93
    decode_default,                                       //0x94
    decode_default,                                       //0x95
    decode_default,                                       //0x96
    decode_default,                                       //0x97
    decode_default,                                       //0x98
    decode_default,                                       //0x99
    decode_default,                                       //0x9a
    decode_default,                                       //0x9b
    decode_default,                                       //0x9c
    decode_default,                                       //0x9d
    decode_default,                                       //0x9e
    decode_default,                                       //0x9f
    decode_default,                                       //0xa0
    decode_default,                                       //0xa1
    decode_default,                                       //0xa2
    decode_default,                                       //0xa3
    decode_default,                                       //0xa4
    decode_default,                                       //0xa5
    decode_default,                                       //0xa6
    decode_default,                                       //0xa7
    decode_default,                                       //0xa8
    decode_default,                                       //0xa9
    decode_default,                                       //0xaa
    decode_default,                                       //0xab
    decode_default,                                       //0xac
    decode_default,                                       //0xad
    decode_default,                                       //0xae
    decode_default,                                       //0xaf
    decode_default,                                       //0xb0
    decode_default,                                       //0xb1
    decode_default,                                       //0xb2
    decode_default,                                       //0xb3
    decode_default,                                       //0xb4
    decode_default,                                       //0xb5
    decode_default,                                       //0xb6
    decode_default,                                       //0xb7
    decode_default,                                       //0xb8
    decode_default,                                       //0xb9
    decode_default,                                       //0xba
    decode_default,                                       //0xbb
    decode_default,                                       //0xbc
    decode_default,                                       //0xbd
    decode_default,                                       //0xbe
    decode_default,                                       //0xbf
    decode_default,                                       //0xc0
    decode_default,                                       //0xc1
    decode_default,                                       //0xc2
    decode_default,                                       //0xc3
    decode_default,                                       //0xc4
    decode_default,                                       //0xc5
    decode_default,                                       //0xc6
    decode_default,                                       //0xc7
    decode_default,                                       //0xc8
    decode_default,                                       //0xc9
    decode_default,                                       //0xca
    decode_default,                                       //0xcb
    decode_default,                                       //0xcc
    decode_default,                                       //0xcd
    decode_default,                                       //0xce
    decode_default,                                       //0xcf
    decode_default,                                       //0xd0
    decode_default,                                       //0xd1
    decode_default,                                       //0xd2
    decode_default,                                       //0xd3
    decode_default,                                       //0xd4
    decode_default,                                       //0xd5
    decode_default,                                       //0xd6
    decode_default,                                       //0xd7
    decode_default,                                       //0xd8
    decode_default,                                       //0xd9
    decode_default,                                       //0xda
    decode_default,                                       //0xdb
    decode_default,                                       //0xdc
    decode_default,                                       //0xdd
    decode_default,                                       //0xde
    decode_default,                                       //0xdf
    decode_default,                                       //0xe0
    decode_default,                                       //0xe1
    decode_default,                                       //0xe2
    decode_default,                                       //0xe3
    decode_default,                                       //0xe4
    decode_default,                                       //0xe5
    decode_default,                                       //0xe6
    decode_default,                                       //0xe7
    decode_default,                                       //0xe8
    decode_default,                                       //0xe9
    decode_default,                                       //0xea
    decode_default,                                       //0xeb
    decode_default,                                       //0xec
    decode_default,                                       //0xed
    decode_default,                                       //0xee
    decode_default,                                       //0xef
    decode_default,                                       //0xf0
    decode_default,                                       //0xf1
    decode_default,                                       //0xf2
    decode_default,                                       //0xf3
    decode_default,                                       //0xf4
    decode_default,                                       //0xf5
    decode_default,                                       //0xf6
    decode_default,                                       //0xf7
    decode_default,                                       //0xf8
    decode_default,                                       //0xf9
    decode_default,                                       //0xfa
    decode_default,                                       //0xfb
    decode_default,                                       //0xfc
    decode_default,                                       //0xfd
    decode_default,                                       //0xfe
    decode_default,                                       //0xff
};




SDT_DESCRIPTOR_COMMON * decode_desc(unsigned char * byteptr, int this_section_length)
{
    unsigned descriptor_tag = byteptr[0];
    unsigned char * b = byteptr;
    int l = this_section_length;
    unsigned int desc_len = 0;
    SDT_DESCRIPTOR_COMMON * this_descriptor = NULL;
        
    this_descriptor = (*do_decode_desc[descriptor_tag - 0x40])(b,l);
    
    desc_len = this_descriptor->descriptor_length + 2;

    b += desc_len;
    l -= desc_len;

    if(l > 0)
		this_descriptor->next_desc = decode_desc(b, l);

    return this_descriptor;
}



void free_desc(void* phead)
{
	//void* this_descriptor = phead;
	unsigned char* b = (unsigned char *)phead;

	if(b == NULL){
		return;
	}

    switch(b[0]){
	case 0x40:  // NETWORK_NAME_DESC
		free_network_name_desc((NETWORK_NAME_DESC*)phead);
		break;
	case 0x41: //SERVICE_LIST_DESC
		free_servicelist_desc((SERVICE_LIST_DESC*)phead);
		break;
	case 0x48: //SERVICE_DESC
		free_service_desc((SERVICE_DESC*)phead);
		break;
	case 0x4d: //SHORT_EVENT_DESC
		free_short_evt_desc((SHORT_EVENT_DESC*)phead);
		break;
	default:
		//free_desc();
		return;
	}
}



void show_desc_default(SDT_DESCRIPTOR_COMMON *ptmp)
{
    SDT_DESCRIPTOR_COMMON * tmp = ptmp;
	uprintf("\t\tdescriptor_tag     :   0x%x\n",tmp->descriptor_tag);
	uprintf("\t\tdescriptor_name    :   user defined descriptor(unknown)\n");
	uprintf("\t\tdescriptor_length  :   0x%x\n\n",tmp->descriptor_length);
}



void (*do_show_descriptors_info[192])(SDT_DESCRIPTOR_COMMON *ptmp) = {
    /*show info for each descriptors*/
    show_network_name_descriptor,               //0x40
    show_service_list_descriptor,               //0x41
    show_desc_default,                                       //0x42
    show_desc_default,                                       //0x43
    show_desc_default,                                       //0x44
    show_desc_default,                                       //0x45
    show_desc_default,                                       //0x46
    show_desc_default,                                       //0x47
    show_service_descriptor,                    //0x48
    show_desc_default,                                       //0x49
    show_desc_default,                                       //0x4a
    show_desc_default,                                       //0x4b
    show_desc_default,                                       //0x4c
    show_short_event_descriptor,                //0x4d
    show_desc_default,                                       //0x4e
    show_desc_default,                                       //0x4f
    show_desc_default,                                       //0x50
    show_desc_default,                                       //0x51
    show_desc_default,                                       //0x52
    show_desc_default,                                       //0x53
    show_desc_default,                                       //0x54
    show_desc_default,                                       //0x55
    show_desc_default,                                       //0x56
    show_desc_default,                                       //0x57
    show_desc_default,                                       //0x58
    show_desc_default,                                       //0x59
    show_desc_default,                                       //0x5a
    show_desc_default,                                       //0x5b
    show_desc_default,                                       //0x5c
    show_desc_default,                                       //0x5d
    show_desc_default,                                       //0x5e
    show_desc_default,                                       //0x5f
    show_desc_default,                                       //0x60
    show_desc_default,                                       //0x61
    show_desc_default,                                       //0x62
    show_desc_default,                                       //0x63
    show_desc_default,                                       //0x64
    show_desc_default,                                       //0x65
    show_desc_default,                                       //0x66
    show_desc_default,                                       //0x67
    show_desc_default,                                       //0x68
    show_desc_default,                                       //0x69
    show_desc_default,                                       //0x6a
    show_desc_default,                                       //0x6b
    show_desc_default,                                       //0x6c
    show_desc_default,                                       //0x6d
    show_desc_default,                                       //0x6e
    show_desc_default,                                       //0x6f
    show_desc_default,                                       //0x70
    show_desc_default,                                       //0x71
    show_desc_default,                                       //0x72
    show_desc_default,                                       //0x73
    show_desc_default,                                       //0x74
    show_desc_default,                                       //0x75
    show_desc_default,                                       //0x76
    show_desc_default,                                       //0x77
    show_desc_default,                                       //0x78
    show_desc_default,                                       //0x79
    show_desc_default,                                       //0x7a
    show_desc_default,                                       //0x7b
    show_desc_default,                                       //0x7c
    show_desc_default,                                       //0x7d
    show_desc_default,                                       //0x7e
    show_desc_default,                                       //0x7f
    show_desc_default,                                       //0x80
    show_desc_default,                                       //0x81
    show_desc_default,                                       //0x82
    show_desc_default,                                       //0x83
    show_desc_default,                                       //0x84
    show_desc_default,                                       //0x85
    show_desc_default,                                       //0x86
    show_desc_default,                                       //0x87
    show_desc_default,                                       //0x88
    show_desc_default,                                       //0x89
    show_desc_default,                                       //0x8a
    show_desc_default,                                       //0x8b
    show_desc_default,                                       //0x8c
    show_desc_default,                                       //0x8d
    show_desc_default,                                       //0x8e
    show_desc_default,                                       //0x8f
    show_desc_default,                                       //0x90
    show_desc_default,                                       //0x91
    show_desc_default,                                       //0x92
    show_desc_default,                                       //0x93
    show_desc_default,                                       //0x94
    show_desc_default,                                       //0x95
    show_desc_default,                                       //0x96
    show_desc_default,                                       //0x97
    show_desc_default,                                       //0x98
    show_desc_default,                                       //0x99
    show_desc_default,                                       //0x9a
    show_desc_default,                                       //0x9b
    show_desc_default,                                       //0x9c
    show_desc_default,                                       //0x9d
    show_desc_default,                                       //0x9e
    show_desc_default,                                       //0x9f
    show_desc_default,                                       //0xa0
    show_desc_default,                                       //0xa1
    show_desc_default,                                       //0xa2
    show_desc_default,                                       //0xa3
    show_desc_default,                                       //0xa4
    show_desc_default,                                       //0xa5
    show_desc_default,                                       //0xa6
    show_desc_default,                                       //0xa7
    show_desc_default,                                       //0xa8
    show_desc_default,                                       //0xa9
    show_desc_default,                                       //0xaa
    show_desc_default,                                       //0xab
    show_desc_default,                                       //0xac
    show_desc_default,                                       //0xad
    show_desc_default,                                       //0xae
    show_desc_default,                                       //0xaf
    show_desc_default,                                       //0xb0
    show_desc_default,                                       //0xb1
    show_desc_default,                                       //0xb2
    show_desc_default,                                       //0xb3
    show_desc_default,                                       //0xb4
    show_desc_default,                                       //0xb5
    show_desc_default,                                       //0xb6
    show_desc_default,                                       //0xb7
    show_desc_default,                                       //0xb8
    show_desc_default,                                       //0xb9
    show_desc_default,                                       //0xba
    show_desc_default,                                       //0xbb
    show_desc_default,                                       //0xbc
    show_desc_default,                                       //0xbd
    show_desc_default,                                       //0xbe
    show_desc_default,                                       //0xbf
    show_desc_default,                                       //0xc0
    show_desc_default,                                       //0xc1
    show_desc_default,                                       //0xc2
    show_desc_default,                                       //0xc3
    show_desc_default,                                       //0xc4
    show_desc_default,                                       //0xc5
    show_desc_default,                                       //0xc6
    show_desc_default,                                       //0xc7
    show_desc_default,                                       //0xc8
    show_desc_default,                                       //0xc9
    show_desc_default,                                       //0xca
    show_desc_default,                                       //0xcb
    show_desc_default,                                       //0xcc
    show_desc_default,                                       //0xcd
    show_desc_default,                                       //0xce
    show_desc_default,                                       //0xcf
    show_desc_default,                                       //0xd0
    show_desc_default,                                       //0xd1
    show_desc_default,                                       //0xd2
    show_desc_default,                                       //0xd3
    show_desc_default,                                       //0xd4
    show_desc_default,                                       //0xd5
    show_desc_default,                                       //0xd6
    show_desc_default,                                       //0xd7
    show_desc_default,                                       //0xd8
    show_desc_default,                                       //0xd9
    show_desc_default,                                       //0xda
    show_desc_default,                                       //0xdb
    show_desc_default,                                       //0xdc
    show_desc_default,                                       //0xdd
    show_desc_default,                                       //0xde
    show_desc_default,                                       //0xdf
    show_desc_default,                                       //0xe0
    show_desc_default,                                       //0xe1
    show_desc_default,                                       //0xe2
    show_desc_default,                                       //0xe3
    show_desc_default,                                       //0xe4
    show_desc_default,                                       //0xe5
    show_desc_default,                                       //0xe6
    show_desc_default,                                       //0xe7
    show_desc_default,                                       //0xe8
    show_desc_default,                                       //0xe9
    show_desc_default,                                       //0xea
    show_desc_default,                                       //0xeb
    show_desc_default,                                       //0xec
    show_desc_default,                                       //0xed
    show_desc_default,                                       //0xee
    show_desc_default,                                       //0xef
    show_desc_default,                                       //0xf0
    show_desc_default,                                       //0xf1
    show_desc_default,                                       //0xf2
    show_desc_default,                                       //0xf3
    show_desc_default,                                       //0xf4
    show_desc_default,                                       //0xf5
    show_desc_default,                                       //0xf6
    show_desc_default,                                       //0xf7
    show_desc_default,                                       //0xf8
    show_desc_default,                                       //0xf9
    show_desc_default,                                       //0xfa
    show_desc_default,                                       //0xfb
    show_desc_default,                                       //0xfc
    show_desc_default,                                       //0xfd
    show_desc_default,                                       //0xfe
    show_desc_default,                                       //0xff
};


