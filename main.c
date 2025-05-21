// Libraries and the other files
#include "decrypt.h"
#include "encrypt.h"
#include "file_io.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Error numbers
#define INPUT_ERROR -1
#define MEMORY_ERROR -2
#define FILE_ERROR -3
#define NOT_IMPLEMENTED_ERROR -4

int main(void)
{
    int mode = choose_mode();

    if (mode == 1)
    {
        // === Begin Encryption Process ===
        printf("\nWhat do you want to encrypt?\n \n");

        int type = choose_input_type();

        if (type == 1)
        {
            //Encrypting raw text
            int cipher_selected = get_text_method();
            if (cipher_selected == INPUT_ERROR)
            {
                printf("INVALID INPUT!!\n");
                return INPUT_ERROR;
            }

            char *key = NULL;
            int int_key = 0;
            char *text = get_cipher_key(cipher_selected, &key, &int_key);

            if (cipher_selected == 1)
            {
                // Encrypting Caesar Cipher
                //FREE IT AFTER USING IT//
                char *cipher_text = encrypt_caesar(text, int_key);
                if (cipher_text == NULL)
                {
                    printf("\nNo enough memory!! \n");

                    free(text);
                    return MEMORY_ERROR;
                }

                printf("\nThe Cipher Text is: %s\n", cipher_text);

                free(cipher_text);
                free(text);

                return 0;
            }
            else if (cipher_selected == 2)
            {
                //Encrypting Vigenere
                char *cipher_text = encrypt_vigenere(text, key);
                if (cipher_text == NULL)
                {
                    printf("\nNo enough memory!\n");

                    free(text);
                    free(key);
                    return MEMORY_ERROR;
                }

                printf("\nThe Cipher Text is: %s\n", cipher_text);

                free(cipher_text);
                free(text);
                free(key);

                return 0;
            }
            else if (cipher_selected == 3)
            {
                //Encrypting XOR
                size_t text_length = strlen(text);

                XOR_cipher(text, text_length, key);

                printf("\nThe Cipher Text is: ");
                print_hex(text, text_length);

                free(text);
                free(key);

                return 0;
            }
            else if (cipher_selected == 4)
            {
                //Encrypting playfair
                char matrix[5][5];

                char digraphs[100][2];
                int counter = 0;

                trimming(&key);
                matrix_5x5(matrix, key);
                generate_digraphs(text, digraphs, &counter);

                char *ciphertext = encrypt_playfair(matrix, digraphs, counter);
                printf("\nThe cipher text is: %s\n", ciphertext);

                free(ciphertext);
                free(key);

                return 0;
            }
            else if (cipher_selected == 5)
            {
                //Encrypting Rail Fence Cipher
                char *ciphertext = encrypt_railfence(text, int_key);
                if (ciphertext == NULL)
                {
                    printf("\nNo enough memroy!\n");
                    return MEMORY_ERROR;
                    free(text);
                }

                printf("\nThe cipher text is: %s\n", ciphertext);

                free(ciphertext);
                free(text);

                return 0;
            }
            else
            {
                printf("\nINVALID INPUT!\n");

                return INPUT_ERROR;
            }
        }
        else if (type == 2)
        {
            // === File Encrypting ===

            printf("\nWhat type of file do you want to encrypt?\n \n");

            int file_type_selected = choose_file_type();
            char *file_input = get_file_name();

            FILE *input_file;
            FILE *output_file;

            if (file_type_selected == 1)
            {
                input_file = fopen(file_input, "r");
                if (input_file == NULL)
                {
                    printf("\nCouldn't open the file!!\n");

                    free(file_input);
                    return FILE_ERROR;
                }
            }
            else
            {
                input_file = fopen(file_input, "rb");
                if (input_file == NULL)
                {
                    printf("\nCouldn't open the file!!\n");

                    free(file_input);
                    return FILE_ERROR;
                }
            }

            char *file_extension = get_file_extension(file_input);

            char *file_output = get_output_file(file_extension, file_type_selected);
            if (file_output == NULL)
            {
                free(file_input);
                free(file_extension);

                fclose(input_file);
                return FILE_ERROR;
            }

            if (file_type_selected == 1)
            {
                output_file = fopen(file_output, "w");
                if (output_file == NULL)
                {
                    printf("\nCouldn't open the file\n");

                    free(file_input);
                    free(file_extension);
                    free(file_output);

                    fclose(input_file);
                    return FILE_ERROR;
                }
            }
            else
            {
                output_file = fopen(file_output, "wb");
                if (output_file == NULL)
                {
                    printf("\nCouldn't open the file\n");

                    free(file_input);
                    free(file_extension);
                    free(file_output);

                    fclose(input_file);
                    return FILE_ERROR;
                }
            }

            if (file_type_selected == 1)
            {
                //==== Text files ====
                int cipher_selected = text_file_cipher_menu();

                if (strcasecmp(file_extension, ".txt") != 0 && strcasecmp(file_extension, ".csv") != 0)
                {
                    printf("\nINVALID file extension!!\n");

                    free(file_input);
                    free(file_extension);

                    fclose(input_file);
                    return FILE_ERROR;
                }

                if (cipher_selected == 1)
                {
                    printf("\nThe key must be a number greater than or equal 1 and less than or equal 25\n");
                    int key = get_int("\nEnter the key: ");

                    while (1)
                    {
                        if (key < 1 || key > 25)
                        {
                            printf("\nINVALID INPUT!!\n");
                            printf("The key must be a number greater than or equal 1 and less than or equal 25\n");

                            key = get_int("\nEnter the key: ");
                            continue;
                        }

                        break;
                    }

                    encrypt_file_caesar(input_file, output_file, key);
                    printf("\nSuccess!!\n");
                    printf("\nYour encrypted file is saved as: %s\n", file_output);

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    return 0;
                }
                else if (cipher_selected == 2)
                {
                    printf("\nNote: For better security, a key length equal to the text length is recommended.\n");
                    printf("If your key is too short, it will be repeated.\n");
                    printf("If your key is too long, the extra characters will be ignored.\n");

                    char *key = get_string("\nEnter the key: ");
                    if (key == NULL)
                    {
                        printf("\nNo enough memory!!\n");

                        free(file_input);
                        free(file_output);
                        free(file_extension);

                        fclose(input_file);
                        fclose(output_file);

                        return MEMORY_ERROR;
                    }

                    XOR_file_cipher(input_file, output_file, key);
                    printf("\nSuccess!!\n");
                    printf("\nYour encrypted file is saved as: %s\n", file_output);

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    return 0;
                }
                else
                {
                    printf("\nINVALID INPUT\n");

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    fclose(input_file);
                    fclose(output_file);

                    return INPUT_ERROR;
                }
            }
            else if (file_type_selected == 2)
            {
                //==== Binary files ====
                int cipher_selected = binary_file_cipher_menu();

                if (cipher_selected == 1)
                {
                    char *key = get_string("\nEnter the key: ");
                    if (key == NULL)
                    {
                        printf("\nNo enough memory!!\n");

                        free(file_input);
                        free(file_output);
                        free(file_extension);

                        fclose(input_file);
                        fclose(output_file);

                        return MEMORY_ERROR;
                    }

                    XOR_file_cipher(input_file, output_file, key);
                    printf("\nSuccess!!\n");
                    printf("\nYour encrypted file is saved as: %s\n", file_output);

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    return 0;
                }
                else if (cipher_selected == 2)
                {
                    printf("\nThe key must be a number greater than or equal 1 and less than or equal 7\n");
                    int key = get_int("\nEnter the key: ");

                    while (1)
                    {
                        if (key < 1 || key > 7)
                        {
                            printf("\nINVALID INPUT!!\n");
                            printf("The key must be a number greater than or equal 1 and less than or equal 7\n");

                            key = get_int("\nEnter the key: ");
                            continue;
                        }

                        break;
                    }

                    encrypt_file_bit_rotation(input_file, output_file, key);
                    printf("\nSuccess!!\n");
                    printf("\nYour encrypted file is saved as: %s\n", file_output);

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    return 0;
                }
                else
                {
                    printf("\nINVALID INPUT\n");

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    fclose(input_file);
                    fclose(output_file);

                    return INPUT_ERROR;
                }
            }
            else
            {
                printf("\nINVALID INPUT!!\n");
                return INPUT_ERROR;
            }
        }
        else
        {

            printf("\nINVALID INPUT!!\n");

            return INPUT_ERROR;
        }
    }
    else if (mode == 2)
    {
        printf("\nWhat do you want to decrypt?\n \n");

        int type = choose_input_type();

        if (type == 1)
        {
            // Decrypting Raw Text
            int cipher_selected = get_text_method();
            if (cipher_selected == INPUT_ERROR)
            {
                printf("\nINVALID INPUT!!\n");
                return INPUT_ERROR;
            }

            char *key = NULL;
            int int_key = 0;
            char *text = get_cipher_key(cipher_selected, &key, &int_key);

            if (cipher_selected == 1)
            {
                // Decrypting Caesar Cipher
                //FREE IT AFTER USING IT//
                char * decrypted_message = decrypt_caesar(text, int_key);
                if (decrypted_message == NULL)
                {
                    printf("\nNo enough memory!! \n");

                    free(text);

                    return MEMORY_ERROR;
                }

                printf("\nThe decrypted message is: %s\n", decrypted_message);

                free(decrypted_message);
                free(text);

                return 0;
            }
            else if (cipher_selected == 2)
            {
                //Decrypting Vigenere
                char *original_text = decrypt_vigenere(text, key);
                if (original_text == NULL)
                {
                    printf("\nNo enough memory!\n");

                    return MEMORY_ERROR;
                }

                printf("\nThe Original Text is: %s\n", original_text);

                free(original_text);
                free(text);
                free(key);

                return 0;
            }
            else if (cipher_selected == 3)
            {
                //Decrypting XOR
                size_t text_length;

                unsigned char *result = hex_to_bytes(text, &text_length);
                if (result == NULL)
                {
                    printf("\nInvalid hex input.\n");
                    return INPUT_ERROR;
                }

                XOR_cipher((char *)result, text_length, key);
                printf("\nThe Original Text is: %.*s\n", (int)text_length, result);

                free(text);
                free(key);

                return 0;
            }
            else if (cipher_selected == 4)
            {
                //Decrypting playfair
                char matrix[5][5];

                char digraphs[100][3];
                int counter = 0;

                trimming(&key);
                matrix_5x5(matrix, key);
                splitting_digraphs(text, digraphs, &counter);

                char *originaltext = decrypt_playfair(matrix, digraphs, counter);
                int length = strlen(originaltext);
                int k = 0, i = 0;
                char *temp = malloc(length + 1);

                while (i < length)
                {
                    if (i + 2 < length && originaltext[i] == originaltext[i + 2] && originaltext[i + 1] == 'x')
                    {
                        temp[k++] = originaltext[i];
                        temp[k++] = originaltext[i + 2];
                        i += 3;
                    }
                    else
                    {
                        temp[k++] = originaltext[i++];
                    }
                }

                if (k > 0 && temp[k - 1] == 'x')
                {
                    k--;
                }

                temp[k] = '\0';
                free(originaltext);
                originaltext = temp;

                printf("\nThe origianl text is: %s\n", originaltext);

                free(originaltext);
                free(key);

                return 0;
            }
            else if (cipher_selected == 5)
            {
                //Decrypting Rail Fence
                char *originaltext = decrypt_railfence(text, int_key);
                if (originaltext == NULL)
                {
                    printf("\nNo enough memory!\n");
                    free(text);
                    return MEMORY_ERROR;
                }

                printf("\nThe original text is: %s\n", originaltext);

                free(originaltext);
                free(text);

                return 0;
            }
            else
            {
                printf("\nINVALID IPUT!\n");

                return INPUT_ERROR;
            }
        }
        else if (type == 2)
        {
            // === File Decrypting ===
            printf("\nWhat type of file do you want to decrypt?\n \n");

            int file_type_selected = choose_file_type();
            char *file_input = get_file_name();

            FILE *input_file = fopen(file_input, "r");
            if (input_file == NULL)
            {
                printf("\nCouldn't open the file!!\n");

                free(file_input);
                return FILE_ERROR;
            }

            char *file_extension = get_file_extension(file_input);

            char *file_output = get_output_file(file_extension, file_type_selected);
            if (file_output == NULL)
            {
                free(file_input);
                free(file_extension);

                fclose(input_file);
                return FILE_ERROR;
            }

            FILE *output_file = fopen(file_output, "w");
            if (output_file == NULL)
            {
                printf("\nCouldn't open the file\n");

                free(file_input);
                free(file_extension);
                free(file_output);

                fclose(input_file);
                return FILE_ERROR;
            }

            if (file_type_selected == 1)
            {
                //==== Text files ====
                int cipher_selected = text_file_cipher_menu();

                if (strcasecmp(file_extension, ".txt") != 0 && strcasecmp(file_extension, ".csv") != 0)
                {
                    printf("\nINVALID file extension!!\n");

                    free(file_input);
                    free(file_extension);

                    fclose(input_file);
                    return FILE_ERROR;
                }

                if (cipher_selected == 1)
                {
                    printf("\nThe key must be a number greater than or equal 1 and less than or equal 25\n");
                    int key = get_int("\nEnter the key: ");

                    while (1)
                    {
                        if (key < 1 || key > 25)
                        {
                            printf("\nINVALID INPUT!!\n");
                            printf("The key must be a number greater than or equal 1 and less than or equal 25\n");

                            key = get_int("\nEnter the key: ");
                            continue;
                        }

                        break;
                    }

                    decrypt_file_caesar(input_file, output_file, key);
                    printf("\nSuccess!!\n");
                    printf("\nYour decrypted file is saved as: %s\n", file_output);

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    return 0;
                }
                else if (cipher_selected == 2)
                {
                    char *key = get_string("\nEnter the key: ");
                    if (key == NULL)
                    {
                        printf("\nNo enough memory!!\n");

                        free(file_input);
                        free(file_output);
                        free(file_extension);

                        fclose(input_file);
                        fclose(output_file);

                        return MEMORY_ERROR;
                    }

                    XOR_file_cipher(input_file, output_file, key);
                    printf("\nSuccess!!\n");
                    printf("\nYour decrypted file is saved as: %s\n", file_output);

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    return 0;
                }
                else
                {
                    printf("\nINVALID INPUT\n");

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    fclose(input_file);
                    fclose(output_file);

                    return INPUT_ERROR;
                }
            }
            else if (file_type_selected == 2)
            {
                //==== Binary files ====
                int cipher_selected = binary_file_cipher_menu();

                if (cipher_selected == 1)
                {
                    char *key = get_string("\nEnter the key: ");
                    if (key == NULL)
                    {
                        printf("\nNo enough memory!!\n");

                        free(file_input);
                        free(file_output);
                        free(file_extension);

                        fclose(input_file);
                        fclose(output_file);

                        return MEMORY_ERROR;
                    }

                    XOR_file_cipher(input_file, output_file, key);
                    printf("\nSuccess!!\n");
                    printf("\nYour decrypted file is saved as: %s\n", file_output);

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    return 0;
                }
                else if (cipher_selected == 2)
                {
                    printf("\nThe key must be a number greater than or equal 1 and less than or equal 7\n");
                    int key = get_int("\nEnter the key: ");

                    while (1)
                    {
                        if (key < 1 || key > 7)
                        {
                            printf("\nINVALID INPUT!!\n");
                            printf("The key must be a number greater than or equal 1 and less than or equal 7\n");

                            key = get_int("\nEnter the key: ");
                            continue;
                        }

                        break;
                    }

                    decrypt_file_bit_rotation(input_file, output_file, key);
                    printf("\nSuccess!!\n");
                    printf("\nYour decrypted file is saved as: %s\n", file_output);

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    return 0;
                }
                else
                {
                    printf("\nINVALID INPUT\n");

                    free(file_input);
                    free(file_output);
                    free(file_extension);

                    fclose(input_file);
                    fclose(output_file);

                    return INPUT_ERROR;
                }
            }
            else
            {
                printf("\nINVALID INPUT!!\n");
                return INPUT_ERROR;
            }
        }
        else
        {
            printf("\nINVALID INPUT!!\n");

            return INPUT_ERROR;
        }
    }
    else if (mode == 3)
    {
        return 0;
    }
    else
    {
        printf("INVALID INPUT!!\n");

        return INPUT_ERROR;
    }

}
