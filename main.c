// Libraries and the other files
#include "flow_logic.h"
#include "helpers.h"
#include <stdio.h>


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
            return handling_text_encryption();
        }
        else if (type == 2)
        {
            // === File Encrypting ===
            return handling_file_encryption();            
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
            return handling_text_decryption();
        }
        else if (type == 2)
        {
            // === File Decrypting ===
            return handling_file_decryption();
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
