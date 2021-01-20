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
#define FIXED_Q 0.17	// Do not reach zero nor do not exceed 2. Lower values = more resonance.
#define SAMPLE_RATE 48000
#define TWOPI 6.283185307 

class ChamberlinFilter
{
	private:
		float delay_1;	// Used internally by the filter (1-sample delay for bandpass filter)
		float delay_2;	// Used internally by the filter (1-sample delay for low pass filter)
		float f1;		// Used internally by the filter (frequency)
		float fc;		// Filter cutoff frequency (Hz), although not quite expressed as 1:1 for frequency
		float fq;		// Filter resonance. This is defined as 1/Q, so a value of 2 = no resonance, and lower values = more resonance. A value of 0 may silence the filter however.
		float out_hp;	// High pass filter output value
		float out_bp;	// Band pass filter output value
		float out_lp;	// Low pass filter output value
		float out_n;	// Notch filter output value
		float fs;		// Sample rate (samples/sec)

	public:
		ChamberlinFilter();
		ChamberlinFilter(float fc, float q);
		ChamberlinFilter(float fc, float q, float fs);
		void process(float x);
		float GetHP() const {return out_hp;}
		float GetBP() const {return out_bp;}
		float GetLP() const {return out_lp;}
		float GetN() const {return out_n;}
		// TODO: make real setters with validation checks
		void SetFc(float new_fc) {fc = new_fc;}
		void SetQ(float new_q) {fq = new_q;}

};
