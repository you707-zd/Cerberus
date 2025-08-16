#include <ctype.h>
#include "helpers.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//Error Numbers
#define INPUT_ERROR -1
#define MEMORY_ERROR -2
#define FILE_ERROR -3
#define NOT_IMPLEMENTED_ERROR -4

char *get_string(char *prompt)
{
    //My own version of get_string
    printf("%s", prompt);

    size_t buffer_size = 16;
    size_t length = 0;

    char *buffer = malloc(buffer_size);
    if (buffer == NULL)
    {
        printf("No enough memory!! \n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        buffer[length++] = (char) c;

        if (length == buffer_size)
        {
            buffer_size *= 2;

            char *temp = realloc(buffer, buffer_size);
            if (temp == NULL)
            {
                free(buffer);
                printf("No enough memory!! \n");
                return NULL;
            }

            buffer = temp;
        }
    }

    buffer[length] = '\0';
    return buffer;
}

int get_int(char *prompt)
{
    //My own version of get_int
    char buffer[100];
    char *endptr;
    long value;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            continue;
        }

        value = strtol(buffer, &endptr, 10);

        while (*endptr == ' ' || *endptr == '\t')
            endptr++;

        if (*endptr == '\n' || *endptr == '\0')
        {
            return (int) value;
        }
    }
}

unsigned char *hex_to_bytes(const char *hex, size_t *out_len)
{
    size_t len = strlen(hex);
    if (len % 2 != 0)
    {
        return NULL;
    }

    *out_len = len / 2;
    unsigned char *bytes = malloc(*out_len);
    if (bytes == NULL)
    {
        return NULL;
    }

    for (size_t i = 0; i < *out_len; ++i)
    {
        sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
    }

    return bytes;
}

void print_hex(const char *data, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%02x", (unsigned char)data[i]);
    }
    printf("\n");
}

int choose_mode(void)
{
    printf("\n===CERBERUS===\n");
    printf("Welcome to CERBERUS!\n \n");
    printf("Choose from the Menu:\n \n");

    char *choose1[3] = {"Encrypt", "Decrypt", "Exit"};
    for (int i = 0; i < 3; i++)
    {
        printf("[%i] %s\n", i + 1, choose1[i]);
    }

    // Declaring a pointer to the user's usage, !!! YOU NEED TO FREE IT !!!
    char *main_menu = get_string("\nEnter: ");
    if (main_menu == NULL)
    {
        printf("No enough memory!! \n");
        return MEMORY_ERROR;
    }

    int num_input = atoi(main_menu);
    int return_value = INPUT_ERROR;

    if (num_input == 1 || strcasecmp(main_menu, "encrypt") == 0)
    {
        return_value = 1;
    }
    else if (num_input == 2 || strcasecmp(main_menu, "decrypt") == 0)
    {
        return_value = 2;
    }
    else if (num_input == 3 || strcasecmp(main_menu, "exit") == 0)
    {
        return_value = 3;
    }

    free(main_menu);
    return return_value;
}

int choose_input_type(void)
{
    char *choose2[2] = {"Raw text", "File"};
    for (int i = 0; i < 2; i++)
    {
        printf("[%i] %s\n", i + 1, choose2[i]);
    }

    // Declaring a pointer to the user's type of encrypting choice, !!! YOU NEED TO FREE IT, TOO !!!
    char *input_type_menu = get_string("\nEnter: ");
    if (input_type_menu == NULL)
    {
        printf("No enough memory!! \n");
        return MEMORY_ERROR;
    }

    int num_input = atoi(input_type_menu);
    int return_value = INPUT_ERROR;

    if (num_input == 1 || strcasecmp(input_type_menu, "raw text") == 0)
    {
        return_value = 1;
    }
    else if (num_input == 2 || strcasecmp(input_type_menu, "file") == 0)
    {
        return_value = 2;
    }

    free(input_type_menu);
    return return_value;
}

