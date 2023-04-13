#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    long double width;
    long double num_of_rectangle;
    int given_i;
    char* fifo = argv[4];
    sscanf(argv[1], "%Lf", &width);
    sscanf(argv[2], "%Lf", &num_of_rectangle);
    sscanf(argv[3], "%i", &given_i);

    FILE* fifo_open = fopen(fifo, "a+");

    if(fifo_open == NULL){
        perror("can't open file");
        return -1;
    }

    long double current_x = given_i * num_of_rectangle * width;
    long double rectangle_area = 0.0;
    for (int j = 0; j != num_of_rectangle; j++){
        current_x += width;
        rectangle_area += 4.0/(current_x * current_x + 1) * width;
    }

    fwrite(&rectangle_area, sizeof(rectangle_area), 1, fifo_open);
    fclose(fifo_open);

    return 0;
}