#include <stdio.h>
#include <string.h>
#define DEBUG 0
#define LINE_SIZE 2048
#define CLEAR_SIZE 30
#define LINE_N 256
#define FILENAME_SIZE 30
#define TOOL_HEADER '-'

typedef struct
{
    char line[LINE_N][LINE_SIZE];
    int linesize;
} text;
int isFigure(char ch);
int str2int(char *str, int *result);
int intAfter(char *str, char c, int *result);
void getRange(char *str, int *startRow, int *endRow);
int in(char *input);
void showhelp();
void inittext(text *txt);
int loadtext(text *txt, char *filename);
int savetext(text *txt, char *filename);
void printText(text *txt, int startRow, int N, int cursorRow);
void printText(text *txt, int cursorRow);
int insertLine(text *txt, char *str, int at);
void delLine(text *txt, int startRow, int N);
void editLine(text *txt, int startRow, int N);
void searchLine(text *txt, char *str);

int main(int argc, char *argv[])
{
    text txt;
    text clipboard;
    inittext(&txt);
    inittext(&clipboard);
    char filename[FILENAME_SIZE] = "result.txt";
    if (argc == 2)
    {
        memcpy(filename, argv[1], FILENAME_SIZE);
    }
    loadtext(&txt, argv[1]);
    char input[LINE_SIZE];
    printf("Oneline text editor created by yt-koike\n");
    printf("%s on edit\n", filename);
    printf("%ch help\n", TOOL_HEADER);
    int cursorRow = -1;
    while (1)
    {
        printf("1le$ ");
        in(input);
        if (DEBUG)
            printf("START %c\n", input[0]);

        if (input[0] == '\\')
        {
            insertLine(&txt, &input[1], cursorRow);
            if (cursorRow > 0)
                cursorRow++;
            continue;
        }
        if (input[0] != TOOL_HEADER)
        {
            insertLine(&txt, input, cursorRow);
            if (cursorRow > 0)
                cursorRow++;
            continue;
        }
        int startRow, endRow;

        if (input[0] == TOOL_HEADER)
        {
            switch (input[1])
            {
            case 'c':
                for (int i = 0; i < CLEAR_SIZE; i++)
                {
                    printf("\n");
                }
                break;
            case 'd':
                if (!isFigure(input[2]))
                {
                    printf("Please select line or range.\n");
                    break;
                }
                getRange(&input[2], &startRow, &endRow);
                delLine(&txt, startRow, endRow - startRow + 1);
                break;
            case 'e':
                if (!isFigure(input[2]))
                {
                    printf("Please select line or range.\n");
                    break;
                }
                getRange(&input[2], &startRow, &endRow);
                editLine(&txt, startRow, endRow - startRow + 1);
                break;
            case 'h':
                showhelp();
                break;
            case 'i':
                if (str2int(&input[2], &cursorRow))
                {
                    cursorRow = -1;
                }
                break;
            case 'p':
                if (!(isFigure(input[2]) || input[2] == '~'))
                {
                    printText(&txt, cursorRow);
                    break;
                }
                getRange(&input[2], &startRow, &endRow);
                if (startRow < 1)
                {
                    startRow = 1;
                }
                printText(&txt, startRow, endRow - startRow + 1, cursorRow);
                break;
            case 'q':
            case 'x':
                printf("Save change? (y/n):");
                char confirm;
                scanf("%c", &confirm);
                if (!(confirm == 'N' || confirm == 'n'))
                {
                    savetext(&txt, filename);
                    printf("Saved\n");
                }
                return 0;
                break;
            case 's':
                savetext(&txt, filename);
                printf("\nSaved\n");
                break;
            case 'w':
                searchLine(&txt, &input[2]);
                break;
            default:
                printf("Error: invalid option\n");
                break;
            }
        }
    }
    return 0;
}

int isFigure(char ch)
{
    return '0' <= ch && ch <= '9';
}

int str2int(char *str, int *result)
{
    int negFlag = 0;
    int x = 0;
    if (str[0] == '-')
    {
        negFlag = 1;
        str++;
    }
    if (!isFigure(*str))
        return 1;
    while (isFigure(*str))
    {
        x *= 10;
        x += (*str) - '0';
        str++;
    }
    if (negFlag)
    {
        x *= -1;
    }
    *result = x;
    return 0;
}

