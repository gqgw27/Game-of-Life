#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gol.h"

int main(int argc, char *argv[]){
    struct universe u;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    int(*rule)(struct universe *u, int column, int row);
    int count_i = 0;
    int count_o = 0;
    int count_g = 0;

    char name_i[20];
    char name_o[20];
    int generations = 5;
    int stats = 0;
    int torus = 0;

    for(int i = 1;i<argc;i++){
        if (argv[i][0] == '-'){
            switch (argv[i][1]){
                case 'i':
                    if (i+1 > argc-1){
                        fprintf(stderr, "Valid input not specified\n");
                        exit(1);
                    }
                    if (count_i == 0){
                        infile = fopen(argv[i+1], "r");
                        count_i = 1;
                        strcpy(name_i,argv[i+1]);
                    }
                    else {
                        if (strcmp(name_i,argv[i+1])!= 0){
                            fprintf(stderr, "Conflicting file input names specified\n");
                            exit(1);
                        }
                    }
                    if (argv[i+1][0] == '-'){
                        i++;
                    }
                    break;
                case 'o':
                    if (i+1 > argc-1){
                        fprintf(stderr, "Valid output not specified\n");
                        exit(1);
                    }
                    if (count_o == 0){
                        outfile = fopen(argv[i+1], "w");
                        count_o = 1;
                        strcpy(name_o,argv[i+1]);
                    }
                    else {
                        if (strcmp(name_o,argv[i+1])!= 0){
                            fprintf(stderr, "Conflicting file output names specified\n");
                            exit(1);
                        }
                    }
                    if (argv[i+1][0] == '-'){
                        i++;
                    }
                    break;
                case 'g':
                    if (i+1 > argc-1 || argv[i+1][0] == '-'){
                        fprintf(stderr, "No generations specified\n");
                        exit(1);
                    }
                    if (count_g > 0){
                        if (generations != atoi(argv[i+1])){
                            fprintf(stderr, "Conflicting arguments given for generations\n");
                            exit(1);
                        }
                    }
                    count_g++;
                    generations = atoi(argv[i+1]);
                    if (generations == 0){
                        if (argv[i+1][0] == '0'){
                            generations = 0;
                        }
                        else {
                            fprintf(stderr, "Generations must be a valid number\n");
                            exit(1);
                        }
                    }
                    break;
                case 's':
                    if (i+1 < argc && argv[i+1][0] != '-'){
                        fprintf(stderr, "Print statistics takes no arguments\n");
                        exit(1);
                    }
                    stats = 1;
                    break;
                case 't':
                    if (i+1 < argc && argv[i+1][0] != '-'){
                        fprintf(stderr, "Torus takes no arguments\n");
                        exit(1);
                    }
                    torus = 1;
                    break;
                default:
                    fprintf(stderr, "Invalid option entered\n");
                    exit(1);
            }
        }
    }

    read_in_file(infile, &u);

    rule = will_be_alive;
    if (torus){
        rule = will_be_alive_torus;
    }

    for (int i = 0;i < generations;i++){
        evolve(&u,rule);
    }

    write_out_file(outfile, &u);

    if (stats){
        print_statistics(&u);
    }

    return 0;
}
