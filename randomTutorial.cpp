////////////////////////////////////////////////////////////////////////
//
// "random" lfo demo tutorial
//
// (c) 2019 hammondeggsmusic
//
// A simple 'random' LFO modfx (using a fixed table set) demo 
// in straightforward C for simplicity and efficiency. 
//
// this is provided for informational purposes only and is to be
// used at your own risk.
//
////////////////////////////////////////////////////////////////////////
//Includes
#include "usermodfx.h"
#include "randomtable.h"

#include "chamberlin_filter.hpp"

// Defines
#define FIXED_Q 0.17                   // Do not reach zero nor do not exceed 2. Lower values = more resonance.
#define TWOPI 6.283185307              // 2*pi constant used by the filter   

#define RANDOM_TABLE_MASK 0x3FF        // The table has 1024 (0x400) values, we simply mask the increment register with this value vs "checking if value >= max then set to 0". 
#define SAMPLE_RATE 48000
#define CENTRE_FREQUENCY 1200          // This is the centre frequency of the filter. When the depth is set to 0, the filter will be at this frequency.
                                       // * When version 2.00 of the minilogue xd is released, you could use shift+depth to set this...
#define INITIAL_FREQUENCY 800          // On effect load this is the frequency to assign to the filter initially
#define INITIAL_FREQUENCY_DEVIATION 800 // On effect load this is the 'depth' amout to assign initially
#define INITIAL_LFO_FREQUENCY 6        // On effect load this is the starting LFO rate
                              
#define MAX_FREQUENCY_DEVIATION 1100   // This should not exceed the centre frequency! This sets how much the frequency will deviate when the depth knob is at 'full'.
#define MAX_LFO_RATE 20                // This is the maximum rate the LFO for the "sample and hold/randomizer" will run

float phaseAccumLFO1 = 0;              // LFO phase accumulator - this will count from 0 to (just under)1 , cycling at the LFO rate.
float LFORate1 = INITIAL_LFO_FREQUENCY;// LFO frequency 
float valTime = 0;                     // Float value of the "Time" knob from 0 to 1
float valDepth = 0;                    // Float value of the "Depth" knob from 0 to 1
float effectDepth = 0;                 // Pre-calculated effect deviation (calculated at the knob turn event)
uint16_t randomIndex = 0;              // Current index into the random table. 
float currentFrequency = INITIAL_FREQUENCY;


////////////////////////////////////////////////////////////////////////
// Effect Load event
////////////////////////////////////////////////////////////////////////
void MODFX_INIT(uint32_t platform, uint32_t api)
{
   // This occurs once when the mod fx is selected. This is not executed when the effect is enabled / disabled
   
   // Reset the phase accumulator
   phaseAccumLFO1 = 0;      
   
   // Set a default filter LFO rate
   LFORate1 = INITIAL_LFO_FREQUENCY;
   
   // Initialize the random table index
   randomIndex = 0; //index in the random table
   
   // Initialize the filter
   filter = ChamberlinFilter{INITIAL_FREQUENCY, FIXED_Q};
   effectDepth = INITIAL_FREQUENCY_DEVIATION;   //Initialize the initial effect depth
   
}



