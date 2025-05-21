#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stddef.h>

//function prototype for caesar function

//caesar cipher
char *encrypt_caesar(const char *text, int key);
//vigenere cipher
char *encrypt_vigenere(char *text, char *key);
//xor cipher
void XOR_cipher(char *text, size_t text_len, char *key);
//playfair cipher
void trimming(char **keyword);
void matrix_5x5(char matrix[5][5], char *keyword);
void generate_digraphs(char *text, char digraphs[][2], int *count);
void find_position(char matrix[5][5], char letter, int *row, int *col);
char *encrypt_playfair(char matrix[5][5], char digraphs[][2], int digraphs_count);
//Rail Fence cipher
char *encrypt_railfence(char *text, int rails);

#endif
