#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


/*------------------------STRUCTURES------------------------*/
/** 
 * @struct neurone
 * @brief Represents a neuron in a Self-Organizing Map (SOM).
 *
 * This structure holds the feature vector associated with a neuron,
 * its Euclidean distance to an input vector, and its class label.
 * 
 * @Author @Fab.16
 */
typedef struct neurone {
    double* vecteur;              /**< Feature vector of the neuron */
    double distanceEuclidienne;  /**< Euclidean distance to the current input */
    char* etiquette;             /**< Label or class associated with the neuron */
} neurone;

/**
 * @struct dataset
 * @brief Represents a single data point in the dataset.
 *
 * Each data point has a feature vector, its norm, and an associated label.
 */
typedef struct dataset {
    double* vecteur;    /**< Feature vector of the data point */
    double norme;       /**< Norm (magnitude) of the feature vector */
    char* etiquette;    /**< Label or class of the data point */
} dataset;

/**
 * @struct paramDataset
 * @brief Holds metadata and buffers for managing a dataset.
 *
 * Includes the full dataset array, vector size information, the mean vector,
 * and a shuffled index array for random sampling.
 */
typedef struct paramDataset {
    dataset* mesDataset;      /**< Array of data points */
    int tailleTab;            /**< Number of data points */
    int tailleVec;            /**< Size of each feature vector */
    double* vecteurMoyen;     /**< Mean vector of the dataset */
    int* tabMelanger;         /**< Index array for shuffling the dataset */
} paramDataset;

/**
 * @struct ParamMatrice
 * @brief Configuration and container for the SOM neuron matrix.
 *
 * Stores the matrix dimensions, neuron grid, learning rate,
 * and number of neighbors to update during training.
 */
typedef struct ParamMatrice {
    int longueur;         /**< Number of rows in the neuron matrix */
    int largeur;          /**< Number of columns in the neuron matrix */
    neurone** matrice;    /**< 2D array of neurons */
    double alpha;         /**< Learning rate */
    int nbVoisin;         /**< Number of neighboring neurons to update */
} ParamMatrice;

/**
 * @struct Element
 * @brief Represents a node in a singly linked list of BMUs (Best Matching Units).
 *
 * Stores the coordinates (x, y) of a neuron and a pointer to the next element.
 */
typedef struct Element {
    int x;                /**< Row index of the BMU in the neuron matrix */
    int y;                /**< Column index of the BMU in the neuron matrix */
    struct Element* suivant; /**< Pointer to the next element in the list */
} Element;

/**
 * @struct Liste
 * @brief Represents a singly linked list of BMUs.
 *
 * Points to the first element (head) of the list.
 */
typedef struct Liste {
    Element* premier;     /**< Head of the linked list */
} Liste;

/**
 * @struct ParamListeChaine
 * @brief Contains a list of BMUs and its size.
 *
 * Used to store and process the sequence of BMUs found during dataset traversal.
 */
typedef struct ParamListeChaine {
    Liste* lesBMU;            /**< Linked list of BMUs */
    int tailleListeBMU;       /**< Number of elements in the list */
} ParamListeChaine;






/*------------------------PROTOTYPES------------------------*/

paramDataset traitementFichier(char*,char*);
dataset donneeToStructure(char*, char*, int);
void afficherData(int, int, dataset*);
void afficherVecteurNeurone(int, int, neurone*);
dataset * NormaliserVecteur(dataset*, int, int);
void vecteurMoyen(dataset*, int, int, double*);
neurone* genereVecteurDouble(int, int, double, double, double*);
int * indiceMelange(dataset*, int, int);
ParamMatrice genererMatriceNeurone(ParamMatrice, neurone*, int);
void afficherMatriceNeurone(ParamMatrice, int);
double distanceEuclidienne (double*, double*, int);
void distanceEuclidienneMatrice(double*, neurone**, int, int, int);
void rapprochement(paramDataset, ParamMatrice, double, int);
ParamListeChaine rechercheBMU(ParamMatrice);
Liste * initialisationListe(int, int);
void ajouterElement(Liste*, int, int);
void supprimerListe(ParamListeChaine);
Element * randomElementListeBMU(Liste*, int);
void apprentissage(Element*, ParamMatrice, double, int, int, double*);
void putEtiquette(ParamMatrice, paramDataset);
int indexPetitTab(double[], int);
void afficheEtiquette(ParamMatrice);
int equals(char*, char*);




/*------------------------FONCTIONS------------------------*/


/**
 * @brief Reads a data file and stores its contents in a structured dataset.
 *
 * This function opens a file specified by its name, reads it line by line,
 * splits each line using the given delimiter, and converts the data into
 * a `dataset` structure. It also computes and stores the number of vectors,
 * their size, and allocates space for the mean vector.
 *
 * @param nomFichier The name of the file to read (CSV or similar format).
 * @param delimiteur The delimiter used to split each line (e.g., "," or "\t").
 * @return A fully populated `paramDataset` structure containing:
 *         - an array of dataset entries,
 *         - the size of the dataset,
 *         - the dimension of the vectors,
 *         - a placeholder for the mean vector.
 *
 * @note Each line of the file is expected to contain a vector followed by a label.
 *       The number of delimiter occurrences in the first line defines the vector size.
 * @warning If the file cannot be opened, a warning is printed to the console and
 *          the returned structure may be incomplete or invalid.
 */