int get_text_method(void)
{
    printf("\nChoose the cipher: \n\n");

    char *text_ciphers[5] = {"Caesar Cipher", "Vigenere Cipher", "XOR Cipher", "Playfair Cipher", "Rail Fence Cipher"};
    for (int j = 0; j < 5; j++)
    {
        printf("[%i] %s\n", j + 1, text_ciphers[j]);
    }

    //Declaring a pointer to the user's cipher choice, !!! You guessed it right, YOU NEED TO FREE IT !!!
    char *text_ciphers_menu = get_string("\nEnter: ");
    if (text_ciphers_menu == NULL)
    {
        printf("No enough memory!! \n");
        return MEMORY_ERROR;
    }

    int num_cipher = atoi(text_ciphers_menu);
    int return_value = INPUT_ERROR;

    if (num_cipher == 1 || strcasecmp(text_ciphers_menu, "Caesar Cipher") == 0)
    {
        return_value = 1;
    }
    else if (num_cipher == 2 || strcasecmp(text_ciphers_menu, "Vigenere Cipher") == 0)
    {
        return_value = 2;
    }
    else if (num_cipher == 3 || strcasecmp(text_ciphers_menu, "XOR Cipher") == 0)
    {
        return_value = 3;
    }
    else if (num_cipher == 4 || strcasecmp(text_ciphers_menu, "Playfair Cipher") == 0)
    {
        return_value = 4;
    }
    else if (num_cipher == 5 || strcasecmp(text_ciphers_menu, "Rail Fence Cipher") == 0)
    {
        return_value = 5;
    }

    free(text_ciphers_menu);
    return return_value;
}

char *get_cipher_key(int cipher_type, char **key, int *int_key)
{
    char *text = get_string("\nEnter the text: ");
    if (text == NULL)
    {
        printf("\nNot enough memory!! \n");
        return NULL;
    }

    if (cipher_type == 1 || cipher_type == 5)
    {
        printf("\nThe key must be numeric\n");

        if (cipher_type ==  1)
        {
            printf("The key must be a number greater than or equal 1 and less than or equal 25\n");
            *int_key = get_int("\nEnter the Key: ");

            while (1)
            {
                if (*int_key < 1 || *int_key > 25)
                {
                    printf("\nINVALID INPUT!!\n");
                    printf("\nThe key must be a number greater than or equal 1 and less than or equal 25\n");

                    *int_key = get_int("\nEnter the key: ");
                    continue;
                }

                break;
            }
        }
        else
        {
            printf("The key must be greater then or equal 2 and less than or equal the length of the text\n");
            *int_key = get_int("\nEnter the key: ");

            int length = strlen(text);

            while(1)
            {
                if (*int_key < 2 || *int_key > length)
                {
                    printf("\nINVALID INPUT!!\n");
                    printf("\nThe key must be greater then or equal 2 and less than or equal the length of the text\n");

                    *int_key = get_int("\nEnter the key: ");
                    continue;
                }

                break;
            }
        }
    }
    else if (cipher_type == 2 || cipher_type == 4)
    {
        if (cipher_type == 4)
        {
            printf("\nThis cipher is used to encrypt only alphabetical characters\nAny numbers, punctuations, or spaces will be ignored and won't be accepted\n");
        }

        printf("\nThe key must be Alphabetical only.\nNo spaces, numbers, or punctuation will be accepted.\n");
        printf("The key's length must be less than or equal to the text length\n");

        *key = get_string("\nEnter the key: ");
        if (*key == NULL)
        {
            printf("\nNot enough memory!\n");
            free(text);
            return NULL;
        }

        while (1)
        {
            int length_flag = 0;
            int alpha_flag = 1;

            int key_length = strlen(*key);
            int text_length = strlen(text);

            if (key_length <= text_length)
            {
                length_flag = 1;
            }

            for (int i = 0; i < key_length; i++)
            {
                if (!isalpha((*key)[i]))
                {
                    alpha_flag = 0;
                    break;
                }
            }

            if (alpha_flag == 1 && length_flag == 1)
            {
                break;
            }
            else
            {
                printf("\nINVALID INPUT!\n");
                printf("\nKey must be alphabetical and less than or equal to the length of the text\nNo spaces, punctuations, numbers are accepted\n");

                        char *temp = get_string("\nEnter the key: ");
        if (temp == NULL)
        {
            printf("\nNot enough memory!\n");
            free(text);
            free(*key);
            return NULL;
        }

        free(*key);
        *key = temp;
        // Note: temp is now owned by *key, so we don't free it here
            }
        }
    }
    else if (cipher_type == 3)
    {
        printf("\nNote: For better security, a key length equal to the text length is recommended.\n");
        printf("If your key is too short, it will be repeated.\n");
        printf("If your key is too long, the extra characters will be ignored.\n");

        *key = get_string("\nEnter the key: ");
        if (*key == NULL)
        {
            printf("\nNot enough memory!\n");
            free(text);
            return NULL;
        }
    }

    return text;
}

