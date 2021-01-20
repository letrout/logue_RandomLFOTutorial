#pragma once
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

#include "fixed_math.h"

#include "randomtable.h"

#define RANDOM_TABLE_MASK 0x3FF        // The table has 1024 (0x400) values, we simply mask the increment register with this value vs "checking if value >= max then set to 0". 
#define INITIAL_LFO_FREQUENCY 6	// On effect load this is the starting LFO rate
#define MIN_LFO_FREQ 20			// This is the minimum rate the LFO for the "sample and hold/randomizer" will run
#define MAX_LFO_FREQ 20			// This is the maximum rate the LFO for the "sample and hold/randomizer" will run
#define SAMPLE_RATE 48000		// Default sample rate (samples/sec)

class RandomSampleHold
{
	private:
		float phase_accum;
		float lfo_freq;
		uint16_t random_index;
		float random_value;
		float fs;		// Sample rate (samples/sec)

	public:
		RandomSampleHold();
		RandomSampleHold(float freq, float sample_rate);
		void TickLfo();
		float GetRandomValue();
		float GetPhaseAccum() const {return phase_accum;}
		// TODO: validation on setters
		void SetLfoFreq(float freq) {lfo_freq = freq;}

};
