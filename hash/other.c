#include <stdio.h>
#include <string.h>

// Biblioteca de hash genérica para elementos de tipo genérico

// Estrututuras: 

typedef struct Linked linked;
typedef struct Table table;

struct Linked{
    void *element;
    unsigned int type_size;
    linked *next;
    unsigned int length;
};

struct Table{
    linked *slot;
    unsigned int type_size;
    int(*function_pointer)(void *element);
    unsigned int length;
};

typedef enum {
    STRING,
    CHAR,
    INT,
    FLOAT,
    DOUBLE
} TYPE;


// Funções:

linked *createLinkedList(unsigned int type_size);
unsigned int insertLinkedList(linked *my_list, void *content);
unsigned int removeLinkedList(linked *my_list, void *content, unsigned int op);
linked *getFromLinkedList(linked *my_list, unsigned int position);
unsigned int findInLinkedList(linked *my_list, void *content);
void printLinkedList(linked *my_list, unsigned int(*function_pointer)(void *element));

table *createHashTable(unsigned int type_size, int(*function_pointer)(void *element));
unsigned int *insertHashtable(table *my_table, void *content);
unsigned int *removeHashTable(table *my_table, void *content);
unsigned int findInTable(table *my_table, void *content);

linked *createLinkedList(unsigned int type_size){
    linked *created;
    created = malloc(sizeof(linked));
    created->element = NULL;
    created->type_size = type_size;
    created->next = NULL;
    created->length = 0;
    return created;
}

unsigned int insertLinkedList(linked *my_list, void *content){
    if(my_list == NULL){
        return 1;
    }
    else if(my_list->element == NULL){
        my_list->element = malloc(sizeof(char) * my_list->type_size);
        memcpy(my_list->element, content, my_list->type_size);
    }
    else {
        linked *aux_pointer = my_list->next;
        while(aux_pointer != NULL){
            aux_pointer = my_list->next;
        }
        aux_pointer->element = malloc(sizeof(char) * my_list->type_size);
        memcpy(my_list->element, content, my_list->type_size);
    }
    return 0;
}

unsigned int removeLinkedList(linked *my_list, void *content, unsigned int op){
    if(op == 1){
        int n = *(int *)content;
        linked *current;
        current = my_list;
        linked *predecessor;
        predecessor = NULL;
        for (int i = 0; i < n && current != NULL; i++){
               predecessor = current;
               current = current->next;
        }
        if(current == NULL){
            return 1;
        }
        else{
            if(predecessor != NULL){
                predecessor->next = current->next;
            }
            else{
                my_list = NULL;
            }
            free(current);
        }
    }
    else{
        linked *predecessor;
        linked *current;
        current = my_list;
        predecessor = NULL;
        while (current != NULL && current != (linked *)content)
        {
            current = current->next;
        }
        if(current != NULL){
            if(predecessor != NULL){
                predecessor->next = current->next;
            }
            else{
                my_list = NULL;
            }
            free(current);
        }
    }
    return 0;
}

table *createHashTable(unsigned int type_size, int(*function_pointer)(void *element)){
    table *created;
    created = malloc(sizeof(table));
    created->slot = NULL;
    created->type_size = type_size;
    created->function_pointer = function_pointer;
    created->length = 0;
    return created;
}

int main(void){

}