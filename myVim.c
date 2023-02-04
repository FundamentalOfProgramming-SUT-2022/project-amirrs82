#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#define MAX_SIZE 1000
#define SIZE 100

char address[SIZE], command[MAX_SIZE], clipboard[MAX_SIZE], undoFileAddress[SIZE][SIZE], fileName[SIZE], fileName2[SIZE], armanText[SIZE][MAX_SIZE], c;
FILE *myFile, *myTempFile, *undoFile[SIZE];
int undoFileCounter = 0, armanCounter = 0;
bool arman = false;

void createFile();
void insert(bool arman);
void cat();
void remove_();
void copy();
void cut();
void paste();
void find(bool arman);
void replace(bool arman);
void grep(bool arman);
void undo();
void closing_pairs();
void compare();
void tree(char *, int, int);
void armanInput();
int modifyArray(char arr[], int n);
void insert_to_array(char arr[], char c, int position, int size);
void delete_from_array(char arr[], int position, int size);
void undoBackup();
int openFile(int n, char *mode);

int main()
{
    scanf("%s", command);
    while ((strcmp(command, "exit")) && (strcmp(command, "quit")))
    {
        memset(address, 0, sizeof(address));
        memset(fileName, 0, sizeof(fileName));
        memset(fileName2, 0, sizeof(fileName2));
        memset(armanText, 0, sizeof(armanText));
        armanCounter = 0;

        if (!strcmp(command, "createfile"))
            createFile();
        else if (!strcmp(command, "insertstr"))
            insert(false);
        else if (!strcmp(command, "cat"))
            cat();
        else if (!strcmp(command, "removestr"))
            remove_();
        else if (!strcmp(command, "copystr"))
            copy();
        else if (!strcmp(command, "cutstr"))
            cut();
        else if (!strcmp(command, "pastestr"))
            paste();
        else if (!strcmp(command, "find"))
            find(false);
        else if (!strcmp(command, "replace"))
            replace(false);
        else if (!strcmp(command, "grep"))
            grep(false);
        else if (!strcmp(command, "undo"))
            undo();
        else if (!strcmp(command, "auto-indent"))
            closing_pairs();
        else if (!strcmp(command, "compare"))
            compare();
        else if (!strcmp(command, "tree"))
        {
            int depth;
            scanf("%d", &depth);
            if (depth < -1)
            {
                printf("Wrong depth\n");
                gets(command);
                scanf("%s", command);
                continue;
            }
            if ((c = getchar()) != '\n')
            {
                getchar();
                getchar();
                arman = true;
            }
            tree("../root", 0, depth);
        }
        else
        {
            printf("Wrong Command! Please try again\n");
            gets(command);
        }
        scanf("%s", command);
        arman = false;
    }
}

int modifyArray(char arr[], int n)
{
    bool star = false;
    int i = 0;
    getchar();
    c = getchar();
    if (c != '"')
    {
        arr[0] = c;
        i = 1;
        while (true)
        {
            c = getchar();
            if (c == ' ' || c == '\n')
                break;
            arr[i] = c;
            i++;
        }
        arr[i] = '\0';
    }
    else if (c == '"')
    {
        while (true)
        {
            if ((c = getchar()) == '"' && arr[i - 1] != '\\')
                break;
            arr[i] = c;
            i++;
        }
        arr[i] = '\0';
    }

    for (size_t i = 0; i < strlen(arr); i++)
    {
        if (arr[i] == '*' && arr[i - 1] != '\\')
            star = true;
        if (arr[i] == '\\')
        {
            if (arr[i + 1] == 'n' && arr[i - 1] != '\\')
            {
                arr[i] = '\n';
                delete_from_array(arr, i + 1, strlen(arr));
            }
            if (arr[i + 1] == '"' && arr[i - 1] != '\\')
            {
                arr[i] = '"';
                delete_from_array(arr, i + 1, strlen(arr));
            }
            if (arr[i + 1] == '*' && arr[i - 1] != '\\')
            {
                arr[i] = '*';
                delete_from_array(arr, i + 1, strlen(arr));
            }
            if (arr[i + 1] == '\\')
            {
                delete_from_array(arr, i, strlen(arr));
                while (arr[i] == '\\')
                    i++;
            }
        }
    }
    return star;
}

void insert_to_array(char arr[], char c, int position, int size)
{
    for (int i = size; i >= position; i--)
    {
        arr[i] = arr[i - 1];
    }
    arr[position] = c;
    arr[size + 1] = '\0';
}

void delete_from_array(char arr[], int position, int size)
{
    int i;
    for (i = position; i < size; i++)
        arr[i] = arr[i + 1];
    arr[i] = '\0';
}

void undoBackup()
{
    char text[MAX_SIZE];
    strcpy(undoFileAddress[undoFileCounter], address);
    strcat(undoFileAddress[undoFileCounter], "____undo.txt");
    remove(undoFileAddress[undoFileCounter]);
    undoFile[undoFileCounter] = fopen(undoFileAddress[undoFileCounter], "w");
    while (true)
    {
        fgets(text, MAX_SIZE, myFile);
        fputs(text, undoFile[undoFileCounter]);
        if (feof(myFile))
            break;
    }
    rewind(myFile);
    SetFileAttributes(undoFileAddress[undoFileCounter], FILE_ATTRIBUTE_HIDDEN);
    fclose(undoFile[undoFileCounter]);
    undoFileCounter++;
}

