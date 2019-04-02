#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct keyval {
    char* key;
    unsigned val;
    struct keyval* next;
};

struct ht {
    struct keyval** table;
    unsigned size;
};

struct ht* ht_malloc(unsigned size)
{
    struct ht *hash_table = malloc(sizeof(struct ht));

    hash_table->table = calloc(sizeof(struct keyval*), size);

    hash_table->size = size;

    return hash_table;
}


void ht_free(struct ht* table)
{
    if (!table)
        return;

    unsigned i = 0;

    for (i = 0; i < table->size; ++i)
    {
        free(table->table[i]);
    }
    free(table->table);
    free(table);
}

struct keyval* ht_find_or_put(char* word, unsigned value,
        struct ht* hashtable,
        unsigned (*hash)(char*))
{
    if (!hashtable)
        return NULL;

    unsigned bucket = hash(word)%hashtable->size;

    struct keyval *current = hashtable->table[bucket];
    struct keyval *previous = NULL;

    if (!current)
    {
        hashtable->table[bucket] = malloc (sizeof(struct keyval));
        hashtable->table[bucket]->val = value;
        hashtable->table[bucket]->key = word;
        hashtable->table[bucket]->next = NULL;
        previous = hashtable->table[bucket];
        return previous;
    }

    while (current)
    {
        if (memcmp(current->key, word, 24) == 0)
        {
            return NULL;
        }
        previous = current;
        current = current->next;
    }

    struct keyval *new = malloc (sizeof(struct keyval));
    new->val = value;
    new->next = NULL;
    new->key = word;

    previous->next = new;

    return new;
}

unsigned hash_assignement(char *value)
{
    unsigned *valuei = (unsigned *)value;
    size_t i = 0;

    for (i = 0; valuei[i] != '\0'; ++i)
    {
    
    }
}

void
cube_func()
{
    int ct = 0;
    int level = 0;

    cubetype cube = cube_alloc();
    cube_init_solved(cube);

    struct ht* table = ht_malloc(10000000);
    ht_find_or_put(cube, 1, table, hash_cube);

    struct clist *current = malloc(sizeof(struct clist));
    current->cube = cube;
    current->next = NULL;

    struct clist *next = NULL;

    while (current != NULL)
    {
        printf("level : %d - size : %d\n", level, ct);

        ct = 0;
        
        for (; current != NULL; current = current->next)
        {
            
            for (int rot = 0; rot < 6; ++rot)
            {
                cubetype cuberot = cube_rotate(current->cube, rot);

                if (ht_find_or_put(cuberot, 1, table, hash_cube))
                {
                    struct clist *new = malloc(sizeof(struct clist));
                    new->cube = cuberot;
                    new->next = next;
                    next = new;
                }
            }

            ++ct;
        }
        current = next;
        next = NULL;
        ++level;
    }

    ht_free(table);
}
