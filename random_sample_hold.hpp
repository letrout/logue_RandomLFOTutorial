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
#define LFO_FREQ_INIT 6	// On effect load this is the starting LFO rate
#define LFO_FREQ_MIN 0			// This is the minimum rate the LFO for the "sample and hold/randomizer" will run
#define LFO_FREQ_MAX 20			// This is the maximum rate the LFO for the "sample and hold/randomizer" will run
#define SAMPLE_RATE 48000		// Default sample rate (samples/sec)

class RandomSampleHold
{
	private:
		float phase_accum = 0;
		float lfo_freq = LFO_FREQ_INIT;
		uint16_t random_index = 0;
		float random_value = 0;
		float fs;		// Sample rate (samples/sec)

	public:
		RandomSampleHold();
		RandomSampleHold(float freq, float sample_rate);
		float GetRandomValue();
		float GetPhaseAccum() const {return phase_accum;}
		int SetLfoFreq(float freq);
		/**
		* Increment the LFO accumulator, call every sample
		*/
		inline __attribute__((optimize("Ofast"),always_inline))
		void TickLfo() {phase_accum += (lfo_freq / fs);}
};
