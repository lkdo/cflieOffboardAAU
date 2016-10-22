
#include <math.h>

#include "CPositionControllerSimple.h"
#include "clockgettime.h"

CPositionController::CPositionController(double *ctrl_ref_XYZ, CLogs *log)
{
	m_log = log;

	// Logging 
	m_log->REF(currentTime(), ctrl_ref_XYZ);
	
	m_PidRoll = new CPid(_PID_ROLL_KP, _PID_ROLL_KI, _PID_ROLL_KD, _PID_ROLL_iLIMITHIGH, _PID_ROLL_iLIMITLOW, _PID_ROLL_MAXVALUE, _PID_ROLL_MINVALUE, _PID_ROLL_OFFSET, ctrl_ref_XYZ[1]);
	m_PidPitch = new CPid(_PID_PITCH_KP, _PID_PITCH_KI, _PID_PITCH_KD, _PID_PITCH_iLIMITHIGH, _PID_PITCH_iLIMITLOW, _PID_PITCH_MAXVALUE, _PID_PITCH_MINVALUE, _PID_PITCH_OFFSET, -ctrl_ref_XYZ[0]);
	m_PidThrust = new CPid(_PID_THRUST_KP, _PID_THRUST_KI, _PID_THRUST_KD, _PID_THRUST_iLIMITHIGH, _PID_THRUST_iLIMITLOW, _PID_THRUST_MAXVALUE, _PID_THRUST_MINVALUE, _PID_THRUST_OFFSET, -ctrl_ref_XYZ[2]);

}

CPositionController::~CPositionController()
{
	delete m_PidRoll;
	delete m_PidPitch;
	delete m_PidThrust;
}

void CPositionController::update(double *sensor_XYZ, double sensor_dt)
{
	m_cmdRPYT[0] = m_PidRoll->update(sensor_XYZ[1], sensor_dt);          // roll
	m_cmdRPYT[1] = m_PidPitch->update(-sensor_XYZ[0], sensor_dt);         // pitch
	m_cmdRPYT[2] = 0;                                                    // yaw 
	m_cmdRPYT[3] = m_PidThrust->update(-sensor_XYZ[2], sensor_dt);        // thrust

// Logging 
m_log->CTRL(currentTime(), m_cmdRPYT);

}

void CPositionController::setRef(double *ctrl_ref_XYZ)
{

    // Logging 
	m_log->REF(currentTime(), ctrl_ref_XYZ);

	m_PidPitch->setRef(-ctrl_ref_XYZ[0]);
	m_PidRoll->setRef(ctrl_ref_XYZ[1]);
	m_PidThrust->setRef(-ctrl_ref_XYZ[2]);

}

void CPositionController::giveCmd(double *ctrl_cmd_RPYT)
{
	ctrl_cmd_RPYT[0] = m_cmdRPYT[0];
	ctrl_cmd_RPYT[1] = m_cmdRPYT[1];
	ctrl_cmd_RPYT[2] = m_cmdRPYT[2];
	ctrl_cmd_RPYT[3] = m_cmdRPYT[3];
}
