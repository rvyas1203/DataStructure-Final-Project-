/*
* FILE       : project.cpp
* PROJECT    : Data Structure Final Project
* PROGRAMMER : Raj Mahesh Vyas - 8908964
* FIRST VERSION : 08-12-2024
* DESCRIPTION :
 *    This program implements a parcel management system using a hash table
 *    and binary search tree (BST). It allows storing, searching, and managing
 *    parcel data based on destination, weight, and valuation.
 *    The system supports operations such as inserting parcels, searching by country,
 *    calculating total weight and valuation, and finding the lightest/heaviest and
 *    cheapest/most expensive parcels.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 127

 // Define a structure for each parcel
typedef struct Parcel {
    char* destination;
    int weight;
    float valuation;
    struct Parcel* left;
    struct Parcel* right;
} Parcel;

// Define a structure for the hash table
typedef struct HashTable {
    Parcel* root; // Root of the BST for each hash table entry
} HashTable;

// Function prototypes
unsigned long hash(unsigned char* str);
Parcel* createParcel(const char* destination, int weight, float valuation);
Parcel* insert(Parcel* root, const char* destination, int weight, float valuation);
void printAllParcels(Parcel* root);
void freeParcels(Parcel* root);
void initHashTable(HashTable table[]);
void insertIntoHashTable(HashTable table[], const char* destination, int weight, float valuation);
void searchByCountry(HashTable table[], const char* country);
void searchByCountryAndWeight(HashTable table[], const char* country, int weight, int higher);
void calculateTotal(HashTable table[], const char* country);
void findCheapestAndMostExpensive(HashTable table[], const char* country);
void findLightestAndHeaviest(HashTable table[], const char* country);

// Hash function (djb2)
/*
 * Function: hash
 * ----------------------------
 *   Computes the hash value of a given string.
 *
 *   str: The input string to be hashed.
 *
 *   returns: The computed hash value as an unsigned long, modulo TABLE_SIZE.
 */

unsigned long hash(unsigned char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % TABLE_SIZE;
}

// Create a new parcel node
/*
 * Function: createParcel
 * ----------------------------
 *   Dynamically allocates and initializes a new parcel node.
 *
 *   destination: The destination country of the parcel.
 *   weight: The weight of the parcel.
 *   valuation: The monetary valuation of the parcel.
 *
 *   returns: A pointer to the newly created Parcel struct.
 */

Parcel* createParcel(const char* destination, int weight, float valuation) {
    // Allocate memory for a new Parcel struct
    Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel));

    // Duplicate the destination string to store in the Parcel
    newParcel->destination = _strdup(destination);

    // Initialize the Parcel fields
    newParcel->weight = weight;
    newParcel->valuation = valuation;
    newParcel->left = newParcel->right = NULL;  // Initialize child pointers

    return newParcel;  // Return the created Parcel node
}

// Insert parcel into BST
/*
 * Function: insert
 * ----------------------------
 *   Inserts a parcel into a binary search tree (BST) based on its weight.
 *
 *   root: The root of the BST.
 *   destination: The destination country of the parcel.
 *   weight: The weight of the parcel.
 *   valuation: The monetary valuation of the parcel.
 *
 *   returns: The root of the BST after insertion.
 */
Parcel* insert(Parcel* root, const char* destination, int weight, float valuation) {
    // Base case: If the tree is empty, create a new Parcel node
    if (root == NULL) return createParcel(destination, weight, valuation);

    // Recursively insert the parcel in the left or right subtree based on weight
    if (weight < root->weight)
        root->left = insert(root->left, destination, weight, valuation);
    else
        root->right = insert(root->right, destination, weight, valuation);

    return root;  // Return the root of the tree after insertion
}


// Print all parcels in BST using in-order traversal
/*
 * Function: printAllParcels
 * ----------------------------
 *   Recursively prints all parcels in a binary search tree (BST) in ascending order of weight.
 *
 *   root: The root of the BST.
 */
void printAllParcels(Parcel* root) {
    if (root != NULL) {
        // Traverse the left subtree
        printAllParcels(root->left);

        // Print the current parcel
        printf("Destination: %s, Weight: %d, Valuation: %.2f\n", root->destination, root->weight, root->valuation);

        // Traverse the right subtree
        printAllParcels(root->right);
    }
}


// Traverse the tree to calculate total weight and valuation
/*
 * Function: traverseAndCalculate
 * ----------------------------
 *   Recursively traverses a binary search tree (BST) to calculate the total weight and valuation of parcels.
 *
 *   root: The root of the BST.
 *   totalWeight: Pointer to the total weight accumulator.
 *   totalValuation: Pointer to the total valuation accumulator.
 */
void traverseAndCalculate(Parcel* root, int* totalWeight, float* totalValuation) {
    if (root != NULL) {
        // Traverse the left subtree and update accumulators
        traverseAndCalculate(root->left, totalWeight, totalValuation);

        // Accumulate weight and valuation of the current parcel
        *totalWeight += root->weight;
        *totalValuation += root->valuation;

        // Traverse the right subtree and update accumulators
        traverseAndCalculate(root->right, totalWeight, totalValuation);
    }
}

// Calculate total weight and valuation for a country
/*
 * Function: calculateTotal
 * ----------------------------
 *   Calculates and displays the total weight and valuation of all parcels for a given country.
 *
 *   table: The hash table containing parcel data.
 *   country: The country name for which to calculate the totals.
 */
