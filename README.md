# Tweet Generator (Markov Chain Implementation)

A lightweight, memory-efficient C application that generates synthetic tweets by modeling text data through **Markov Chains**. The program parses text input, analyzes token frequencies, constructs state transitions dynamically, and probabilistically generates coherent sentences based on existing patterns.

---

## 🔑 Key Features & Technical Highlights

* **Markov Chain Model**: Implemented a probabilistic state-machine graph where word transitions are selected based on occurrence frequencies in source text.
* **Dynamic Memory Management**: Utilized dynamic arrays and re-allocations (`malloc`, `realloc`, `free`) to scale text processing dynamically. Guaranteed **zero memory leaks**, validated with **Valgrind**.
* **Custom Data Structures**: Built state nodes, frequency tracking arrays, and a general linked-list database structure without reliance on standard C container libraries.
* **File I/O & Text Parsing**: Managed standard input streaming (`fgets`, `sscanf`) to clean, tokenize, and process large multi-line corpora robustly.
* **Pseudo-Random Sequence Handling**: Controlled random sequence generation (`srand`, `rand`) using configurable seeds for consistent test reproducibility.

---

## 🛠 Project Structure

```text
.
├── markov_chain.h      # Declarations for Markov structs & processing API
├── markov_chain.c      # Core Markov logic (frequency updates, node lookups, chain building)
├── tweets_generator.c  # Main driver script (Handles CLI arguments, file reading & execution)
├── linked_list.h / .c  # Custom generic dynamic linked list implementation
└── justdoit_tweets.txt # Sample corpus file containing processed tweets (~4400 sentences)
```

---
## 🚀 Build & Run Instructions
**Compilation**
Compile the program using standard strict standard flags:
Bashgcc -Wall -Wextra -Wvla -std=c99 tweets_generator.c markov_chain.c linked_list.c -o tweets_generator

**Usage**
Bash./tweets_generator <seed> <num_tweets> <path_to_corpus> [words_to_read]

**Arguments**
seed: Integer seed value for the pseudo-random number 
generator.num_tweets: Number of generated sentences/tweets to print to standard output.
path_to_corpus: Path to the text input file used for processing training data.
words_to_read (Optional): Limit how many words are ingested from the training corpus.

**Example Execution
**
./tweets_generator 454545 5 "justdoit_tweets.txt" 1000

**Example Output:**
Tweet 1: hello, nice to meet you.
Tweet 2: keep going and finish strong.
Tweet 3: start working today.
Tweet 4: make it happen.
Tweet 5: dream big and chase it.

## 🧠 Algorithmic FlowLearning Stage: 
1. Reads words sequentially from the input text file, adding each unique word as a MarkovNode in the dynamic database list.
2. Frequency Updates: For every state word, tracks subsequent word frequencies inside dynamic MarkovNodeFrequency arrays.
3. Generation Stage:
   - Selects an initial valid starting word at random (excluding sentence-ending words ending with ".").
   - Picks next transitions probabilistically based on frequency weightings until hitting a period termination character or maximum length constraints.  