int choose_file_type(void)
{
    char *choose2[2] = {"text (.txt, .csv)", "binary (.png, .pdf, .jpg, ...etc)"};
    for (int i = 0; i < 2; i++)
    {
        printf("[%i] %s\n", i + 1, choose2[i]);
    }

    // Declaring a pointer to the user's type of file, !!! YOU NEED TO FREE IT, TOO !!!
    char *file_type = get_string("\nEnter: ");
    if (file_type == NULL)
    {
        printf("No enough memory!! \n");
        return MEMORY_ERROR;
    }

    int num_input = atoi(file_type);
    int return_value = INPUT_ERROR;

    if (num_input == 1 || strcasecmp(file_type, "text") == 0 || strcasecmp(file_type, "text (.txt, .csv)") == 0)
    {
        return_value = 1;
    }
    else if (num_input == 2 || strcasecmp(file_type, "binary") == 0 || strcasecmp(file_type, "binary (.png, .pdf, .jpg, ...etc)") == 0)
    {
        return_value = 2;
    }

    free(file_type);
    return return_value;
}

char *get_file_name(void)
{
    printf("\nNOTE!! THE FILE MUST BE IN THE SAME FOLDER AS THE PROGRAM\n");
    char *file_name = get_string("\nEnter the file's name: ");
    int length = strlen(file_name);
    char *temp = malloc(length + 1);
    if (temp == NULL)
    {
        printf("No enough memory!!\n");
        return NULL;
    }

    int k = 0;
    for (int i = 0; file_name[i] != '\0'; i++)
    {
        if (!isspace(file_name[i]))
        {
            temp[k++] = file_name[i];
        }
    }

    temp[k] = '\0';

    free(file_name);

    return temp;
}

char *get_file_extension(char *file_name)
{
    int length = strlen(file_name);
    char *extension = malloc(50);
    if (extension == NULL)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    int i = length - 1;
    while (i >= 0 && file_name[i] != '.')
    {
        i--;
    }

    if (i < 0)
    {
        free(extension);
        return NULL;
    }

    int j = 0;
    while (file_name[i] != '\0')
    {
        extension[j++] = file_name[i++];
    }
    extension[j] = '\0';

    return extension;
}

char *get_extensionless_file_name(char *file_name)
{
    int length = strlen(file_name);
    char *result = malloc(length + 1);
    if (result == NULL)
    {
        printf("No enough memroy!\n");
        return NULL;
    }

    int indicator;
    for (int i = 0; i < length; i++)
    {
        indicator = 1;

        if (file_name[i] == '.')
        {
            for (int j = i + 1; j < length; j++)
            {
                if (file_name[j] == '.')
                {
                    indicator = 0;
                    break;
                }
            }

            if (indicator)
            {
                indicator = i;
                break;
            }
        }
    }

    for (int i = 0; i < indicator; i++)
    {
        result[i] = file_name[i];
    }

    result[indicator] = '\0';
    return result;
}

