#ifndef DECRYPT_H
#define DECRYPT_H

//function prototype for Caesar function
//caesar cipher
char *decrypt_caesar(const char *text, int key);
//vigenere cipher
char *decrypt_vigenere(char *text, char *key);
//playfair cipher
void splitting_digraphs(char *text, char digraphs[100][3], int *counter);
char *decrypt_playfair(char matrix[5][5], char digraphs[][3], int digraphs_count);
//rail fence cipher
char *decrypt_railfence(char *text, int rails);

#endif
