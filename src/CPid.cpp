#include "CPid.h"

CPid::CPid(double kp, double ki, double kd, double iLimitHigh, double iLimitLow, double maxValue, double minValue, double offset, double ref)
{
	m_kp = kp;
	m_ki = ki;
	m_kd = kd;

	m_iLimitHigh = iLimitHigh;
	m_iLimitLow = iLimitLow;
	m_maxValue = maxValue;
	m_minValue = minValue;
	m_offset = offset;

	m_ref = ref;

	m_prevError = 0;
	m_prevMeas = -99999.0;
	m_integrError = 0;
}

void CPid::setRef(double new_ref)
{
	m_ref = new_ref;
}

double CPid::update(double measured, double dt)
{

	if (m_prevMeas < -99998.0)
	{
		m_prevMeas = measured;
	}

	double error = m_ref - measured;
	m_integrError += error*dt;
	if (m_integrError > m_iLimitHigh)
	{
		m_integrError = m_iLimitHigh;
	}
	else if (m_integrError < m_iLimitLow)
	{
		m_integrError = m_iLimitLow;
	}
	
    // double deriv = (error - m_prevError) / dt;
	// m_prevError = error;
	
	double deriv = -(measured - m_prevMeas) / dt;
	m_prevMeas = measured;

	double out = m_kp*error + m_ki*m_integrError + m_kd*deriv + m_offset;

	if (out > m_maxValue)
	{
		return m_maxValue;
	}
	else if (out < m_minValue)
	{
		return m_minValue;
	}

	return out;
}

CPid::~CPid()
{
}
