#include <ctype.h>
#include "encrypt.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//==== Caesar Cipher ====
char *encrypt_caesar(const char *text, int key)
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
            temp[i] = (text[i] - base + key) % 26 + base;
        }
        else
        {
            temp[i] = text[i];
        }
    }

    temp[length] = '\0';
    return temp;
}

//==== Vigenere Cipher ====
char *encrypt_vigenere(char *text, char *key)
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

    char *cipher_message = malloc(text_length + 1);
    if (cipher_message == NULL)
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

            cipher_message[j] = (text[j] - base + k - base) % 26 + base;
            alpha++;
        }
        else
        {
            cipher_message[j] = text[j];
        }
    }

    free(temp);
    cipher_message[text_length] = '\0';

    return cipher_message;
}

//==== XOR Cipher ====
void XOR_cipher(char *text, size_t text_len, char *key)
{
    size_t key_len = strlen(key);
    for (size_t i = 0; i < text_len; i++)
    {
        text[i] ^= key[i % key_len];
    }
}

// ==== Playfair cipher ====
bool trimming(char **keyword)
{
    int length = strlen(*keyword);

    char *temp = malloc(length + 1);
    if (temp == NULL)
    {
        printf("No enough memory!\n");
        return false;
    }

    int k = 0;

    for (int i = 0; i < length; i++)
    {
        int indicator = 1;

        if (!isspace((*keyword)[i]))
        {
            if (tolower((*keyword)[i]) == 'j') (*keyword)[i] = 'i';
            for (int j = 0; j < k; j++)
            {
                if (tolower((*keyword)[i]) == temp[j])
                {
                    indicator = 0;
                    break;
                }
            }

            if (indicator == 1)
            {
                temp[k++] = tolower((*keyword)[i]);
            }
        }
    }

    temp[k] = '\0';
    free(*keyword);
    *keyword = strdup(temp);
    free(temp);
    return true;
}

void matrix_5x5(char matrix[5][5], char *keyword)
{
    int i = 0, j = 0;
    int used[26] = {0};
    used['j' - 'a'] = 1;


    for (int x = 0; keyword[x] != '\0'; x++)
    {
        if (!used[keyword[x] - 'a'])
        {
            matrix[i][j++] = keyword[x];
            used[keyword[x] - 'a'] = 1;

            if (j == 5)
            {
                j = 0;
                i++;
            }
        }
    }

    for (char c = 'a'; c <= 'z'; c++)
    {
        if (used[c - 'a']) continue;

        matrix[i][j++] = c;
        used[c - 'a'] = 1;

        if (j == 5)
        {
            j = 0;
            i++;
        }
    }
}

bool generate_digraphs(char *text, char digraphs[][2], int *count)
{
    int length = strlen(text);
    int k = 0;

    char *temp = malloc(length + 1);
    if (temp == NULL)
    {
        printf("No enough memory!\n");
        return false;
    }

    for (int i = 0; i < length; i++)
    {
        if (!isspace(text[i]) && isalpha(text[i]))
        {
            char c = tolower(text[i]);
            if (c == 'j') c = 'i';
            temp[k++] = c;
        }
    }

    temp[k] = '\0';
    strcpy(text, temp);
    free(temp);

    int i = 0, digraph_index = 0;

    while (text[i] != '\0')
    {
        char first = text[i];
        char second = text[i + 1];

        if (second == '\0' || first == second)
        {
            digraphs[digraph_index][0] = first;
            digraphs[digraph_index][1] = 'x';
            digraph_index++;
            i++;
        }
        else
        {
            digraphs[digraph_index][0] = first;
            digraphs[digraph_index][1] = second;
            digraph_index++;
            i += 2;
        }
    }

    *count = digraph_index;
    return true;
}

void find_position(char matrix[5][5], char letter, int *row, int *col)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (matrix[i][j] == tolower(letter))
            {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

char *encrypt_playfair(char matrix[5][5], char digraphs[][2], int digraphs_count)
{
    int length = digraphs_count * 2;
    int k = 0;

    char *ciphertext = malloc(length + 1);
    if (ciphertext == NULL)
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
            ciphertext[k++] = matrix[row1][(column1 + 1) % 5];
            ciphertext[k++] = matrix[row2][(column2 + 1) % 5];
        }
        else if (column1 == column2)
        {
            ciphertext[k++] = matrix[(row1 + 1) % 5][column1];
            ciphertext[k++] = matrix[(row2 + 1) % 5][column2];
        }
        else
        {
            ciphertext[k++] = matrix[row1][column2];
            ciphertext[k++] = matrix[row2][column1];
        }
    }

    ciphertext[k] = '\0';
    return ciphertext;
}

//==== Rail Fence cipher ====
char *encrypt_railfence(char *text, int rails)
{
    int length = strlen(text);
    char fence[rails][length];
    memset(fence, '*', sizeof(fence));
    char *ciphertext = malloc(length + 1);
    if (ciphertext == NULL)
    {
        printf("No enough memory!\n");
        return NULL;
    }

    int x = 0, y = 0, down = 1;
    for (int i = 0; i < length; i++)
    {
        fence[x][y++] = text[i];

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
    for (int a = 0; a < rails; a++)
    {
        for (int b = 0; b < length; b++)
        {
            if (fence[a][b] != '*')
            {
                ciphertext[k++] = fence[a][b];
            }
        }
    }

    ciphertext[k] = '\0';
    return ciphertext;
}
