#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

void ht_print(FILE* out, struct ht* hashtable)
{
    unsigned i = 0;
    unsigned size = hashtable->size;

    for (i = 0; i < size; ++i)
    {
        if (hashtable->table[i])
        {
            struct keyval *current = hashtable->table[i];
            fprintf(out, "Bucket[%d]:\t", i);

            while (current)
            {
                fprintf(out, "(%s,%s)", current->key, current->val);

                if (current->next)
                {
                    fprintf(out, " -> ");
                }
               else
                {
                    fprintf(out, "\n");
                }
                current = current->next;
            }
        }
    }
}

struct keyval* ht_find_or_put(char* word, char *value,
        struct ht* hashtable,
        unsigned (*hash)(char*))
{
    if (!hashtable)
    {
        return NULL;
    }
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


struct ht* ht_malloc(unsigned size)
{
    struct ht *hash_table = malloc(sizeof(struct ht));

    hash_table->table = calloc(sizeof(struct keyval*), size);

    hash_table->size = size;

    return hash_table;
}


//struct ht* compute_histogram(char** words, unsigned words_size,
//        unsigned ht_size, unsigned (*hash)(char*))
//{
//    int count = 0;
//    struct ht *hash_table = ht_malloc(ht_size);
//
//    for (unsigned i = 0; i < words_size; ++i)
//    {
//        for (unsigned j = 0; j < words_size; ++j)
//        {
//            if (strcmp(words[i], words[j]) == 0)
//                ++count;
//        }
//
//        ht_find_or_put(words[i], count, hash_table, hash);
//        count = 0;
//    }
//    return hash_table;
//}

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

unsigned nb_collisions(struct ht* hashtable)
{
    unsigned i = 0;
    unsigned count = 0;
    struct keyval *current = NULL;
    unsigned max = 0;
    for (i = 0; i < hashtable->size; ++i)
    {
        current = hashtable->table[i];
        while (current)
        {
            ++count;
            current = current->next;
        }
        max = count > max ? count : max;
        count = 0; 
    }

    return max;
}

unsigned fvn_hash(char* word)
{
    unsigned count = 2166136261;
    unsigned i = 0;

    for (i = 0; word[i] != '\0'; ++i)
    {
        count *= 16777619;
        count ^= word[i];
    }

    return count;
}

unsigned djb_hash(char* word)
{
    unsigned i = 0;
    unsigned count = 0;

    for (i = 0; word[i] != '\0'; ++i)
    {
        int tmp = count * 33;
        count = tmp + word[i];
    }
    return count;

}

unsigned xor_add_hash(char* word)
{
    unsigned i = 0;
    unsigned count = 0;

    for (i = 0; word[i] != '\0'; ++i)
    {
        int tmp = count ^ word[i];
        count = tmp + word[i];
    }
    return count;

}

unsigned xor_hash(char* word)
{
    unsigned i = 0;
    unsigned count = 0;
    for (i = 0; word[i] != '\0'; ++i)
    {
        count ^= word[i];
    }
    return count;

}

unsigned bad_hash(char* word)
{
    unsigned i = 0;
    unsigned count = 0;
    for (i = 0; word[i] != '\0'; ++i)
    {
        count += word[i];
    }
    return count;
}


void display_words(FILE* out, unsigned size, char** words)
{
    unsigned i = 0;

    for (i = 0; i < size; ++i)
    {
        fprintf(out, "%s\n", words[i]);
    }
}

int find_empty_bucket(struct ht* hashtable)
{
    unsigned i = 0;
    unsigned count = 0;
    struct keyval *current = NULL;
    unsigned found = 0;

    for (i = 0; i < hashtable->size; ++i)
    {
        current = hashtable->table[i];
        while (current)
        {
            if (count > 0)
            {
                found = 1;
                break;
            }
            ++count;
            current = current->next;
        }
        if (found)
            break;
    }

    for (i = 0; i < hashtable->size; ++i)
    {
        if (hashtable->table[i] == NULL)
            return i;
    }

    return -1;
}
