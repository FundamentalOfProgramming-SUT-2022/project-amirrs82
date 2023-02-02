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

char address[100], command[100], clipboard[1000], undoFileAddress[100][100], fileName[100], fileName2[100], armanText[100][1000], c;
FILE *myFile, *myTempFile, *undoFile[100];
int undoFileCounter = 0, armanCounter = 0;
bool arman = false;

void createFile();
void insert();
void cat();
void remove_();
void copy();
void cut();
void paste();
void find(bool replace);
void replace();
void grep();
void undo();
void closing_pairs();
void compare();
void tree();
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

        if (!strcmp(command, "createfile"))
            createFile();
        else if (!strcmp(command, "insertstr"))
            insert();
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
            replace();
        else if (!strcmp(command, "grep"))
            grep();
        else if (!strcmp(command, "undo"))
            undo();
        else if (!strcmp(command, "auto-indent"))
            closing_pairs();
        else if (!strcmp(command, "compare"))
            compare();
        else if (!strcmp(command, "tree"))
            tree();
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
                delete_from_array(arr, i + 1, strlen(arr));
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
    char text[1000];
    strcpy(undoFileAddress[undoFileCounter], address);
    strcat(undoFileAddress[undoFileCounter], "____undo.txt");
    remove(undoFileAddress[undoFileCounter]);
    undoFile[undoFileCounter] = fopen(undoFileAddress[undoFileCounter], "w");
    while (true)
    {
        fgets(text, 1000, myFile);
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
    char tempAddress[100];
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

void insert()
{
    int line, position, i = 0, j = 0, currentLine = 1;
    char text[1000], buffText[1000], c;
    memset(text, 0, sizeof(text));
    memset(text, 0, sizeof(buffText));
    scanf("%s", command); // file or arman skipped

    if (!openFile(2, "w+"))
        return;
    undoBackup();

    scanf("%s", command); // " --str" skipped
    modifyArray(text, 1);

    scanf("%s", command); //" --pos" skipped
    scanf("%d:%d", &line, &position);

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
    char text[1000];
    int i = 0;
    scanf("%s", command); // arman or --file skipped
    if (!strcmp(command, "=D"))
        arman = true;

    if (!openFile(1, "r"))
        return;
    while (true)
    {
        fgets(text, 1000, myFile);
        if (arman)
        {
            strcpy(armanText[armanCounter], text);
            armanCounter++;
        }
        else
            printf("%s", text);

        if (feof(myFile))
            break;
    }
    if (!arman)
        printf("\n");
}

void remove_()
{
    char text[1000], c;
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
    char text[1000], c;
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
    char text[1000], c;
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
    char text[1000], c;
    scanf("%s", command); //--file skipped

    if (!openFile(2, "w+"))
        return;
    undoBackup();

    scanf("%s", command); //--pos skipped
    scanf("%d:%d", &line, &position);

    while (true)
    {
        fgets(text, 1000, myFile);
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

void find(bool replace) // wildcard
{
    int i = 0, j = 0, k = 0, state = 0, byWordPosition[100], byCharPosition[100], end[100], at;
    char text[1000], text_to_find[1000], number[10], *stringPtr, *tokenPtr1, *tokenPtr2;
    bool star = false, flag = false;
    memset(byWordPosition, 0, sizeof(byWordPosition));
    memset(byCharPosition, 0, sizeof(byCharPosition));

    scanf("%s", command); // "--file" skipped
    if (!openFile(1, "r"))
        return;

    scanf("%s", command); // " --str" skipped

    if (modifyArray(text_to_find, 1))
        star = true;

    if (c != '\n')
        gets(command);
    if (strstr(command, "-count"))
        state += 1;
    if (strstr(command, "-at"))
    {
        state += 10;
        while (!isdigit(command[i]))
            i++;
        while (isdigit(command[i]))
        {
            number[j] = command[i];
            i++;
        }
        at = strtod(number, &stringPtr);
    }
    if (strstr(command, "-byword"))
        state += 100;
    if (strstr(command, "-all"))
        state += 1000;
    // 1, 10 ,100 ,1000 , 110, 1100 are possible states

    fgets(text, 1000, myFile);

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
            // tokenPtr1 = strtok(text_to_find, '*');
            // tokenPtr2 = strtok(NULL, '*');
        }
    }
    else
    {
        for (size_t i = 0; i < strlen(text); i++)
        {
            if (strstr(text + i, text_to_find))
            {

                byCharPosition[j] = strstr(text + i, text_to_find) - text;
                end[j] = byCharPosition[j] + strlen(text_to_find);

                if (byCharPosition[j] != 0 && i == 0)
                    i = byCharPosition[j];

                for (int k = byCharPosition[j]; k > -1; k--)
                {
                    if (text[k] == ' ' && text[k - 1] != ' ')
                        byWordPosition[j]++;
                }

                if (j)
                    i += byCharPosition[j] - byCharPosition[j - 1] - 1;
                j++;
                flag = true; // founded
            }
        }
        rewind(myFile);
    }

    switch (state)
    {
    case 0: // no options
        if (flag)
            printf("%d\n", byCharPosition[0]);
        else
            printf("-1\n");
        break;
    case 1: // count
        printf("%d\n", j);
        j = 0;
        break;
    case 10: // at
        if (at > j)
            printf("-1\n");
        else if (!at)
            printf("Invalid Position\n");
        else
            printf("%d\n", byCharPosition[at - 1]);
        break;
    case 100: // byword
        if (flag)
            printf("%d\n", byWordPosition[0]);
        else
            printf("-1\n");
        break;
    case 1000: // all
        if (flag)
        {
            for (size_t i = 0; i < j; i++)
            {
                if (i)
                    printf(", %d", byCharPosition[i]);
                else
                    printf("%d", byCharPosition[i]);
            }
            printf("\n");
        }
        else
            printf("-1\n");
        break;
    case 110: // at & byword
        if (at > j)
            printf("-1\n");
        else if (!at)
            printf("Invalid Position\n");
        else
            printf("%d\n", byWordPosition[at - 1]);
        break;
    case 1100: // byword & all
        if (flag)
        {
            for (size_t i = 0; i < j; i++)
            {
                if (i)
                    printf(", %d", byWordPosition[i]);
                else
                    printf("%d", byWordPosition[i]);
            }
            printf("\n");
        }
        else
            printf("-1\n");
        break;

    default:
        printf("Invalid combination\n");
        break;
    }
    fclose(myFile);
}

void replace() // wildcard
{
    int i = 0, j = 0, k = 0, state = 0, byWordPosition[100], byCharPosition[100], end[100], at;
    char text[1000], temptext[1000], text_to_find[1000], text_to_replace[1000], *stringPtr, number[10];
    bool star = false, flag = false;
    memset(byWordPosition, 0, sizeof(byWordPosition));

    scanf("%s", command); //" --file skipped"

    if (!openFile(2, "w+"))
        return;
    undoBackup();

    scanf("%s", command); // " --str1" skipped
    if (modifyArray(text_to_find, 1))
        star = true;
    scanf("%s", command); // " --str2" skipped
    modifyArray(text_to_replace, 2);

    if (c != '\n')
    {
        gets(command);
        if (strstr(command, "-at"))
        {
            state += 1;
            while (!isdigit(command[i]))
                i++;
            while (isdigit(command[i]))
            {
                number[j] = command[i];
                i++;
                j++;
            }
            at = strtod(number, &stringPtr);
        }
        if (strstr(command, "-all"))
            state += 10;
    }

    j = 0;

    fgets(text, 1000, myFile);

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
            // tokenPtr1 = strtok(text_to_find, '*');
            // tokenPtr2 = strtok(NULL, '*');
        }
    }
    else
    {
        for (size_t i = 0; i < strlen(text); i++)
        {
            if (strstr(text + i, text_to_find))
            {

                byCharPosition[j] = strstr(text + i, text_to_find) - text;
                end[j] = byCharPosition[j] + strlen(text_to_find);

                if (byCharPosition[j] != 0 && i == 0)
                    i = byCharPosition[j];

                for (int k = byCharPosition[j]; k > -1; k--)
                {
                    if (text[k] == ' ' && text[k - 1] != ' ')
                        byWordPosition[j]++;
                }

                if (j)
                    i += byCharPosition[j] - byCharPosition[j - 1] - 1;
                j++;
                flag = true; // founded
            }
        }
        rewind(myFile);
    }
    // printf("AFADS");
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
        // printf("TREE");
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

