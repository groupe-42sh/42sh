#ifndef HASHMAP_H
#define HASHMAP_H

struct keyval {
    char* key;
    char *val;
    struct keyval* next;
};

struct ht {
    struct keyval** table;
    unsigned size;
};

void ht_print(FILE* out, struct ht* hashtable);
struct keyval* ht_find_or_put(char* word, char *value, struct ht* hashtable,
    unsigned (*hash)(char*));
struct ht* ht_malloc(unsigned size);
void ht_free(struct ht* table);
unsigned fvn_hash(char* word);


#endif
