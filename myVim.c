#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
char address[100], tempAddress[100], command[100], c;
int i = 0, j = 0;
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

// void hidingFile
// {
//     DWORD attributes = GetFileAttributes(tempAddress);
//     SetFileAttributes(tempAddress, attributes + FILE_ATTRIBUTE_HIDDEN);
// }

void createFile();
void insert();
void cat();
void remove_();

int main()
{
    scanf("%s", command);
    while ((strcmp(command, "exit")) && (strcmp(command, "quit")))
    {
        if (!strcmp(command, "createfile"))
            createFile();
        else if (!strcmp(command, "insertstr"))
            insert();
        else if (!strcmp(command, "cat"))
            cat();
        else if (!strcmp(command, "removestr"))
            remove_();
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

void insert()
{
    FILE *myTempFile;
    int line, position;
    char buffText[1000], text[1000], number[10], tempFileName[100], tempFileName2[100], *stringPtr;
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
            strcpy(tempFileName, tempAddress);
            strncpy(tempFileName2, tempAddress, strlen(tempAddress) - 4);
            strcat(tempFileName2, "__temp.txt");
            myFile = fopen(tempFileName, "r");
            myTempFile = fopen(tempFileName2, "w+");
            tempAddress[j - 6] = c;
            j = 0;
            i++;
            if (address[i] == '"')
                quotation = true;
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
                        for (j; j < strlen(text); j++) // shift
                            text[j + 1] = text[j + 2];
                        text[j] = '\0';
                    }
                    if (text[i + 1] == '"' && text[i - 1] != '\\')
                    {
                        text[i] = '"';
                        j = i;
                        for (j; j < strlen(text); j++) // shift
                            text[j + 1] = text[j + 2];
                        text[j] = '\0';
                    }
                    if (text[i + 1] == '\\')
                    {
                        j = i;
                        for (j; j < strlen(text); j++) // shift
                            text[j + 1] = text[j + 2];
                        text[j] = '\0';
                    }
                }
            }
            int currentLine = 1;
            while (true)
            {
                fgets(buffText, 1000, myFile);
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
                if (feof(myFile))
                    break;
            }
            fclose(myTempFile);
            fclose(myFile);
            remove(tempFileName);
            rename(tempFileName2, tempFileName);
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
            strcpy(tempFileName, tempAddress);
            strncpy(tempFileName2, tempAddress, strlen(tempAddress) - 4);
            strcat(tempFileName2, "__temp.txt");
            myFile = fopen(tempFileName, "r");
            myTempFile = fopen(tempFileName2, "w+");
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
                    for (j; j < strlen(text); j++) // shift
                        text[j + 1] = text[j + 2];
                    text[j] = '\0';
                }
                if (text[i + 1] == '"' && text[i - 1] != '\\')
                {
                    text[i] = '"';
                    j = i;
                    for (j; j < strlen(text); j++) // shift
                        text[j + 1] = text[j + 2];
                    text[j] = '\0';
                }
                if (text[i + 1] == '\\')
                {
                    j = i;
                    for (j; j < strlen(text); j++) // shift
                        text[j + 1] = text[j + 2];
                    text[j] = '\0';
                }
            }
        }
        int currentLine = 1;
        while (true)
        {
            fgets(buffText, 1000, myFile);
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
            if (feof(myFile))
                break;
        }
        fclose(myTempFile);
        fclose(myFile);
        remove(tempFileName);
        rename(tempFileName2, tempFileName);
    }
    memset(tempAddress, '\0', sizeof(tempAddress));
    memset(address, '\0', sizeof(address));
    memset(text, '\0', sizeof(text));
    i = 0;
}

void cat()
{
    char buffText[1000];
    scanf("%s", command);
    if (!strcmp(command, "--file"))
    {
        getchar();
        gets(address);
        if (!access(address, F_OK))
        {
            myFile = fopen(address, "r");
            while (true)
            {
                fgets(buffText, 1000, myFile);
                printf("%s", buffText);
                if (feof(myFile))
                    break;
            }
            printf("\n");
        }
    }
    else
        printf("wrong command, Please try again");
}

