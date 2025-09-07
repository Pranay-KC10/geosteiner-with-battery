#!/bin/bash
set -e

# === 1. Make 4-point input ===
cat > points.txt <<EOF
0.0 0.0
1.0 0.0
0.0 1.0
1.0 1.0
EOF

# Battery file: weight=1.0, levels per terminal
cat > batteries.txt <<EOF
1.0
90
45
85
70
EOF

echo "[info] Input files created: points.txt, batteries.txt"

# === 2. Run EFST normally (no penalty) ===
./efst < points.txt > normal.fst
echo "[info] Generated normal.fst"

./dumpfst -l < normal.fst | head -10

# === 3. Run EFST with battery penalty ===
./efst -b batteries.txt < points.txt > battery.fst
echo "[info] Generated battery.fst"

./dumpfst -l < battery.fst | head -10

# === 4. Plot both versions ===
(cat prelude.ps; ./bb < normal.fst)  | ps2pdf - normal.pdf
(cat prelude.ps; ./bb < battery.fst) | ps2pdf - battery.pdf
echo "[info] Plots saved: normal.pdf, battery.pdf"

# === 5. (Optional) Prune top-2 FSTs (only makes sense if prunefst is patched) ===
if [ -x ./prunefst ]; then
    ./prunefst -k 2 < battery.fst > battery_top2.fst
    ./dumpfst -l < battery_top2.fst | head -10
    (cat prelude.ps; ./bb < battery_top2.fst) | ps2pdf - battery_top2.pdf
    echo "[info] Top-2 pruned plot saved: battery_top2.pdf"
else
    echo "[warn] prunefst not found or not executable — skipping pruning step."
fi
