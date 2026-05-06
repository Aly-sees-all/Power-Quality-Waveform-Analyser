//
// Created by Am2-munir on 22/04/2026.
//
/* waveform.h - struct definition and function declarations */

#ifndef WAVEFORM_H
#define WAVEFORM_H

typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample;

/* Analysis results for a single phase */
typedef struct {
    double rms;
    double peak_to_peak;
    double dc_offset;
    int    clipping_count;
    int    compliant;
} PhaseResult;

/* Function declarations */
PhaseResult analyse_phase(WaveformSample *samples, int count, int phase);
void check_compliance(PhaseResult *result);

#endif