paramDataset traitementFichier(char* nomFichier, char * delimiteur){

    FILE* fichier = NULL;
    char caractereActuel;
    char * ligneActuel = (char*)malloc(sizeof(char));
    int i = 0, nbDelimiteur = 0, nbDonnee = 0;
    paramDataset paramGlobal;
    paramGlobal.mesDataset = (dataset*)malloc(sizeof(dataset));


    fichier = fopen(nomFichier, "r");

    if (fichier != NULL){

        while ((caractereActuel = fgetc(fichier)) != EOF){

            //printf("%c",caractereActuel); // affichage de tout le fichier caractère par caractère

            if(caractereActuel!='\n'){  // récuperation d'une ligne

                ligneActuel[i] = caractereActuel;
                i++;
                ligneActuel = realloc(ligneActuel, i+1);

                if(caractereActuel==delimiteur[0] && nbDonnee==0){ // calcule de la taille des vecteurs
                    nbDelimiteur++;
                }
            }

            else{   // traitement des donnees d'une ligne
                ligneActuel[i] = '\0';
                paramGlobal.mesDataset[nbDonnee] = donneeToStructure(ligneActuel, delimiteur, nbDelimiteur);
                nbDonnee++;
                paramGlobal.mesDataset = realloc(paramGlobal.mesDataset, (nbDonnee+1) * sizeof(neurone));
                i = 0;
            }
        }
            fclose(fichier);
    }

    else{
        printf("impossible d'ouvrir le fichier");
    }

    // remplissage des paramètres globale lié au dataset
    paramGlobal.tailleTab = nbDonnee;
    paramGlobal.tailleVec = nbDelimiteur;
    paramGlobal.vecteurMoyen = malloc(nbDelimiteur * sizeof(double));

    return paramGlobal;
}


/**
 * @brief Converts a delimited string into a dataset structure.
 *
 * This function parses a string containing numeric values followed by a label,
 * splits it using the provided delimiter, and stores the values in a `dataset`
 * structure. The numeric values are stored in a vector, and the final token
 * is treated as the label.
 *
 * @param donnee The input string representing one line of data.
 * @param delimiteur The delimiter used to split the string (e.g., "," or "\t").
 * @param tailleVecteur The expected number of numeric values (dimension of the vector).
 * @return A `dataset` structure containing the parsed vector and its associated label.
 *
 * @note The function assumes that the number of numeric tokens is exactly `tailleVecteur`,
 *       and the label is the last element in the string.
 * @warning Memory is dynamically allocated for both the vector and the label.
 *          The caller is responsible for freeing this memory.
 */
dataset donneeToStructure(char * donnee, char* delimiteur, int tailleVecteur){
    dataset data;
    int i = 0;
    int j = 0;
    char * token = NULL;
    data.vecteur = (double*)malloc(tailleVecteur * sizeof(double));
    data.etiquette = (char*)malloc(sizeof(char));



    token = strtok(donnee, delimiteur);

    while( token != NULL){
        if(i<tailleVecteur){
            data.vecteur[i] = strtod(token,NULL);
            //printf("%f\n",vecteur[i]);
        }

        else{
            while(token[j]!='\0'){
                data.etiquette[j] = token[j];
                j++;
                data.etiquette = realloc(data.etiquette, (j+1)* sizeof(char));
            }
            data.etiquette[j] = '\0';
        }
        i++;

        token = strtok(NULL,delimiteur);
    }
    i = 0;

    return data;

}


/**
 * @brief Displays the contents of a dataset array in a formatted manner.
 *
 * This function prints each dataset entry line by line, displaying the vector values
 * separated by commas, followed by the corresponding label.
 *
 * @param nbVecteur The number of data entries in the dataset.
 * @param tailleVecteur The size of each feature vector.
 * @param mesData A pointer to the array of dataset structures to display.
 *
 * @note The output format is: `value1,value2,...,valueN,label`
 */
void afficherData(int nbVecteur, int tailleVecteur, dataset *mesData){
    int i,j;

    for(i=0;i<nbVecteur;i++){
        for(j=0;j<tailleVecteur;j++){
            printf("%f",mesData[i].vecteur[j]);

            if(j<tailleVecteur-1){
                printf(",");
            }
        }
        printf(",%s", mesData[i].etiquette);
        printf("\n");
    }
}


/**
 * @brief Displays the weight vectors of an array of neurons.
 *
 * This function iterates through an array of neurons and prints each neuron's
 * vector components separated by commas.
 *
 * @param nbVecteur The number of neurons to display.
 * @param tailleVecteur The size of each neuron's weight vector.
 * @param mesNeurone A pointer to the array of neurons.
 *
 * @note Only the vector components are printed; labels and distances are not displayed.
 * @warning The output includes an extra comma before the newline due to redundant printf calls.
 */
