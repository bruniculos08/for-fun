#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 50
#define MAX_STRING 1024

// For sake of simplicity let's suppose that every struct we want to use in our...
// ... table has field called "name" which will always be used for hashing.

// These are the itens we will save in our hash table:
typedef struct Movie
{
    // name is going to be the hash camp
    char name[MAX_STRING];
    unsigned int year;
    float score;
} movie;

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
hash_table *createTable(unsigned int size){
    hash_table *T;
    T = malloc(sizeof(hash_table));
    (*T).size = size;
    (*T).set = malloc(sizeof(box) * size);
    for(int i = 0; i < size; i++){
        (*T).set[i].num = 0;
        (*T).set[i].vector = NULL;
    } 
    return T;
}

// Hash function:
unsigned int hash(char *key_str, unsigned int table_size){
    unsigned int hash_value = 0;
    int lenght = strlen(key_str);
    for (int i = 0; i < lenght; i++)
    {
        hash_value += key_str[i];
        hash_value = (hash_value * key_str[i]) % table_size;
        i++;
    }
    return hash_value;
}

// Create a generic item for inserting in the hash table:
generic_item *createGenericItem(void *content, unsigned int bytes_num, char *key_str){
    generic_item *stuff;
    stuff = malloc(sizeof(generic_item));

    // There is no need no initiate "(*stuff).key_str" because he is declared as char[MAX_STRING].
    strcpy((*stuff).key_str, key_str);
    (*stuff).bytes_num = bytes_num;

    // Remember (attention!): it is necessary to initiate "(*stuff).item" before using memcpy in it...
    // ... (I'm just noting this because I took a time to find this mistake and I think it's very common). 
    (*stuff).item = malloc(bytes_num);
    memcpy((*stuff).item, content, bytes_num);

    return stuff;
}

// Insert in hash table:
void insertHashTable(hash_table *T, void *content, char *key_str, unsigned int bytes_size){
    generic_item *item;
    item = createGenericItem(content, bytes_size, key_str);

    unsigned int hash_value = hash(key_str, (*T).size);
    if((*T).set[hash_value].vector == NULL){
        // Aloccate one generic item in the box:
        (*T).set[hash_value].vector = malloc(sizeof(generic_item));
        // Now the number of (generic) items in the box will be one:
        (*T).set[hash_value].num = 1;
        // Copy the first (generic) item in the address of the generic item:
        memcpy(&(*T).set[hash_value].vector[0], item, sizeof(generic_item));
    }
    else{
        // Aloccate one new generic item in the box:
        (*T).set[hash_value].vector = realloc((*T).set[hash_value].vector, sizeof(generic_item) * ((*T).set[hash_value].num + 1));
        // Now the number of (generic) items in the box will increase by one:
        (*T).set[hash_value].num ++;
        // Copy the new (generic) item in the address of the new generic item:
        int p = (*T).set[hash_value].num - 1;
        memcpy(&(*T).set[hash_value].vector[p], item, sizeof(generic_item));
    }

}

// Delete from hash table:
bool deleteHashTable(hash_table *T, char *key_str){
    unsigned int hash_value = hash(key_str, (*T).size);
    if((*T).set[hash_value].vector == NULL) return false;
    else{
        for (int i = 0; i < (*T).set[hash_value].num; i++)
        {
            // If we find the item with the same "key_str" we are looking for then...
            // ... we must change it in place with the last item and then decrease...
            // ... the considered size of that box (and free all the pointers):
            if(strcmp((*T).set[hash_value].vector[i].key_str, key_str) == 0){

                // It is ok to free a (void *) because the malloc lib keeps track of how many...
                // ... bytes were allocated in that address:
                free((*T).set[hash_value].vector[i].item);

                unsigned int last_position = (*T).set[hash_value].num - 1;
                
                memcpy(&(*T).set[hash_value].vector[i], &(*T).set[hash_value].vector[last_position], sizeof(generic_item));

                (*T).set[hash_value].vector = realloc((*T).set[hash_value].vector, sizeof(generic_item) * last_position);
                (*T).set[hash_value].num--;

                if((*T).set[hash_value].num == 0){
                    free((*T).set[hash_value].vector);
                    (*T).set[hash_value].vector = NULL;   
                }

                return true;
            }    
        }
    }
    return false;
}

// Look-up in for item in the hash table:
void *lookUpHashTable(hash_table *T, char *key_str){
    unsigned int hash_value = hash(key_str, (*T).size);
    if((*T).set[hash_value].vector == NULL){
        return NULL;
    }
    else for (int i = 0; i < (*T).set[hash_value].num; i++) {
        if(strcmp((*T).set[hash_value].vector[i].key_str, key_str) == 0)
            return (*T).set[hash_value].vector[i].item;
    }
    return NULL;
}

// Print hash table:
void printHashTable(hash_table *T){
    printf("\n---- Start Table: ----\n");
    for (int i = 0; i < (*T).size; i++)
    {
        if((*T).set[i].vector == NULL){
            printf(" %i \t --- \n", i);
        }
        else{
            printf(" %i ", i);
            for (int j = 0; j < (*T).set[i].num; j++)
            {
                printf("\t %s |", (*T).set[i].vector[j].key_str);
            }
            printf("\n");
        }
    }   
    printf("---- End Table. ----\n\n");
}

int main(void){
    hash_table *T;
    T = createTable(100);

    movie empirestrikes = {.name = "Star Wars: Episode V - The Empire Strikes Back", .score = 8.7, .year = 1980};
    insertHashTable(T, (void *) &empirestrikes, empirestrikes.name, sizeof(movie));
    insertHashTable(T, (void *) &empirestrikes, empirestrikes.name, sizeof(movie));

    movie shawshank = {.name = "The Shawshank Redemption", .score = 9.3, .year = 1994};
    insertHashTable(T, (void *) &shawshank, shawshank.name, sizeof(movie));

    movie godfather = {.name = "The Godfather", .score = 9.2, .year = 1972};
    insertHashTable(T, (void *) &godfather, godfather.name, sizeof(movie));

    movie darkknight = {.name = "The Dark Knight", .score = 9.0, .year = 2008};
    insertHashTable(T, (void *) &darkknight, darkknight.name, sizeof(movie));

    printHashTable(T);

    printf("Searched pointer: %p\n\n", lookUpHashTable(T,empirestrikes.name));

    deleteHashTable(T, empirestrikes.name);
    printHashTable(T);

    printf("Searched pointer: %p\n\n", lookUpHashTable(T,empirestrikes.name));

    deleteHashTable(T, empirestrikes.name);
    printf("Searched pointer: %p\n\n", lookUpHashTable(T,empirestrikes.name));
}