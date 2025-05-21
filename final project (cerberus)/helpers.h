#ifndef HELPERS_H
#define HELPERS_H

#include <stddef.h>

//My own get input
char *get_string(char *prompt);
int get_int(char *prompt);

//hexadecimal properties
unsigned char *hex_to_bytes(const char *hex, size_t *out_len);
void print_hex(const char *data, int length);

//menu control
int choose_mode(void);
int choose_input_type(void);
int choose_file_type(void);
int get_text_method(void);
char *get_cipher_key(int cipher_type, char **key, int *int_key);

//file control
char *get_file_name(void);
char *get_file_extension(char *file_name);
char *get_output_file(char *file_extension, int indicator);
int text_file_cipher_menu(void);
int binary_file_cipher_menu(void);

#endif
