#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
char address[100], tempAddress[100], command[100], text[1000], buffText[1000], temp[100], number[10], clipboard[100], *stringPtr, c;
int i = 0, j = 0, line, position;
bool quotation;
FILE *myFile;
// void shift(char arr[], int currentLocation)
// {
//     for (size_t i = currentLocation; i < strlen(arr); i++)
//     {
//         arr[i] = arr[i + 1];
//     }
//     arr[i] = '\0';
// }
void createFile();
void insertStr()
{
    bool quotation;
    char address[100], tempAddress[100], command[100], temp[100], text[1000], buffText[1000], number[10], *stringPtr;
    int i = 0, j = 0, line, position;
    FILE *myTempFile;
    scanf("%s", command);
    getchar();
    if (!strcmp(command, "--file"))
    {
        gets(address);
    }
    else
    {
        printf("Wrong Command! Please try again\n");
        return;
    }
    if (address[0] == '/')
    {
        for (i; i < strlen(address); i++)
        {
            if (strstr(tempAddress, " --str"))
            {
                c = tempAddress[j - 6];
                tempAddress[j - 6] = '\0';
                break;
            }
            tempAddress[i] = address[i];
            j++;
        }
        if (!access(tempAddress, F_OK))
        {
            myFile = fopen(tempAddress, "r");
            strncpy(temp, tempAddress, strlen(tempAddress) - 4);
            strcat(temp, "_temp.txt");
            strcpy(clipboard, temp);
            myTempFile = fopen(temp, "w+");
            strcpy(temp, tempAddress);
            tempAddress[j - 6] = c;
            j = 0;
            i++;
            if (address[i] == '"')
            {
                quotation = true;
            }
            else
                quotation = false;
            for (i; i < strlen(address); i++)
            {
                if (strstr(tempAddress, " --pos"))
                {
                    if (quotation)
                        text[j - 7] = '\0';
                    else
                        text[j - 6] = '\0';
                    break;
                }
                if (quotation)
                {
                    tempAddress[i - 1] = address[i + 1];
                    text[j] = address[i + 1];
                }
                else
                {
                    tempAddress[i - 1] = address[i];
                    text[j] = address[i];
                }

                j++;
            }
            j = 0;
            i++;
            for (i; i < strlen(address); i++, j++)
            {
                if (address[i] == ':')
                {
                    i++;
                    break;
                }
                number[j] = address[i];
            }
            number[j] = '\0';
            j = 0;
            line = strtod(number, &stringPtr);
            for (i; i < strlen(address); i++, j++)
            {
                if (address[i] == '\0')
                    break;
                number[j] = address[i];
            }
            number[j] = '\0';
            position = strtod(number, &stringPtr);
            for (size_t i = 0; i < strlen(text); i++)
            {
                if (text[i] == '\\')
                {
                    if (text[i + 1] == 'n' && text[i - 1] != '\\')
                    {
                        text[i] = '\n';
                        j = i;
                        // shift(text,i);
                        for (j; j < strlen(text); j++)
                            text[j + 1] = text[j + 2];
                        text[j] = '\0';
                    }
                    if (text[i + 1] == '"' && text[i - 1] != '\\')
                    {
                        text[i] = '"';
                        j = i;
                        // shift(text,i);
                        for (j; j < strlen(text); j++)
                            text[j + 1] = text[j + 2];
                        text[j] = '\0';
                    }
                    if (text[i + 1] == '\\')
                    {
                        j = i;
                        // shift(text,i);
                        for (j; j < strlen(text); j++)
                            text[j + 1] = text[j + 2];
                        text[j] = '\0';
                    }
                }
            }
            int currentLine = 1;
            while (true)
            {
                fgets(buffText, 1000, myFile);
                if (feof(myFile))
                    break;
                if (line == currentLine)
                {
                    for (size_t i = 0; i < strlen(buffText); i++)
                    {
                        if (i == position)
                            fputs(text, myTempFile);
                        fputc(buffText[i], myTempFile);
                    }
                    if (strlen(buffText) == position)
                    {
                        fputs(text, myTempFile);
                    }
                }
                else
                {
                    fputs(buffText, myTempFile);
                }
                currentLine++;
            }
            if (currentLine == line)
            {
                fputc('\n', myTempFile);
                fputs(text, myTempFile);
            }
            fclose(myTempFile);
            fclose(myFile);
            remove(temp);
            rename(clipboard, temp);
            // memset()
        }
        else
        {
            printf("The file does not exist\n");
            memset(tempAddress, '\0', sizeof(tempAddress));
            memset(address, '\0', sizeof(address));
            memset(text, '\0', sizeof(text));
            return;
        }
        i = 0;
    }
    else if (address[0] == '"')
    {
        i = 1;
        for (i; i < strlen(address); i++)
        {
            if (address[i] == '"')
            {
                i++;
                break;
            }
            tempAddress[i - 1] = address[i];
        }
        i -= 2;
        tempAddress[i] = '\0';
        for (i; i < strlen(address); i++)
        {
            if (strstr(tempAddress, " --str"))
            {
                break;
            }
            tempAddress[i] = address[i + 2];
        }
        c = tempAddress[i - 6];
        tempAddress[i - 6] = '\0';
        if (!access(tempAddress, F_OK))
        {
            myFile = fopen(tempAddress, "r");
        }
        else
        {
            printf("The file does not exist\n");
            memset(tempAddress, '\0', sizeof(tempAddress));
            memset(address, '\0', sizeof(address));
            memset(text, '\0', sizeof(text));
            return;
        }
        tempAddress[i - 6] = c;
        if (address[i + 3] == '"')
        {
            quotation = true;
            i++;
        }
        else
            quotation = false;
        for (i; i < strlen(address); i++)
        {
            if (strstr(tempAddress, " --pos"))
            {
                if (quotation)
                {
                    text[j - 8] = '\0';
                    text[j] = '\0';
                }
                else
                {
                    text[j - 7] = '\0';
                }
                break;
            }
            text[j] = address[i + 3];
            tempAddress[i - 1] = address[i + 2];
            j++;
        }
        j = 0;
        i++;
        for (i; i < strlen(address); i++, j++)
        {
            if (address[i + 1] == ':')
                break;
            number[j] = address[i + 1];
        }
        number[j] = '\0';
        j = 0;
        line = strtod(number, &stringPtr);
        for (i; i < strlen(address); i++, j++)
        {
            if (address[i + 2] == '\0')
                break;
            number[j] = address[i + 2];
        }
        number[j] = '\0';
        j = 0;
        position = strtod(number, &stringPtr);
        for (size_t i = 0; i < strlen(text); i++)
        {
            if (text[i] == '\\')
            {
                if (text[i + 1] == 'n' && text[i - 1] != '\\')
                {
                    text[i] = '\n';
                    j = i;
                    // shift(text,i);
                    for (j; j < strlen(text); j++)
                        text[j + 1] = text[j + 2];
                    text[j] = '\0';
                }
                if (text[i + 1] == '"' && text[i - 1] != '\\')
                {
                    text[i] = '"';
                    j = i;
                    // shift(text,i);
                    for (j; j < strlen(text); j++)
                        text[j + 1] = text[j + 2];
                    text[j] = '\0';
                }
                if (text[i + 1] == '\\')
                {
                    j = i;
                    // shift(text,i);
                    for (j; j < strlen(text); j++)
                        text[j + 1] = text[j + 2];
                    text[j] = '\0';
                }
            }
        }
        fprintf(myFile, "%s", text);
        fclose(myFile);
    }
    memset(tempAddress, '\0', sizeof(tempAddress));
    memset(address, '\0', sizeof(address));
    memset(text, '\0', sizeof(text));
}
int main()
{
    scanf("%s", command);
    while ((strcmp(command, "exit")) && (strcmp(command, "quit")))
    {
        if (!strcmp(command, "createfile"))
            createFile();
        else if (!strcmp(command, "insertstr"))
        {
            insertStr();
        }
        else
            printf("Wrong Command! Please try again\n");
        scanf("%s", command);
    }
}
void createFile()
{
    scanf("%s", command);
    getchar();
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
            myFile = fopen(tempAddress, "w");
            memset(tempAddress, 0, sizeof(tempAddress));
            fclose(myFile);
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
            myFile = fopen(tempAddress, "w");
            memset(tempAddress, 0, sizeof(tempAddress));
            fclose(myFile);
            return;
        }
        else
        {
            printf("The file already exists!\n");
            return;
        }
    }
}
