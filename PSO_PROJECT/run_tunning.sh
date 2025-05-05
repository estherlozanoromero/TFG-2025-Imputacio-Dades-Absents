#!/bin/bash

# ========== CONFIG ==========

EXEC="./pso_imputation"
MAKE_CMD="make"
DATASETS_DIR="../datasets"
OUT_DIR="../dataset_out"
CORR_DIR="../output_correlations"
RESULTS_DIR="../results"
SUMMARY_FILE="$RESULTS_DIR/summary.csv"

# Tuning params
PARTICLES_LIST=(20 40 60)
ITERATIONS_LIST=(30 50 80)
C1_LIST=(1.2 1.5)
C2_LIST=(1.2 1.5)
W_LIST=(0.5 0.7)
RATIO_LIST=(0.3 0.5)
INIT_TYPES=(1 2)

# ========== SETUP ==========

mkdir -p "$OUT_DIR" "$CORR_DIR" "$RESULTS_DIR"

echo "[INFO] Compiling project..."
$MAKE_CMD || { echo "[ERROR] Compilation failed"; exit 1; }

echo "[INFO] Writing summary to $SUMMARY_FILE"
echo "dataset,particles,iterations,c1,c2,w,ratio,init_type,time_sec,fitness" > "$SUMMARY_FILE"

# ========== FUNCTION: SELECT REPRESENTATIVE DATASETS ==========

select_datasets() {
    ls "$DATASETS_DIR"/*.csv | grep "_input_\\.csv$" | grep -E "rows_(500|1000|5000|10000)_columns_(20|100|500)_comp_0\\.[2-5]_" | sort
}

# ========== MAIN LOOP ==========

for dataset in $(select_datasets); do
    base_name=$(basename "$dataset" .csv)

    for p in "${PARTICLES_LIST[@]}"; do
        for m in "${ITERATIONS_LIST[@]}"; do
            for c1 in "${C1_LIST[@]}"; do
                for c2 in "${C2_LIST[@]}"; do
                    for w in "${W_LIST[@]}"; do
                        for r in "${RATIO_LIST[@]}"; do
                            for i in "${INIT_TYPES[@]}"; do

                                log_name="${base_name}_p${p}_m${m}_c1${c1}_c2${c2}_w${w}_r${r}_i${i}"
                                log_out="$RESULTS_DIR/${log_name}.log"
                                echo "[RUN] $log_name"

                                start=$(date +%s.%N)
                                output=$($EXEC -d "$dataset" -p $p -m $m -c1 $c1 -c2 $c2 -w $w -r $r -i $i 2>&1)
                                end=$(date +%s.%N)

                                runtime=$(echo "$end - $start" | bc -l)

                                fitness=$(echo "$output" | grep "Global Best Fitness" | tail -n 1 | awk '{print $NF}')
                                fitness=${fitness:-nan}

                                echo "$output" > "$log_out"

                                echo "$base_name,$p,$m,$c1,$c2,$w,$r,$i,$runtime,$fitness" >> "$SUMMARY_FILE"

                            done
                        done
                    done
                done
            done
        done
    done
done

echo "[INFO] Tuning completed. Summary saved to $SUMMARY_FILE"