char *get_output_file(char *file_extension, int indicator)
{
    printf("\nEnter the name of the output file with its extension, please\nIf no name provided your output will be saved in result.csv/.txt\n");
    char *file_output = get_file_name();

    if (*file_output == '\0')
    {
        char *default_name;
        if (indicator == 1)
        {
            default_name = (strcasecmp(file_extension, ".csv") == 0)
                                        ? "result.csv"
                                        : "result.txt";
        }
        else
        {
            default_name = malloc(strlen("result.png") + 4);
            sprintf(default_name, "result.%s", file_extension);
        }

        file_output = malloc(strlen(default_name) + 1);
        if (file_output == NULL)
        {
            printf("Memory allocation failed!\n");
            return NULL;
        }

        strcpy(file_output, default_name);
    }

    char *output_extension = get_file_extension(file_output);
    if (!output_extension)
    {
        printf("\nINVALID INPUT\n");

        free(output_extension);
        free(file_output);

        return NULL;
    }

    if (indicator == 1 && strcasecmp(output_extension, ".txt") != 0 && strcasecmp(output_extension, ".csv") != 0)
    {
        printf("\nINVALID EXTENSION\n");

        free(output_extension);
        free(file_output);

        return NULL;
    }

    char foo[32];
    strcpy(foo, file_output);
    int x = 0;

    FILE *output_checking = fopen(foo, "r");
    file_output = get_extensionless_file_name(file_output);
    while (output_checking != NULL)
    {
        fclose(output_checking);

        x++;
        sprintf(foo, "%s%i%s", file_output, x, output_extension);
        FILE *temp = fopen(foo, "r");
        output_checking = temp;
    }

    free(output_extension);
    strcpy(file_output, foo);

    return file_output;
}

int text_file_cipher_menu(void)
{
    printf("\nChoose the cipher: \n\n");

    char *text_ciphers[2] = {"Caesar Cipher", "XOR Cipher"};
    for (int j = 0; j < 2; j++)
    {
        printf("[%i] %s\n", j + 1, text_ciphers[j]);
    }

    char *file_cipher_selected = get_string("\nEnter: ");
    if (file_cipher_selected == NULL)
    {
        printf("No enough memory!! \n");

        return MEMORY_ERROR;
    }

    int num_cipher = atoi(file_cipher_selected);
    int return_value = INPUT_ERROR;

    if (num_cipher == 1 || strcasecmp(file_cipher_selected, "Caesar Cipher") == 0)
    {
        return_value = 1;
    }
    else if (num_cipher == 2 || strcasecmp(file_cipher_selected, "XOR Cipher") == 0)
    {
        return_value = 2;
    }

    free(file_cipher_selected);
    return return_value;
}

int binary_file_cipher_menu(void)
{
    printf("\nChoose the cipher: \n\n");

    char *text_ciphers[2] = {"XOR Cipher", "Bit Rotation Cipher"};
    for (int j = 0; j < 2; j++)
    {
        printf("[%i] %s\n", j + 1, text_ciphers[j]);
    }

    char *file_cipher_selected = get_string("\nEnter: ");
    if (file_cipher_selected == NULL)
    {
        printf("No enough memory!! \n");

        return MEMORY_ERROR;
    }

    int num_cipher = atoi(file_cipher_selected);
    int return_value = INPUT_ERROR;

    if (num_cipher == 1 || strcasecmp(file_cipher_selected, "XOR Cipher") == 0)
    {
        return_value = 1;
    }
    else if (num_cipher == 2 || strcasecmp(file_cipher_selected, "Bit Rotation Cipher") == 0)
    {
        return_value = 2;
    }

    free(file_cipher_selected);
    return return_value;
}
