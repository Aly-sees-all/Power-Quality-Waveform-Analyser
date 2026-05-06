# PQWA
# Power Quality Waveform Analyser (PQWA)

Analyses three-phase power quality data from a CSV file and produces a text report (`results.txt`) and an interactive HTML waveform report (`results.html`).

---

## Files

| File | Purpose |
|---|---|
| `main.c` | Entry point — loads data, runs analysis, writes output |
| `waveform.c` / `waveform.h` | Core analysis: `analyse_phase`, `check_compliance` |
| `IO.c` / `IO.h` | File I/O: `load_csv`, `write_results`, `write_results_with_samples` |
| `power_quality_log.csv` | Sample input data (1001 rows, three-phase 230 V) |

---

## How to Compile and Run

### Option 1 — GCC (command line)

**Compile:**

```bash
gcc -o analyser main.c waveform.c IO.c -lm
```

> The `-lm` flag links the maths library (required for `sqrt` in `waveform.c`).

**Run:**

```bash
./analyser power_quality_log.csv
```

**Expected output:**

```
Analysis complete. Results written to results.txt and results.html
```

`results.txt` and `results.html` will be created in the current directory.

---

### Option 2 — CLion (IDE)

1. Open CLion and select **File → Open**, then choose the project folder.
2. CLion will detect `CMakeLists.txt` and configure the project automatically.
3. To set the input file argument:
   - Go to **Run → Edit Configurations...**
   - In the **Program arguments** field, enter: `power_quality_log.csv`
   - Click **OK**.
4. Click the green **Run** button (or press `Shift+F10`).
5. Output will appear in the Run console. `results.txt` and `results.html` will be created in the `cmake-build-debug/` directory.

---

## Usage

```
./analyser <filename.csv>
```

| Argument | Description |
|---|---|
| `<filename.csv>` | Path to a CSV file with the expected 8-column format |

If no argument is provided, the program prints:

```
Usage: analyser <filename.csv>
```

---

## CSV Format

The input CSV must have a header row followed by data rows in this column order:

```
timestamp, phase_A_voltage, phase_B_voltage, phase_C_voltage,
line_current, frequency, power_factor, thd_percent
```

---

## Output

| File | Contents |
|---|---|
| `results.txt` | Plain text report — RMS, peak-to-peak, DC offset, clipping count, compliance per phase |
| `results.html` | Interactive Chart.js graph of all three phase waveforms plus a results table. Open in any browser. |

---

## Compliance Check

RMS voltage is checked against the UK mains spec: **230 V ±10% (207 V – 253 V)**. Result is reported as `YES` or `NO`.

