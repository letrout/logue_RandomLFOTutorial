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

#include "random_sample_hold.hpp"

RandomSampleHold::RandomSampleHold():
phase_accum(0), lfo_freq(LFO_FREQ_INIT), random_index(0), fs(SAMPLE_RATE)
{
}

RandomSampleHold::RandomSampleHold(float freq, float sample_rate)
{
	phase_accum = 0.;
	lfo_freq = freq;
	random_index = 0;
	fs = sample_rate;
}

/**
 * Get random value.
 * If the LFO has completed a cycle, get a new random value from the LUT.
 * If the LFO has not completed a cycle, return the current random_value.
 *
 * @return the random value
 */
float RandomSampleHold::GetRandomValue()
{
	if (phase_accum >= 1)
	{
		// Restrict the lfo phase accumulator to 0-1
		phase_accum -= (uint32_t) phase_accum;
		
		// Get the value from the table. *Note, the random table provided by random.org is a value from 0-1. Again, to save time, I've converted this table
		// to -1 to +1 ((val*2)-1) to save potentially wasted CPU time.
		
		random_value = randTable[random_index]; // Get a 'random' value from -1 to 1
		// Increment and roll over the random table index
		random_index++;
		
		random_index &= RANDOM_TABLE_MASK;   // This is the same as "if value >= # of entries then value = 0" but by using a table size that can be bitmasked
                                             // (e.g.1,2,4,8,16,32,64,128 etc, ours is 1024), we merely mask off the exceeding bits to again save time. We don't
                                             // *need* a table with say 1000 or 950 entries, a table of 1024 is perfectly fine.
	}
	return random_value;
}

/**
 * Set the LFO frequency.
 * If set below MIN_LFO_FREQ or above MAX_LFO_FREQ, set to MIN_LFO_FREQ or MAX_LFO_FREQ respectively
 *
 * @param freq New LFO frequency (Hz).
 * @return 0 on success
 * 	nonzero - failure to set, return the LFO freq
 */
int RandomSampleHold::SetLfoFreq(float freq)
{
	if (freq < LFO_FREQ_MIN)
	{
		lfo_freq = LFO_FREQ_MIN;
		return lfo_freq;
	}
	else if (freq > LFO_FREQ_MAX)
	{
		lfo_freq = LFO_FREQ_MAX;
		return lfo_freq;
	}
	lfo_freq = freq;
	return 0;
}