////////////////////////////////////////////////////////////////////////
// Effect process event
////////////////////////////////////////////////////////////////////////
void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames)
{
   // This is called each time the synthesizer feeds your effect some data
   // Variables passed to the effect:
   
   // *main_xn : Pointer to floating point INPUT data for the "main" channel (used by the minilogue xd and by the prologue in non-split/layer mode)
   // *sub_xn   : Pointer to floating point INPUT data for the "sub" channel (currently used by the prologue in split/layer mode for the other channel)
   // *main_yn : Pointer to floating point OUTPUT data for the "main" channel
   // *sub_yn  : Pointer to floating point OUTPUT data for the "sub" channel.   
   // frames : Number of samples to process
   
   // Note, since the effects are stereo, there are two channels in each buffer. They appear to be stored as LEFT/RIGHT thus the end pointer is calculated as + 2*frames
   
   // It's important to note, that the "sub" channel, should effectively be treated as a separate effect. That is, any variables etc should be unique 
   // to run this effect. This doubles the CPU time spent, and is not necessary on the minilogue xd (and likely the nts1) as it currently does not have a 'sub' channel. 
   // That said, the prologue currently cannot run both delay and reverb at the same time (unlike the minilogue), so you can likely safely "double up" for the prologue and
   // restrict to a single channel for the minilogue.
   
   // As defined by the supplied demo code, the pointers are copied here:
   const float * mx = main_xn;      
   float * __restrict my = main_yn;
   const float * my_e = my + 2*frames;

   const float *sx = sub_xn;      
   float * __restrict sy = sub_yn;

   float sigOut;                 // Signal output value
   float sigIn;                  // Signal input value

   float randomValueF;           // Floating point random value to get from table
   
   // Loop through the samples
   for (; my != my_e; ) 
   {
      // Pass sub through for prologue for now (L,R), you will need a separate filter at least if you want to process this properly
      *(sy++) = *(sx++);    // Copy *sy to *sx (Left channel)
      *(sy++) = *(sx++);    // Copy *sy to *sx again (right channel)
      
      // Run the LFO oscillator
      // *Usually for an oscillator, I will run the LFO 'outside' of the DSP loop, as the oscillators currently run at 16 samples per loop, resulting
      //  in 48000/16 = 3000hz update rate. The effects however currently use 64 samples, which equals a 750hz update rate. Especially as this effect
      //  doesn't do a whole lot, we will keep the LFO in the DSP loop
      
      // *As always, the LFO itself could even be in a separate class etc however for this demo it is of courser kept simple. It is also entirely possible 
      // that doing so also may result in using less CPU time, which is imperative as this is realtime DSP code!
      
      phaseAccumLFO1 += (LFORate1 / 48000); //48000 is the sample rate      
      
      // Since we want a new random sample each time it loops, we'll check to see if it's overflowed here:
      if (phaseAccumLFO1 >= 1)
      {
         // Restrict the lfo phase accumulator to 0-1
         phaseAccumLFO1 -= (uint32_t) phaseAccumLFO1;
         
         // Calculate the new filter frequency:
         
         // Get the value from the table. *Note, the random table provided by random.org is a value from 0-1. Again, to save time, I've converted this table
         // to -1 to +1 ((val*2)-1) to save potentially wasted CPU time.
         
         randomValueF = randTable[randomIndex]; // Get a 'random' value from -1 to 1
         
         // Set the filter frequency here
         F = CENTRE_FREQUENCY + (randomValueF*effectDepth); 
		 filter.SetFc(F);
         
         // Increment and roll over the random table index
         randomIndex++;
         
         randomIndex &= RANDOM_TABLE_MASK;   // This is the same as "if value >= # of entries then value = 0" but by using a table size that can be bitmasked
                                             // (e.g.1,2,4,8,16,32,64,128 etc, ours is 1024), we merely mask off the exceeding bits to again save time. We don't
                                             // *need* a table with say 1000 or 950 entries, a table of 1024 is perfectly fine.
      }
      
      // Get the input signal from the left channel (note effects are processed chorus->delay->reverb, and the synthesizer is currently mono up until the effects,
      //  so we'll just use the left channel input for now)
      
      sigIn = (*mx++);   
      mx++; // Advance the input pointer once more to skip the right channel input - we only use the left channel as an input.      
      
      //Run the chamberlin filter: 
	  filter.process(sigIn);
     
      //*you can actually optimize this filter by removing the delays (D1 and D2) and simply using the 'last' bandpass and lowpass value, but 
      // for simplicities sake this is again kept as straightforward as possible.
      
      // We'll use the L - lowpass output. Change this to H for highpass, N for notch, and B for bandpass if you wish..
      sigOut = filter.GetLP();     
      
      // Send this out to the left channel      
      *(my++) = sigOut;
      // Send the same out to the right channel too, this is a mono effect.
      *(my++) = sigOut;      
   } 
}

////////////////////////////////////////////////////////////////////////
// Parameter change event
////////////////////////////////////////////////////////////////////////
void MODFX_PARAM(uint8_t index, int32_t value)
{
   // This occurs any time a parameter (in the case of effects, currently the "time" and "depth" knob (on minilogue))
   const float valf = q31_to_f32(value);   //Get the incoming value as a float from 0-1
  
   switch (index) 
    {
      case 0:
      // Time knob - this will set the LFO rate(s)
      
      // valf, is the knob value as a float, from 0 to 1.
      valTime = valf; //Store this if we wish to use it later
      
        // Set the LFO rate here, no need to calculate it every time the sample loop runs!
      LFORate1 = MAX_LFO_RATE * valf; 
      return;
   
   case 1:
      // Depth knob.
      valDepth = valf;  //Store this if we wish to use it later.
      
      // Pre-calculate the deviation here, no need to do costly multiplications in the dsp loop for values that only change when the knob is turned!
      effectDepth = valf * MAX_FREQUENCY_DEVIATION;       
      return;
   
   default:
      break;
  }
}

