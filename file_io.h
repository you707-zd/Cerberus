#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>

//Caesar Cipher
void encrypt_file_caesar(FILE *file_input, FILE *file_output, int key);
void decrypt_file_caesar(FILE *file_input, FILE *file_output, int key);
//XOR Cipher
void XOR_file_cipher(FILE *file_input, FILE *file_output, char *key);
//Bit Rotation Cipher
void encrypt_file_bit_rotation(FILE *file_input, FILE *file_output, int key);
void decrypt_file_bit_rotation(FILE *file_input, FILE *file_output, int key);

#endif
