
    float LpfAlpha;         // Computed value for alpha.
    long  LpfFiltReg;       // Lowpass Filter Register.
    long  LpfFiltRegSnapshot;   // Working copy for decimated operations.
    unsigned long  LastLpfFiltReg;   // Storage for the last value.
    unsigned long  LastLpfFiltRegSnapshot; // Working copy for decimated operations.
    int   LpfFiltShift;     // Positive shifts right.
    unsigned long  LpfOut;           // Lpf result after zero applied.

    float WnLoop;           // Natural frequency for target system.
    float Tau1;             // Time constant Tau1

    // Highpass filter
    long  HpfFiltReg;
    long  LastHpfFiltReg;
    long  HpfOut;

    #define PWR_MEAS_T_SETT       0.100   // Settling time (4*Tau1)
    #define PWR_MEAS_ZETA         1.0     // Damping coefficient.
    #define FS 44100


    WnLoop = 4.0 / (PWR_MEAS_ZETA * PWR_MEAS_T_SETT);           // Eq. 5.21
    Tau1 = 1.0 / (2.0 * PWR_MEAS_ZETA * WnLoop);        // Eq 5.27
    
    LpfAlpha = 1.0 - 1.0 / (Tau1 * FS); // Eq. 8.45
    LpfFiltShift = (int) (-log(1.0 - LpfAlpha) / log(2.0) + 0.5);
    
    int measAC(int AdcResult) {
        
        LpfFiltReg = 0L;
        LoopInteg = 0L;
        AmplExcQuanta = 0L;
        
        AdcResult = (AdcResult - 2047); //im not sure if we need this, but supposedly this is to "remove nominal DC from the ADCresult"

        // Highpass filter the input with IIR filter. This filter mimics an RC (single pole) lowpass filter, but has a zero at FS/2.
        // Highpass is realized by Highpass = 1 - Lowpass. Highpass cutoff is 100 Hz.
        LastHpfFiltReg = HpfFiltReg;      // Store the last register value.
        HpfFiltReg = HpfFiltReg - ((HpfFiltReg) >> 6) + AdcResult;
        // Realize zero at Fs/2, set for unity passband gain, and implement lowpass to highpass.
        HpfOut = AdcResult - (int) (((HpfFiltReg) + LastHpfFiltReg) >> (6+1));

        // Lowpass filter the square of the input. This filter mimics an RC (single pole) lowpass filter, but has a zero at FS/2.
        // Note that the register is a ulong. this gives an extra bit of dynamic range since the input will always be positive.
        LastLpfFiltReg = LpfFiltReg;      // Store the last register value.
        LpfFiltReg = LpfFiltReg - ((LpfFiltReg) >> (LpfFiltShift)) + (long)HpfOut * HpfOut;

        LpfFiltRegSnapshot = LpfFiltReg;         // Working copy <--- again, im not sure if we need these, i think dorr had this and the next line 
                                                 // in his code because his function was decimating the sampling. these 2 lines lets him save his accumulated values, 
                                                 // the way i think we can implement it may be different but we should definitely discuss the implementation
        LastLpfFiltRegSnapshot = LastLpfFiltReg;

        // Implement the zero in the LPF. <--- unsure what this means exactly tbh, might need some clarification
        LpfOut = (LpfFiltRegSnapshot + LastLpfFiltRegSnapshot) >> (LpfFiltShift + 1);

        return LpfOut; // the way i have it implemented as of right now following our data-flow diagram for measureAC: ADCOutputs -> Highpass Filt -> x^2 -> Lowpass Filt -> return LPF output

    }


    /* my idea of implementation is somewhat like this:


        static unsigned long accumulatedOut = 0;
        int i = 0;

        while(i < numSamples) { // do this for the number of samples we need for an accurate result

        while(!interruptFlag); <- do nothing until the next sample
        interruptFlag = 0; <- reset the flag

        accumulatedOut = measAC(AdcResult); <- this will store the values from our data-flow mentioned above, but keep in mind we still have to accumulate enough samples and then also do the square-root function

        }

        //now that we have accumulated all our samples, and each sample has been LPFed, HPFed, squared, and summed, we can take the square root

        rms = sqrt(accumulatedOut);

        // i should mention that dorr has this in his code:

        // Compute the square root of twice the filter output to get the peak value.
        MeasPeakQuanta = (long)(sqrt(2.0 * ResPtr->LpfOut)+0.5); 


        cannot explain why its twice the filter output, something to ask him


    */