void afficherVecteurNeurone(int nbVecteur, int tailleVecteur, neurone *mesNeurone){
    int i,j;

    for(i=0;i<nbVecteur;i++){
        for(j=0;j<tailleVecteur;j++){
            printf("%f,",mesNeurone[i].vecteur[j]);

            if(j<tailleVecteur-1){
                printf(",");
            }
        }
        printf("\n");
    }
}


/**
 * @brief Normalizes the feature vectors of a dataset.
 *
 * This function computes the Euclidean norm of each vector in the dataset
 * and normalizes each component by dividing it by the corresponding norm.
 * The normalization is done in-place.
 *
 * @param mesStructure A pointer to the dataset array to normalize.
 * @param tailleVecteur The size (dimension) of each feature vector.
 * @param nbVecteur The number of vectors in the dataset.
 * @return A pointer to the same dataset array, now with normalized vectors.
 *
 * @note This function modifies the input dataset directly.
 * @warning Memory for `norme` is allocated with `calloc`, but never freed — this may cause a memory leak.
 */
dataset * NormaliserVecteur(dataset * mesStructure, int tailleVecteur, int nbVecteur){
    int i,j;
    double * norme = (double*)calloc(nbVecteur,nbVecteur * sizeof(double));

    for(i=0; i<nbVecteur; i++){
        for(j=0; j<tailleVecteur; j++){
            norme[i] += pow(mesStructure[i].vecteur[j],2);
        }

        norme[i] = sqrt(norme[i]);


        for(j=0; j<tailleVecteur; j++){
            mesStructure[i].vecteur[j] = (mesStructure[i].vecteur[j]/norme[i]);
        }

    }

    return mesStructure;

}


/**
 * @brief Computes the mean vector from a dataset.
 *
 * This function calculates the component-wise average of all vectors in the dataset
 * and stores the result in the provided `vecteurMoyen` array.
 *
 * @param mesStructure A pointer to the dataset array.
 * @param tailleVecteur The size (dimension) of each feature vector.
 * @param nbVecteur The number of vectors in the dataset.
 * @param vecteurMoyen A pointer to a pre-allocated array where the resulting mean vector will be stored.
 *
 * @note The mean is computed across all dataset entries for each vector component.
 * @warning The array `vecteurMoyen` must be allocated before calling this function.
 */
void vecteurMoyen(dataset * mesStructure, int tailleVecteur, int nbVecteur, double * vecteurMoyen){
    int i,j;
    double somme = 0.0;

    for(i=0;i<tailleVecteur;i++){
        for(j=0;j<nbVecteur;j++){
            somme += mesStructure[j].vecteur[i];
        }
        vecteurMoyen[i] = somme/nbVecteur;
        somme = 0.0;
        //printf("%f\n", vecteurMoyen[i]);
    }
}


/**
 * @brief Generates an array of neurons with random vectors based on a mean vector.
 *
 * This function creates `nbVecteur` neurons, each containing a vector of random double values.
 * The random values are uniformly distributed between `vecteurMoyen[i] - min` and `vecteurMoyen[i] + max`
 * for each component `i`. The number of vectors is adjusted to be a multiple of 10.
 *
 * @param nbVecteur The number of neurons (will be rounded down to nearest multiple of 10).
 * @param tailleVecteurs The size (dimension) of each neuron's vector.
 * @param min The minimum offset from the mean for random generation.
 * @param max The maximum offset from the mean for random generation.
 * @param vecteurMoyen The reference mean vector used to center the random values.
 * @return A pointer to the array of generated neurons.
 *
 * @note Memory is dynamically allocated for each neuron's vector and the resulting array.
 *       Caller is responsible for freeing the returned `neurone*` and its internal vectors.
 * @warning The use of `srand(time(NULL))` on repeated calls in quick succession may produce similar outputs.
 */
neurone* genereVecteurDouble(int nbVecteur, int tailleVecteurs, double min, double max, double* vecteurMoyen){

    int i,j;
    double * borneSupp = (double*)malloc(sizeof(double) * tailleVecteurs);
    double * borneInf = (double*)malloc(sizeof(double) * tailleVecteurs);
    neurone* mesNeurones = (neurone*)malloc(sizeof(neurone)*nbVecteur);

    srand(time(NULL));

    nbVecteur = nbVecteur - (nbVecteur%10);


    for(i=0; i<tailleVecteurs; i++){
        borneInf[i] = vecteurMoyen[i] - min;
        borneSupp[i] = vecteurMoyen[i] + max;

    }

    for(i=0;i<nbVecteur;i++){
        neurone monNeuronne;
        monNeuronne.vecteur = (double*)malloc(sizeof(double)*tailleVecteurs);


        for(j=0;j<tailleVecteurs;j++){
            monNeuronne.vecteur[j]= (rand()/ (RAND_MAX  / (borneSupp[j] - borneInf[j])) + borneInf[j]);

            //printf("%f;",monNeuronne.vecteur[j]);
        }
        //printf("\n");
        mesNeurones[i] = monNeuronne;

    }

    free(borneInf);
    free(borneSupp);

    return mesNeurones;
}


