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


int main(){
    bool check = false;
    char *file_path = "/home/appsec/NYUAPPSEC/spell_checker/wordlist.txt";
    hashmap_t hashtable[HASH_SIZE];
    check = load_dictionary(file_path, hashtable);

    FILE *fp = fopen("/home/appsec/NYUAPPSEC/spell_checker/test1.txt", "r");
    char *misspelled[MAX_MISSPELLED];
    int num_misspelled = check_words(fp, hashtable, misspelled);
    printf("Number of misspeled word are %d", num_misspelled);
    for(int i = 0; misspelled[i]; i++)
        printf("\n %s",misspelled[i]);
    return 0;
}
