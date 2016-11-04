
#include <conio.h>

#include "clockgettime.h"
#include "CPositionSensor.h"

CPositionSensor::CPositionSensor(const char* host, const char* port, CLogs * log)
{
	m_ConnectSocket = createReadSocket(host, port);
	m_log = log;
}

void CPositionSensor::init(int transID)
{
	int ch=0;
	int nbr_bytes_read = 0;
	long int GoT_timestamp, last_GoT_timestamp=0;
	int nbr_params_read = 0;
	int tID;

	m_transmitter_ID = transID;
	
	do
	{
		nbr_bytes_read = readFromSocket(m_ConnectSocket, (char *)m_buffer, _PS_BUFFERSIZE);
        m_timestamp_raw[0] = currentTime();

		std::cout << (char *) m_buffer << std::endl;
		std::cout << nbr_bytes_read << std::endl;

		std::istringstream stream_buffer((char *)m_buffer);
		char split_char = '\n';
		std::string each, last_packet;
		
		while (!std::getline(stream_buffer, each, split_char).eof())
		{
			nbr_params_read = sscanf_s(each.c_str(), "$GTPOS,%ld,%d,%*d,%*d,%*d\n", &GoT_timestamp, &tID);
			if (nbr_params_read == 2)  // if line parsed correctly 
			{
				if (m_transmitter_ID)
				{
					if (tID != m_transmitter_ID)
						continue; // skip line if not belonging to the right transmitter ID 
				}
				if (GoT_timestamp > last_GoT_timestamp)
				{ 
					last_packet = each;
					last_GoT_timestamp = GoT_timestamp;
				}
			}
		} 

		nbr_params_read = sscanf_s(last_packet.c_str(), "$GTPOS,%ld,%d,%d,%d,%d\n", &GoT_timestamp, &tID, &m_P_raw[0][0], &m_P_raw[0][1], &m_P_raw[0][2]);
		if (nbr_params_read == 5) // succesfully parsed and extracted 4 parameters
		{ 
			std::cout << " Measured Position:" << GoT_timestamp << " " << tID << " "  << m_P_raw[0][0] << " " << m_P_raw[0][1] << " " << m_P_raw[0][2] << std::endl;
			std::cout << " Accept as start measurement? (y/n)";
			ch = _getch();
			std::cout << (char)ch << std::endl;
		}
	} 
	while (ch != 121);

	// This was the human filter, so accept it as good data 
	m_P[0][0] = m_P_raw[0][0];
	m_P[0][1] = m_P_raw[0][1];
	m_P[0][2] = m_P_raw[0][2];
	m_timestamp[0] = m_timestamp_raw[0];
	m_GoT_timestamp[0] = GoT_timestamp;
	m_GoT_timestamp_raw[0] = GoT_timestamp;
}

