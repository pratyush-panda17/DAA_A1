#include <stdio.h>
#include <stdlib.h>
#include "plot_staircase.h"

void plotStaircase(FILE* staircaseFile,FILE* pointsfile) {
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!gnuplotPipe) {
        printf("Error: Could not open Gnuplot.\n");
    }
    // Gnuplot commands
    fprintf(gnuplotPipe, "set title 'Staircase Plot'\n");
    fprintf(gnuplotPipe, "set xlabel 'X'\n");
    fprintf(gnuplotPipe, "set ylabel 'Y'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set xrange [%d:%d]\n", 0,135);
    fprintf(gnuplotPipe, "set yrange [%d:%d]\n", 0, 135);
    fprintf(gnuplotPipe, "plot 'staircase.txt' using 1:2 with lines lc rgb 'blue' title 'Staircase Path', \\\n");
    fprintf(gnuplotPipe, "     'points.txt' using 1:2 with points pt 7 lc rgb 'red' title 'Original Points'\n");

    fclose(gnuplotPipe);
    return ;
}