int openFile(int n, char *mode)
{
    int i = 0;
    getchar();
    char c = getchar();

    if (c == '/')
    {
        scanf("%s", address);
        insert_to_array(address, '/', 0, strlen(address));
    }

    else if (c == '"')
    {
        while ((c = getchar()) != '"')
        {
            address[i] = c;
            i++;
        }
        address[i] = '\0';
    }

    int fileAccess = access(address, F_OK);
    switch (fileAccess)
    {
    case 0:
        switch (n)
        {
        case 0:
            myTempFile = fopen(address, mode); // for compare
            break;
        case 1: // one file to open
            myFile = fopen(address, mode);
            break;
        case 2: // two files to open
            strcpy(fileName, address);
            strncpy(fileName2, address, strlen(address) - 4);
            strcat(fileName2, "__temp.txt");
            myFile = fopen(fileName, "r");
            myTempFile = fopen(fileName2, mode);
            break;
        default:
            break;
        }
        return 1;
        break;

    default:
        printf("The file does not exist\n");
        memset(address, '\0', sizeof(address));
        return 0;
        break;
    }
}

void createFile()
{
    FILE *myFile;
    char tempAddress[SIZE];
    int i;
    scanf("%s", command); // " --file skipped"
    getchar();            // space skipped
    char c = getchar();
    if (c == '/')
    {
        scanf("%s", address);
        insert_to_array(address, '/', 0, strlen(address));
    }
    else if (c == '"')
    {
        while ((c = getchar()) != '"')
        {
            address[i] = c;
            i++;
        }
        address[i] = '\0';
    }
    if (access(address, F_OK))
    {
        for (i = 0; i < strlen(address); i++)
        {
            if (address[i] == '/')
            {
                mkdir(tempAddress);
            }
            tempAddress[i] = address[i];
        }
        tempAddress[i] = '\0';
        myFile = fopen(tempAddress, "w");
        fclose(myFile);
        memset(tempAddress, 0, sizeof(tempAddress));
        return;
    }
    else
    {
        printf("The file already exists!\n");
        memset(tempAddress, 0, sizeof(tempAddress));
        return;
    }
}

void insert(bool arman)
{
    int line, position, i = 0, j = 0, currentLine = 1;
    char text[MAX_SIZE], buffText[MAX_SIZE];
    memset(text, 0, sizeof(text));
    memset(text, 0, sizeof(buffText));
    scanf("%s", command); // " --file" skipped

    if (!openFile(2, "w+"))
        return;
    undoBackup();

    if (arman)
    {
        for (size_t i = 0; i < armanCounter; i++)
            strcat(text, armanText[i]);
    }

    else
    {
        scanf("%s", command); // " --str" skipped
        modifyArray(text, 1);
    }

    arman = false;

    scanf("%s", command); //" --pos" skipped
    scanf("%d:%d", &line, &position);

    while (true)
    {
        fgets(buffText, MAX_SIZE, myFile);
        if (line == currentLine)
        {
            for (size_t i = 0; i < strlen(buffText); i++)
            {
                if (i == position)
                    fputs(text, myTempFile);
                fputc(buffText[i], myTempFile);
            }
            if (strlen(buffText) == position)
                fputs(text, myTempFile);
        }
        else
            fputs(buffText, myTempFile);

        currentLine++;

        if (feof(myFile))
            break;
    }
    fclose(myTempFile);
    fclose(myFile);
    remove(fileName);
    rename(fileName2, fileName);
}

void cat()
{
    char text[MAX_SIZE];
    int i = 0;
    scanf("%s", command); // --file skipped

    if (!openFile(1, "r"))
        return;
    c = getchar();
    if (c != '\n')
    {
        scanf("%s", command);
        arman = true;
    }
    while (true)
    {
        fgets(text, MAX_SIZE, myFile);
        sprintf(armanText[armanCounter], "%s", text);
        armanCounter++;
        if (feof(myFile))
            break;
    }
    fclose(myFile);

    if (arman)
        armanInput();
    else
    {
        for (size_t i = 0; i < armanCounter; i++)
            (printf("%s", armanText[i]));
    }
}

void remove_()
{
    char text[MAX_SIZE], c;
    int line, position, length, j = 0, i = 0, exactPosition = 0, currentLine = 1, flag = 1;
    scanf("%s", command); // " --file" skipped
    if (!openFile(2, "w+"))
        return;
    undoBackup();

    scanf("%s", command); // " --pos" skipped
    scanf("%d:%d", &line, &position);

    scanf("%s", command); // " -size" skipped
    scanf("%d", &length);

    while ((c = fgetc(myFile)) != EOF)
    {
        text[i] = c;

        if (line == currentLine && flag)
        {
            exactPosition = i + position;
            flag = 0;
        }

        if (c == '\n')
            currentLine++;
        i++;
    }
    text[i] = '\0';
    scanf("%s", command); // -f or -b
    if (strstr(command, "-f"))
    {
        for (size_t k = 0; k < length; k++)
            delete_from_array(text, exactPosition, strlen(text));
    }
    else if (strstr(command, "-b"))
    {
        for (size_t k = 0; k < length; k++)
        {
            delete_from_array(text, exactPosition, strlen(text));
            exactPosition--;
        }
    }

    for (size_t i = 0; i < strlen(text); i++)
        fputc(text[i], myTempFile);

    fclose(myTempFile);
    fclose(myFile);
    remove(fileName);
    rename(fileName2, fileName);
}