void grep()
{
    int i = 0, j = 0, state = 0, at;
    char text[1000], text_to_find[1000], fileName[100][1000], foundedText[100][1000], c;
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

    modifyArray(text_to_find, 1);

    scanf("%s", command); //--files skipped
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
        }
        if (!access(address, F_OK))
        {
            myFile = fopen(address, "r");
            while (true)
            {
                fgets(text, 1000, myFile);
                if (strstr(text, text_to_find))
                {
                    strcpy(fileName[j], address);
                    strcpy(foundedText[j], text);
                    flag = true;
                    // if (text[strlen(text) - 1] != '\n')
                    //     insert_to_array(foundedText[j], '\n', strlen(text), strlen(text)); // add enter if there is not one
                    j++;
                }
                if (feof(myFile))
                    break;
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
            for (size_t i = 0; i < j; i++)
                printf("%s: %s", fileName[i], foundedText[i]);
            printf("\n");
        }
        else
            printf("-1\n");
        break;

    case 1:
        printf("%d\n", j);
        break;
    case 10:
        if (flag)
        {
            for (size_t i = 0; i < j; i++)
            {
                printf("%s\n", fileName[i]);
                while (!strcmp(fileName[i], fileName[i + 1]))
                    i++;
            }
        }
        else
            printf("-1\n");
        break;

    default:
        break;
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
    char text[1000];
    int i = 0, j = 0, k = 0, tabCounter = 0, spaceCounter;
    bool flag = false, emptyContent;
    if (!openFile(2, "w+"))
        return;
    undoBackup();

    fgets(text, 1000, myFile);

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
    char text1[100][1000], text2[100][1000], text[1000];
    int i = 1, end1, end2, state, file_1_line = 0, file_2_line;
    bool different[100] = {false};

    if (!openFile(1, "r"))
        return;

    if (!openFile(0, "r"))
        return;

    while (true)
    {
        fgets(text1[i], 1000, myFile);
        fgets(text2[i], 1000, myTempFile);
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
                printf("========== #%d ==========", j);
                printf("%s%s", text1[j], text2[j]);
            }
        }
        printf("\n");
        break;
    case 1:
        for (size_t j = 1; j <= end2; j++)
        {
            if (different[j])
            {
                printf("========== #%d ==========\n", j);
                printf("%s%s", text1[j], text2[j]);
            }
        }
        printf("\n");
        while (true)
        {
            fgets(text1[i++], 1000, myFile);
            if (feof(myFile))
                break;
        }
        i--;
        for (size_t j = end2 + 1; j <= i; j++)
        {
            printf("<<<<<<<<<< #%d - #%d <<<<<<<<<<\n", j, i);
            printf("%s", text1[j]);
        }
        printf("\n");

        break;
    case 10:
        for (size_t j = 1; j <= end1; j++)
        {
            if (different[j])
            {
                printf("========== #%d ==========", j);
                printf("%s%s", text1[j], text2[j]);
            }
        }
        printf("\n");
        while (true)
        {
            fgets(text2[i++], 1000, myTempFile);
            if (feof(myTempFile))
                break;
        }
        i--;
        for (size_t j = end1 + 1; j <= i; j++)
        {
            printf(">>>>>>>>>> #%d - #%d >>>>>>>>>>", j, i);
            printf("%s", text2[j]);
        }
        printf("\n");
        break;

    default:
        break;
    }
    fclose(myFile);
    fclose(myTempFile);
}

void tree()
{
    int depth;
    scanf("%d", &depth);
    if (depth < -1)
        printf("Wrong depth\n");
}
