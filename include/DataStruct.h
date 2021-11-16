#pragma once

#define MAX_ADJUST_METER_NUM	30

typedef struct
{
	unsigned long m_sLongitude;
	unsigned long m_sLatitude;
	unsigned long Course;
}NAV_DATA_STRUCT_Save;

typedef struct TANKMETERDATA
{
	long meter_AD_DATA[MAX_ADJUST_METER_NUM];

	long meter_runmotortime;
	long meter_totalmotorhour;
	long meter_currentmilege;
	long meter_totalmilege;
	long meter_tankspeed;
	long meter_motorrotate;
	NAV_DATA_STRUCT_Save NAV_DATA;
}TANKMETERDATA;

typedef struct
{
	__time32_t curtime;
	TANKMETERDATA Meterdata;
} TANK_INFO_STRUCT_JSY;

typedef struct
{
	int isfull;
	unsigned int lastrecordno;
	unsigned int offset;
	unsigned int type_id;
} TSI_FILE_INFO_STRUCT;

//#define BigLittleSwap32(A) ((((INT)A & 0xFF000000) >> 24) | (((INT)A & 0x00FF0000) >> 8) | (((INT)A & 0x0000FF00) << 8) | (((INT)A & 0x000000FF) << 24))
#define BigLittleSwap32(A) (int((((UINT)A & 0xFF000000) >> 24) | (((UINT)A & 0x00FF0000) >> 8) | (((UINT)A & 0x0000FF00) << 8) | (((UINT)A & 0x000000FF) << 24)))