void copy()
{
    char text[MAX_SIZE], c;
    int line, position, length, j = 0, i = 0, exactPosition = 0, currentLine = 1, flag = 1;
    scanf("%s", command); // "--file" skipped
    if (!openFile(1, "r"))
        return;

    scanf("%s", command); // " --pos" skipped
    scanf("%d:%d", &line, &position);

    scanf("%s", command); // " -size" skipped
    scanf("%d", &length);
    while ((c = fgetc(myFile)) != EOF)
    {
        text[i] = c;
        if (line == currentLine && flag)
        {
            exactPosition = i + position;
            flag = 0;
        }
        i++;
        if (c == '\n')
            currentLine++;
    }
    text[i] = '\0';

    scanf("%s", command); // -f or -b
    if (strstr(command, "-f"))
    {
        for (i = exactPosition; i < exactPosition + length; i++, j++)
            clipboard[j] = text[i];
    }
    else if (strstr(command, "-b"))
    {
        for (i = exactPosition - length; i < exactPosition; i++, j++)
            clipboard[j] = text[i];
    }
    clipboard[j] = '\0';
    fclose(myFile);
}

void cut()
{
    char text[MAX_SIZE], c;
    int line, position, length, j = 0, i = 0, exactPosition = 0, currentLine = 1, flag = 1;
    scanf("%s", command); //--file skipped
    if (!openFile(2, "w+"))
        return;
    undoBackup();

    scanf("%s", command); // " --pos" skipped
    scanf("%d:%d", &line, &position);

    scanf("%s", command); // " -size" skipped
    scanf("%d", &length);

    while ((c = fgetc(myFile)) != EOF)
    {
        text[i] = c;
        if (line == currentLine && flag)
        {
            exactPosition = i + position;
            flag = 0;
        }
        i++;
        if (c == '\n')
            currentLine++;
    }
    text[i] = '\0';

    scanf("%s", command); // -f or -b

    if (strstr(command, "-f"))
    {
        for (i = exactPosition; i < exactPosition + length; i++, j++)
            clipboard[j] = text[i]; // copy
        clipboard[j] = '\0';

        for (size_t i = 0; i < length; i++)
            delete_from_array(text, exactPosition, strlen(text));
    }

    else if (strstr(command, "-b"))
    {
        for (i = exactPosition - length; i < exactPosition; i++, j++)
            clipboard[j] = text[i]; // copy
        clipboard[j] = '\0';

        for (size_t i = 0; i < length; i++)
            delete_from_array(text, exactPosition--, strlen(text));
    }
    else
        printf("Wrong command, Please try again\n");

    for (size_t i = 0; i < strlen(text); i++)
        fputc(text[i], myTempFile);

    fclose(myTempFile);
    fclose(myFile);
    remove(fileName);
    rename(fileName2, fileName);
}

void paste()
{
    int line, position, i = 0, j = 0, currentLine = 1;
    char text[MAX_SIZE], c;
    scanf("%s", command); //--file skipped

    if (!openFile(2, "w+"))
        return;
    undoBackup();

    scanf("%s", command); //--pos skipped
    scanf("%d:%d", &line, &position);

    while (true)
    {
        fgets(text, MAX_SIZE, myFile);
        if (line == currentLine)
        {
            for (size_t i = 0; i < strlen(text); i++)
            {
                if (i == position)
                    fputs(clipboard, myTempFile);
                fputc(text[i], myTempFile);
            }
            if (strlen(text) == position)
                fputs(clipboard, myTempFile);
        }
        else
            fputs(text, myTempFile);

        currentLine++;

        if (feof(myFile))
            break;
    }
    fclose(myTempFile);
    fclose(myFile);
    remove(fileName);
    rename(fileName2, fileName);
}

