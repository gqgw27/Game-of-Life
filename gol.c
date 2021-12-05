#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gol.h"


void read_in_file(FILE *infile, struct universe *u){
    if (infile == NULL){
        fprintf( stderr, "Error opening file\n");
        exit(1);
    }
    char *store = malloc(sizeof(char)*512);

    if (store == NULL){
        fprintf( stderr, "Sufficient memory could not be allocated\n");
        exit(1);
    }

    int c;
    int rows = 0;
    int cols = 0;
    int col_temp = 0;
    char ch;

    u->dead = 0;
    u->alive = 0;

    while ((c = fgetc(infile)) != EOF){
        if (c == 13){
            continue;
        }
        ch = (char)c;
        if (ch == '\n'){
            if (cols != 0){
                if (col_temp != cols){
                    fprintf( stderr, "Row length is not uniform\n");
                    exit(1);
                }
            }

            rows++;
            cols = col_temp;
            col_temp = 0;
            store = realloc(store,sizeof(char)*(rows+1)*cols);
            if (store == NULL){
                fprintf( stderr, "Sufficient memory could not be allocated\n");
                exit(1);
            }
            continue;
        }

        else if (ch != ' '){
            col_temp++;
            if (col_temp > 512){
                fprintf( stderr, "Number of columns exceed 512\n");
                exit(1);
            }

            if (cols > 0){
                if (ch == '.'){
                    store[rows*cols + col_temp-1] = '.';
                    u->dead += 1;
                }
                else if (ch =='*'){
                    store[rows*cols + col_temp-1] = '*';
                    u->alive += 1;
                }
                else {
                    fprintf( stderr, "Invalid input file\n");
                    exit(1);
                }
            }
            else {
                if (ch == '.'){
                    store[col_temp-1] = '.';
                    u->dead += 1;
                }
                else if (ch =='*'){
                    store[col_temp-1] = '*';
                    u->alive += 1;
                }
                else {
                    fprintf( stderr, "Invalid input file\n");
                    exit(1);
                }
            }
        }
    }

    u->rows=rows;
	u->cols=cols;
	u->world=store;
    u->cum_dead = u->dead;
    u->cum_alive = u->alive;
	store=NULL;
}

void write_out_file(FILE *outfile, struct universe *u){
    int rows = u->rows;
	int cols = u->cols;
    char *world = u->world;
    for (int x = 0;x < rows;x++){
        for (int y = 0;y < cols;y++){
            fprintf(outfile,"%c",world[x*cols+y]);
        }
        fprintf(outfile,"\n");
    }
}

int is_alive(struct universe *u, int column, int row){
    int cols = u->cols;
    char *world = u->world;
    if (world[row*cols+column] == '.'){
        return 0;
    }
    else {
        return 1;
    }

}

int will_be_alive(struct universe *u, int column, int row){
    // Part 1 will be for the case when it is > 3x3 grid
    int rows = u->rows;
	int cols = u->cols;
    int alive = 0;

    for (int x = -1;x < 2;x++){
        for (int y = -1;y < 2;y++){
            if (column+y < 0 || column+y > cols-1 || row+x < 0 || row+x > rows-1){
                continue;
            }
            else if (x==y && x==0){
                continue;
            }
            else {
                alive += is_alive(u,column+y,row+x);
            }
        }
    }

    if (is_alive(u,column,row)){
        if (alive == 2 || alive == 3){
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        if (alive == 3){
            return 1;
        }
        else {
            return 0;
        }
    }
}

int will_be_alive_torus(struct universe *u, int column, int row){
    // Part 1 will be for the case when it is > 3x3 grid
    int rows = u->rows;
	int cols = u->cols;
    int alive = 0;

    if (row == 0 || column == 0 || row == rows-1 || column == cols-1){
        // logic for the corners
        if (row == 0 && column == 0){
            alive += is_alive(u,0,rows-1);
            alive += is_alive(u,cols-1,0);
            alive += is_alive(u,cols-1,rows-1);
            alive += is_alive(u,cols-1,1);
            alive += is_alive(u,1,rows-1);
        }
        else if (row == 0 && column == cols-1){
            alive += is_alive(u,cols-1,rows-1);
            alive += is_alive(u,0,0);
            alive += is_alive(u,0,rows-1);
            alive += is_alive(u,cols-2,rows-1);
            alive += is_alive(u,0,1);
        }
        else if (row == rows-1 && column == 0){
            alive += is_alive(u,0,0);
            alive += is_alive(u,cols-1,rows-1);
            alive += is_alive(u,cols-1,0);
            alive += is_alive(u,1,0);
            alive += is_alive(u,cols-1,rows-2);
        }
        else if (row == rows-1 && column == cols-1){
            alive += is_alive(u,cols-1,0);
            alive += is_alive(u,0,rows-1);
            alive += is_alive(u,0,0);
            alive += is_alive(u,cols-2,0);
            alive += is_alive(u,0,rows-2);
        }
        // logic for the edges
        else if (row == 0){
            alive += is_alive(u,column,rows-1);
            alive += is_alive(u,column-1,rows-1);
            alive += is_alive(u,column+1,rows-1);
        }
        else if (row == rows-1){
            alive += is_alive(u,column,0);
            alive += is_alive(u,column-1,0);
            alive += is_alive(u,column+1,0);
        }
        else if (column == 0){
            alive += is_alive(u,cols-1,row);
            alive += is_alive(u,cols-1,row-1);
            alive += is_alive(u,cols-1,row+1);
        }
        else if (column == cols-1){
            alive += is_alive(u,0,row);
            alive += is_alive(u,0,row-1);
            alive += is_alive(u,0,row+1);
        }
        for (int x = -1;x < 2;x++){
            for (int y = -1;y < 2;y++){
                if (column+y < 0 || column+y > cols-1 || row+x < 0 || row+x > rows-1){
                    continue;
                }
                else if (x==y && x==0){
                    continue;
                }
                else {
                    alive += is_alive(u,column+y,row+x);
                }
            }
        }
    }
    else {
    // This will check the 8 neighbours
        for (int x = -1;x < 2;x++){
            for (int y = -1;y < 2;y++){
                int nrow = row+x;
                int ncol = column+y;

                if (x==y && x==0){
                    continue;
                }
                else {
                    alive += is_alive(u,ncol,nrow);
                }
            }
        }
    }

    if (is_alive(u,column,row)){
        if (alive == 2 || alive == 3){
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        if (alive == 3){
            return 1;
        }
        else {
            return 0;
        }
    }
}

void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)){
    int rows = u->rows;
    int cols = u->cols;
    char *new_world = (char*) malloc(sizeof(char)*rows*cols);
    if (new_world == NULL){
        fprintf( stderr, "Sufficient memory could not be allocated\n");
        exit(1);
    }
    u->dead = 0;
    u->alive = 0;

    for (int x = 0;x < rows;x++){
        for (int y = 0;y < cols;y++){
            if (rule(u, y, x)){
                new_world[x*cols+y] = '*';
                u->alive += 1;
            }
            else {
                new_world[x*cols+y] = '.';
                u->dead += 1;
            }
        }
    }
    free(u->world);
    u->world = new_world;
    u->cum_dead += u->dead;
    u->cum_alive += u->alive;


}

void print_statistics(struct universe *u){
    double alive = 100*(u->alive)/(u->alive+u->dead);
    double average = 100*(u->cum_alive)/(u->cum_alive + u->cum_dead);
	fprintf(stdout,"%.3f%% of cells currently alive\n",alive);
    fprintf(stdout,"%.3f%% of cells alive on average\n",average);
}
