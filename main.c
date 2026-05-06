#include <stdio.h>
#include <stdlib.h>
#include "IO.h"
#include "WaveformSample.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: analyser <filename.csv>\n");
        return 1;
    }

    int count = 0;
    WaveformSample *samples = load_csv(argv[1], &count);

    if (samples == NULL) {
        return 1;
    }

    PhaseResult results[3];
    results[0] = analyse_phase(samples, count, 0);
    results[1] = analyse_phase(samples, count, 1);
    results[2] = analyse_phase(samples, count, 2);

    write_results(results);
    write_results_with_samples(results, samples, count);
    printf("Analysis complete. Results written to results.txt and results.html\n");

    free(samples);
    return 0;
}
