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
