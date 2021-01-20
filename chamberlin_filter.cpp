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

// TODO: pass fs to constructor
ChamberlinFilter::ChamberlinFilter():
delay_1(0), delay_2(0), f1(0), fc(INITIAL_FREQUENCY), fq(FIXED_Q), out_bp(0), out_hp(0), out_lp(0), out_n(0), fs(SAMPLE_RATE)
{
}

ChamberlinFilter::ChamberlinFilter(float fc, float q):
delay_1(0), delay_2(0), f1(0), fc(fc), fq(q), out_bp(0), out_hp(0), out_lp(0), out_n(0), fs(SAMPLE_RATE)
{
}

ChamberlinFilter::ChamberlinFilter(float fc, float q, float fs):
delay_1(0), delay_2(0), f1(0), fc(fc), fq(q), out_bp(0), out_hp(0), out_lp(0), out_n(0), fs(fs)
{
}

/**
 * Set the filter center frequency.
 * If set below FC_MIN or above FC_MAX, set to FC_MIN ir FC_MAX respectively
 *
 * @param new_fc New center frequency (Hz).
 * @return 0 on success
 * 	nonzero - failure to set, return the center freq
 */
int ChamberlinFilter::SetFc(float new_fc)
{
	if (new_fc < FC_MIN)
	{
		new_fc = FC_MIN;
		return fc;
	}
	else if (new_fc > FC_MAX)
	{
		// TODO: Should take sample rate into consideration as well
		// (without oversampling, fc should be < fs/6)
		new_fc = FC_MAX;
		return fc;
	}
	fc = new_fc;
	return 0;
}
