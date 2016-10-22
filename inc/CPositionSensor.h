#ifndef __C_CPOSITION_SENSOR_H__
#define __C_CPOSITION_SENSOR_H__

#include "CSocket.h"
#include "CLogs.h"

#define _PS_BUFFERSIZE 10000

class CPositionSensor
{
private:
 	SOCKET m_ConnectSocket;
	
	uint8_t m_buffer[_PS_BUFFERSIZE];
	
	int m_P[2][3]; // last two good one(s) 
	double m_timestamp[2];

	int m_P_raw[2][3]; // last two raw one(s) 
	double m_timestamp_raw[2];
	
    // used inside readNewData() 
	bool isThisGoodData();
	bool isThisGoodData2();

	CLogs *m_log; // the pointer to my logging object

public:

	CPositionSensor(const char* host, const char* port, CLogs * log);
	
	// Get a first measurement from GoT 
	void init();
    
	// read new data 
	bool readNewData();

	// This contains the transformation of axes signs, and from mm to meters 
	double givePRaw(double *XYZ);
	double giveP(double *XYZ);
	double giveTimestampOld();
	double giveTimestamp();

	~CPositionSensor();
};

#endif // __C_CPOSITION_SENSOR_H__