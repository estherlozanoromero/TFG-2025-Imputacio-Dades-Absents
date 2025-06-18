# main_knn.py
from knn import KNNImputerRunner
import sys

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python main_knn.py <dataset.csv> <output_dir> [n_neighbors]")
        exit(1)

    dataset = sys.argv[1]
    output_dir = sys.argv[2]
    n_neighbors = int(sys.argv[3]) if len(sys.argv) > 3 else 5

    runner = KNNImputerRunner(dataset, output_dir, n_neighbors=n_neighbors)
    runner.run()
