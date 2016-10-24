#ifndef __C_CLOGS_H__
#define __C_CLOGS_H__

#include <iostream>
#include <fstream>

#include "CCrazyflie.h"

class CLogs
{
private:

	std::ofstream *m_outCFGyro; 
	std::ofstream *m_outCFAcc; 
	std::ofstream *m_outCFMag; 
	std::ofstream *m_outCFMotor;
	std::ofstream *m_outCFBaro; 
	std::ofstream *m_outCFBat; 
	std::ofstream *m_outCFStab; 
	std::ofstream *m_outCFSMRM_roll;
	std::ofstream *m_outCFExtPos; // as received by the firmware, and send back through the logs

	std::ofstream *m_outGoT;
	std::ofstream *m_outGoTRaw;
	std::ofstream *m_outCTRL;
	std::ofstream *m_outREF;
	
public:

	CLogs();
	
	void GoT(double timestamp, double *XYZ);
	void GoT_raw(double timestamp, double *XYZ);

	void CTRL(double timestamp, double *RPYT);
	void REF(double timestamp, double *ref_XYZ);
	void CF(CCrazyflie *cflieCopter);
	
	~CLogs();
};

#endif // __C_CRAZYFLIE_H__
