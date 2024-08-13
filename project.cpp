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
