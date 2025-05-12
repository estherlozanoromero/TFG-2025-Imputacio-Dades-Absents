#!/bin/bash
#SBATCH --job-name=pso_tunning
#SBATCH --output=slurm_logs/tunning_%j.out
#SBATCH --error=slurm_logs/tunning_%j.err
#SBATCH --time=12:00:00
#SBATCH --partition=short
#SBATCH --cpus-per-task=4
#SBATCH --mem=8G

# Carga de módulos si es necesario (descomenta según tu entorno)
# module load gcc/10.2.0
# module load cmake

echo "[SLURM] Job started on $(hostname) at $(date)"

# Asegúrate de estar en la ruta del proyecto
cd "$HOME"

# Lanza el script bash
bash run_tuning.sh

echo "[SLURM] Job finished at $(date)"