int intAfter(char *str, char c, int *result)
{
    while (*str)
    {
        if (*str == c)
        {
            if (!str2int(str + 1, result))
                return 0;
        }
        str++;
    }
    return 1;
}

void getRange(char *str, int *startRow, int *endRow)
{
    // "a"   => a ~ a
    // "~b"  => 1 ~ b
    // "a~b" => a ~ b
    // "aNb" => a ~ a+N-1
    if (str2int(str, startRow))
    {
        *startRow = 1;
    }
    if (intAfter(str, '~', endRow))
    {
        int linenum;
        if (intAfter(str, 'N', &linenum))
        {
            *endRow = *startRow;
        }
        else
        {
            *endRow = *startRow + linenum - 1;
        }
    }
    return;
}

int in(char *input)
{
    do
    {
        fgets(input, LINE_SIZE, stdin);
    } while (strlen(input) <= 0 || input[0] == '\n');
    input[strlen(input) - 1] = '\0';
    return strlen(input);
}

void showhelp()
{
    printf("Options:\n -d del\n -e edit\n -p map\n -q or -x save and exit\n -h help\n -i insert\n -w search\n");
    return;
}

void inittext(text *txt)
{
    int i;
    for (i = 0; i < LINE_N; i++)
        txt->line[i][0] = '\0';
    txt->linesize = 0;
    return;
}

int loadtext(text *txt, char *filename)
{
    FILE *fpr = fopen(filename, "r");
    if (!fpr)
        return 1;
    int i = 0;
    while (fgets(txt->line[i], LINE_SIZE, fpr))
    {
        if (i >= LINE_N)
            return -1;
        txt->line[i][strlen(txt->line[i]) - 1] = '\0';
        i++;
        if (i >= LINE_SIZE)
        {
            printf("Error: file too long to open.\n");
            return 1;
        }
    }
    txt->linesize = i;
    fclose(fpr);
    return 0;
}

int savetext(text *txt, char *filename)
{
    FILE *fpw = fopen(filename, "w");
    if (!fpw)
        return -1;
    int i;
    for (i = 0; i < txt->linesize; i++)
        fprintf(fpw, "%s\n", txt->line[i]);
    fclose(fpw);
    return 0;
}

void printText(text *txt, int startRow, int N, int cursorRow)
{
    if (N < 0)
    {
        startRow = startRow + N + 1;
        N = -N;
    }
    if (startRow + N - 1 > txt->linesize)
    {
        N = txt->linesize - startRow + 1;
    }
    if (cursorRow == 0)
    {
        printf("$:\n");
    }
    for (int row = startRow; row < startRow + N; row++)
    {
        printf("%d:%s\n", row, txt->line[row - 1]);
        if (row == cursorRow)
            printf("$:\n");
    }
    if (cursorRow == -1)
    {
        printf("$:\n");
    }
}

void printText(text *txt, int cursorRow)
{
    printText(txt, 1, txt->linesize, cursorRow);
}

int insertLine(text *txt, char *str, int at)
{ // at=-1:at the end ,otherwise at var"at"
    if (DEBUG)
        printf("insert:%s at %d\n", str, at);
    txt->linesize++;
    if (at < 0)
    {
        at = txt->linesize - 1;
    }
    else
    {
        if (at + 1 > LINE_N)
            return -1;
        int i;
        for (i = txt->linesize; i >= at; i--)
            memcpy(txt->line[i + 1], txt->line[i], LINE_SIZE);
    }
    memcpy(txt->line[at], str, LINE_SIZE);
    return 0;
}

void editLine(text *txt, int startRow, int N)
{
    char editinput[LINE_SIZE];
    for (int i = startRow; i < startRow + N; i++)
    {
        printf("old %d: %s\n", i, txt->line[i - 1]);
        printf("%d:", i);
        in(editinput);
        memcpy(txt->line[i - 1], editinput, LINE_SIZE);
    }
    return;
}

void delLine(text *txt, int startRow, int N)
{
    int i;
    for (i = startRow - 1; i < txt->linesize; i++)
    {
        memcpy(txt->line[i], txt->line[i + N], LINE_SIZE);
    }
    txt->linesize -= N;
    return;
}

void searchLine(text *txt, char *str)
{
    for (int i = 0; i < txt->linesize; i++)
    {
        char *line = txt->line[i];
        if (strstr(line, str))
        {
            printf("%d:%s\n", i + 1, line);
        }
    }
}
