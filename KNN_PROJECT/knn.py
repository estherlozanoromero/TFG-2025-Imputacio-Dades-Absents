# knn_imputer.py
import os
import pandas as pd
import numpy as np
import time
from sklearn.impute import KNNImputer

class KNNImputerRunner:
    def __init__(self, dataset_path, output_dir, n_neighbors=5):
        self.dataset_path = dataset_path
        self.output_dir = output_dir
        self.base_name = os.path.basename(dataset_path).split('.c')[0]
        self.df_raw = pd.read_csv(dataset_path)
        self.df_clean = self.df_raw.dropna()
        self.imputer = KNNImputer(n_neighbors=n_neighbors)
        self.start_time = None
        self.end_time = None

        self._prepare_dirs()

    def _prepare_dirs(self):
        os.makedirs(self.output_dir, exist_ok=True)
        os.makedirs(os.path.join(self.output_dir, "dataset_out"), exist_ok=True)
        os.makedirs(os.path.join(self.output_dir, "output_correlations"), exist_ok=True)
        os.makedirs(os.path.join(self.output_dir, "results"), exist_ok=True)

    def _correlation_matrix(self, df):
        return df.corr()

    def run(self):
        self.start_time = time.time()

        corr_original = self._correlation_matrix(self.df_clean)

        df_numeric = self.df_raw.select_dtypes(include=[np.number])
        imputed_array = self.imputer.fit_transform(df_numeric)
        df_imputed = pd.DataFrame(imputed_array, columns=df_numeric.columns)

        corr_imputed = self._correlation_matrix(df_imputed)

        self.end_time = time.time()
        self._export_outputs(df_imputed, corr_original, corr_imputed)

    def _export_outputs(self, df_imputed, corr_original, corr_imputed):
        suffix = "_knn"

        csv_out = f"{self.output_dir}/dataset_out/{self.base_name}{suffix}_imputed.csv"
        csv_corr_orig = f"{self.output_dir}/output_correlations/{self.base_name}{suffix}_corr_original.csv"
        csv_corr_imputed = f"{self.output_dir}/output_correlations/{self.base_name}{suffix}_corr_imputed.csv"
        summary_csv = f"{self.output_dir}/results/summary.csv"

        df_imputed.to_csv(csv_out, index=False)
        corr_original.to_csv(csv_corr_orig)
        corr_imputed.to_csv(csv_corr_imputed)

        duration = round(self.end_time - self.start_time, 4)
        total_cells = self.df_raw.size
        missing_count = self.df_raw.isna().sum().sum()
        rows, cols = self.df_raw.shape
        missing_percent = 100.0 * missing_count / total_cells
        file_size_mb = os.path.getsize(self.dataset_path) / (1024 * 1024)

        write_header = not os.path.exists(summary_csv)
        with open(summary_csv, 'a') as f:
            if write_header:
                f.write("dataset,rows,columns,missing_values,missing_percent,file_MB,"
                        "method,time_sec\n")
            f.write(f"{self.base_name},{rows},{cols},{missing_count},"
                    f"{missing_percent:.2f},{file_size_mb:.2f},KNN,{duration}\n")
