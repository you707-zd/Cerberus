#ifndef FLOW_LOGIC_H
#define FLOW_LOGIC_H

int handling_text_encryption(void);
int handling_text_decryption(void);
int handling_file_encryption(void);
int handling_file_decryption(void);
int file_input_handling(int file_type_selected, FILE **input_file, char *name_input);
int file_output_handling(int file_type_selected, FILE **output_file, char *name_output);

#endif
