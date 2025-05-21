#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  ===================== TEXT FILES ========================
//Caesar Cipher
void encrypt_file_caesar(FILE *file_input, FILE *file_output, int key)
{
    char c;
    while (fread(&c, sizeof(char), 1, file_input) == 1)
    {
        if (isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base + key) % 26 + base;

            fprintf(file_output, "%c", c);
            continue;
        }

        fprintf(file_output, "%c", c);
    }

    fclose(file_input);
    fclose(file_output);
}

void decrypt_file_caesar(FILE *file_input, FILE *file_output, int key)
{
    char c;
    while (fread(&c, sizeof(char), 1, file_input) == 1)
    {
        if (isalpha(c))
        {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base - key + 26) % 26 + base;

            fprintf(file_output, "%c", c);
            continue;
        }

        fprintf(file_output, "%c", c);
    }

    fclose(file_input);
    fclose(file_output);
}

//XOR Cipher
void XOR_file_cipher(FILE *file_input, FILE *file_output, char *key)
{
    int length = strlen(key);
    int i = 0;

    char c;
    while(fread(&c, sizeof(char), 1, file_input) == 1)
    {
        fputc(c ^ key[i++ % (int)length], file_output);
    }

    fclose(file_input);
    fclose(file_output);
}

//================================ BINARY FILES ======================================
//Bit Rotation Cipher
void encrypt_file_bit_rotation(FILE *file_input, FILE *file_output, int key)
{
    unsigned char c;
    while(fread(&c, sizeof(unsigned char), 1, file_input) == 1)
    {
        c = (c << key) | (c >> (8 - key));
        fputc(c, file_output);
    }

    fclose(file_input);
    fclose(file_output);
}

void decrypt_file_bit_rotation(FILE *file_input, FILE *file_output, int key)
{
    unsigned char c;
    while(fread(&c, sizeof(unsigned char), 1, file_input) == 1)
    {
        c = (c >> key) | (c << (8 - key));
        fputc(c, file_output);
    }

    fclose(file_input);
    fclose(file_output);
}
