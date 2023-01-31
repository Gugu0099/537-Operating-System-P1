#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /*
    badger-fortune -f <file> -n <number> (optionally: -o <output file>)
    0               1   2     3  4          5
    badger-fortune -f <file> -b <batch file> (optionally: -o <output file>)
    */
    int batch_mode = 0; // for -n (0) and -b (1)
    char *fortuneFile;
    char *batchFile;
    FILE *fp;
    int row = 0;
    int col = 0;
    char dimLine[100];
    // Error: invalid number of arguments
    if (argc < 5)
    {
        printf("USAGE: \n\tbadger-fortune -f <file> -n <number> (optionally: -o <output file>) \n\t\t OR \n\tbadger-fortune -f <file> -b <batch file> (optionally: -o <output file>)\n");
        return 1;
    }
    // reading the fortune file
    fortuneFile = argv[2]; // grab the second argument
    printf("%s\n", fortuneFile);
    fp = fopen(fortuneFile, "r");
    if (fp == NULL)
    {
        printf("error");
    }

    // get dimensions of the fortune file
    int idx = 0; // start point (line number)
    while (1 && idx < 2)
    {
        if (idx == 0)
        {
            fgets(dimLine, 100, fp);
            row = atoi(dimLine);
        }
        if (idx == 1)
        {
            fgets(dimLine, 100, fp);
            col = atoi(dimLine);
        }
        idx++;
    }
    char *token;

    char **fortuneArr = (char **)malloc(sizeof(char *) * row);
    for (int i = 0; i < row; i++)
    {
        fortuneArr[i] = (char *)malloc(sizeof(char) * col);
    }
    idx = 0;
    char *currLine = malloc(sizeof(char) * col);
    char *currQuote = malloc(sizeof(char) * col); // malloc makes everything null (no garbage values)
    fgets(currLine, col, fp);                     // skip the first %
    while (row)
    {
        if (fgets(currLine, col, fp) == 0)
        { // reach the end of file
            // even though it's in the if its copying into the buffer;
            // printf("Current Quote: %s\n", currQuote);
            strcpy(fortuneArr[idx], currQuote);
            // printf("Current Line: %s\n", fortuneArr[idx]);
            strcpy(currQuote, "");
            break;
        }
        else
        {
            if (currLine[0] != '%')
            { // keep adding to the current quote
                strcat(currQuote, currLine);
            }
            else
            { // reached end of the quote so reset it
                // printf("Current Quote: %s\n", currQuote);
                strcpy(fortuneArr[idx], currQuote);
                // printf("Current Line: %s\n", fortuneArr[idx]);
                strcpy(currQuote, "");
                idx++;
                row--;
            }
        }
    }
    // check if we're in batch mode
    if (strcmp(argv[3], "-b") == 0)
    {
        batch_mode = 1;
    }
    /*
    int i = 0;
    while(1){
        if (fortuneArr[i] == NULL){
            break;
        }
        printf("%s\n",fortuneArr[i]);
        i++;
    }

    printf("%d\n", i);
    */

   // -n number
    printf("In batch: %d\n", batch_mode);
    if (batch_mode == 0)
    {
        // do some error handling here
        int line = atoi(argv[4]);
        printf("%s\n", fortuneArr[line - 1]);
    }

    if (batch_mode == 1)
    {
        batchFile = argv[4];
        printf("%s\n", batchFile);
        fp = fopen(batchFile, "r");
        if (fp == NULL)
        {
            printf("error");
        }
    }

    fclose(fp);

    // free fortune array at the end
    for (int i = 0; i < row; i++)
    {
        free(fortuneArr[i]);
    }
    free(fortuneArr);
    return 0;
}