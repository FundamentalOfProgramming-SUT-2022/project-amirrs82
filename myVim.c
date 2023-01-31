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

char address[100], command[100], clipboard[1000], tempFileName[100], tempFileName2[100], tempFileName3[100], c;
FILE *myFile, *myTempFile;

void createFile();
void insert();
void cat();
void remove_();
void copy();
void cut();
void paste();
void find();
void replace();
void grep();
void undo();
void closing_pairs();
void modifyArray(char arr[], int n);
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
        memset(tempFileName, 0, sizeof(tempFileName));
        memset(tempFileName2, 0, sizeof(tempFileName2));

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
            find();
        else if (!strcmp(command, "replace"))
            replace();
        else if (!strcmp(command, "grep"))
            grep();
        else if (!strcmp(command, "undo"))
            undo();
        else if (!strcmp(command, "auto-indent"))
            closing_pairs();
        else
            printf("Wrong Command! Please try again\n");
        scanf("%s", command);
    }
}

void modifyArray(char arr[], int n)
{
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
    char buffText[1000];
    FILE *undoFile;
    strncpy(tempFileName3, address, strlen(address) - 4);
    strcat(tempFileName3, "____temp.txt");
    undoFile = fopen(tempFileName3, "w");
    while (true)
    {
        fgets(buffText, 1000, myFile);
        fputs(buffText, undoFile);
        if (feof(myFile))
            break;
    }
    rewind(myFile);
    SetFileAttributes(tempFileName3, FILE_ATTRIBUTE_HIDDEN);
    fclose(undoFile);
}