/**
 * @brief Generates a shuffled array of indices for the dataset.
 *
 * This function initializes an array of indices from 0 to `nbVecteur - 1`
 * and randomly shuffles them using a basic Fisher–Yates algorithm.
 *
 * @param mesData Pointer to the dataset array (not used in the current implementation).
 * @param nbVecteur The number of data points (size of the dataset).
 * @param tailleVecteur The size (dimension) of each feature vector (unused).
 * @return A pointer to an array of shuffled indices.
 *
 * @note The returned array must be freed by the caller.
 * @warning The `mesData` and `tailleVecteur` parameters are not used and could be removed.
 * @warning The random number generation line is incorrect and may result in out-of-bounds access.
 */
int * indiceMelange(dataset * mesData, int nbVecteur, int tailleVecteur){

    int * indexAleatoire = malloc(sizeof(int)*nbVecteur);
    int i,j,tmp,random;


    for(i=0; i<nbVecteur; i++){
        indexAleatoire[i]=i;

    }


    for(i=0; i<nbVecteur; i++){
        random = (rand()/ (RAND_MAX  / nbVecteur));
        tmp = indexAleatoire[i];
        indexAleatoire[i]=indexAleatoire[random];
        indexAleatoire[random] = tmp;
    }

    for(i=0; i<nbVecteur; i++){
            for(j=0; j<tailleVecteur; j++){

                if(j<tailleVecteur){

                }
            }
    }

    return indexAleatoire;
}


/**
 * @brief Creates and fills a matrix of neurons from a linear list of neurons.
 *
 * This function initializes a 2D matrix of neurons from a given 1D array.
 * The matrix dimensions are computed so that the width is set to one-tenth of the total number
 * of neurons (`nbNeurone / 10`), and the height is derived accordingly to fit all neurons.
 *
 * @param dataMatrice A `ParamMatrice` structure, partially filled and passed by value.
 * @param mesNeurone A linear array of `neurone` elements to fill the matrix.
 * @param nbNeurone The total number of neurons to place in the matrix.
 * @return The updated `ParamMatrice` structure containing the filled matrix.
 *
 * @note The matrix is filled row by row (left to right, top to bottom).
 *       Memory is dynamically allocated for each row of the matrix.
 *       Caller is responsible for freeing the matrix afterward.
 * @warning Assumes `nbNeurone` is divisible by 10. No check is performed to validate this.
 */
ParamMatrice genererMatriceNeurone(ParamMatrice dataMatrice, neurone * mesNeurone, int nbNeurone){
    int i,j,k = 0;

    dataMatrice.largeur = nbNeurone/10;
    dataMatrice.longueur = nbNeurone/dataMatrice.largeur;

    neurone** matriceNeurone = malloc(sizeof(neurone*) * dataMatrice.largeur);


    for(i=0; i<dataMatrice.largeur; i++){
        matriceNeurone[i] = malloc(sizeof(neurone) * dataMatrice.longueur);
    }

    for(i=0; i<dataMatrice.largeur; i++){
        for(j=0; j<dataMatrice.longueur; j++){
            matriceNeurone[i][j] = mesNeurone[k];
            k++;
        }
    }

    dataMatrice.matrice = matriceNeurone;

    return dataMatrice;
}


/**
 * @brief Displays the vectors of a matrix of neurons.
 *
 * This function iterates through the 2D matrix of neurons and prints each neuron's vector 
 * element by element. Each vector is displayed on a new line.
 *
 * @param dataMatrice A `ParamMatrice` structure containing the matrix of neurons.
 * @param tailleVec The size (dimension) of each neuron's vector to display.
 *
 * @note This function prints each vector of the neuron matrix in a row-major order.
 */
void afficherMatriceNeurone(ParamMatrice dataMatrice, int tailleVec){
    int i,j,k;

    for(i=0; i<dataMatrice.largeur; i++){
        for(j=0; j<dataMatrice.longueur; j++){
            for(k=0; k<tailleVec; k++){
                printf("%f ", dataMatrice.matrice[i][j].vecteur[k]);
            }
            printf("\n");
        }

    }

}


/**
 * @brief Computes the Euclidean distance between a given vector and all the vectors in a neuron matrix.
 *
 * This function iterates through a 2D matrix of neurons and computes the Euclidean distance 
 * between a given vector and each neuron's vector. The distance is stored in the `distanceEuclidienne` 
 * attribute of each corresponding neuron in the matrix.
 *
 * @param vecteur The vector to compare with all vectors in the neuron matrix.
 * @param matrice The 2D matrix of neurons.
 * @param longueur The number of rows in the matrix (height).
 * @param largeur The number of columns in the matrix (width).
 * @param tailleVec The dimension (size) of each vector.
 *
 * @note The computed distance is stored directly in the `distanceEuclidienne` field of each neuron.
 */
void distanceEuclidienneMatrice(double * vecteur, neurone ** matrice, int longueur, int largeur, int tailleVec){
    int i, j, k=0;

   for(i=0; i<largeur; i++){
        for(j=0; j<longueur; j++){
            matrice[i][j].distanceEuclidienne = distanceEuclidienne(vecteur, matrice[i][j].vecteur, tailleVec);
            k++;
        }
    }
}


