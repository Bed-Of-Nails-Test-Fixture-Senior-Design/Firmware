/* Program "MeasPwr.h"  

Author:       Barry L. Dorr, P.E.
Created:      June 27, 2021
Last Modified:  June 27, 2021

This file defines the structure and constants for the power measurement function.
*/

#define PWR_MEAS_DEC_RATE     96 // Decimation rate. Loop sample rate is 1002.27 Hz.
#define PWR_MEAS_DEC_PRELOAD  (PWR_MEAS_DEC_RATE - 1) 
#define PWR_MEAS_NCO_INCR     48    // Test frequency is 1033.6 Hz.
#define PWR_MEAS_T_SETT       0.100   // Settling time (4*Tau1)
#define PWR_MEAS_ZETA         1.0     // Damping coefficient.
#define TEST_FIXTURE_GAIN     (22.0/180.0 * 150.0/(150.0+91.0))  // Cascaded analog gain for TX and RX paths.


struct pwrMeasStruct
{
  // User inputs.
  float PwrSetpoint;      // Desired output power (setpoint)in Watts, (input)
  float AmpGainDbApprox;  // Approximate gain in dB (use to control loop dynamics) (input)
  float MeasTimeSec;      // Allowed measurement time for feedback loop. (input)

  // Converted user inputs.
  int   LoopSetpoint;     // Setpoint in peak quanta. Used to compute loop error.
  float AmpGainLinApprox; // Approximate Amplifier gain converted to linear.
  

  // Control system design parameters.
  float WnLoop;           // Natural frequency for target system.
  float Tau1;             // Time constant Tau1
  float Tau2;             // Time constant Tau2
  float GScale;           // Loop gain that must be applied.  

  // Integrator registers and parameters
  long  LoopInteg;        // Loop Integrator.
  long  LoopIntClamp;     // Loop Int value that causes maximum input voltage.

  // Operational Loop variables  
  int   LoopGainShift;    // Positive shifts left. 
  long  LoopError;         // Loop error.
  long  MeasPeakQuanta;   // Measured peak value of the input signal in quanta.


  // Lowpass filter
  float LpfAlpha;         // Computed value for alpha.
  long  LpfFiltReg;       // Lowpass Filter Register.
  long  LpfFiltRegSnapshot;   // Working copy for decimated operations.
  ulong  LastLpfFiltReg;   // Storage for the last value.
  ulong  LastLpfFiltRegSnapshot; // Working copy for decimated operations.
  int   LpfFiltShift;     // Positive shifts right.
  ulong  LpfOut;           // Lpf result after zero applied.

  // Highpass filter
  long  HpfFiltReg;
  long  LastHpfFiltReg;
  long  HpfOut;

  // Decimation and system control
  int   IterCount;        // Iterations (at decimated frequency) 
  int   DecCount;         // Down counter for decimation.

  // Excitation
  int   NcoPhase;         // NCO phase for TX.
  int   AmplExcQuanta;    // Scaling value for sinewave output. (Q11).
  int   AmplExcQuantaNext;  // Amplitude to be applied in next decimated iteration. 
//  float VinRms;           // Amplifier input voltage. (controlled output)
 
  // Values computed after the loop settles.
  float MeasPwrOutWatts;  // Power measurement (output)
  float MeasuredGainDb;   // Measured amplifier gain in dB. 

 
};
typedef struct pwrMeasStruct PwrMeasStruct;
