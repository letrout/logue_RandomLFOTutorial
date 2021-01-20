///////////////////////////////////////////////////////////
//
// Chamberlin filter DSP implementation
//
// (c) 2021 Joel Luth
//
// Based on the work of hammondeggsmusic
// https://github.com/hammondeggs/logue_RandomLFOTutorial
//
///////////////////////////////////////////////////////////

#include "chamberlin_filter.hpp"

ChamberlinFilter::ChamberlinFilter():
delay_1(0), delay_2(0), f1(0), fc(INITIAL_FREQUENCY), fq(FIXED_Q), out_bp(0), out_hp(0), out_lp(0), out_n(0), fs(SAMPLE_RATE)
{
}

ChamberlinFilter::ChamberlinFilter(float filterc, float filterq, float sample_rate)
{
	delay_1 = 0.;
	delay_2 = 0.;
	f1 = 0.;
	SetFc(filterc);
	SetQ(filterq);
	out_hp = 0.;
	out_bp = 0.;
	out_lp = 0;
	out_n = 0;
	fs = sample_rate;
}

/**
 * Set the filter center frequency.
 * If set below FC_MIN or above FC_MAX, set to FC_MIN or FC_MAX respectively
 *
 * @param new_fc New center frequency (Hz).
 * @return 0 on success
 * 	nonzero - failure to set, return the center freq
 */
int ChamberlinFilter::SetFc(float new_fc)
{
	if (new_fc < FC_MIN)
	{
		fc = FC_MIN;
		return fc;
	}
	else if (new_fc > FC_MAX)
	{
		// TODO: Should take sample rate into consideration as well
		// (without oversampling, fc should be < fs/6)
		fc = FC_MAX;
		return fc;
	}
	fc = new_fc;
	return 0;
}

/**
 * Set the filter resonance.
 * If set below Q_MIN or above Q_MAX, set to Q_MIN or QAX respectively
 *
 * @param new_fq New Q
 * @return 0 on success
 * 	nonzero - failure to set, return the Q
 */
int ChamberlinFilter::SetQ(float new_fq)
{
	if (new_fq < Q_MIN)
	{
		fq = Q_MIN;
		return fq;
	}
	else if (new_fq > Q_MAX)
	{
		fq = Q_MAX;
		return fq;
	}
	fq = new_fq;
	return 0;
}
