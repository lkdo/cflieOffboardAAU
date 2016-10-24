
#include "CLogs.h"

CLogs::CLogs()
{

	/* CF Logs, each logging block */
	m_outCFGyro = new std::ofstream(); 
	m_outCFGyro->open("output_CF_gyro.txt", std::ofstream::out);
	
	m_outCFAcc = new std::ofstream();
	m_outCFAcc->open("output_CF_acc.txt", std::ofstream::out);
	
	m_outCFMag = new std::ofstream();
	m_outCFMag->open("output_CF_mag.txt", std::ofstream::out);
	
	m_outCFMotor = new std::ofstream();
	m_outCFMotor->open("output_CF_motor.txt", std::ofstream::out);
	
	m_outCFBaro = new std::ofstream();
	m_outCFBaro->open("output_CF_baro.txt", std::ofstream::out);
	
	m_outCFBat = new std::ofstream();
	m_outCFBat->open("output_CF_bat.txt", std::ofstream::out);
	
	m_outCFStab = new std::ofstream();
	m_outCFStab->open("output_CF_stab.txt", std::ofstream::out);
	
	m_outCFSMRM_roll = new std::ofstream();
	m_outCFSMRM_roll->open("output_CF_SMRM_roll.txt", std::ofstream::out);

	m_outCFExtPos = new std::ofstream();
	m_outCFExtPos->open("output_ExtPos.txt", std::ofstream::out);

	m_outGoT = new std::ofstream();
	m_outGoT->open("output_GoT.txt", std::ofstream::out);

	m_outGoTRaw = new std::ofstream();
	m_outGoTRaw->open("output_GoT_raw.txt", std::ofstream::out);

	m_outCTRL = new std::ofstream();
	m_outCTRL->open("output_CTRL.txt", std::ofstream::out);

	m_outREF = new std::ofstream();
	m_outREF->open("output_REF.txt", std::ofstream::out);

}

void CLogs::GoT(double timestamp, double *XYZ)
{
	(*m_outGoT) << timestamp << " " << XYZ[0] << " " << XYZ[1] << " " << XYZ[2] << std::endl;
}

void CLogs::GoT_raw(double timestamp, double *XYZ)
{
	(*m_outGoTRaw) << timestamp << " " << XYZ[0] << " " << XYZ[1] << " " << XYZ[2] << std::endl;
}


void CLogs::CTRL(double timestamp, double *RPYT)
{
	(*m_outCTRL) << timestamp << " " << RPYT[0] << " " << RPYT[1] << " " << RPYT[2] << " " << RPYT[3] << std::endl;

}

void CLogs::REF(double timestamp, double *ref_XYZ)
{
	(*m_outREF) << timestamp << " " << ref_XYZ[0] << " " << ref_XYZ[1] << " " << ref_XYZ[2] << std::endl;
}

CLogs::~CLogs()
{
	m_outCFGyro->close();
	m_outCFAcc->close();
	m_outCFMag->close();
	m_outCFMotor->close();
	m_outCFBaro->close();
	m_outCFStab->close();
	m_outCFBat->close();
	m_outCFSMRM_roll->close();
	m_outCFExtPos->close();

	m_outGoT->close();
	m_outGoTRaw->close();
	m_outCTRL->close();
	m_outREF->close();
}

void CLogs::CF(CCrazyflie *cflieCopter)
{
	if (cflieCopter->gyroNewData())
	{
		(*m_outCFGyro) << cflieCopter->gyroTimestamp() << " " << cflieCopter->gyroLocalTimestamp() << " " << cflieCopter->gyroX() << " " << cflieCopter->gyroY() << " " << cflieCopter->gyroZ() << std::endl;
	}

	if (cflieCopter->accNewData())
	{
		(*m_outCFAcc) << cflieCopter->accTimestamp() << " " << cflieCopter->accLocalTimestamp() << " " << cflieCopter->accX() << " " << cflieCopter->accY() << " " << cflieCopter->accZ() << " " << cflieCopter->accZW() << " " << cflieCopter->accMag2() << std::endl;
	}

	if (cflieCopter->magNewData())
	{
		*m_outCFMag << cflieCopter->magTimestamp() << " " << cflieCopter->magLocalTimestamp() << " " << cflieCopter->magX() << " " << cflieCopter->magY() << " " << cflieCopter->magZ() << std::endl;
	}

	if (cflieCopter->motorNewData())
	{
		(*m_outCFMotor) << cflieCopter->motorTimestamp() << " " << cflieCopter->motorLocalTimestamp() << " " <<  cflieCopter->motor1() << " " << cflieCopter->motor2() << " " << cflieCopter->motor3() << " " << cflieCopter->motor4() << std::endl;
	}

	if (cflieCopter->baroNewData())
	{
		(*m_outCFBaro) << cflieCopter->baroTimestamp() << " " << cflieCopter->baroLocalTimestamp() << " " <<  cflieCopter->baroAsl() << " " << cflieCopter->baroAslLong() << " " << cflieCopter->baroAslRaw() << " " << cflieCopter->baroPressure() << " " << cflieCopter->baroTemp() << std::endl;
	}

	if (cflieCopter->stabNewData())
	{
		(*m_outCFStab) << cflieCopter->stabTimestamp() << " " << cflieCopter->stabLocalTimestamp() << " " <<  cflieCopter->stabRoll() << " " << cflieCopter->stabPitch() << " " << cflieCopter->stabYaw() << " " << cflieCopter->stabThrust() << std::endl;
	}

	if (cflieCopter->batNewData())
	{
		(*m_outCFBat) << cflieCopter->batTimestamp() << " " << cflieCopter->batLocalTimestamp() << " " << cflieCopter->batLevel() << " " << cflieCopter->batState() << " " << cflieCopter->batChargeCurrent() << std::endl;
	}
	
	if (cflieCopter->SMRM_rollNewData())
	{
		(*m_outCFSMRM_roll) << cflieCopter->SMRM_rollTimestamp() << " " << cflieCopter->SMRM_rollLocalTimestamp() << " " << cflieCopter->SMRM_rollX_hat() << " " << cflieCopter->SMRM_rollV_hat() << " " << cflieCopter->SMRM_rollTh_hat() << " " << cflieCopter->SMRM_rollOm_hat() << std::endl;
	}

	if (cflieCopter->extPosNewData())
	{
		(*m_outCFExtPos) << cflieCopter->extPosTimestamp() << " " << cflieCopter->extPosLocalTimestamp() << " " << cflieCopter->extPosX() << " " << cflieCopter->extPosY() << " " << cflieCopter->extPosZ() << std::endl;
	}

}


