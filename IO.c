/* io.c - loading CSV and writing results */

#include "IO.h"
#include <stdio.h>
#include <stdlib.h>

WaveformSample *load_csv(const char *filename, int *count) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: could not open file %s\n", filename);
        return NULL;
    }

    /* Skip header line */
    char line[256];
    fgets(line, sizeof(line), f);

    /* Count rows */
    int rows = 0;
    while (fgets(line, sizeof(line), f) != NULL) {
        rows++;
    }

    /* Allocate memory */
    WaveformSample *samples = malloc(rows * sizeof(WaveformSample));
    if (samples == NULL) {
        printf("Error: memory allocation failed\n");
        fclose(f);
        return NULL;
    }

    /* Go back to start and skip header again */
    rewind(f);
    fgets(line, sizeof(line), f);

    /* Read data */
    int i = 0;
    while (fgets(line, sizeof(line), f) != NULL) {
        sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
               &samples[i].timestamp,
               &samples[i].phase_A_voltage,
               &samples[i].phase_B_voltage,
               &samples[i].phase_C_voltage,
               &samples[i].line_current,
               &samples[i].frequency,
               &samples[i].power_factor,
               &samples[i].thd_percent
        );
        i++;
    }

    *count = rows;

    fclose(f);
    return samples;
}

void write_results(PhaseResult results[3]) {
    FILE *f = fopen("results.txt", "w");
    if (f == NULL) {
        printf("Error: could not open results.txt for writing\n");
        return;
    }

    const char *phase_names[] = {"A", "B", "C"};
    int i;

    fprintf(f, "=== Power Quality Analysis Report ===\n\n");

    for (i = 0; i < 3; i++) {
        fprintf(f, "Phase %s:\n", phase_names[i]);
        fprintf(f, "  RMS Voltage    : %.2f V\n", results[i].rms);
        fprintf(f, "  Peak-to-Peak   : %.2f V\n", results[i].peak_to_peak);
        fprintf(f, "  DC Offset      : %.4f V\n", results[i].dc_offset);
        fprintf(f, "  Clipping events: %d\n",     results[i].clipping_count);
        fprintf(f, "  Compliant      : %s\n",     results[i].compliant ? "YES" : "NO");
        fprintf(f, "\n");
    }

    fclose(f);
}

void write_results_with_samples(PhaseResult results[3], WaveformSample *samples, int count) {
    FILE *f = fopen("results.html", "w");
    if (f == NULL) {
        printf("Error: could not open results.html for writing\n");
        return;
    }

    int i, p;
    const char *phase_names[] = {"A", "B", "C"};
    const char *colors[]      = {"#e74c3c", "#2ecc71", "#3498db"};

    fprintf(f,
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "<meta charset=\"UTF-8\">\n"
            "<title>Power Quality Analysis Report</title>\n"
            "<script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>\n"
            "<style>\n"
            "  body { font-family: Comic sans MS, Calibri ; background:#1a1a2e; color:#eee; margin:0; padding:20px; }\n"
            "  h1   { text-align:center; color:#00d4ff; margin-bottom:4px; }\n"
            "  p.sub{ text-align:center; color:#888; margin-top:0; margin-bottom:30px; }\n"
            "  .card{ background:#16213e; border-radius:10px; padding:20px; margin-bottom:24px; box-shadow:0 4px 12px rgba(0,0,0,0.4); }\n"
            "  canvas { width:100%%!important; }\n"
            "  table { width:100%%; border-collapse:collapse; margin-top:10px; }\n"
            "  th    { background:#0f3460; padding:10px; text-align:left; color:#00d4ff; }\n"
            "  td    { padding:10px; border-bottom:1px solid #0f3460; }\n"
            "  tr:hover td { background:#0f3460; }\n"
            "  .yes  { color:#2ecc71; font-weight:bold; }\n"
            "  .no   { color:#e74c3c; font-weight:bold; }\n"
            "</style>\n"
            "</head>\n"
            "<body>\n"
            "<h1>Power Quality Analysis Report</h1>\n"
            "<p class=\"sub\">Three-phase waveform analysis</p>\n"
    );

    fprintf(f,
            "<div class=\"card\">\n"
            "<h2 style=\"color:#00d4ff\">Phase Voltages</h2>\n"
            "<canvas id=\"waveChart\"></canvas>\n"
            "</div>\n"
    );

    fprintf(f,
            "<div class=\"card\">\n"
            "<h2 style=\"color:#00d4ff\">Analysis Results</h2>\n"
            "<table>\n"
            "<tr><th>Metric</th><th>Phase A</th><th>Phase B</th><th>Phase C</th></tr>\n"
            "<tr><td>RMS Voltage</td>"
    );
    for (i = 0; i < 3; i++)
        fprintf(f, "<td>%.2f V</td>", results[i].rms);
    fprintf(f, "</tr>\n<tr><td>Peak-to-Peak</td>");
    for (i = 0; i < 3; i++)
        fprintf(f, "<td>%.2f V</td>", results[i].peak_to_peak);
    fprintf(f, "</tr>\n<tr><td>DC Offset</td>");
    for (i = 0; i < 3; i++)
        fprintf(f, "<td>%.4f V</td>", results[i].dc_offset);
    fprintf(f, "</tr>\n<tr><td>Clipping Events</td>");
    for (i = 0; i < 3; i++)
        fprintf(f, "<td>%d</td>", results[i].clipping_count);
    fprintf(f, "</tr>\n<tr><td>Compliant (230V +/-10%%)</td>");
    for (i = 0; i < 3; i++)
        fprintf(f, "<td class=\"%s\">%s</td>",
                results[i].compliant ? "yes" : "no",
                results[i].compliant ? "YES" : "NO");
    fprintf(f, "</tr>\n</table>\n</div>\n");

    /* Subsample to 200 points for performance */
    int step = count > 200 ? count / 200 : 1;

    fprintf(f, "<script>\nconst labels = [");
    for (i = 0; i < count; i += step)
        fprintf(f, "\"%.4f\"%s", samples[i].timestamp, (i + step < count) ? "," : "");
    fprintf(f, "];\nconst datasets = [\n");

    for (p = 0; p < 3; p++) {
        double *v;
        fprintf(f, "{ label:'Phase %s', borderColor:'%s', backgroundColor:'transparent', borderWidth:1.5, pointRadius:0, data:[",
                phase_names[p], colors[p]);
        for (i = 0; i < count; i += step) {
            if (p == 0)      v = &samples[i].phase_A_voltage;
            else if (p == 1) v = &samples[i].phase_B_voltage;
            else             v = &samples[i].phase_C_voltage;
            fprintf(f, "%.4f%s", *v, (i + step < count) ? "," : "");
        }
        fprintf(f, "] }%s\n", p < 2 ? "," : "");
    }

    fprintf(f,
            "];\n"
            "new Chart(document.getElementById('waveChart'), {\n"
            "  type: 'line',\n"
            "  data: { labels, datasets },\n"
            "  options: {\n"
            "    animation: false,\n"
            "    responsive: true,\n"
            "    plugins: { legend: { labels: { color:'#eee' } } },\n"
            "    scales: {\n"
            "      x: { ticks:{ color:'#aaa', maxTicksLimit:10 }, grid:{ color:'#333' }, title:{ display:true, text:'Time (s)', color:'#aaa' } },\n"
            "      y: { ticks:{ color:'#aaa' }, grid:{ color:'#333' }, title:{ display:true, text:'Voltage (V)', color:'#aaa' } }\n"
            "    }\n"
            "  }\n"
            "});\n"
            "</script>\n"
            "</body>\n</html>\n"
    );

    fclose(f);
}