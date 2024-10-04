#ifndef HASHMAP_H
#define HASHMAP_H
#include "utils.h"
#include <stdbool.h>
#define TABLE_SIZE 10

// Structure for a symptom
typedef struct Symptom
{
    int id;
    char name[50];
    struct Symptom *next;
    struct Symptom *related_symptom;
} Symptom;

// Structure for a category
typedef struct Category
{
    int id;
    char name[50];
    Symptom *symptoms;
    struct Category *next;
} Category;

// External variable declaration
extern Category *hash_table[TABLE_SIZE];

// Function to display all categories
void display_categories();

// Function to check if hash table is empty or not
bool is_hash_table_empty();

// Function to insert a category into the hash table
void insert_category(int id, char *name);

// Function to parse JSON and populate the hash table
void parse_JSON(char *filename, User *user);

// Function to display symptoms for a selected category
void display_symptoms_for_category(int category_id);

// Function to display selected symptoms
void display_selected_symptoms(Symptom *selected_symptoms);

// Function to add symptoms to a category
void add_symptoms_to_category(int category_id, Symptom *symptoms);

// Function to display related symptoms for a selected symptom
Symptom *display_related_symptoms(int symptom_id, int category_id);

#endif
