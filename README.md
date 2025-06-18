# 📌 Estudi de l'eficàcia dels mètodes d’imputació de dades absents en conjunts d’altes dimensions i baixa completesa

Aquest repositori conté el codi, els experiments i l’anàlisi del **Treball de Fi de Grau**. L’objectiu principal és investigar i comparar tècniques d’imputació de valors absents en dades d’alta dimensionalitat i baixa completesa, mitjançant l’ús d’un algoritme propi basat en *Particle Swarm Optimization (PSO)*.

---

## 📁 Estructura del repositori

```
├── PSO/                    # Algoritme principal (C++)
│   ├── main.cc             # Entrada principal
│   ├── pso/                # Carpeta amb les classes de l'algoritme PSO
|   |   ├── Fitness.cc      # Classe amb la codificació del Fitness del PSO
|   |   ├── Particle.cc     # Classe que representa una Particula
|   |   └── PSO.cc          # Classe que aplica l'algoritme PSO
│   ├── utils/              # Carpeta amb altres classes d'utilitat
|   |   ├── Dataset.cc      # Classe encarregada de llegir i escriure els datasets
|   |   ├── Initializer.cc  # Classe encarregada de dir amb quin tipus d'inicialització comença el PSO
|   |   └── Utils.cc        # Classe per la implementació de mètodes gènerics
│   └── Makefile            # Compilació

├── MICE/                   # Imputació amb MICE (Python)
│   ├── main_mice.py
│   └── mice.py

├── KNN/                    # Imputació amb KNN (Python)
│   ├── main_knn.py
│   └── knn.py

├── Experiments/            # Resultats i notebooks
│   ├── *.xlsx              # Fitxers amb resultats
│   └── *.ipynb             # Notebooks d'anàlisi

└── README.md               # Aquest fitxer
```

---

## ⚙️ Requisits

### 🧩 C++ (PSO)

- C++17 o superior
- Make
- Entorn Unix/Linux

#### Compilar:
```bash
cd PSO
make
```

#### Executar:
```bash
./pso_imputation -d <dataset.csv> -o <output_dir> [opcions]
```

**Paràmetres opcionals:**

| Flag | Descripció | Valor per defecte |
|------|------------|-------------------|
| `-p` | Nombre de partícules | 30 |
| `-m` | Iteracions màximes | 50 |
| `-c1`, `-c2` | Factors d'aprenentatge | 1.5 |
| `-w` | Pes d'inèrcia | 0.7 |
| `-v` | Velocitat màxima relativa | 0.2 |
| `-r` | Percentatge de random en la inicialització híbrida | 0.5 |
| `-i` | Tipus d'inicialització (`0`, `1`, `2`) | 2 |
| `-a` | Atributs actius (opcional) | — |

**Exemple d’execució:**
```bash
./pso_imputation -d data.csv -o output -p 50 -m 100 -i 2 -r 0.3
```

---

### 🐍 Python (KNN i MICE)

Es recomana crear un entorn virtual:

```bash
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

#### MICE:
```bash
python main_mice.py <dataset.csv> <output_dir>
```

#### KNN:
```bash
python main_knn.py <dataset.csv> <output_dir> [n_neighbors]
```

---

## 📊 Experiments

A la carpeta `Experiments/` hi trobaràs:

- 📈 Fitxers `.xlsx` amb resultats dels experiments
- 📒 Notebooks amb anàlisi de:
  - Correlacions originals vs imputades
  - Fitness de PSO
  - Comparació de temps d’execució i precisió entre PSO, KNN i MICE

---

## 📚 Documentació

Aquest projecte disposa d’una **memòria escrita en LaTeX** on es detallen:

- Els fonaments teòrics de la imputació
- El disseny i la implementació de l’algorisme PSO
- L’anàlisi comparativa amb altres mètodes
- Conclusions i resultats obtinguts

---

## 👩‍💻 Autora

**Esther Lozano Romero**  
Facultat d’Informàtica de Barcelona – UPC  
Grau en Enginyeria Informàtica (Computació)
