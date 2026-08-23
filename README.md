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