int openFile(int n, char *mode)
{
    int i = 0;
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

    int fileAccess = access(address, F_OK);
    switch (fileAccess)
    {
    case 0:
        switch (n)
        {
        case 1: // one file to open
            myFile = fopen(address, mode);
            break;
        case 2: // two files to open
            strcpy(tempFileName, address);
            strncpy(tempFileName2, address, strlen(address) - 4);
            strcat(tempFileName2, "__temp.txt");
            myFile = fopen(tempFileName, "r");
            myTempFile = fopen(tempFileName2, "w+");
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
    char buffText[1000], text[1000], c;

    scanf("%s", command);
    getchar();
    if (!openFile(2, "r"))
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
    remove(tempFileName);
    rename(tempFileName2, tempFileName);
}

void cat()
{
    char buffText[1000];
    int i = 0;
    scanf("%s", command);
    getchar();
    if (!openFile(1, "r"))
        return;

    while (true)
    {
        fgets(buffText, 1000, myFile);
        printf("%s", buffText);
        if (feof(myFile))
            break;
    }
    printf("\n");
}

void remove_()
{
    char buffText[1000], c;
    int line, position, length, j = 0, i = 0, exactPosition = 0, currentLine = 1, flag = 1;
    scanf("%s", command); // " --file" skipped
    getchar();
    if (!openFile(2, "r"))
        return;
    undoBackup();

    scanf("%s", command); // " --pos" skipped
    scanf("%d:%d", &line, &position);

    scanf("%s", command); // " -size" skipped
    scanf("%d", &length);

    while ((c = fgetc(myFile)) != EOF)
    {
        buffText[i] = c;

        if (line == currentLine && flag)
        {
            exactPosition = i + position;
            flag = 0;
        }

        if (c == '\n')
            currentLine++;
        i++;
    }
    buffText[i] = '\0';
    scanf("%s", command); // -f or -b
    if (strstr(command, "-f"))
    {
        for (size_t k = 0; k < length; k++)
            delete_from_array(buffText, exactPosition, strlen(buffText));
    }
    else if (strstr(command, "-b"))
    {
        for (size_t k = 0; k < length; k++)
        {
            delete_from_array(buffText, exactPosition, strlen(buffText));
            exactPosition--;
        }
    }

    for (size_t i = 0; i < strlen(buffText); i++)
        fputc(buffText[i], myTempFile);

    fclose(myTempFile);
    fclose(myFile);
    remove(tempFileName);
    rename(tempFileName2, tempFileName);
}

void copy()
{
    char buffText[1000], c;
    int line, position, length, j = 0, i = 0, exactPosition = 0, currentLine = 1, flag = 1;
    scanf("%s", command); // "--file" skipped
    getchar();            // space skipped
    if (!openFile(1, "r"))
        return;

    scanf("%s", command); // " --pos" skipped
    scanf("%d:%d", &line, &position);

    scanf("%s", command); // " -size" skipped
    scanf("%d", &length);
    while ((c = fgetc(myFile)) != EOF)
    {
        buffText[i] = c;
        if (line == currentLine && flag)
        {
            exactPosition = i + position;
            flag = 0;
        }
        i++;
        if (c == '\n')
            currentLine++;
    }
    buffText[i] = '\0';

    scanf("%s", command); // -f or -b
    if (strstr(command, "-f"))
    {
        for (i = exactPosition; i < exactPosition + length; i++, j++)
            clipboard[j] = buffText[i];
    }
    else if (strstr(command, "-b"))
    {
        for (i = exactPosition - length; i < exactPosition; i++, j++)
            clipboard[j] = buffText[i];
    }
    clipboard[j] = '\0';
    fclose(myFile);
}

void cut()
{
    char buffText[1000], c;
    int line, position, length, j = 0, i = 0, exactPosition = 0, currentLine = 1, flag = 1;
    scanf("%s", command); //--file skipped
    getchar();
    if (!openFile(2, "r"))
        return;
    undoBackup();

    scanf("%s", command); // " --pos" skipped
    scanf("%d:%d", &line, &position);

    scanf("%s", command); // " -size" skipped
    scanf("%d", &length);

    while ((c = fgetc(myFile)) != EOF)
    {
        buffText[i] = c;
        if (line == currentLine && flag)
        {
            exactPosition = i + position;
            flag = 0;
        }
        i++;
        if (c == '\n')
            currentLine++;
    }
    buffText[i] = '\0';

    scanf("%s", command); // -f or -b

    if (strstr(command, "-f"))
    {
        for (i = exactPosition; i < exactPosition + length; i++, j++)
            clipboard[j] = buffText[i]; // copy
        clipboard[j] = '\0';

        for (size_t i = 0; i < length; i++)
            delete_from_array(buffText, exactPosition, strlen(buffText));
    }

    else if (strstr(command, "-b"))
    {
        for (i = exactPosition - length; i < exactPosition; i++, j++)
            clipboard[j] = buffText[i]; // copy
        clipboard[j] = '\0';

        for (size_t i = 0; i < length; i++)
        {
            delete_from_array(buffText, exactPosition, strlen(buffText));
            exactPosition--;
        }
    }
    else
        printf("Wrong command, Please try again\n");

    for (size_t i = 0; i < strlen(buffText); i++)
        fputc(buffText[i], myTempFile);

    fclose(myTempFile);
    fclose(myFile);
    remove(tempFileName);
    rename(tempFileName2, tempFileName);
}

void paste()
{
    int line, position, i = 0, j = 0, currentLine = 1;
    char buffText[1000], c;
    scanf("%s", command); //--file skipped
    getchar();
    if (!openFile(2, "r"))
        return;
    undoBackup();

    scanf("%s", command); //--pos skipped
    scanf("%d:%d", &line, &position);

    while (true)
    {
        fgets(buffText, 1000, myFile);
        if (line == currentLine)
        {
            for (size_t i = 0; i < strlen(buffText); i++)
            {
                if (i == position)
                    fputs(clipboard, myTempFile);
                fputc(buffText[i], myTempFile);
            }
            if (strlen(buffText) == position)
                fputs(clipboard, myTempFile);
        }
        else
            fputs(buffText, myTempFile);

        currentLine++;

        if (feof(myFile))
            break;
    }
    fclose(myTempFile);
    fclose(myFile);
    remove(tempFileName);
    rename(tempFileName2, tempFileName);
}

void find()
{
    int i = 0, j = 0, state = 0, at;
    char buffText[1000], text_to_find[1000], *byWordPosition[100], *byCharPosition[100], number[10], *stringPtr;
    bool star = false, flag = false;
    memset(byWordPosition, 0, sizeof(byWordPosition));
    memset(byCharPosition, 0, sizeof(byCharPosition));

    scanf("%s", command); // "--file" skipped
    getchar();
    if (!openFile(1, "r"))
        return;
    // openFile(1, "r");
    scanf("%s", command); // " --str" skipped

    modifyArray(text_to_find, 1);

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

    fgets(buffText, 1000, myFile);
    printf("%s ", text_to_find);

    if (star)
    {
        // if (text_to_find[0] == '*')
    }
    else
    {
        j = 0;
        for (size_t i = 0; i < strlen(buffText); i++)
        {
            if (strstr(buffText + i, text_to_find))
            {

                byCharPosition[j] = strstr(buffText + i, text_to_find);
                byCharPosition[j] = byCharPosition[j] - buffText;

                if (byCharPosition[j] != 0 && i == 0)
                    i = byCharPosition[j];

                for (int k = byCharPosition[j]; k > -1; k--)
                {
                    if (buffText[k] == ' ' && buffText[k - 1] != ' ')
                        byWordPosition[j]++;
                }

                if (j)
                    i += byCharPosition[j] - byCharPosition[j - 1] - 1;
                j++;
                flag = true; // founded
            }
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
                printf("%d\n", byWordPosition[0] + 1);
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
                printf("%d\n", byWordPosition[at - 1] + 1);
            break;
        case 1100: // byword & all
            if (flag)
            {
                for (size_t i = 0; i < j; i++)
                {
                    if (i)
                        printf(", %d", byWordPosition[i] + 1);
                    else
                        printf("%d", byWordPosition[i] + 1);
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
    }
    fclose(myFile);
}

void replace()
{
    int i = 0, j = 0, state = 0, at;
    char buffText[1000], tempBuffText[1000], text_to_find[1000], text_to_replace[1000], *byWordPosition[100], *byCharPosition[100], *stringPtr, number[10], c;
    bool star = false, flag = false;
    memset(byWordPosition, 0, sizeof(byWordPosition));

    scanf("%s", command); //" --file skipped"
    getchar();
    if (!openFile(2, "r"))
        return;
    undoBackup();

    scanf("%s", command); // " --str1" skipped
    modifyArray(text_to_find, 1);
    scanf("%s", command); // " --str2" skipped
    modifyArray(text_to_replace, 2);
    i = 0;
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

    fgets(buffText, 1000, myFile);
    if (star)
    {
    }
    else
    {
        for (size_t i = 0; i < strlen(buffText); i++)
        {
            if (strstr(buffText + i, text_to_find))
            {

                byCharPosition[j] = strstr(buffText + i, text_to_find);
                byCharPosition[j] = byCharPosition[j] - buffText;

                if (byCharPosition[j] - buffText != 0 && i == 0)
                    i = byCharPosition[j];

                for (int k = byCharPosition[j]; k > -1; k--)
                {
                    if (buffText[k] == ' ' && buffText[k - 1] != ' ')
                        byWordPosition[j]++;
                }

                if (j)
                    i += byCharPosition[j] - byCharPosition[j - 1] - 1;
                j++;
                flag = true; // founded
            }
        }
    }
    rewind(myFile);

    switch (state)
    {
    case 0:
        if (flag)
        {
            i = 0;
            while ((c = fgetc(myFile)) != EOF)
            {
                if (i == byCharPosition[0])
                    fputs(text_to_replace, myTempFile);
                if (!((i >= byCharPosition[0]) && (i < byCharPosition[0] + strlen(text_to_find))))
                    fputc(c, myTempFile);
                i++;
            }
        }
        else
            printf("-1\n");
        break;
    case 1:
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
                if (!(i >= byCharPosition[at - 1] && i < byCharPosition[at - 1] + strlen(text_to_find)))
                    fputc(c, myTempFile);
                i++;
            }
        }
        else
            printf("-1\n");
        break;
    case 10:
        if (flag)
        {
            j = 0;
            i = 0;
            while ((c = fgetc(myFile)) != EOF)
            {
                if (i == byCharPosition[j] + strlen(text_to_find))
                    j++;
                if (i == byCharPosition[j])
                    fputs(text_to_replace, myTempFile);
                if (!(i >= byCharPosition[j] && i < byCharPosition[j] + strlen(text_to_find)))
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
    fclose(myTempFile);
    fclose(myFile);
    remove(tempFileName);
    rename(tempFileName2, tempFileName);
}

void grep()
{
    int i = 0, j = 0, state = 0, at;
    char buffText[1000], text_to_find[1000], fileName[100][1000], foundedText[100][1000], c;
    bool flag = false;
    // memset(byWordPosition, 0, sizeof(byWordPosition));
    scanf("%s", command); // --str skipped or option

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
    getchar();
    c = getchar();
    if (c != '"')
    {
        text_to_find[0] = c;
        i = 1;
        while (true)
        {
            c = getchar();
            if (c == ' ' || c == '\n')
                break;
            text_to_find[i] = c;
            i++;
        }
        text_to_find[i] = '\0';
    }
    else if (c == '"')
    {
        while ((c = getchar()) != '"')
        {
            if (c == '"' && text_to_find[i - 1] != '\\')
                break;
            text_to_find[i] = c;
            i++;
        }
        text_to_find[i] = '\0';
    }

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
                fgets(buffText, 1000, myFile);
                if (strstr(buffText, text_to_find))
                {
                    strcpy(fileName[j], address);
                    strcpy(foundedText[j], buffText);
                    flag = true;
                    if (buffText[strlen(buffText) - 1] != '\n')
                        insert_to_array(foundedText[j], '\n', strlen(buffText), strlen(buffText)); // add enter if there is not one
                    j++;
                }
                if (feof(myFile))
                    break;
            }
        }
        else
        {
            printf("The file does not exist\n");
            // return;
        }
    }
    switch (state)
    {
    case 0:
        if (flag)
        {
            for (size_t i = 0; i < j; i++)
                printf("%s: %s", fileName[i], foundedText[i]);
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
                printf("%s\n", fileName[i]);
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
        remove(address);
        SetFileAttributes(tempFileName3, FILE_ATTRIBUTE_NORMAL);
        rename(tempFileName3, address);
    }
    else
        printf("The file does not exist!\n");
    memset(tempFileName3, 0, sizeof(tempFileName3));
}

void closing_pairs()
{
    char buffText[1000];
    int i = 0, j = 0, k = 0, tabCounter = 0, spaceCounter;
    bool flag = false, emptyContent;
    scanf("%s", command);
    getchar();
    if (!openFile(2, "r"))
        return;
    undoBackup();

    fgets(buffText, 1000, myFile);

    for (size_t i = 0; i < strlen(buffText); i++) // delete all whitspaces
    {
        if (buffText[i] == '{' || buffText[i] == '}')
        {
            j = i;
            while (buffText[--j] == ' ')
                delete_from_array(buffText, j, strlen(buffText));

            j += 2; // skip '{' or '}'
            while (buffText[j] == ' ')
                delete_from_array(buffText, j, strlen(buffText));
            i = --j;
        }
    }

    for (i; i < strlen(buffText); i++)
    {
        if (buffText[i] == '{')
        {
            tabCounter++;
            if (!flag)
                insert_to_array(buffText, ' ', i++, strlen(buffText));
            insert_to_array(buffText, '\n', ++i, strlen(buffText));
            for (j = ++i; j <= tabCounter * 4 + i; j++)
                insert_to_array(buffText, ' ', j, strlen(buffText));

            i += 4 * tabCounter;
            if (buffText[i + 1] == '{')
                flag = true;
            else if (buffText[i + 1] == '}')
                emptyContent = true;
            else
                flag = false;
        }
        if (buffText[i] == '}')
        {
            tabCounter--;
            if (emptyContent)
            {
                for (k = i; k > i - 4; k--)
                    delete_from_array(buffText, k - 1, strlen(buffText));
                emptyContent = false;
                i -= 4;
            }
            else
            {
                insert_to_array(buffText, '\n', i++, strlen(buffText));
                for (j = i++; j < (tabCounter)*4 + i; j++)
                    insert_to_array(buffText, ' ', j, strlen(buffText));
                i += 4 * tabCounter;
            }
        }
    }
    fputs(buffText, myTempFile);
    fclose(myFile);
    fclose(myTempFile);
    remove(tempFileName);
    rename(tempFileName2, tempFileName);
}