bool CPositionSensor::readNewData()
{
	static int nbr = 0;
	static int tmp[3];
	static long int GoT_timestamp, last_GoT_timestamp = 0;
	static double tmp2[3], tmp3;
	int nbr_params_read = 0;
	int tID;

	nbr = readFromSocket(m_ConnectSocket, (char *)m_buffer, _PS_BUFFERSIZE);
	tmp3 = currentTime();

	std::istringstream stream_buffer((char *)m_buffer);
	char split_char = '\n';
	std::string each, last_packet;

	while (!std::getline(stream_buffer, each, split_char).eof())
	{
		nbr_params_read = sscanf_s(each.c_str(), "$GTPOS,%ld,%d,%*d,%*d,%*d\n", &GoT_timestamp, &tID);
		if (nbr_params_read == 2) // if line parsed correctly
		{
			if (m_transmitter_ID)
			{
				if (tID != m_transmitter_ID)
				{
					continue; 
				}
			}
			if (GoT_timestamp > last_GoT_timestamp)
			{
				last_packet = each;
				last_GoT_timestamp = GoT_timestamp;
			}
		}
	}

	if (last_packet.length() > 0 ) // new raw data has been read 
	{
		nbr_params_read = sscanf_s(last_packet.c_str(), "$GTPOS,%ld,%d,%d,%d,%d\n", &GoT_timestamp, &tID, &tmp[0], &tmp[1], &tmp[2]);
		if (nbr_params_read == 5) // Succesfully read raw data
		{

			// Move the raw data back
			m_P_raw[1][0] = m_P_raw[0][0];
			m_P_raw[1][1] = m_P_raw[0][1];
			m_P_raw[1][2] = m_P_raw[0][2];
			m_timestamp_raw[1] = m_timestamp_raw[0];
			m_GoT_timestamp_raw[1] = m_GoT_timestamp_raw[0];

			// Put the new data in 
			m_P_raw[0][0] = tmp[0];
			m_P_raw[0][1] = tmp[1];
			m_P_raw[0][2] = tmp[2];
			m_timestamp_raw[0] = tmp3;
			m_GoT_timestamp_raw[0] = GoT_timestamp;

// Logging
this->givePRaw(tmp2);
m_log->GoT_raw(m_timestamp_raw[0], m_GoT_timestamp_raw[0], tmp2);

			if (this->isThisGoodData())
			{
				
				// Shift good data
				m_P[1][0] = m_P[0][0];
				m_P[1][1] = m_P[0][1];
				m_P[1][2] = m_P[0][2];
				m_timestamp[1] = m_timestamp[0];
				m_GoT_timestamp[1] = m_GoT_timestamp[0];

				// Copy new good data from raw
				m_P[0][0] = m_P_raw[0][0];
				m_P[0][1] = m_P_raw[0][1];
				m_P[0][2] = m_P_raw[0][2];
				m_timestamp[0] = m_timestamp_raw[0];
				m_GoT_timestamp[0] = GoT_timestamp;
// Logging
this->giveP(tmp2);
m_log->GoT(m_timestamp[0], m_GoT_timestamp[0], tmp2);

				return true;
			}
		}
	}

	return false;
}

// Make the transform to XYZ NED, meters
double CPositionSensor::givePRaw(double *XYZ)
{
	XYZ[0] = m_P_raw[0][0]/1000.0;
	XYZ[1] = m_P_raw[0][1]/1000.0;
	XYZ[2] = m_P_raw[0][2]/1000.0;
	return (m_timestamp_raw[0] - m_timestamp_raw[1]);
}

// Make the transform to XYZ NED, meters
double CPositionSensor::giveP(double *XYZ)
{
	XYZ[0] = m_P[0][0]/1000.0;
	XYZ[1] = m_P[0][1]/1000.0;
	XYZ[2] = m_P[0][2]/1000.0;
	return (m_timestamp[0] - m_timestamp[1]);
}


double CPositionSensor::giveTimestampOld()
{
	return m_timestamp[1];
}

double CPositionSensor::giveTimestamp()
{
	return m_timestamp[0];
}

bool CPositionSensor::isThisGoodData()
{
	// Version 1
	if ((m_P_raw[0][0] - m_P[0][0])*(m_P_raw[0][0] - m_P[0][0]) +
		(m_P_raw[0][1] - m_P[0][1])*(m_P_raw[0][1] - m_P[0][1]) +
		(m_P_raw[0][2] - m_P[0][2])*(m_P_raw[0][2] - m_P[0][2]) <
		   2000*2000*(m_timestamp_raw[0] - m_timestamp[0])*(m_timestamp_raw[0] - m_timestamp[0]) )  // max accep. square speed in (mm/s)^2
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool CPositionSensor::isThisGoodData2()
{
	// Version 2
	if ((m_P_raw[0][0] - m_P_raw[1][0])*(m_P_raw[0][0] - m_P_raw[1][0]) +
		(m_P_raw[0][1] - m_P_raw[1][1])*(m_P_raw[0][1] - m_P_raw[1][1]) +
		(m_P_raw[0][2] - m_P_raw[1][2])*(m_P_raw[0][2] - m_P_raw[1][2]) < 100*100)  // dt should be about 0.1			
	{
		return true;
	}
	else
	{
		return false;
	}

}

CPositionSensor::~CPositionSensor()
{
	closesocket(m_ConnectSocket);
	WSACleanup();

}
