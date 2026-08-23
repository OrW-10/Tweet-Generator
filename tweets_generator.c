#include "markov_chain.h"
#include "string.h"
#include <stdlib.h>

#define MIN_ARGUMENTS_NUM 4
#define MAX_ARGUMENTS_NUM 5
#define MAX_CHAR_SENTENCE 1001
#define MAX_CHAR_WORD 101
#define START_OF_ACSII_SIGNS 32
#define END_OF_ACSII_SIGNS 126
#define MAX_LENGTH 20

/* fills up the database until word_count==words_to_read or doc finish */
int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain){
    char sentence[MAX_CHAR_SENTENCE] = "", word[MAX_CHAR_WORD] = "";
    int word_count = 0, sentence_index = 0, word_index = 0;
    Node *current_word = NULL, *new_word = NULL;
    //runs through sentences
    while(fgets(sentence, MAX_CHAR_SENTENCE, fp) != NULL){
        sentence_index = 0;
        //runs through chars in the sentence until sentence fin or word_count
        while(sentence_index <= (int) strlen(sentence)){
            if((word_count>=words_to_read) && (words_to_read>0)){
                return 0;
            }
            if(!((sentence[sentence_index] > START_OF_ACSII_SIGNS) && \
            (sentence[sentence_index] < END_OF_ACSII_SIGNS))){ //word ended
                if(word_index>0){  //if not an empty word ''
                    word[word_index] = '\0'; //end word
                    word_count++;
                    new_word = add_to_database(markov_chain,word);
                    if(new_word == NULL){ //memory allocation problem!
                        free_database(&markov_chain);
                        return 1;
                    }
                    if(current_word != NULL){ //last word added exists
                        if(add_node_to_frequencies_list(current_word->data, \
                        new_word->data)==false){
                            free_database(&markov_chain);
                            return 1;
                        }
                    }
                    if(word[word_index-1] != '.'){
                        current_word = new_word;
                    }
                    else{
                        current_word = NULL;
                    }
                    new_word = NULL;
                    for(;word_index>=0;word_index--){
                        word[word_index]='\0';
                    }
                    word_index++;
                }
                sentence_index++;
                continue;
            }
            word[word_index]=sentence[sentence_index];
            word_index++;
            sentence_index++;
        }
    }
    return 0;
}

/* uses an input document to make a database, generates tweet*/
int main (int argc, char *argv[]) {
    // check input
    if (argc>MAX_ARGUMENTS_NUM || argc<MIN_ARGUMENTS_NUM){
        printf("Usage: too many/too few arguments!");
        return EXIT_FAILURE;
    }
    char *input_file_path = argv[3];
    FILE* in_file = fopen(input_file_path,"r");
    if (in_file == NULL){
        printf("Error: The given file could not be opened.");
        return EXIT_FAILURE;
    }
    int words_to_read=-1;
    if (argc == MAX_ARGUMENTS_NUM){
        sscanf(argv[4],"%d",&words_to_read);
    }

    // build database
    MarkovChain *chain = malloc(sizeof(*chain));
    if (chain==NULL){
        printf("Allocation failure: Not enough memory.");
        return EXIT_FAILURE;
    }
    if(fill_database(in_file,words_to_read,chain)==1){
        printf("Allocation failure: Not enough memory.");
        return EXIT_FAILURE;
    }

    //generate tweets
    int seed = 0;
    int num_of_tweets = 0;
    sscanf(argv[1],"%d",&seed);
    sscanf(argv[2],"%d",&num_of_tweets);
    srand(seed);
    MarkovNode *first_node = NULL;
    for(int i=1;i<=num_of_tweets;i++) {
        first_node = get_first_random_node(chain);
        printf("Tweet %i:", i);
        generate_tweet(chain, first_node, MAX_LENGTH);
        printf("\n");
    }
    free_database(&chain);
    fclose(in_file);
    return EXIT_SUCCESS;
}

