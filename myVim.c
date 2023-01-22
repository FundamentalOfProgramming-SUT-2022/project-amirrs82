#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
char address[100], tempAddress[100], command[100];

void createFile();
void insertStr()
{
}
int main()
{
    scanf("%s", command);
    while ((strcmp(command, "exit")) && (strcmp(command, "quit")))
    {
        if (!strcmp(command, "createfile"))
            createFile();
        else if (!strcmp(command, "insertstr"))
            insertStr();
        else
            printf("Wrong Command! Please try again\n");
        scanf("%s", command);
    }
}
void createFile()
{
    char address[100], tempAddress[100], command[100];
    scanf("%s", command);
    getchar();
    // char c = getchar();
    if (!strcmp(command, "--file"))
    {
        gets(address);
    }
    else
    {
        printf("Wrong Command! Please try again\n");
        return;
    }
    if (address[0] == '"')
    {
        address[strlen(address) - 1] = '\0';
        for (size_t i = 0; i < strlen(address); i++)
        {
            if (i == strlen(address) - 1)
            {
                address[i] = '\0';
                break;
            }
            address[i] = address[i + 1];
        }

        if (access(address, F_OK))
        {
            for (size_t i = 0; i < strlen(address); i++)
            {
                if (address[i] == '/')
                {
                    mkdir(tempAddress);
                }
                tempAddress[i] = address[i];
            }
            tempAddress[strlen(address)] = '\0';
            FILE *myfile = fopen(tempAddress, "w");
            memset(tempAddress, 0, sizeof(tempAddress));
            fclose(myfile);
            return;
        }
        else
        {
            printf("The file already exists!\n");
            return;
        }
    }
    else if (address[0] == '/')
    {
        // tempAddress[0] = c;
        if (access(address, F_OK))
        {
            for (size_t i = 0; i <= strlen(address); i++)
            {
                if (address[i] == '/')
                {
                    mkdir(tempAddress);
                }
                tempAddress[i] = address[i];
            }
            FILE *myfile = fopen(tempAddress, "w");
            memset(tempAddress, 0, sizeof(tempAddress));
            fclose(myfile);
            return;
        }
        else
        {
            printf("The file already exists!\n");
            return;
        }
    }
}