/**
 * @brief Computes the Euclidean distance between two vectors.
 *
 * This function calculates the Euclidean distance between two vectors in an n-dimensional space.
 * The distance is computed as the square root of the sum of squared differences between corresponding elements.
 *
 * @param vecteur The first vector.
 * @param vector The second vector.
 * @param taille The dimension (number of elements) of the vectors.
 * @return The Euclidean distance between the two vectors.
 */
double distanceEuclidienne (double* vecteur, double* vector, int taille){
    int i;
    double distance = 0.0;


   for(i=0; i<taille; i++){
        distance += pow( (vector[i] - vecteur[i]),2);
    }

    distance = sqrt(distance);
    return distance;
}


/**
 * @brief Performs the training process using the Self-Organizing Map (SOM) algorithm.
 *
 * This function simulates the learning process of the Self-Organizing Map (SOM) algorithm, where 
 * a dataset is progressively mapped to a matrix of neurons. The function adjusts the neurons' weights 
 * based on the Euclidean distance between the dataset vectors and the neurons in the matrix.
 * The learning rate (`alpha`) is progressively decreased, and the neighborhood size (`nbVoisin`) is adjusted
 * throughout the process.
 *
 * The training consists of two phases:
 * - **Phase 1:** In the first 20% of the training, the neighborhood size (`nbVoisin`) is gradually reduced
 *   from 3 to 1, and the learning rate (`alpha`) is decreased.
 * - **Phase 2:** In the remaining 80% of the training, the neighborhood size continues to decrease from 2 to 1,
 *   and the learning rate (`alpha`) is decreased further.
 *
 * The function selects the Best Matching Unit (BMU) for each data point and performs learning based on the 
 * BMU’s position in the matrix.
 *
 * @param data A `paramDataset` structure containing the dataset to be mapped to the neuron matrix.
 * @param dataMatrice A `ParamMatrice` structure containing the matrix of neurons to be updated.
 * @param alpha The initial learning rate (alpha).
 * @param temps The number of iterations for training.
 *
 * @note The function progressively decreases the learning rate and adjusts the neighborhood size during training.
 * The training operates in two phases, with three sub-phases within the first phase and two sub-phases in the second.
 */
void rapprochement(paramDataset data, ParamMatrice dataMatrice, double alpha, int temps){
    int i,j, index;
    int phase = 0;
    ParamListeChaine dataListeChaine;
    Element * BMU;
    double alphaDepart = alpha;


    for(i=0; i<temps; i++){
        for(j=0; j<data.tailleTab; j++){
            index = data.tabMelanger[j];
            distanceEuclidienneMatrice(data.mesDataset[index].vecteur, dataMatrice.matrice, dataMatrice.longueur, dataMatrice.largeur, data.tailleVec);
            dataListeChaine = rechercheBMU(dataMatrice);
            BMU = randomElementListeBMU(dataListeChaine.lesBMU, dataListeChaine.tailleListeBMU);
            //printf("BMU choisi %d %d\n", BMU->x, BMU->y);

           if(i < ( (1.0/5.0) * temps ) ){  //phase 1
                    phase = 1;

					if(i < (1.0/5.0) * temps /3 ){ // sous phase 1
                        dataMatrice.nbVoisin = 3;
					}

					if(i >= ( (1.0/5.0) * temps /3 ) && i < 2* ( (1.0/5.0) * temps  /3 ) ){ // sous phase 2
                        dataMatrice.nbVoisin = 2;
					}

					if(i >= 2 * ( ( (1.0/5.0) * temps) /3 ) ){ //sous phase 3
                        dataMatrice.nbVoisin = 1;
					}
				}

				else{ // phase 2
                    phase = 2;
                    alpha = alpha/100;

					if(i < (4/5) * temps /2 ){ //sous phase 1
                        dataMatrice.nbVoisin = 2;
					}

					if(i >= (4/5) * temps /2 ){ //sous phase 2
                        dataMatrice.nbVoisin = 1;
					}
            }

            // apprentissage
            apprentissage(BMU, dataMatrice, alpha, data.tailleVec, dataMatrice.nbVoisin, data.mesDataset[index].vecteur);

            // change le coef alpha
            if( (phase == 1 && alpha > alphaDepart * 0.1) || (phase = 2 && alpha > (alphaDepart/100) * 0.1) ){
                alpha = alphaDepart * (1 - (i/temps));
            }
       }

    }

    supprimerListe(dataListeChaine);

}


/**
 * @brief Searches for the Best Matching Unit (BMU) in a neuron matrix.
 *
 * This function scans through the entire neuron matrix and finds the neuron (or neurons) that have the smallest 
 * Euclidean distance to the input vector. The neuron(s) with the minimum distance are considered the Best 
 * Matching Unit (BMU). The function also collects additional neurons that have the same minimum distance.
 *
 * The search is done in the entire matrix, and a linked list is created containing the coordinates (x, y) of 
 * the neurons that are identified as BMUs. The function returns a structure containing the linked list and the 
 * total number of BMUs found.
 *
 * @param dataMatrice A `ParamMatrice` structure containing the neuron matrix to be searched.
 *
 * @return A `ParamListeChaine` structure containing:
 * - `lesBMU`: A pointer to the linked list of BMU coordinates (x, y).
 * - `tailleListeBMU`: The number of BMUs found in the matrix.
 *
 * @note The function uses the Euclidean distance between the input vector and the neuron vectors in the matrix 
 * to identify the BMU. If multiple neurons share the same minimum distance, they are all considered BMUs and 
 * added to the linked list.
 */
