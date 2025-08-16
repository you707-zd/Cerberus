#include <ctype.h>
#include "decrypt.h"
#include "encrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Caesar Cipher
char *decrypt_caesar(const char *text, int key)
{
    char *temp = malloc(strlen(text) + 1);
    if (temp == NULL)
    {
        return NULL;
    }

    int length = strlen(text);

    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            char base = isupper(text[i]) ? 'A' : 'a';
            temp[i] = (text[i] - base - key + 26) % 26 + base;
        }
        else
        {
            temp[i] = text[i];
        }
    }

    temp[length] = '\0';
    return temp;
}

//Vigenere Cipher
char *decrypt_vigenere(char *text, char *key)
{
    int text_length = strlen(text);
    int desired_key_length = 0;

    for (int i = 0; i < text_length; i++)
    {
        if (isalpha(text[i]))
        {
            desired_key_length++;
        }
    }

    char *temp = malloc(desired_key_length + 1);
    if (temp == NULL)
    {
        return NULL;
    }

    for (int y = 0; y < desired_key_length; y++)
    {
        temp[y] = key[y % strlen(key)];
    }

    temp[desired_key_length] = '\0';

    char *original_message = malloc(text_length + 1);
    if (original_message == NULL)
    {
        return NULL;
    }

    int alpha = 0;
    for (int j = 0; j < text_length; j++)
    {
        if (isalpha(text[j]))
        {
            char base = isupper(text[j]) ? 'A' : 'a';
            char k = isupper(text[j]) ? toupper(temp[alpha]) : tolower(temp[alpha]);

            original_message[j] = (text[j] - base - (k - base) + 26) % 26 + base;
            alpha++;
        }
        else
        {
            original_message[j] = text[j];
        }
    }

    free(temp);
    original_message[text_length] = '\0';

    return original_message;
}

// XOR cipher uses the same logic for encryption and decryption.
// The implementation is defined in Encrypt.c to avoid duplication.

//==== Playfair Cipher ====

//keyword trimming is being handelled by the trimming function in encrypt.c
//creating the matrix is also being handelled by the matrix function in encrypt.c

void splitting_digraphs(char *text, char digraphs[100][3], int *counter)
{
    int length = strlen(text);
    int k = 0;

    for (int i = 0; i < length; i += 2)
    {
        digraphs[k][0] = text[i];
        digraphs[k][1] = text[i + 1];
        digraphs[k][2] = '\0';
        k++;
    }

    *counter = k;
}

char *decrypt_playfair(char matrix[5][5], char digraphs[][3], int digraphs_count)
{
    int length = digraphs_count * 2;
    int k = 0;

    char *originaltext = malloc(length + 1);
    if (originaltext == NULL)
    {
        printf("No enough memory!\n");
        return NULL;
    }

    for (int i = 0; i < digraphs_count; i++)
    {
        int row1;
        int column1;
        find_position(matrix, digraphs[i][0], &row1, &column1);

        int row2;
        int column2;
        find_position(matrix, digraphs[i][1], &row2, &column2);

        if (row1 == row2)
        {
            originaltext[k++] = matrix[row1][((column1 - 1) + 5) % 5];
            originaltext[k++] = matrix[row2][((column2 - 1) + 5) % 5];
        }
        else if (column1 == column2)
        {
            originaltext[k++] = matrix[((row1 - 1) + 5) % 5][column1];
            originaltext[k++] = matrix[((row2 - 1) + 5) % 5][column2];
        }
        else
        {
            originaltext[k++] = matrix[row1][column2];
            originaltext[k++] = matrix[row2][column1];
        }
    }

    originaltext[k] = '\0';
    return originaltext;
}

//==== Rail Fence cipher ====
char *decrypt_railfence(char *text, int rails)
{
    int length = strlen(text);
    char fence[rails][length];

    char *originaltext = malloc(length + 1);
    if (originaltext == NULL)
    {
        printf("No enough memory!\n");
        return NULL;
    }

    int x = 0, y = 0, down = 1;;
    for (int i = 0; i < length; i++)
    {
        fence[x][y++] = '*';

        if (down)
            x++;
        else
            x--;

        if (x == rails - 1)
            down = 0;
        else if (x == 0)
            down = 1;
    }

    int k = 0;
    for (int i = 0; i < rails; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (fence[i][j] == '*')
            {
                fence[i][j] = text[k++];
            }
        }
    }

    x = y = 0;
    down = 1;
    int index = 0;
    for (int i = 0; i < length; i++)
    {
        originaltext[index++] = fence[x][y++];

        if (down)
            x++;
        else
            x--;

        if (x == rails - 1)
            down = 0;
        else if (x == 0)
            down = 1;
    }

    originaltext[index] = '\0';
    return originaltext;
}
