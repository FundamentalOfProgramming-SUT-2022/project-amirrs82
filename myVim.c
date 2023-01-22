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
        {

            createFile();
        }
        // else if (!strcmp(command, "insertstr"))
        //     insertstr();
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
    char c = getchar();
    if (c == '"')
    {
        if (!strcmp(command, "--file"))
        {
            gets(address);
        }
        else
        {
            printf("Wrong Command! Please try again\n");
            return;
        }
        address[strlen(address) - 1] = '\0';
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
    else if (c == '/')
    {
        if (!strcmp(command, "--file"))
        {
            gets(address);
        }
        else
        {
            printf("Wrong Command! Please try again\n");
            return;
        }
        tempAddress[0] = c;
        if (access(address, F_OK))
        {
            for (size_t i = 1; i <= strlen(address); i++)
            {
                if (address[i-1] == '/')
                {
                    mkdir(tempAddress);
                }
                tempAddress[i] = address[i-1];
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
    // }
}
