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
    fortuneFile = argv[2];
    printf("%s\n", fortuneFile);
    fp = fopen(fortuneFile, "r");
    if (fp == NULL)
    {
        printf("error");
    }

    // count lines
    /*
    char cr;
    size_t lines = 0;

    while( cr != EOF){
        if( cr == '\n') {
            lines++;
        }
        cr = getc(fp);
    }
    // used to reposition the file pointer associated with stream to the beginning of the file
    rewind(fp);
    */

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

    char **fortuneArr = (char **) malloc(sizeof(char*)*row);
        for(int i = 0; i < row; i++){
            fortuneArr[i] = (char *) malloc(sizeof(char)*col);
        }
    idx = 0;
    char *currLine = malloc(sizeof(char) * col);
    char *currQuote = malloc(sizeof(char) * col); // malloc makes everything null (no garbage values)
    fgets(currLine, col, fp); // skip the first %
    while(1) {
        fgets(currLine, col, fp);
        if(currLine[0] == 0){
            printf("Current Quote: %s\n", currQuote);
            strcpy(fortuneArr[idx], currQuote);
            printf("Current Line: %s\n", fortuneArr[idx]);
            strcpy(currQuote, "");
            break;
        }
        //printf("Current Line: %s\n", currLine);
        if (currLine[0] != '%'){ // keep adding to the current quote 
            strcat(currQuote,currLine);
        } else{ // reached end of the quote so reset it
            printf("Current Quote: %s\n", currQuote);
            strcpy(fortuneArr[idx], currQuote);
            printf("Current Line: %s\n", fortuneArr[idx]);
            strcpy(currQuote, "");
            idx++;
        }
    }
    /*
    for(int i = 0; i < row; i++){
        printf("%s\n", fortuneArr[i]);
    }
    */
   
//    while(fgets(currLine, col, fp)) {
//         printf("%lu\n", strlen(currLine));
//     //    if(currLine[0] == '%'){ // ignore the % separator
//     //        idx++;
//     //        printf("%%%%%%%%%%%%");
//     //        continue;
//     //    }
    
//        printf("%s\n", currLine);
//        strcpy(fortuneArr[idx-2],currLine);
//        /*
//        for(int i = 0; i < col && currLine[i] != '\0';i++){
//            fortuneArr[idx-2][i] = currLine[i];
//        }
//        */
//        idx++;
//        //printf("%s\n", fortuneArr[idx-2]);
//    }
    /*
    // parsing fortune file into an array ie reading each command line by line

    if (strcmp("-b", argv[3]) == 0){
        batch_mode = 1;
        // TODO: read from batch file
    }
    */
    fclose(fp);

    // free fortune array at the end
    for(int i = 0; i < row; i++){
        free(fortuneArr[i]);
    }
    free(fortuneArr);
    return 0;
}