ParamListeChaine rechercheBMU(ParamMatrice dataMatrice){
    int i,j, x, y;
    double distanceMin = dataMatrice.matrice[0][0].distanceEuclidienne;
    Liste * listeChaine;
    int tailleChaine = 0;
    ParamListeChaine dataListeChaine;


    for(i=0; i<dataMatrice.largeur; i++){
        for(j=1; j<dataMatrice.longueur; j++){

            if(dataMatrice.matrice[i][j].distanceEuclidienne < distanceMin){
                distanceMin = dataMatrice.matrice[i][j].distanceEuclidienne;
                x = i;
                y = j;
                tailleChaine = 1;
            }
        }
    }

    //printf("BMU %d %d %f\n",x,y,distanceMin);

    listeChaine = initialisationListe(x,y);

     for(i=0; i<dataMatrice.largeur; i++){
        for(j=0; j<dataMatrice.longueur; j++){

            if(i!= x && j != y && dataMatrice.matrice[i][j].distanceEuclidienne == distanceMin){
                ajouterElement(listeChaine, i, j);
                tailleChaine++;
            }
        }
    }

    dataListeChaine.lesBMU = listeChaine;
    dataListeChaine.tailleListeBMU = tailleChaine;

    return dataListeChaine;
}


/**
 * @brief Initializes a linked list for storing BMU coordinates.
 *
 * This function creates the first element of a linked list that will be used to store the coordinates (x, y) 
 * of Best Matching Units (BMUs). The first element is initialized with the provided values `a` (x-coordinate) 
 * and `b` (y-coordinate), and it is the only element in the list at the time of initialization.
 *
 * @param a The x-coordinate of the first BMU.
 * @param b The y-coordinate of the first BMU.
 *
 * @return A pointer to a `Liste` structure that contains the first element of the list, with the coordinates 
 * (a, b), and the `suivant` pointer set to `NULL`.
 *
 * @note The function creates and returns a new linked list with a single element initialized to the 
 * provided coordinates. This element will be the first in the list, and the `suivant` pointer is set to `NULL`.
 */
Liste * initialisationListe(int a, int b){

    Liste * liste = malloc(sizeof(*liste));
    Element * element = malloc(sizeof(*element));

    element->x = a;
    element->y = b;
    element->suivant = NULL;

    liste->premier = element;

    return liste;
}


/**
 * @brief Adds a new element to the beginning of the linked list.
 *
 * This function creates a new element with the given coordinates (a, b) and adds it to the front of the 
 * linked list. The new element becomes the first element of the list, with its `suivant` pointer 
 * pointing to the previous first element.
 *
 * @param liste A pointer to the linked list where the new element will be added.
 * @param a The x-coordinate of the new element.
 * @param b The y-coordinate of the new element.
 *
 * @note The new element is added to the front of the list. After the operation, the list's first element
 * is the newly added element, and its `suivant` pointer points to the previous first element.
 */
void ajouterElement(Liste * liste, int a, int b){
    Element *nouveau = malloc(sizeof(*nouveau));

    nouveau->x = a;
    nouveau->y = b;
    nouveau->suivant = liste->premier;

    liste->premier = nouveau;
}

/**
 * @brief Frees the memory used by the linked list.
 *
 * This function traverses the linked list and frees the memory allocated for each element. It starts by 
 * freeing the first element and continues until all elements are removed from the list.
 *
 * @param dataListe A structure containing the linked list (`lesBMU`) to be deleted.
 *
 * @note This function assumes that the linked list is not empty. After calling this function, the memory 
 * used by the entire linked list will be deallocated. The list will no longer be accessible.
 */

void supprimerListe(ParamListeChaine dataListe) {
    Element *actuel = dataListe.lesBMU->premier;
    Element *suivant;
    while (actuel != NULL) {
        suivant = actuel->suivant;
        free(actuel);
        actuel = suivant;
    }
}

/**
 * @brief Returns a pointer to a random element from the linked list.
 *
 * This function selects a random element from the provided linked list by generating a random index.
 * It then traverses the list until the corresponding element is found and returns a pointer to that element.
 *
 * @param listeChaine A pointer to the linked list from which the random element will be chosen.
 * @param tailleListe The total number of elements in the linked list.
 *
 * @return A pointer to the randomly chosen element from the list.
 *
 * @note If the list is empty or the size is incorrect, this function may lead to undefined behavior.
 */
Element * randomElementListeBMU(Liste * listeChaine, int tailleListe){
    int i;
    int index = rand()%tailleListe;
    Element * elementActuel = listeChaine->premier;
    Element * BMUchoisi;

    for(i=0; i<=index; i++){
        BMUchoisi = elementActuel;

        if(elementActuel->suivant != NULL){
            elementActuel = elementActuel->suivant;
        }
    }

    return BMUchoisi;
}