void remove_()
{
    FILE *myTempFile;
    char number[10], tempAddress[100], buffText[1000], tempFileName[100], tempFileName2[100];
    int line, position, length, j = 0, i = 0, exactPosition = 0;
    scanf("%s", command);
    if (!strcmp(command, "--file"))
    {
        getchar(); // for space
        gets(address);
    }
    else
    {
        printf("Wrong command, please try again");
        memset(address, '\0', sizeof(address));
        return;
    }
    if (address[0] == '/')
    {
        quotation = false;
        for (i; i < strlen(address); i++, j++)
        {
            if (strstr(tempAddress, " --pos"))
            {
                c = tempAddress[j - 6];
                tempAddress[j - 6] = '\0';
                break;
            }
            tempAddress[i] = address[i];
        }
    }
    else if (address[0] == '"')
    {
        i = 1;
        quotation = true;
        for (i; i < strlen(address); i++)
        {
            if (address[i] == '"')
            {
                i++;
                break;
            }
            tempAddress[i - 1] = address[i];
        }
        for (int k = 0; k < 6; i++, k++)
            tempAddress[i - 2] = address[i];
        c = tempAddress[i - 8];
        tempAddress[i - 8] = '\0';
        i -= 2;
        j = i;
    }
    if (!access(tempAddress, F_OK))
    {
        strcpy(tempFileName, tempAddress);
        strncpy(tempFileName2, tempAddress, strlen(tempAddress) - 4);
        strcat(tempFileName2, "__temp.txt");
        myFile = fopen(tempFileName, "r");
        myTempFile = fopen(tempFileName2, "w+");
        tempAddress[j - 6] = c;
        j = 0;
        if (quotation)
        {
            for (i; i < strlen(address); i++)
            {
                if (strstr(tempAddress, ":"))
                    break;
                tempAddress[i] = address[i + 2];
                number[j] = address[i + 2];
                j++;
            }
            number[j] = '\0';
        }
        else
        {
            for (i; i < strlen(address); i++)
            {
                if (strstr(tempAddress, ":"))
                    break;
                tempAddress[i] = address[i];
                number[j] = address[i];
                j++;
            }
            number[j] = '\0';
        }
        j = 0;

        char *ptr;
        line = strtod(number, &ptr);
        if (quotation)
        {
            for (i; i < strlen(address); i++)
            {
                if (strstr(tempAddress, " -size"))
                    break;
                tempAddress[i] = address[i + 2];
                number[j] = address[i + 2];
                j++;
            }
        }
        else
        {
            for (i; i < strlen(address); i++)
            {
                if (strstr(tempAddress, " -size"))
                    break;
                tempAddress[i] = address[i];
                number[j] = address[i];
                j++;
            }
        }
        number[j - 6] = '\0';
        j = 0;
        position = strtod(number, &ptr);
        i++;
        if (quotation)
        {
            while (isdigit(address[i + 2]))
            {
                number[j] = address[i + 2];
                i++;
                j++;
            }
        }
        else
        {
            while (isdigit(address[i]))
            {
                number[j] = address[i];
                i++;
                j++;
            }
        }
        number[j] = '\0';
        j = 0;
        i += 2; // space and dash skipped
        length = strtod(number, &ptr);
        int currentLine = 1, flag = 1;
        while (true)
        {
            char c = fgetc(myFile);
            if (feof(myFile))
                break;
            buffText[j] = c;
            if (line == currentLine && flag)
            {
                exactPosition = j + position;
                flag = 0;
            }
            j++;
            if (c == '\n')
                currentLine++;
        }
        if (quotation)
            i += 2;
        if (address[i] == 'f')
        {
            for (size_t k = 0; k < length; k++)
            {
                for (j = exactPosition; j < strlen(buffText); j++)
                {
                    buffText[j] = buffText[j + 1];
                    i = j;
                }
                buffText[j] = '\0';
            }
        }
        else if (address[i] == 'b')
        {
            for (size_t k = 0; k < length; k++)
            {
                for (j = exactPosition; j < strlen(buffText); j++)
                {
                    buffText[j] = buffText[j + 1];
                }
                exactPosition--;
                buffText[j] = '\0';
            }
        }
        else
        {
            printf("Wrong command, Please try again\n");
            memset(address, '\0', sizeof(address));
            memset(tempAddress, '\0', sizeof(tempAddress));
            return;
        }
        for (size_t i = 0; i < strlen(buffText); i++)
            fputc(buffText[i], myTempFile);

        fclose(myTempFile);
        fclose(myFile);
        remove(tempFileName);
        rename(tempFileName2, tempFileName);
    }
    else
        printf("The file does not exist!\n");
    memset(address, '\0', sizeof(address));
    memset(tempAddress, '\0', sizeof(tempAddress));
    memset(tempFileName, '\0', sizeof(tempFileName));
    memset(tempFileName2, '\0', sizeof(tempFileName2));
}
