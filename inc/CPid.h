#ifndef __C_CPID_H__
#define __C_CPID_H__
class CPid
{
private:
	double m_ki;
	double m_kp;
	double m_kd;

	double m_prevError;
	double m_prevMeas; // alternativt D
	double m_integrError;
	double m_ref;

	double m_iLimitHigh;
	double m_iLimitLow;
	double m_minValue;
	double m_maxValue;

	double m_offset;

public:

	CPid(double kp, double ki, double kd, double iLimitHigh, double iLimitLow, double maxValue, double minValue, double offset, double ref);
	
	void setRef(double new_ref);

	double update(double measured, double dt);

	~CPid();
};
#endif // __C_CPID_H__