/**
 * @brief Performs the learning (weight update) for the BMU (Best Matching Unit) and its neighbors.
 *
 * This function updates the weights of the neurons in the matrix based on the difference between
 * the BMU's vector and the provided input vector. The update is performed for the BMU and its neighbors
 * within a specified radius (defined by `voisin`). The learning rate (`alpha`) is used to control
 * the magnitude of the weight updates.
 *
 * @param BMU A pointer to the BMU (Best Matching Unit) element whose weights will be updated.
 * @param dataMatrice The matrix of neurons to be updated.
 * @param alpha The learning rate, which controls the size of the weight updates.
 * @param tailleVec The dimension of the vector (i.e., the number of elements in each neuron's vector).
 * @param voisin The radius around the BMU within which the neighbors' weights will be updated.
 * @param dataVec The input vector to be used for the learning process.
 *
 * @note The function updates the weights of neurons that are within the specified radius (`voisin`) from the BMU.
 *       If the radius is too large, it may cause more neurons to be affected by the update.
 *       The learning rate (`alpha`) is used to control how much the weights are adjusted during each learning step.
 */
void apprentissage(Element * BMU, ParamMatrice dataMatrice, double alpha, int tailleVec, int voisin, double * dataVec){
    int i,j,k;
    double val = 0.0;


    for(i=0; i<dataMatrice.largeur; i++){
        for(j=0; j<dataMatrice.longueur; j++){

            if(i <= (BMU->x + voisin) && i >= (BMU->x - voisin) ){
                if(j <= (BMU->y + voisin) && j >= (BMU->y - voisin) ){

                    for(k=0; k<tailleVec; k++){
                        val = dataMatrice.matrice[i][j].vecteur[k] + alpha * (dataVec[k] - dataMatrice.matrice[i][j].vecteur[k]);
                        dataMatrice.matrice[i][j].vecteur[k] = val;
                    }
                }

            }

        }
    }

}


/**
 * @brief Assigns labels to the neurons in the matrix based on the closest dataset vector.
 *
 * This function computes the Euclidean distance between each neuron's vector in the matrix and 
 * each dataset vector, and assigns the label of the closest dataset vector to the neuron.
 * This process effectively labels the neurons based on the dataset.
 *
 * @param dataMatrice The matrix of neurons that will be labeled.
 * @param data The dataset containing the vectors and their corresponding labels.
 *
 * @note The function uses the Euclidean distance to find the closest dataset vector for each neuron,
 *       and assigns the label of the closest dataset vector to the neuron.
 */
void putEtiquette(ParamMatrice dataMatrice, paramDataset data){
    int i,j,k;
    double tab[data.tailleTab];
    int indexBMU;

    for(i=0; i<dataMatrice.largeur; i++){
        for(j=0; j<dataMatrice.longueur; j++){
            for(k=0; k<data.tailleTab; k++){
                tab[k] = distanceEuclidienne(dataMatrice.matrice[i][j].vecteur, data.mesDataset[k].vecteur, data.tailleVec);

            }
                indexBMU = indexPetitTab(tab, data.tailleTab);

                dataMatrice.matrice[i][j].etiquette = data.mesDataset[indexBMU].etiquette;
        }
    }

}

/**
 * @brief Finds the index of the smallest value in an array of doubles.
 *
 * This function iterates through the array and identifies the smallest value, 
 * returning the index of that value. The index of the first occurrence of the 
 * smallest value is returned in case of multiple occurrences.
 *
 * @param tab The array of doubles to search through.
 * @param tailleTab The size of the array.
 *
 * @return The index of the smallest value in the array.
 */
int indexPetitTab(double tab[], int tailleTab){
    int i;
    int indexPetit = 0;
    double min = tab[0];

    for(i=1; i<tailleTab; i++){
        if(tab[i]<min){
            indexPetit = i;
            min =tab[i];
        }
    }

    return indexPetit;
}


/**
 * @brief Shortens the labels (etiquettes) in the neuron matrix.
 *
 * This function modifies the labels (etiquettes) in the neuron matrix by
 * replacing full species names with shorter representations:
 * - "Iris-virginica" is replaced with 'a'
 * - "Iris-versicolor" is replaced with 'b'
 * - "Iris-setosa" is replaced with 'c'
 *
 * It uses dynamic memory allocation to adjust the size of the label strings.
 *
 * @param dataMatrice The matrix of neurons whose labels need to be shortened.
 */
