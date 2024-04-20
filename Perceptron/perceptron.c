#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 500

typedef struct Perceptron
{
    double **weights;
    int width, height;
} perceptron;


int clampi(int k, int low, int high)
{
    assert(low < high);
    if(k < low) k = low;
    if(k > high) k = high;
    return k;
}

float triangleArea(double xa, double ya, double xb, double yb, double xc, double yc)
{
    double AB[] = {xa - xb, ya - yb};
    double AC[] = {xa - xc, ya - yc};
    double cross_product = AB[0] * AC[1] - AB[1] * AC[0];
    return fabs(cross_product)/2;
}

bool pointInsideTriangle(double xp, double yp, double xa, double ya, double xb, double yb, double xc, double yc)
{
    double area = triangleArea(xa, ya, xb, yb, xc, yc);
    double area_sum = 0.0;
    area_sum += triangleArea(xa, ya, xb, yb, xp, yp);
    area_sum += triangleArea(xa, ya, xc, yc, xp, yp);
    area_sum += triangleArea(xb, yb, xc, yc, xp, yp);

    return (bool) (area == area_sum);
}

void generateRect(double **input, int input_width, int input_height, double value, int x0, int y0, int width, int height)
{
    y0 = clampi(y0, 0, input_height);
    x0 = clampi(x0, 0, input_width);
    height = clampi(width, 1, input_height - y0);
    width = clampi(width, 1, input_width - x0);

    for(int y = 0; y < input_height; y++)
    {
        for(int x = 0; x < input_width; x++)
        {
            if(y0 <= y && y < y0 + height && x0 <= x && x < x0 + width) input[y][x] = value;
            else input[y][x] = 0.0;
        }
    }
}


int main(void)
{
    printf("teste = %s\n", (1 == 1) ? "true" : "false");
    printf("teste = %d\n", (bool) (-10000));
}