void find(bool arman)
{
    int i = 0, j = 0, k = 0, state = 0, byWordPosition[SIZE], byCharPosition[SIZE], end[SIZE], at;
    char text[MAX_SIZE], text_to_find[MAX_SIZE], number[10], *stringPtr, *tokenPtr1, *tokenPtr2;
    bool star = false, flag = false;
    memset(byWordPosition, 0, sizeof(byWordPosition));
    memset(byCharPosition, 0, sizeof(byCharPosition));

    scanf("%s", command); // "--file" skipped
    if (!openFile(1, "r"))
        return;

    if (arman)
    {
        c = getchar(); // for options
        for (size_t i = 0; i < armanCounter; i++)
            strcat(text_to_find, armanText[i]);
    }
    else
    {
        scanf("%s", command); // " --str" skipped
        if (modifyArray(text_to_find, 1))
            star = true;
    }

    arman = false;

    while (c != '\n')
    {
        scanf("%s", command);

        if (!strcmp(command, "-count"))
            state += 1;
        if (!strcmp(command, "-at"))
        {
            state += 10;
            scanf("%d", &at);
        }
        if (!strcmp(command, "-byword"))
            state += 100;
        if (!strcmp(command, "-all"))
            state += 1000;
        if (!strcmp(command, "=D"))
        {
            arman = true;
            break;
        }
        c = getchar();
    }
    // 1, 10 ,SIZE ,MAX_SIZE , 110, 1SIZE are possible states
    j = 0;
    i = 0;
    fgets(text, MAX_SIZE, myFile);

    if (star)
    {
        if (text_to_find[0] == '*')
        {
            delete_from_array(text_to_find, 0, strlen(text_to_find));
            for (i = 0; i < strlen(text); i++)
            {
                while (strncmp(text_to_find, text + i, strlen(text_to_find)) && i < strlen(text))
                    i++;
                if (i < strlen(text))
                {
                    i += strlen(text_to_find);
                    end[j] = i;
                    while (text[i] != ' ' && text[i] == '\0')
                        i++;

                    k = end[j] - strlen(text_to_find);

                    while (true)
                    {
                        if (text[k - 1] == ' ' || k == 0)
                            break;
                        k--;
                    }
                    byCharPosition[j] = k;
                    for (int k = byCharPosition[j]; k > -1; k--)
                    {
                        if ((text[k] == ' ' && text[k - 1] != ' ') || k == 0)
                            byWordPosition[j]++;
                    }
                    flag = true;
                    j++;
                }
            }
        }

        else if (text_to_find[strlen(text_to_find) - 1] == '*')
        {
            delete_from_array(text_to_find, strlen(text_to_find) - 1, strlen(text_to_find));
            for (i = 0; i < strlen(text); i++)
            {
                if (!strncmp(text_to_find, text + i, strlen(text_to_find)))
                {
                    byCharPosition[j] = i;

                    for (int k = byCharPosition[j]; k > -1; k--)
                    {
                        if ((text[k] == ' ' && text[k - 1] != ' ') || k == 0)
                            byWordPosition[j]++;
                    }

                    i = byCharPosition[j] + strlen(text_to_find);
                    while (text[i] != ' ' && i <= strlen(text) - 1)
                        i++;
                    end[j] = i;
                    flag = true;
                    j++;
                }
            }
        }

        else
        {
            char temp1[MAX_SIZE];
            char temp2[MAX_SIZE];
            while (true)
            {
                if (text_to_find[i] == '*' && text_to_find[i + 1] == ' ')
                {
                    temp1[i] = text_to_find[i];
                    i++;
                    break;
                }
                if (text_to_find[i] == '*')
                    break;

                temp1[i] = text_to_find[i];
                i++;
            }
            temp1[i] = '\0';

            j = i;

            while (text_to_find[i] != '\0')
            {
                temp2[k] = text_to_find[i];
                i++;
                k++;
            }
            temp2[k] = '\0';
            if (text_to_find[j - 1] == ' ')
            {
                j = 0;
                k = 0;
                delete_from_array(temp2, 0, strlen(temp2));
                for (i = 0; i < strlen(text); i++)
                {
                    while (strncmp(temp2, text + i, strlen(temp2)) && i < strlen(text))
                        i++;
                    if (i < strlen(text))
                    {
                        i += strlen(temp2);
                        end[j] = i;
                        while (text[i] != ' ' && text[i] != '\0')
                            i++;

                        k = end[j] - strlen(temp2);

                        while (true)
                        {
                            if (text[k - 1] == ' ' || k == 0)
                                break;
                            k--;
                        }
                        if (!strncmp(temp1, text + k - strlen(temp1), strlen(temp1)))
                        {
                            byCharPosition[j] = k - strlen(temp1);
                            for (int k = byCharPosition[j]; k > -1; k--)
                            {
                                if ((text[k] == ' ' && text[k - 1] != ' ') || k == 0)
                                    byWordPosition[j]++;
                            }
                            flag = true;
                            j++;
                        }
                    }
                }
            }

            else
            {
                j = 0;
                delete_from_array(temp1, strlen(temp1) - 1, strlen(temp1));
                for (i = 0; i < strlen(text); i++)
                {
                    if (!strncmp(temp1, text + i, strlen(temp1)))
                    {
                        byCharPosition[j] = i;

                        for (int k = byCharPosition[j]; k > -1; k--)
                        {
                            if ((text[k] == ' ' && text[k - 1] != ' ') || k == 0)
                                byWordPosition[j]++;
                        }

                        i = byCharPosition[j] + strlen(temp1);
                        while (text[i] != ' ' && i <= strlen(text) - 1)
                            i++;
                        if (!strncmp(text + i, temp2, strlen(temp2)))
                        {
                            i += strlen(temp2);
                            end[j] = i;
                            flag = true; // founded
                            j++;
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (size_t i = 0; i < strlen(text); i++)
        {
            if (!strncmp(text + i, text_to_find, strlen(text_to_find)))
            {
                byCharPosition[j] = i;
                end[j] = i + strlen(text_to_find);
                i = end[j];
                for (int k = byCharPosition[j]; k > -1; k--)
                {
                    if (text[k] == ' ' && text[k - 1] != ' ')
                        byWordPosition[j]++;
                }
                j++;
                flag = true; // founded
                while (text[i] != ' ' && i != strlen(text))
                    i++;
            }
        }
    }

    switch (state)
    {
    case 0: // no options
        if (flag)
            sprintf(armanText[0], "%d\n", byCharPosition[0]);
        else
            sprintf(armanText[0], "-1\n");
        armanCounter++;
        break;
    case 1: // count
        sprintf(armanText[0], "%d\n", j);
        j = 0;
        armanCounter++;
        break;
    case 10: // at
        if (at > j || at == 0)
            sprintf(armanText[0], "-1\n");
        else
            sprintf(armanText[0], "%d\n", byCharPosition[at - 1]);
        armanCounter++;
        break;
    case 100: // byword
        if (flag)
            sprintf(armanText[0], "%d\n", byWordPosition[0]);
        else
            sprintf(armanText[0], "-1\n");
        armanCounter++;
        break;
    case 1000: // all
        if (flag)
        {
            for (size_t i = 0; i < j; i++)
            {
                if (i)
                    sprintf(armanText[i], ", %d", byCharPosition[i]);
                else
                    sprintf(armanText[i], "%d", byCharPosition[i]);
                armanCounter++;
            }
            strcat(armanText[armanCounter], "\n");
        }
        else
        {
            sprintf(armanText[0], "-1\n");
            armanCounter++;
        }
        break;
    case 110: // at & byword
        if (at > j)
            sprintf(armanText[0], "-1\n");
        else
            sprintf(armanText[0], "%d\n", byWordPosition[at - 1]);
        armanCounter++;
        break;
    case 1100: // byword & all
        if (flag)
        {
            for (size_t i = 0; i < j; i++)
            {
                if (i)
                    sprintf(armanText[armanCounter], ", %d", byWordPosition[i]);
                else
                    sprintf(armanText[armanCounter], "%d", byWordPosition[i]);
                armanCounter++;
            }
            strcat(armanText[armanCounter], "\n");
        }
        else
        {
            sprintf(armanText[0], "-1\n");
            armanCounter++;
        }
        break;

    default:
        printf("Invalid combination\n");
        break;
    }

    fclose(myFile);

    if (arman)
        armanInput();
    else
    {
        for (size_t i = 0; i < armanCounter; i++)
            printf("%s", armanText[i]);
    }
}

void replace(bool arman)
{
    int i = 0, j = 0, k = 0, state = 0, byWordPosition[SIZE], byCharPosition[SIZE], end[SIZE], at;
    char text[MAX_SIZE], temptext[MAX_SIZE], text_to_find[MAX_SIZE], text_to_replace[MAX_SIZE], *stringPtr, number[10];
    bool star = false, flag = false;
    memset(byWordPosition, 0, sizeof(byWordPosition));

    scanf("%s", command); //" --file skipped"

    if (!openFile(2, "w+"))
        return;

    undoBackup();

    if (arman)
    {
        c = getchar(); // for options
        for (size_t i = 0; i < armanCounter; i++)
            strcat(text_to_find, armanText[i]);
    }

    else
    {
        scanf("%s", command); // " --str1" skipped
        if (modifyArray(text_to_find, 1))
            star = true;
    }

    arman = false;
    scanf("%s", command); // " --str2" skipped
    modifyArray(text_to_replace, 2);

    while (c != '\n')
    {
        scanf("%s", command);
        if (!strcmp(command, "-at"))
        {
            state = 1;
            scanf("%d", &at);
        }
        if (!strcmp(command, "-all"))
            state = 10;
        if (!strcmp(command, "=D"))
        {
            arman = true;
            break;
        }
        c = getchar();
    }

    j = 0;

    fgets(text, MAX_SIZE, myFile);

    if (star)
    {
        if (text_to_find[0] == '*')
        {
            delete_from_array(text_to_find, 0, strlen(text_to_find));
            for (i = 0; i < strlen(text); i++)
            {
                while (strncmp(text_to_find, text + i, strlen(text_to_find)) && i < strlen(text))
                    i++;
                if (i < strlen(text))
                {
                    i += strlen(text_to_find);
                    end[j] = i;
                    while (text[i] != ' ' && text[i] == '\0')
                        i++;

                    k = end[j] - strlen(text_to_find);

                    while (true)
                    {
                        if (text[k - 1] == ' ' || k == 0)
                            break;
                        k--;
                    }
                    byCharPosition[j] = k;

                    flag = true;
                    j++;
                }
            }
        }

        else if (text_to_find[strlen(text_to_find) - 1] == '*')
        {
            delete_from_array(text_to_find, strlen(text_to_find) - 1, strlen(text_to_find));
            for (i = 0; i < strlen(text); i++)
            {
                if (!strncmp(text_to_find, text + i, strlen(text_to_find)))
                {
                    byCharPosition[j] = i;

                    i = byCharPosition[j] + strlen(text_to_find);
                    while (text[i] != ' ' && i <= strlen(text) - 1)
                        i++;
                    end[j] = i;
                    flag = true;
                    j++;
                }
            }
        }

        else
        {
            char temp1[MAX_SIZE];
            char temp2[MAX_SIZE];
            while (true)
            {
                if (text_to_find[i] == '*' && text_to_find[i + 1] == ' ')
                {
                    temp1[i] = text_to_find[i];
                    i++;
                    break;
                }
                if (text_to_find[i] == '*')
                    break;

                temp1[i] = text_to_find[i];
                i++;
            }
            temp1[i] = '\0';

            j = i;

            while (text_to_find[i] != '\0')
            {
                temp2[k] = text_to_find[i];
                i++;
                k++;
            }
            temp2[k] = '\0';
            if (text_to_find[j - 1] == ' ')
            {
                j = 0;
                k = 0;
                delete_from_array(temp2, 0, strlen(temp2));
                for (i = 0; i < strlen(text); i++)
                {
                    while (strncmp(temp2, text + i, strlen(temp2)) && i < strlen(text))
                        i++;
                    if (i < strlen(text))
                    {
                        i += strlen(temp2);
                        end[j] = i;
                        while (text[i] != ' ' && text[i] != '\0')
                            i++;

                        k = end[j] - strlen(temp2);

                        while (true)
                        {
                            if (text[k - 1] == ' ' || k == 0)
                                break;
                            k--;
                        }
                        if (!strncmp(temp1, text + k - strlen(temp1), strlen(temp1)))
                        {
                            byCharPosition[j] = k - strlen(temp1);
                            flag = true;
                            j++;
                        }
                    }
                }
            }

            else
            {
                j = 0;
                delete_from_array(temp1, strlen(temp1) - 1, strlen(temp1));
                for (i = 0; i < strlen(text); i++)
                {
                    if (!strncmp(temp1, text + i, strlen(temp1)))
                    {
                        byCharPosition[j] = i;

                        i = byCharPosition[j] + strlen(temp1);
                        while (text[i] != ' ' && i <= strlen(text) - 1)
                            i++;
                        if (!strncmp(text + i, temp2, strlen(temp2)))
                        {
                            i += strlen(temp2);
                            end[j] = i;
                            flag = true; // founded
                            j++;
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (size_t i = 0; i < strlen(text); i++)
        {
            if (!strncmp(text + i, text_to_find, strlen(text_to_find)))
            {
                byCharPosition[j] = i;
                end[j] = i + strlen(text_to_find);
                i = end[j];
                j++;
                flag = true; // founded
                while (text[i] != ' ' && i != strlen(text))
                    i++;
            }
        }
    }
    rewind(myFile);
    switch (state)
    {
    case 0: // no options
        if (flag)
        {
            i = 0;
            while ((c = fgetc(myFile)) != EOF)
            {
                if (i == byCharPosition[0])
                    fputs(text_to_replace, myTempFile);
                if ((i < byCharPosition[0]) || (i >= end[0]))
                    fputc(c, myTempFile);
                i++;
            }
        }
        else
            printf("-1\n");
        break;
    case 1: // at
        if (flag)
        {
            if (at > j || !at)
            {
                printf("Invalid position\n");
                return;
            }
            i = 0;
            while ((c = fgetc(myFile)) != EOF)
            {
                if (i == byCharPosition[at - 1])
                    fputs(text_to_replace, myTempFile);
                if ((i < byCharPosition[at - 1] || i >= end[at - 1]))
                    fputc(c, myTempFile);
                i++;
            }
        }
        else
            printf("-1\n");
        break;
    case 10: // all
        if (flag)
        {
            j = 0;
            i = 0;
            while ((c = fgetc(myFile)) != EOF)
            {
                if (i == end[j])
                    j++;
                if (i == byCharPosition[j])
                    fputs(text_to_replace, myTempFile);
                if ((i < byCharPosition[j] || i >= end[j]))
                    fputc(c, myTempFile);
                i++;
            }
        }
        else
            printf("-1\n");
        break;
    default:
        printf("Invalid combination\n");
        break;
    }
    if (flag)
    {
        fclose(myTempFile);
        fclose(myFile);
        remove(fileName);
        rename(fileName2, fileName);
    }
    else
    {
        fclose(myTempFile);
        fclose(myFile);
        remove(fileName2);
    }
}

void grep(bool arman)
{
    int i = 0, j = 0, state = 0, at;
    char text[MAX_SIZE], text_to_find[MAX_SIZE], fileName[SIZE][MAX_SIZE], foundedText[SIZE][MAX_SIZE], c, tempC;
    bool flag = false;
    scanf("%s", command); // --str or option skipped

    switch (command[1])
    {
    case 'c':
        state = 1;
        scanf("%s", command);
        break;
    case 'l':
        state = 10;
        scanf("%s", command);
        break;
    default:
        break;
    }

    if (arman)
    {
        for (size_t i = 0; i < armanCounter; i++)
            strcat(text_to_find, armanText[i]);
    }
    else
        modifyArray(text_to_find, 1);
    arman = false;

    scanf("%s", command); // "--files" skipped
    j = 0;

    while (true)
    {
        i = 0;
        c = getchar();

        if (c == '\n')
            break;
        else if (c == ' ')
        {
            c = getchar();
            if (c == '/')
            {
                scanf("%s", address);
                insert_to_array(address, c, 0, strlen(address));
            }
            else if (c == '"')
            {
                while ((c = getchar()) != '"')
                {
                    address[i] = c;
                    i++;
                }
                address[i] = '\0';
            }
            else if (c == '=')
            {
                c = getchar();
                if (c == 'D')
                {
                    arman = true;
                    break;
                }
            }
        }
        if (!access(address, F_OK))
        {
            myFile = fopen(address, "r");
            while (true)
            {
                fgets(text, MAX_SIZE, myFile);
                printf("%s\n", text);
                if (strstr(text, text_to_find))
                {
                    strcpy(fileName[j], address);
                    strcpy(foundedText[j], text);
                    flag = true;
                    j++;
                }
                if (feof(myFile))
                {
                    fclose(myFile);
                    break;
                }
            }
        }
        else
            printf("The file does not exist\n");
    }

    switch (state)
    {
    case 0:
        if (flag)
        {
            for (i = 0; i < j; i++)
                sprintf(armanText[i], "%s: %s", fileName[i], foundedText[i]);
            strcat(armanText[i], "\n");
            armanCounter = i;
        }
        else
        {
            sprintf(armanText[0], "-1\n");
            armanCounter = 1;
        }
        break;

    case 1:
        sprintf(armanText[0], "%d\n", j);
        armanCounter++;
        break;
    case 10:
        if (flag)
        {
            for (i = 0; i < j; i++)
            {
                sprintf(armanText[i], "%s\n", fileName[i]);
                while (!strcmp(fileName[i], fileName[i + 1]))
                    i++;
            }
            armanCounter = i;
        }
        else
        {
            sprintf(armanText[0], "-1\n");
            armanCounter = 1;
        }
        break;

    default:
        break;
    }
    if (arman)
        armanInput();
    else
    {
        for (i = 0; i < armanCounter; i++)
            printf("%s", armanText[i]);
    }
}

void undo()
{
    scanf("%s", command); // --file skipped
    int i = 0;
    getchar();
    char c = getchar();

    if (c == '/')
    {
        scanf("%s", address);
        insert_to_array(address, '/', 0, strlen(address));
    }

    else if (c == '"')
    {
        while ((c = getchar()) != '"')
        {
            address[i] = c;
            i++;
        }
    }
    if (!access(address, F_OK))
    {
        while (strncmp(address, undoFileAddress[i], strlen(address)))
            i++;
        SetFileAttributes(undoFileAddress[i], FILE_ATTRIBUTE_NORMAL);
        if (!access(undoFileAddress[i], F_OK))
        {
            remove(address);
            rename(undoFileAddress[i], address);
        }
        else
            printf("faghat yek undo!\n");
    }
    else
        printf("The file does not exist!\n");
}

void closing_pairs()
{
    char text[MAX_SIZE];
    int i = 0, j = 0, k = 0, tabCounter = 0, spaceCounter;
    bool flag = false, emptyContent;
    if (!openFile(2, "w+"))
        return;
    undoBackup();

    fgets(text, MAX_SIZE, myFile);

    for (size_t i = 0; i < strlen(text); i++) // delete all whitspaces
    {
        if (text[i] == '{' || text[i] == '}')
        {
            j = i;
            while (text[--j] == ' ')
                delete_from_array(text, j, strlen(text));

            j += 2; // skip '{' or '}'
            while (text[j] == ' ')
                delete_from_array(text, j, strlen(text));
            i = --j;
        }
    }

    for (i; i < strlen(text); i++)
    {
        for (i; i < strlen(text); i++)
        {
            if (text[i] == '{')
            {
                tabCounter++;
                if (text[i - 1] == '}')
                {
                    insert_to_array(text, '\n', i++, strlen(text));
                    for (j = i; j < i + (tabCounter - 1) * 4; j++)
                        insert_to_array(text, ' ', j, strlen(text));
                    i = j;
                }
                if (!flag)
                    insert_to_array(text, ' ', i++, strlen(text));
                insert_to_array(text, '\n', ++i, strlen(text));
                for (j = ++i; j <= tabCounter * 4 + i; j++)
                    insert_to_array(text, ' ', j, strlen(text));

                i += 4 * tabCounter;
                if (text[i + 1] == '{')
                    flag = true;
                else
                    flag = false;
                if (text[i + 1] == '}')
                    emptyContent = true;
                spaceCounter = 0;
            }
            if (text[i] == '}')
            {
                tabCounter--;
                if (emptyContent)
                {
                    for (k = i; k > i - 4; k--)
                        delete_from_array(text, k - 1, strlen(text));
                    emptyContent = false;
                    i -= 4;
                }
                else
                {
                    insert_to_array(text, '\n', i++, strlen(text));
                    for (j = i++; j < (tabCounter)*4 + i; j++)
                        insert_to_array(text, ' ', j, strlen(text));
                    i += 4 * tabCounter;
                }
                if (text[i + 1] != '{' && text[i + 1] != '}')
                {
                    insert_to_array(text, '\n', ++i, strlen(text));
                    for (j = ++i; j <= (tabCounter)*4 + i; j++)
                        insert_to_array(text, ' ', j, strlen(text));
                    i += 4 * tabCounter;
                }
            }
        }
        fputs(text, myTempFile);
        fclose(myFile);
        fclose(myTempFile);
        remove(fileName);
        rename(fileName2, fileName);
    }
}

void compare()
{
    char text1[SIZE][MAX_SIZE], text2[SIZE][MAX_SIZE], text[MAX_SIZE];
    int i = 1, end1, end2, state, file_1_line = 0, file_2_line;
    bool different[SIZE] = {false};

    if (!openFile(1, "r"))
        return;

    if (!openFile(0, "r"))
        return;

    c = getchar();
    if (c != '\n')
    {
        scanf("%s", command); // =D
        arman = true;
    }
    while (true)
    {
        fgets(text1[i], MAX_SIZE, myFile);
        fgets(text2[i], MAX_SIZE, myTempFile);
        if (strcmp(text1[i], text2[i]))
            different[i] = true;
        i++;

        if (feof(myFile) && feof(myTempFile)) // equal lines
        {
            state = 0;
            break;
        }

        if (!feof(myFile) && feof(myTempFile)) // first file has more lines
        {
            state = 1;
            end2 = i - 1;
            break;
        }

        if (feof(myFile) && !feof(myTempFile)) // second file has more lines
        {
            state = 10;
            end1 = i - 1;
            break;
        }
    }
    switch (state)
    {
    case 0:
        for (size_t j = 1; j < i; j++)
        {
            if (different[j])
            {
                sprintf(armanText[armanCounter], "========== #%d ==========", j);
                sprintf(armanText[armanCounter + 1], "%s%s", text1[j], text2[j]);
                armanCounter += 2;
            }
        }
        strcat(armanText[armanCounter - 1], "\n");
        break;
    case 1:
        for (size_t j = 1; j <= end2; j++)
        {
            if (different[j])
            {
                sprintf(armanText[armanCounter], "========== #%d ==========\n", j);
                sprintf(armanText[armanCounter + 1], "%s%s", text1[j], text2[j]);
                armanCounter += 2;
            }
        }
        strcat(armanText[armanCounter - 1], "\n");

        while (true)
        {
            fgets(text1[i++], MAX_SIZE, myFile);
            if (feof(myFile))
                break;
        }
        i--;
        for (size_t j = end2 + 1; j <= i; j++)
        {
            sprintf(armanText[armanCounter], "<<<<<<<<<< #%d - #%d <<<<<<<<<<\n", j, i);
            sprintf(armanText[armanCounter + 1], "%s", text1[j]);
            armanCounter += 2;
        }
        strcat(armanText[armanCounter - 1], "\n");

        break;
    case 10:
        for (size_t j = 1; j <= end1; j++)
        {
            if (different[j])
            {
                sprintf(armanText[armanCounter], "========== #%d ==========", j);
                sprintf(armanText[armanCounter + 1], "%s%s", text1[j], text2[j]);
                armanCounter += 2;
            }
        }
        strcat(armanText[armanCounter - 1], "\n");

        while (true)
        {
            fgets(text2[i++], MAX_SIZE, myTempFile);
            if (feof(myTempFile))
                break;
        }
        i--;
        for (size_t j = end1 + 1; j <= i; j++)
        {
            sprintf(armanText[armanCounter], ">>>>>>>>>> #%d - #%d >>>>>>>>>>", j, i);
            sprintf(armanText[armanCounter + 1], "%s", text2[j]);
            armanCounter += 2;
        }
        strcat(armanText[armanCounter - 1], "\n");
        break;

    default:
        break;
    }

    fclose(myFile);
    fclose(myTempFile);

    if (arman)
        armanInput();
    else
    {
        for (size_t i = 0; i < armanCounter; i++)
            printf("%s", armanText[i]);
    }
}

void tree(char *basePath, int currentDepth, int depth) // arman
{
    char next_path[MAX_SIZE], c, text[MAX_SIZE], tempPath[MAX_SIZE];
    memset(next_path, 0, MAX_SIZE);

    struct dirent *current_object;
    DIR *folder = opendir(basePath);

    if (!folder || currentDepth == depth)
        return;

    while ((current_object = readdir(folder)) != NULL)
    {
        strcpy(tempPath, basePath);
        strcat(tempPath, "/");
        strcat(tempPath, current_object->d_name);
        if (GetFileAttributesA(tempPath) != FILE_ATTRIBUTE_HIDDEN)
        {
            if (strcmp(current_object->d_name, ".") && strcmp(current_object->d_name, ".."))
            {

                for (int i = 0; i < 2 * currentDepth; i++)
                {
                    if (i % 2 == 0)
                        sprintf(armanText[armanCounter], "%c", 179);
                    else
                        sprintf(armanText[armanCounter], " ");
                }

                sprintf(armanText[armanCounter], "%c%c%c %s\n", 195, 196, 196, current_object->d_name);

                strcpy(next_path, basePath);
                strcat(next_path, "/");
                strcat(next_path, current_object->d_name);
                armanCounter++;
                tree(next_path, currentDepth + 1, depth);
            }
        }
    }

    if (arman)
        armanInput();
    else
    {
        for (size_t i = 0; i < armanCounter; i++)
            printf("%s", armanText[i]);
    }

    closedir(folder);
}

void armanInput()
{
    scanf("%s", command);
    if (!strcmp(command, "insertstr"))
        insert(true);
    if (!strcmp(command, "find"))
        find(true);
    if (!strcmp(command, "replace"))
        replace(true);
    if (!strcmp(command, "grep"))
        grep(true);
}
