#!/bin/bash

# ========== CONFIGURACIÓN ==========
EXEC="./pso_imputation"
MAKE_CMD="make"
DATASETS_DIR="datasets_1"
OUT_DIR="out_particle_tuning_1"
RESULTS_DIR="$OUT_DIR/results"

# ========== PARÁMETROS DE TUNING ==========
PARTICLES_LIST=(20 40 60 80 100)
ITERATIONS_LIST=(30 40 60 80 100)
C1_LIST=(1.2 1.5 2.0)
C2_LIST=(1.2 1.5 2.0)
W_LIST=(0.5 0.7)
VEL_RATIO=(0.1 0.2 0.3)
RATIO_LIST=(0.3 0.5)
INIT_TYPES=(1 2)

# Seleccionar datasets representativos
select_datasets() {
    ls "$DATASETS_DIR"/*.csv | grep "_input_\\.csv$" | grep -E "rows_(500|1000|5000|10000)_columns_(20|100|500)_comp_0\\.[2-9]_" | sort
}

# ========== COMPILACIÓN ==========
echo "[INFO] Compiling with $MAKE_CMD..."
$MAKE_CMD || { echo "[ERROR] Compilation failed"; exit 1; }

# Create necessary output folders
mkdir -p "$OUT_DIR" "$RESULTS_DIR"

# ========== EJECUCIÓN PRINCIPAL ==========
for dataset in $(select_datasets); do
    base_name=$(basename "$dataset" .csv)

    for p in "${PARTICLES_LIST[@]}"; do
        for m in "${ITERATIONS_LIST[@]}"; do
            for c1 in "${C1_LIST[@]}"; do
                for c2 in "${C2_LIST[@]}"; do
                    for w in "${W_LIST[@]}"; do
                        for r in "${RATIO_LIST[@]}"; do
                            for v in "${VEL_RATIO[@]}"; do
                                for i in "${INIT_TYPES[@]}"; do

                                    log_name="${base_name}_p${p}_m${m}_c1${c1}_c2${c2}_w${w}_r${r}_v${v}_i${i}"
                                    log_out="$RESULTS_DIR/${log_name}.log"
                                    echo "[RUN] $log_name"

                                    start=$(date +%s.%N)
                                    output=$($EXEC -d "$dataset" -o "$OUT_DIR" -p $p -m $m -c1 $c1 -c2 $c2 -w $w -v $v -r $r -i $i 2>&1)
                                    end=$(date +%s.%N)

                                    runtime=$(echo "$end - $start" | bc -l)
                                    fitness=$(echo "$output" | grep "Global Best Fitness" | tail -n 1 | awk '{print $NF}')
                                    fitness=${fitness:-nan}

                                    echo "$output" > "$log_out"
                                done
                            done
                        done
                    done
                done
            done
        done
    done
done

echo "[INFO] Tuning finished."
