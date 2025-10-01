![License: MIT](https://img.shields.io/badge/Licence-MIT-green)
![University: Paris 8](https://img.shields.io/badge/University-Paris%208-red)
![machine: learning](https://img.shields.io/badge/machine-learning-blue)
![C: 9.3.0](https://img.shields.io/badge/c-9.3.0-brightgreen)
![Contributors](https://img.shields.io/badge/contributor-1-orange)
![Stars](https://img.shields.io/github/stars/Fab16BSB/SOM_C?color=orange)
![Fork](https://img.shields.io/github/forks/Fab16BSB/SOM_C?color=orange)
![Watchers](https://img.shields.io/github/watchers/Fab16BSB/SOM_C?color=orange)


<h1> Self Organizing Map </h1>

## 🌍 Versions multilingues du README

- 🇫🇷 [Français (vous êtes ici)](#)
- 🇬🇧 [English](./README.md)
- 🇪🇸 [Español](./README.es.md)

## 📘 Vue d'ensemble du projet

Ce projet consiste en une implémentation simplifiée d'une **Self-Organizing Map (SOM)** réalisée en **Licence 3** à l'**Université Paris 8**. Une version **Java** de ce projet est également disponible. Vous pouvez consulter le dépôt de la version Java [ici](https://github.com/Fab16BSB/SOM_JAVA). L’objectif principal était de comprendre le fonctionnement de l’algorithme SOM en reproduisant une version *from scratch* avec les contraintes suivantes :

- **Aucune variable globale** utilisée.  
- Les **BMU (Best Matching Units)** sont stockés dans une **liste chaînée**.  
- La taille des collections: matrices et listes chaînées doivent être **calculée dynamiquement**.  

---

### 📊 Jeu de données

Le jeu de données utilisé est le **jeu de données Iris**, disponible sur Kaggle :  
[Iris Dataset](https://www.kaggle.com/uciml/iris)

- **150 échantillons**  
- **4 caractéristiques** par échantillon :  
  - Sepal length  
  - Sepal width  
  - Petal length  
  - Petal width  
- **3 classes** :  
  1. Iris-setosa  
  2. Iris-versicolor  
  3. Iris-virginica  

---

### ⚙️ Fonctionnement de l’algorithme SOM

La **Self-Organizing Map (SOM)** est un réseau de neurones non supervisé utilisé pour la réduction de dimensionnalité et la visualisation de données. Voici les grandes étapes :

1. **Initialisation**  
   Les neurones de la carte sont initialisés aléatoirement dans l’espace des caractéristiques.  

2. **Calcul de la distance**  
   Pour chaque donnée d’entrée, on calcule la distance euclidienne entre cette donnée et tous les neurones.  
   Le neurone avec la distance minimale est appelé **BMU** (Best Matching Unit). Ce processus suit le principe du **Winner-Takes-All** (WTA), où seul le neurone le plus proche de la donnée d'entrée est sélectionné, ainsi que ses voisins.

3. **Mise à jour de la carte**  
   Le BMU et ses voisins sont ajustés pour se rapprocher de la donnée d’entrée, selon un taux d’apprentissage α. Cela permet d'ajuster progressivement la carte pour qu'elle représente mieux les données d'entrée.

4. **Itération**  
   Les étapes 2 à 3 sont répétées pendant un nombre d’itérations fixé.  
   - Le taux d’apprentissage **diminue** au fil du temps.  
   - La **taille du voisinage** se rétracte progressivement, ce qui permet à la carte de se spécialiser tout en maintenant la cohérence topologique.

Le résultat est une carte organisée topologiquement, où les classes similaires se retrouvent dans des zones proches. L'algorithme SOM permet ainsi de regrouper les données similaires tout en préservant leur structure.

---

### 🧑‍💻 Technologies utilisées

- **Langage :** C (implémentation from scratch)  
- **Bibliothèques :** uniquement la bibliothèque standard C (`math.h` pour `sqrt`, `pow`, etc.)

---

### 💻 Installer C (Si vous n'avez pas C installé)

Si vous n'avez pas un compilateur C installé, vous pouvez suivre les instructions dans l'une de mes vidéos YouTube pour installer un compilateur C sur différentes plateformes :

- **Linux** : Installer C sur Linux
- **Mac** : Installer C sur Mac
- **Windows** : [Installer C sur Windows](https://www.youtube.com/watch?v=MNPhTlvzSIA)

---

### 📝 Compilation et Exécution

Vous pouvez compiler et exécuter le projet soit **localement**, soit en utilisant **Docker**.

#### ⚡ Option 1 : Compilation et exécution locales

1. **Cloner le dépôt**

   ```bash
   git clone https://github.com/Fab16BSB/SOM_C.git
   cd SOM_C
   ```
2. **Compilation**

   ```bash
   gcc -o som main.c -lm
   ```

3. **Exécution**

   ```bash
   ./som
   ````

### 🐳 Option 2 : Utilisation avec Docker

1. **Cloner le dépôt**
   
  ```bash
  git clone https://github.com/Fab16BSB/SOM_C.git
  cd SOM_C
  ```

2. **Construire l'image Docker**

```bash
docker build -t som_c .
```

3. **Lancer le conteneur Docker**
```bash
docker run --rm som_c
```

---

### 📈 Résultats
a = virginica / b = versicolor / c = setosa  

| c | c | c | c | c | a | a | a | a | a |
|---|---|---|---|---|---|---|---|---|---|
| c | c | c | c | b | a | a | a | a | a |
| c | c | c | c | b | b | b | a | a | a |
| c | c | c | c | b | b | b | b | a | b |
| c | c | c | b | b | b | b | b | a | a |
| c | c | c | b | b | b | b | b | b | a |

---

### 🙌 Remerciements
Je souhaite remercier mon enseignant pour m'avoir proposé ce projet ainsi que pour ses précieux conseils.

