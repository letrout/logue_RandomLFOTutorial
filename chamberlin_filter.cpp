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

// TODO: inline decorator
void ChamberlinFilter::ProcessSample(float x)
{
	f1 = TWOPI * fc / fs;				// Calculate the F1 value(2pi * filter frequency / samplerate)
    out_lp = delay_2 + f1 * delay_1;	// Calculate the low pass portion
    out_hp = x - out_lp - fq * delay_1;	// Calculate the high pass portion
    out_bp = f1 * out_hp + delay_1;		// Calculate the bandpass portion
    out_n = out_hp + out_lp;			// Calculate the notch portion
    delay_1 = out_bp;					// Store the bandpass result into the bandpass delay value 
    delay_2 = out_lp;					// Store the low pass result into the low pass delay value
}