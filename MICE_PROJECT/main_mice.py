# main_mice.py
from mice import MICEImputerRunner

if __name__ == "__main__":
    import sys

    if len(sys.argv) < 3:
        print("Usage: python main_mice.py <dataset.csv> <output_dir>")
        exit(1)

    dataset = sys.argv[1]
    output_dir = sys.argv[2]

    runner = MICEImputerRunner(dataset, output_dir)
    runner.run()
