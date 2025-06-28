#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 50
#define MAX_STRING 1024

// For sake of simplicity let's suppose that every struct we want to use in our...
// ... table has field called "name" which will always be used for hashing.

// These are the itens we will save in our hash table:
typedef struct GenericItem
{
    // name is going to be the hash camp
    char key_str[MAX_STRING];
    void *item;
    unsigned int bytes_num;
} generic_item;

// Each box will represent a hash number, so all the colidade itens will be stored in the same box:
typedef struct Box
{
    // number of movies located in that box
    unsigned int num;
    // we will use a (* void) because we want to do a generalistic hash table
    generic_item *vector;
} box;

// Each set of boxes makes a hash table with the additional information about the number of boxes:
typedef struct HashTable
{
    unsigned int size;
    box *set;
} hash_table;

// Create hash table:
hash_table *createTable(unsigned int size);

// Hash function:
unsigned int hash(char *key_str, unsigned int table_size);

// Create a generic item for inserting in the hash table:
generic_item *createGenericItem(void *content, unsigned int bytes_num, char *key_str);

// Insert in hash table:
void insertHashTable(hash_table *T, void *content, char *key_str, unsigned int bytes_size);

// Delete from hash table:
bool deleteHashTable(hash_table *T, char *key_str);

// Look-up in for item in the hash table:
void *lookUpHashTable(hash_table *T, char *key_str);

// Print hash table:
void printHashTable(hash_table *T);
