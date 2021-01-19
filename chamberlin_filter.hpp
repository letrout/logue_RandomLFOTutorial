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
		void ProcessSample(float x);
		float GetHP() {return out_hp;}
		float GetBP() {return out_bp;}
		float GetLP() {return out_lp;}
		float GetN() {return out_n;}
		void SetFc(float fc) {fc = fc;}
		void SetQ(float q) {q = q;}

};
