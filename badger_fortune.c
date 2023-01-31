#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    // Error: invalid number of arguments
    if(argc < 5){
        printf("USAGE: \n\tbadger-fortune -f <file> -n <number> (optionally: -o <output file>) \n\t\t OR \n\tbadger-fortune -f <file> -b <batch file> (optionally: -o <output file>)\n");
        return 1;   
    }

    
}