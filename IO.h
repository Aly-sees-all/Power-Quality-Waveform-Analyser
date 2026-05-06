/* io.h - file I/O function declarations */

#ifndef IO_H
#define IO_H

#include "WaveformSample.h"

WaveformSample *load_csv(const char *filename, int *count);
void write_results(PhaseResult results[3]);
void write_results_with_samples(PhaseResult results[3], WaveformSample *samples, int count);

#endif
