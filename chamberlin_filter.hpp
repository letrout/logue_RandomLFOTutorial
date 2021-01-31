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

#define INITIAL_FREQUENCY 800
#define FC_MIN 20		// Minimum allowed center frequency (Hz)
#define FC_MAX 22000	// Maximum center frequency (Hz)
#define FIXED_Q 0.17	// Do not reach zero nor do not exceed 2. Lower values = more resonance.
#define Q_MIN 0.001		// Minimum allowed Q
#define Q_MAX 2.0		// Maximum allowed Q
#define SAMPLE_RATE 48000	// Default sample rate (samples/sec)
#define TWOPI 6.283185307 

class ChamberlinFilter
{
	private:
		float delay_1 = 0;	// Used internally by the filter (1-sample delay for bandpass filter)
		float delay_2 = 0;	// Used internally by the filter (1-sample delay for low pass filter)
		float f1 = 0;		// Used internally by the filter (frequency)
		float fc = INITIAL_FREQUENCY;		// Filter center frequency (Hz), although not quite expressed as 1:1 for frequency
		float fq = FIXED_Q;			// Filter resonance. This is defined as 1/Q, so a value of 2 = no resonance, and lower values = more resonance. A value of 0 may silence the filter however.
		float out_hp = 0;	// High pass filter output value
		float out_bp = 0;	// Band pass filter output value
		float out_lp = 0;	// Low pass filter output value
		float out_n = 0;	// Notch filter output value
		float fs;			// Sample rate (samples/sec)

	public:
		ChamberlinFilter();
		ChamberlinFilter(float filterc, float filterq, float sample_rate);
		float GetHP() const {return out_hp;}
		float GetBP() const {return out_bp;}
		float GetLP() const {return out_lp;}
		float GetN() const {return out_n;}
		int SetFc(float new_fc);
		int SetQ(float new_q);
		// TODO: some profiling to see if inlining this does any good
		// TODO: oversampling option
		inline __attribute__((optimize("Ofast"),always_inline))
		void process(float x)
		{
			f1 = TWOPI * fc / fs;				// Calculate the F1 value(2pi * filter frequency / samplerate)
			out_lp = delay_2 + f1 * delay_1;	// Calculate the low pass portion
			out_hp = x - out_lp - fq * delay_1;	// Calculate the high pass portion
			out_bp = f1 * out_hp + delay_1;		// Calculate the bandpass portion
			out_n = out_hp + out_lp;			// Calculate the notch portion
			delay_1 = out_bp;					// Store the bandpass result into the bandpass delay value
			delay_2 = out_lp;					// Store the low pass result into the low pass delay value
		}

};
