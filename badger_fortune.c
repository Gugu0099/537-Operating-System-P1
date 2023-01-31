#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
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
    if(argc < 5){
        printf("USAGE: \n\tbadger-fortune -f <file> -n <number> (optionally: -o <output file>) \n\t\t OR \n\tbadger-fortune -f <file> -b <batch file> (optionally: -o <output file>)\n");
        return 1;   
    }
    // reading the fortune file
    fortuneFile = argv[2];
    printf("%s\n", fortuneFile);
    fp = fopen(fortuneFile, "r");
    if(fp == NULL){
        printf("error");
    }

    // get dimensions of the fortune file
    int idx = 0;
    while (1 && idx < 2){
        if(idx == 0){
            fgets(dimLine, 100, fp);
            row = atoi(dimLine);
        }
        if (idx == 1){
            fgets(dimLine, 100, fp);
            col = atoi(dimLine);
        }
        idx++;
    }

    // creating space for fortune file
    char **fortuneArr = (char **) malloc(sizeof(char*)*row);
    for(int i = 0; i < row; i++){
        fortuneArr[i] = (char *) malloc(sizeof(char)*col);
    }
    printf("%d\n", idx);
    char currLine[col];
    // figure out how to read everything into the f
    while(fgets(currLine, col, fp)) {
        if(strcmp(currLine, "%") == 0){ // ignore the % separator
            idx++;
            continue;
        }
        for(int i = 0; i < col && currLine[i] != '\0';i++){
            fortuneArr[idx-2][i] = currLine[i];
        }
        printf("%s\n", currLine);
        idx++;
        //printf("%s\n", fortuneArr[idx-2]);
    }
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