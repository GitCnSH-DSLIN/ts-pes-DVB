/*********************************************************************
*
* Filename      :   pes.h
* Description   :   
* edited by     :   Jensen Zhen(JensenZhen@zhaoxin.com)
*
*********************************************************************/
#ifndef  _MPEG_TS_SI_PES_H_
#define  _MPEG_TS_SI_PES_H_

int store_pes_stream(FILE *pFile, FILE *storeFile,
        unsigned int ts_packet_length, unsigned int mElementaryPid);

#endif