void raccourciEtiquette(ParamMatrice dataMatrice){
    int i,j;

    for(i=0; i<dataMatrice.largeur; i++){
        for(j=0; j<dataMatrice.longueur; j++){



            if(equals(dataMatrice.matrice[i][j].etiquette,"Iris-virginica")==0){
                dataMatrice.matrice[i][j].etiquette = realloc(dataMatrice.matrice[i][j].etiquette, sizeof(char)*2);
                dataMatrice.matrice[i][j].etiquette[0] = 'a';
                dataMatrice.matrice[i][j].etiquette[1] = '\0';

            }

            if(equals(dataMatrice.matrice[i][j].etiquette,"Iris-versicolor")==0){
                dataMatrice.matrice[i][j].etiquette = realloc(dataMatrice.matrice[i][j].etiquette, sizeof(char)*2);
                dataMatrice.matrice[i][j].etiquette[0] = 'b';
                dataMatrice.matrice[i][j].etiquette[1] = '\0';
            }

            if(equals(dataMatrice.matrice[i][j].etiquette,"Iris-setosa")==0){
                dataMatrice.matrice[i][j].etiquette = realloc(dataMatrice.matrice[i][j].etiquette, sizeof(char)*2);
                dataMatrice.matrice[i][j].etiquette[0] = 'c';
                dataMatrice.matrice[i][j].etiquette[1] = '\0';
            }

        }

    }

    printf("a = virginica  b = versicolor  c = setosa\n\n");
}


/**
 * @brief Compares two strings and checks if they are equal.
 *
 * This function compares two strings character by character and returns:
 * - 0 if the strings are equal.
 * - 1 if the strings are not equal.
 *
 * The comparison stops as soon as a difference is found or when the end of either string is reached.
 *
 * @param mot1 The first string to compare.
 * @param mot2 The second string to compare.
 * 
 * @return 0 if the strings are equal, 1 if they are not equal.
 */
int equals(char* mot1, char* mot2){
    int result = 0;
    int i = 0;

    while( mot1[i]!='\0' && mot2[i]!='\0' && result == 0){

        if(mot1[i]!=mot2[i]){
            result = 1;
        }
        i++;
    }

    return result;
}


/**
 * @brief Displays the labels of a neural network matrix.
 *
 * This function iterates over the neural network matrix and prints the label (`etiquette`) of each neuron.
 * The labels are displayed row by row, separated by a delimiter (`|`), and a separator line is printed
 * after each row for readability.
 *
 * @param dataMatrice The matrix containing the neurons with their respective labels.
 */
void afficheEtiquette(ParamMatrice dataMatrice){

 int i,j;

    for(i=0; i<dataMatrice.largeur; i++){
        for(j=0; j<dataMatrice.longueur; j++){
                printf("%s | ", dataMatrice.matrice[i][j].etiquette);
            }
            printf("\n---------------------------------------\n");
        }
}


/**
 * @brief Frees all dynamically allocated memory for the dataset and neural network matrix.
 *
 * This function is responsible for releasing all dynamically allocated memory used by the dataset and
 * the neural network matrix. It frees the memory for the mixed indices array (`tabMelanger`), the mean vector (`vecteurMoyen`),
 * as well as the vectors and labels for each dataset element. It also frees the matrix of neurons and their vectors.
 * The function ensures that all memory is properly freed to avoid memory leaks.
 *
 * @param data The dataset containing all the data structures that need to be freed.
 * @param dataMatrice The matrix of neurons, including vectors and labels, that needs to be freed.
 */
void freeAll(paramDataset data, ParamMatrice dataMatrice){
    int i,j;

    free(data.tabMelanger);
    free(data.vecteurMoyen);

// free dataset
    for(i=0; i<data.tailleTab; i++){

        free(data.mesDataset[i].vecteur);
        free(data.mesDataset[i].etiquette);
    }

    free(data.mesDataset);

// free matrice
   for(i=0; i<dataMatrice.largeur; i++){
        for(j=0; j<dataMatrice.longueur; j++){
               //free(dataMatrice.matrice[i][j].etiquette);
               free(dataMatrice.matrice[i][j].vecteur);
        }
        free(dataMatrice.matrice[i]);
    }

    free(dataMatrice.matrice);
}





/*------------------------EXECUTION------------------------*/
int main(){
    paramDataset data;
    ParamMatrice dataMatrice;

    data = traitementFichier("iris.data", ",");
    NormaliserVecteur(data.mesDataset, data.tailleVec, data.tailleTab);
    vecteurMoyen(data.mesDataset, data.tailleVec, data.tailleTab, data.vecteurMoyen);
    //afficherData(data.tailleTab,data.tailleVec,data.mesDataset);

    neurone* mesNeurone = genereVecteurDouble(5*sqrt(data.tailleTab), data.tailleVec, 0.3, 0.3, data.vecteurMoyen);
    //afficherVecteurNeurone(5*sqrt(data.tailleTab) - 5*sqrt(data.tailleTab)/10 ,data.tailleVec,mesNeurone);

    dataMatrice = genererMatriceNeurone(dataMatrice, mesNeurone, 5*sqrt(data.tailleTab));
    //afficherMatriceNeurone(dataMatrice, data.tailleVec);


    data.tabMelanger = indiceMelange(data.mesDataset, data.tailleTab,data.tailleVec);


    rapprochement(data,dataMatrice, 0.7, 500);
    //printf("\n\n");
    //afficherMatriceNeurone(dataMatrice, data.tailleVec);


    putEtiquette(dataMatrice, data);
    raccourciEtiquette(dataMatrice);
    afficheEtiquette(dataMatrice);

    freeAll(data,dataMatrice);

    return 0;
}
