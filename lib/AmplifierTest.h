/* Program "AmplifierTest.h"  

Author:       Barry L. Dorr, P.E.
Created:      February May 20, 2021
Last Modified:  May 31, 2021

This file contains calibration data for the Amplifier Test Fixture. See the comments
in CalPcb.ino for a description of the data.
*/

#define COSSINPTS   2048  // Length of CosSin table.
#define PWR_CTL_RELAY 22  // D22 is used for power Control relay.

#define MAX_THD_FREQ  8000.0    // THD is computed on harmoncs with frequency less than MAX_THD_FREQ.
#define SINAD_FILTER_GAIN_CORR  (0.91368*0.1194/0.1 * 3.47/3.79)    // Corrects for IIR filter gains at 990.532 Hz.

struct sinadPwrResStruct
{
  float OutputPwrWatts;
  float NotchedOutputPwrWatts;
  float SinadDb;
  float ThdPct;
  float GainDb;
};
typedef struct sinadPwrResStruct SinadPwrResStruct;

struct freqRespResStruct
{
  float ExcFreqDes;
  float ExcFreqAct;
  float ExcVoltsRms;
  int   IncrExc;
  int   FilterShift;
  long  MeasSamp;
  int   PhaseExc;
  float DistPct;
  float GainDb;
  float PwrOutW;

  long  FilterRegIHarm[10];  // Registers for I and Q LPF's.
  long  FilterRegQHarm[10];
  long  LastRegIHarm[10];    // Last Register I
  long  LastRegQHarm[10];    // Last Register Q
  int   IncrHarm[10];        // NCO Increment for this harmonic.
  float CalPcbCorrHarm[10];  // Receiver gain correction.
  float VoltageHarm[10];     // Voltage measurement for this harmonic.
  int   IncludeHarm[10];     // Set to 1 to include this harmonic for distortion measurement.
  int   PhaseHarm[10];       // NCO Phase

  uint32_t  CosSinTableExc[COSSINPTS];  // This table is scaled to generate the correct excitation voltage.
  uint32_t  CosSinTableCap[COSSINPTS];  // This table is used for local oscillator and has unity peak amplitude..
};
typedef struct freqRespResStruct FreqRespResStruct;

///* Filter Coefficients for the SINAD notch filter, C.F. = 990.5273 Hz, BW = 30 Hz 16-bit coeffs */
#define B11A_NF  -32443     // Section 1 B11A
#define B11B_NF  -32442     // Section 1 B11B
#define A11A_NF  -32386     // Section 1 A11A    
#define A11B_NF  -32386     // Section 1 A11A   
#define A21_NF   32668      // Section 1 A21    
#define B12A_NF  -32443     // Section 2 B12A
#define B12B_NF  -32442     // Section 2 B12B
#define A12A_NF  -32401     // Section 2 A12A    
#define A12B_NF  -32401     // Section 2 A12A 
#define A22_NF   32670      // Section 2 A22    

/* Filter Coefficients for the SINAD highpass filter, Fc = 40 Hz */
#define A11A_HPF  -32596    // Section 1 A11A    
#define A11B_HPF  -32596    // Section 1 A11A   
#define A21_HPF    32425    // Section 1 A21    
#define A12A_HPF  -32696    // Section 2 A12A    
#define A12B_HPF  -32696    // Section 2 A12A 
#define A22_HPF    32625    // Section 2 A22    

/* Filter Coefficients for the SINAD lowpass filter, Fc = 8 kHz */
#define LPF_SCALE  1120   // Output scaling for unity DC gain
#define A11A_LPF  -7443   // Section 1 A11A    
#define A11B_LPF  -7442   // Section 1 A11A   
#define A21_LPF    2861   // Section 1 A21    
#define A12A_LPF  -10158  // Section 2 A12A    
#define A12B_LPF  -10158  // Section 2 A12A 
#define A22_LPF    15860  // Section 2 A22
;
