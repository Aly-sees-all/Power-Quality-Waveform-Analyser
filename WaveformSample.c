/* waveform.c - analysis functions */

#include "WaveformSample.h"
#include <math.h>

static double get_voltage(WaveformSample *s, int phase) {
    if (phase == 0) return s->phase_A_voltage;
    if (phase == 1) return s->phase_B_voltage;
    return s->phase_C_voltage;
}

PhaseResult analyse_phase(WaveformSample *samples, int count, int phase) {
    PhaseResult result;
    double sum = 0.0;
    double sum_sq = 0.0;
    double min_v = get_voltage(&samples[0], phase);
    double max_v = get_voltage(&samples[0], phase);
    int i;
    result.clipping_count = 0;

    for (i = 0; i < count; i++) {
        double v = get_voltage(&samples[i], phase);

        sum += v;
        sum_sq += v * v;

        if (v < min_v) min_v = v;
        if (v > max_v) max_v = v;

        if (v >= 324.9 || v <= -324.9) {
            result.clipping_count++;
        }
    }

    result.dc_offset    = sum / count;
    result.peak_to_peak = max_v - min_v;
    result.rms          = sqrt(sum_sq / count);

    check_compliance(&result);
    return result;
}

void check_compliance(PhaseResult *result) {
    double lower = 230.0 * 0.90;
    double upper = 230.0 * 1.10;
    result->compliant = (result->rms >= lower && result->rms <= upper);
}
