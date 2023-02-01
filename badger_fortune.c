#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /*
    badger-fortune -f <file> -n <number> (optionally: -o <output file>)
    0               1   2     3  4                     5         6
    badger-fortune -f <file> -b <batch file> (optionally: -o <output file>)
    */
    int batch_mode = 0; // for -n (0) and -b (1)
    char *fortuneFile;
    char *batchFile;
    char *outputFile;
    FILE *fp;
    FILE *fp2;
    FILE *fp3;
    int row = 0;
    int col = 0;
    char dimLine[100];
    // Error: invalid number of arguments
    if (argc < 5)
    {
        printf("USAGE: \n\tbadger-fortune -f <file> -n <number> (optionally: -o <output file>) \n\t\t OR \n\tbadger-fortune -f <file> -b <batch file> (optionally: -o <output file>)\n");
        return 1;
    }
    // Error: Invalid Flag
    if ((strcmp(argv[1], "-f")) != 0)
    {
        printf("ERROR: Invalid Flag Types\n");
        return 1;
    }
    if (strcmp(argv[3], "-n") != 0 && strcmp(argv[3], "-b") != 0)
    {
        printf("ERROR: Invalid Flag Types\n");
        return 1;
    }
    if (argc >= 6)
    {
        if ((strcmp(argv[5], "-o")) != 0)
        {
            printf("ERROR: Invalid Flag Types\n");
            return 1;
        }
        // Error: -n then -b:
        if (strcmp(argv[3], "-n") == 0 && strcmp(argv[5], "-b") == 0)
        {
            printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
            return 1;
        }
        // Error: -b then -n
        if (strcmp(argv[3], "-b") == 0 && strcmp(argv[5], "-n") == 0)
        {
            printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
            return 1;
        }
    }
    int number;
    number = atoi(argv[4]);
    // Error: -n followed by number <= 0
    if (strcmp(argv[3], "-n") == 0)
    {
        if (number <= 0)
        {
            printf("ERROR: Invalid Fortune Number\n");
            return 1;
        }
    }

    // reading the fortune file
    fortuneFile = argv[2]; // grab the second argument
    printf("%s\n", fortuneFile);
    fp = fopen(fortuneFile, "r");
    // Error: Fortune File Doesn't Exist
    if (fp == NULL)
    {
        printf("ERROR: Can't open fortune file\n");
        return 1;
    }
    // Error: Fortune File not Specified
    if (strcmp(argv[1], "-f") != 0)
    {
        printf("ERROR: No fortune file was provided\n");
        return 1;
    }
    if (strcmp(argv[2], "") == 0)
    {
        printf("seg3\n");
        printf("ERROR: No fortune file was provided\n");
        return 1;
      } //else {
    //     int len = strlen(outputFile);
    //     if(len > 4){
    //     printf("seg1\n");
    //     char *last_four = &outputFile[len - 4];
    //     printf("seg2\n");
    //     printf("%s\n", last_four);
    //     printf("seg3\n");
    //     if(strcmp(last_four, ".txt") != 0){
    //          printf("ERROR: No fortune file was provided\n");
    //     return 1;
    //     }
    //     }
    // }

    // Error: Empty fortune file
    int sizeFortune;
    if (NULL != fp)
    {
        fseek(fp, 0, SEEK_END);
        sizeFortune = ftell(fp);

        if (0 == sizeFortune)
        {
            printf("ERROR: Fortune File Empty\n");
            return 1;
        }
    }
    rewind(fp);

    if (strcmp(argv[5], "-o") == 0)
    {
        outputFile = argv[6];
        printf("printing output file: %s\n", outputFile);
        fp3 = fopen(outputFile, "w");
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

    // Error: -n followed by number > number of fortunes
    if (number > row)
    {
        printf("ERROR: Invalid Fortune Number\n");
        return 1;
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

    int numRow = row;
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

    // -n number
    printf("In batch: %d\n", batch_mode);
    if (batch_mode == 0)
    {
        // do some error handling here
        int line = atoi(argv[4]);
        if (strcmp(argv[5], "-o") == 0)
        {
            fprintf(fp3, "%s\n", fortuneArr[line - 1]);
            fclose(fp3);
        }
        else
        {
            printf("%s\n", fortuneArr[line - 1]);
        }
    }

    // -b batch file
    if (batch_mode == 1)
    {

        batchFile = argv[4];
        printf("%s\n", batchFile);
        fp2 = fopen(batchFile, "r");
        /*
        while(fgets(currLine, col, fp2)){
            printf("Testing Batches: %s", currLine);
        }
        */
        // Error: Batch File Doesn't Exist

        if (fp2 == NULL)
        {
            printf("ERROR: Can't open batch file\n");
        }
        // Error: Empty batch file
        int sizeBatch;
        if (NULL != fp2)
        {
            fseek(fp2, 0, SEEK_END);
            sizeBatch = ftell(fp2);

            if (0 == sizeBatch)
            {
                printf("ERROR: Batch File Empty\n");
                return 1;
            }
        }
        rewind(fp2);
        int num = 0;
        while (1)
        {
            if (fgets(currLine, col, fp2) == 0)
            {
                break;
            }
            else
            { // convert string to int
                num = atoi(currLine);
                // Error: -b with file containing number <= 0
                // printf("%d\n", num);
                if (num <= 0)
                {
                    printf("ERROR: Invalid Fortune Number\n\n");
                    continue;
                }
                // Error: -b with file containing number > number of fortunes
                if (num > numRow)
                {
                    printf("ERROR: Invalid Fortune Number\n\n");
                    continue;
                }
            }
            if (strcmp(argv[5], "-o") == 0)
            {
                fprintf(fp3, "%s\n", fortuneArr[num - 1]);
            }
            else
            {
                printf("%s\n", fortuneArr[num - 1]);
            }
        }
        fclose(fp3);
        fclose(fp2);
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