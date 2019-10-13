//
//  spell.c
//  appsec_spellcheck
//
//  Created by Danny Smith on 9/22/19.
//  Copyright © 2019 Danny Smith. All rights reserved.
//

#include "dictionary.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int check_words(FILE *fp, hashmap_t hashtable[], char *misspelled[]) {
    if (fp == NULL){
        return 0;
    }
    for (int i = 0 ; i < MAX_MISSPELLED; i++){
        misspelled[i] = NULL;
    }
    int length = 0;
    int num_misspelled = 0;
    char c, word[LENGTH+1];
    while ((c = fgetc(fp)) != EOF){
        if(c == ' ' || c == '\n'){
            while(isalpha(word[length-1]) == 0){
                word[length-1] = '\0';
                length--;
            }
            word[length] = '\0';
	    if((length > 0) && (length < LENGTH+1)){
                if(check_word(word, hashtable) == false){
                    misspelled[num_misspelled] = malloc(sizeof(char*) * length);
                    strcpy(misspelled[num_misspelled], word);
                    num_misspelled++;
                    }
            length = 0;
                }
	    }
        else if(length>LENGTH){
            length = 0;
            continue;
        }
        else{
            if(length < 1){
                if(isalpha(c) == 0)
                continue;
            }
            word[length] = c;
            length++;
            }
        }
    fclose(fp);
    return num_misspelled;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
    int length = strlen(word);
    if(length > LENGTH)
	    return false;
    int bucket = hash_function(word);
    node *cursor = hashtable[bucket];
    while(cursor != NULL){
        if(strcmp(cursor->word, word) == 0){
            return true;
        }
        cursor = cursor->next;
        }
    unsigned long word_length = strlen(word);
    char lower_word[LENGTH+1];
    strcpy(lower_word, word);
    for(int i=0; i<word_length; i++)
        lower_word[i] = tolower(word[i]);
    bucket = hash_function(lower_word);
    cursor = hashtable[bucket];
    while(cursor != NULL){
        if(strcmp(cursor->word, lower_word) == 0){
            return true;
        }
        cursor = cursor->next;
        }
    return false;
    }

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    for(int i=0; i<HASH_SIZE; i++){
        hashtable[i] = NULL;
    }
    FILE *fptr;
    int length = 0;
    fptr = fopen(dictionary_file, "r");
    if (fptr == NULL){
        return false;
    }
    char c, word[LENGTH+1];
    while ((c = fgetc(fptr)) != EOF) {
        if(c == ' ' || c == '\n'){
            if ( length == 0 ){
                length = 0;
                continue;
            }
            word[length] = '\0';
            int bucket = hash_function(word);
            node *temp = hashtable[bucket];
            if ( temp == NULL){
                node* new_node = (node*)malloc(sizeof(node));
                new_node->next = NULL;
                strcpy(new_node->word, word);
                temp = new_node;
                hashtable[bucket] = temp;
                length = 0;
                }
            else{
                node* new_node = (node*)malloc(sizeof(node));
                new_node->next = hashtable[bucket];
                strcpy(new_node->word, word);
                hashtable[bucket] = new_node;
                }
                length = 0;
            }
        else if(length>LENGTH){
            length = 0;
            continue;
            }
        else{word[length] = c;
            length++;
            }
        }
    fclose (fptr);
    return false;
}
