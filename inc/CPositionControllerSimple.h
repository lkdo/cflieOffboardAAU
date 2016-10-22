#ifndef __C_CPOSITIONCONTROLLERSIMPLE_H__
#define __C_CPOSITIONCONTROLLERSIMPLE_H__

#include "CPID.h"
#include "CLogs.h"

// Roll
#define _PID_ROLL_KP  5.0f
#define _PID_ROLL_KI  0.0f
#define _PID_ROLL_KD  6.5f
#define _PID_ROLL_iLIMITHIGH 606.0f
#define _PID_ROLL_iLIMITLOW -606.0f
#define _PID_ROLL_MAXVALUE 35.0f
#define _PID_ROLL_MINVALUE -35.0f
#define _PID_ROLL_OFFSET 0.0f

// Pitch 
#define _PID_PITCH_KP 5.0f
#define _PID_PITCH_KI 0.0f
#define _PID_PITCH_KD 6.5f
#define _PID_PITCH_iLIMITHIGH 606.0f
#define _PID_PITCH_iLIMITLOW -606.0f
#define _PID_PITCH_MAXVALUE 35.0f
#define _PID_PITCH_MINVALUE -35.0f
#define _PID_PITCH_OFFSET 0.0f

// Thrust
#define _PID_THRUST_KP 15000.00f
#define _PID_THRUST_KI 4000.0f
#define _PID_THRUST_KD 12000.0f
#define _PID_THRUST_iLIMITHIGH 20000.0f
#define _PID_THRUST_iLIMITLOW -20000.0f
#define _PID_THRUST_MAXVALUE 65000.0f
#define _PID_THRUST_MINVALUE 0.0f
#define _PID_THRUST_OFFSET 36000.0f

class CPositionController
{
private:

	CPid *m_PidRoll;
	CPid *m_PidPitch;
	CPid *m_PidThrust;

	double m_cmdRPYT[4];

	CLogs * m_log;

public:
	
	CPositionController(double *ref_XYZ, CLogs *log);
	void update(double *sensor_XYZ, double sensor_dt);
	void setRef(double *ctrl_ref_XYZ);
    void giveCmd(double *ctrl_cmd_RPYT);
	~CPositionController();
};
#endif // __C_CPOSITIONCONTROLLERSIMPLE_H__