void calculateTotal(HashTable table[], const char* country) {
    unsigned long index = hash((unsigned char*)country);  // Get the hash index for the country
    Parcel* root = table[index].root;  // Get the root of the BST for the country

    // Check if there are parcels for the given country
    if (root == NULL) {
        printf("No parcels found for this country.\n");
        return;
    }

    int totalWeight = 0;  // Initialize total weight accumulator
    float totalValuation = 0.0;  // Initialize total valuation accumulator

    // Traverse the BST to calculate total weight and valuation
    traverseAndCalculate(root, &totalWeight, &totalValuation);

    // Display the results
    printf("Total Weight: %d, Total Valuation: %.2f\n", totalWeight, totalValuation);
}


// Traverse the tree to find cheapest and most expensive parcels
/*
 * Function: traverseAndFindCheapestAndMostExpensive
 * ----------------------------
 *   Recursively traverses a binary search tree (BST) to find the cheapest and most expensive parcels.
 *
 *   root: The root of the BST.
 *   cheapest: Pointer to the parcel with the lowest valuation.
 *   mostExpensive: Pointer to the parcel with the highest valuation.
 */
void traverseAndFindCheapestAndMostExpensive(Parcel* root, Parcel** cheapest, Parcel** mostExpensive) {
    if (root != NULL) {
        // Traverse the left subtree
        traverseAndFindCheapestAndMostExpensive(root->left, cheapest, mostExpensive);

        // Update the cheapest and most expensive parcels if necessary
        if (*cheapest == NULL || root->valuation < (*cheapest)->valuation) {
            *cheapest = root;
        }
        if (*mostExpensive == NULL || root->valuation > (*mostExpensive)->valuation) {
            *mostExpensive = root;
        }

        // Traverse the right subtree
        traverseAndFindCheapestAndMostExpensive(root->right, cheapest, mostExpensive);
    }
}


// Find the cheapest and most expensive parcels for a country
/*
 * Function: findCheapestAndMostExpensive
 * ----------------------------
 *   Finds and displays the cheapest and most expensive parcels for a given country.
 *
 *   table: The hash table containing parcel data.
 *   country: The country name for which to find the parcels.
 */
void findCheapestAndMostExpensive(HashTable table[], const char* country) {
    unsigned long index = hash((unsigned char*)country);  // Get the hash index for the country
    Parcel* root = table[index].root;  // Get the root of the BST for the country

    // Check if there are parcels for the given country
    if (root == NULL) {
        printf("No parcels found for this country.\n");
        return;
    }


    Parcel* cheapest = NULL;
    Parcel* mostExpensive = NULL;

    traverseAndFindCheapestAndMostExpensive(root, &cheapest, &mostExpensive);

    if (cheapest && mostExpensive) {
        printf("Cheapest Parcel: Destination: %s, Weight: %d, Valuation: %.2f\n", cheapest->destination, cheapest->weight, cheapest->valuation);
        printf("Most Expensive Parcel: Destination: %s, Weight: %d, Valuation: %.2f\n", mostExpensive->destination, mostExpensive->weight, mostExpensive->valuation);
    }
    else {
        printf("No parcels found for this country.\n");
    }
}

// Traverse the tree to find lightest and heaviest parcels
/*
 * Function: traverseAndFindLightestAndHeaviest
 * ----------------------------
 *   Recursively traverses a binary search tree (BST) to find the lightest and heaviest parcels.
 *
 *   root: The root of the BST.
 *   lightest: Pointer to the parcel with the lowest weight.
 *   heaviest: Pointer to the parcel with the highest weight.
 */
void traverseAndFindLightestAndHeaviest(Parcel* root, Parcel** lightest, Parcel** heaviest) {
    if (root != NULL) {
        // Traverse the left subtree
        traverseAndFindLightestAndHeaviest(root->left, lightest, heaviest);

        // Update the lightest and heaviest parcels if necessary
        if (*lightest == NULL || root->weight < (*lightest)->weight) {
            *lightest = root;
        }
        if (*heaviest == NULL || root->weight > (*heaviest)->weight) {
            *heaviest = root;
        }

        // Traverse the right subtree
        traverseAndFindLightestAndHeaviest(root->right, lightest, heaviest);
    }
}


// Find the lightest and heaviest parcels for a country
/*
 * Function: findLightestAndHeaviest
 * ----------------------------
 *   Finds and displays the lightest and heaviest parcels for a given country.
 *
 *   table: The hash table containing parcel data.
 *   country: The country name for which to find the parcels.
 */
void findLightestAndHeaviest(HashTable table[], const char* country) {
    unsigned long index = hash((unsigned char*)country);  // Get the hash index for the country
    Parcel* root = table[index].root;  // Get the root of the BST for the country

    // Check if there are parcels for the given country
    if (root == NULL) {
        printf("No parcels found for this country.\n");
        return;
    }

    Parcel* lightest = NULL;  // Initialize pointer for the lightest parcel
    Parcel* heaviest = NULL;  // Initialize pointer for the heaviest parcel

    // Traverse the BST to find the lightest and heaviest parcels
    traverseAndFindLightestAndHeaviest(root, &lightest, &heaviest);

    // Display the results
    if (lightest != NULL) {
        printf("Lightest Parcel: Destination: %s, Weight: %d, Valuation: %.2f\n",
            lightest->destination, lightest->weight, lightest->valuation);
    }
    if (heaviest != NULL) {
        printf("Heaviest Parcel: Destination: %s, Weight: %d, Valuation: %.2f\n",
            heaviest->destination, heaviest->weight, heaviest->valuation);
    }
}

