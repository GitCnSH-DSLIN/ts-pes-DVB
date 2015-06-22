/*********************************************************************
*
* Filename      :   ts_si.h
* Description   :   given the fundamental struction of si protocol of DVB
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/

#ifndef  _MPEG_TS_SI_H_
#define  _MPEG_TS_SI_H_

/*********************************************************************
 *              SDT releated struction
 *   struction : sdt table --> sdt_service --> descriptor
 *   
 *********************************************************************/
//SDT service
typedef struct sdt_service {
	unsigned service_id : 16;
	unsigned reserved_future_use : 6;
	unsigned EIT_schedule_flag : 1;
	unsigned EIT_present_following_flag : 1;
	unsigned running_status : 3;
	unsigned free_CA_mode : 1;
	unsigned descriptors_loop_length : 12;
	
    void* first_desc; //point to descriptor list.

	struct sdt_service* next_sdt_service;
}SDT_SERVICE, *P_SDT_SERVICE;



//SDT table
typedef struct ts_sdt_table {
	unsigned table_id : 8;
	unsigned section_syntax_indicator : 1;
	unsigned reserved_future_use_1 : 1;
	unsigned reserved_1 : 2;
	unsigned section_length : 12;
	unsigned transport_stream_id : 16;
	unsigned reserved_2 : 2;
	unsigned version_number : 5;
	unsigned current_next_indicator : 1;
	unsigned section_number : 8;
	unsigned last_section_number : 8;
	unsigned original_network_id : 16;
	unsigned reserved_future_use_2 : 8;
	
    SDT_SERVICE* first_sdt_service;
	uint32_t CRC_32;
}TS_SDT_TABLE,*P_TS_SDT_TABLE;

/***************************** End of SDT releated struction **********************/

#endif   /* ----- #ifndef _MPEG_TS_SI_H_  ----- */
