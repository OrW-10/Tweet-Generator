#include "markov_chain.h"
#include <string.h>

/* *
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number maximal number to return (not including).
 * @return Random number
 */
int get_random_number(int max_number){
    return rand() % max_number;
}

/* remember order for freeing! data (word) -> MarkovNode -> Node!
 * freq_list is empty (NULL) */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr) {
    //if there is no database, adds a database. Checks allocated memory.
    if (markov_chain->database == NULL){
        markov_chain->database = malloc(sizeof(LinkedList));
        if (markov_chain->database == NULL){
            return NULL;
        }
    }

    //check if the word is already in the markov chain (linked list)
    Node* new_node = get_node_from_database(markov_chain, data_ptr);
    if (new_node != NULL){
        return new_node;
    }

    //not in list yet-> We add it to the list, make sure allocations succeeded.
    //build word
    char* new_data = malloc(strlen(data_ptr)+1); //strlen + '\0'
    if (new_data == NULL){
        return NULL;
    }
    strcpy(new_data, data_ptr);

    //build MarkovNode
    MarkovNode *new_markov = malloc(sizeof(*new_markov));
    if (new_markov == NULL){
        free(new_data);
        return NULL;
    }
    *new_markov = (MarkovNode){NULL,0, new_data};

    //build Node
    if (add(markov_chain->database, new_markov) == 1){
        free(new_data);
        free(new_markov);
        return NULL;
    }
    new_node = markov_chain->database->last;
    return new_node;
}

/* checks if word already exists in the database */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr){
    if (markov_chain->database == NULL){
        return NULL;
    }
    int list_size = markov_chain->database->size;
    Node *current_node = markov_chain->database->first;
    Node *ret = NULL;
    int i = 0;
    while(i<list_size){
        if (strcmp(current_node->data->data, data_ptr) == 0){
            ret = current_node;
            break;
        }
        current_node = current_node->next;
        i++;
    }
    return ret;
}
/* updates the frequencies_list of the fist_node using the second_node */
bool add_node_to_frequencies_list(MarkovNode *first_node, \
MarkovNode *second_node){
    //check if second_node already appears on freq list for first_node
    MarkovNodeFrequency *checker = first_node->frequencies_list;
    int i = 0;
    while(i<(first_node->size_of_list)){
        if(strcmp(checker[i].markov_node->data,second_node->data)==0){
            checker[i].frequency++;
            return true;
        }
        i++;
    }

    //adds the second_node to first_node's freq_list, checks allocation
    checker = realloc(checker,\
    (first_node->size_of_list+1)*sizeof(MarkovNodeFrequency));
    if (checker == NULL) {
        return false;
    }
    first_node->frequencies_list = checker;
    checker[first_node->size_of_list].frequency=1;
    checker[first_node->size_of_list].markov_node = second_node;
    first_node->size_of_list++;
    return true;
}

/* frees allocated memory in order */
void free_database(MarkovChain ** ptr_chain){
    if ((*ptr_chain)->database == NULL){
        free(*ptr_chain);
        *ptr_chain=NULL;
        return;
    }
    int list_size = (*ptr_chain)->database->size;
    Node* current_node = (*ptr_chain)->database->first;
    while (0<list_size){
        free(current_node->data->frequencies_list);
        current_node->data->frequencies_list = NULL;
        free(current_node->data->data); //markov_node's word
        current_node->data->data = NULL;
        free(current_node->data); //markov_node itself
        current_node->data = NULL;
        (*ptr_chain)->database->first= current_node->next;
        free(current_node); //node itself
        current_node = (*ptr_chain)->database->first;
        list_size--;
    }
    free((*ptr_chain)->database);
    (*ptr_chain)->database = NULL;
    free(*ptr_chain);
    *ptr_chain = NULL;
}

/* gets the first word of the tweet randomly, if end of sentence repeat */
MarkovNode* get_first_random_node(MarkovChain *markov_chain){
    //how many unique words are there in the database (for get_random_num())
    Node *current_node = markov_chain->database->first;
    char init = '.';
    char* rand_word = &init;
    int rand_word_length = 1;
    int counter = 1;
    int max_number= 0;
    int random_num =0;
    while(current_node != markov_chain->database->last){
        current_node = current_node->next;
        counter++;
    }
    max_number = counter;

    //generates random first word, makes sure it doesn't end with a '.'
    while(rand_word[rand_word_length-1] == '.'){
        random_num = get_random_number(max_number);
        current_node = markov_chain->database->first;
        counter=0;
        while(counter<random_num){
            current_node = current_node->next;
            counter++;
        }
        rand_word = current_node->data->data;
        rand_word_length = strlen(rand_word);
    }
    return current_node->data;
}

/* finds the next random node, uses the frequencies lists for probability */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr){
    MarkovNode* current_word = state_struct_ptr;
    int counter= 0;
    int random_number=0;
    int i = 0;
    for(i=0; i<current_word->size_of_list;i++){
        counter+=current_word->frequencies_list[i].frequency;
    }
    random_number = get_random_number(counter);
    while(random_number<counter){
        i--;
        counter-=current_word->frequencies_list[i].frequency;
    }
    return current_word->frequencies_list[i].markov_node;
}

void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length){
    if(first_node == NULL){
        first_node = get_first_random_node(markov_chain);
    }
    MarkovNode *current_markov = first_node;
    char* rand_word = current_markov->data;
    int rand_word_length = strlen(rand_word);
    int i=1;
    while(i<=max_length){
        printf(" %s", current_markov->data);
        current_markov = get_next_random_node(current_markov);
        rand_word = current_markov->data;
        rand_word_length = strlen(rand_word);
        if(rand_word[rand_word_length-1] == '.'){
            printf(" %s", current_markov->data);
            break;
        }
        i++;
    }
}