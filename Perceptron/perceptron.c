#include "perceptron.h"

void printDenseNetwork(network model)
{
    layer *l;
    int layer_index = 0;
    for(l = model.initial_layer; l != NULL; l = l->next)
    {
        printf("layer %i: ", layer_index);
        for(size_t i = 0; i < (size_t) l->height; i++)
        {
            printf("[");
            for (size_t k = 0; k < (size_t) l->neurons[i].input_size; k++)
            {
                printf("w%li = %lf", k+1, l->neurons[i].weights[k]);
                if(k != (l->neurons[i].input_size - 1))
                    printf(", ");
            }
            printf("] ");
            
        }
        layer_index++;
        printf("\n");
    }
}

double **readData(char *file_name, int *data_size, int *input_size, int *output_size)
{
    FILE *fptr;
    double **data;
    fptr = fopen(file_name, "r");
    char buffer[1024];
    if(fgets(buffer, 1024, fptr))
    {
        while(buffer[0] == '#') fgets(buffer, 1024, fptr);
        sscanf(buffer, "%i %i %i", data_size, input_size, output_size);
        data = malloc(sizeof(double *) * (*data_size));
        for(size_t i = 0; i < (*data_size); i++)
        {
            data[i] = malloc(sizeof(double) * ((*input_size) + (*output_size)));
        }
        printf("data stuff: %i %i %i\n", (*data_size), (*input_size), (*output_size));

        int line = 0, column = 0, buffer_index = 0;
        fgets(buffer, 1024, fptr);
        while(line < (*data_size))
        {
            if(buffer[buffer_index] == '#') fgets(buffer, 1024, fptr);
            else if((buffer[buffer_index] == '\n') || (buffer[buffer_index] == '\0'))
            {
                fgets(buffer, 1024, fptr);
                line++;
                column = 0;
                buffer_index = 0;
            }
            else if(buffer[buffer_index] == ' ') buffer_index++;
            else
            {
                sscanf(buffer + buffer_index, "%lf %*s", &data[line][column]);
                column++;
                while((buffer[buffer_index] != ' ') && (buffer[buffer_index] != '\n') && (buffer[buffer_index] != '\0')) 
                {
                    buffer_index++;
                }
            }
        }
    }
    fclose(fptr);
    return data;
}

void trainDenseNetwork(network model, int data_size, double **data)
{
    for(layer *actual_layer = model.initial_layer; actual_layer != NULL; actual_layer = actual_layer->next)
    {
        for(size_t i = 0; i < actual_layer->height; i++)
        {
            for(size_t k = 0; k < actual_layer->neurons[i].input_size; k++)
            {
                double cost_w = costDenseNetwork(model, data_size, data); 
                actual_layer->neurons[i].weights[k] += EPSILON; 
                double cost_w_epsilon = costDenseNetwork(model, data_size, data);
                actual_layer->neurons[i].weights[k] -= EPSILON;
                // actual_layer->neurons[i].weights[k] -= signal(cost_w_epsilon - cost_w) * EPSILON;
                actual_layer->neurons[i].weights[k] -= (cost_w_epsilon - cost_w) * EPSILON;
            }
        }
    }
}

double signal(double x)
{
        if(x >= 0) return 1.0;
        return -1.0;
}

double costDenseNetwork(network model, int data_size, double **data)
{
    int input_size = model.input_size;
    int output_size = model.output_size;
    double carry = 0;
    for(size_t i = 0; i < (size_t) data_size; i++)
        // data[i] = {x1, x2, ..., xn, y1, y2, ..., yk}
        for(size_t k = 0; k < (size_t) output_size; k++)
            if(COST_FUNCTION == 0)
                carry += fabs((data[i][input_size + k] - (evaluateDenseInput(model, data[i]))[k])) / ((double) data_size);
            else if(COST_FUNCTION == 1) 
                carry += pow((data[i][input_size + k] - (evaluateDenseInput(model, data[i]))[k]), 2) / ((double) data_size);
    return carry;
}

double dotProd(int dim, double *v1, double *v2)
{
    double carry = 0.0;
    for(size_t i = 0; i < (size_t) dim; i++)
        carry += (v1[i] * v2[i]);
    return carry;
}

double *evaluateDenseInput(network model, double *input)
{
    layer *actual_layer;
    actual_layer = NULL;
    double *actual_layer_output;
    double *last_layer_output;
    actual_layer_output = NULL;
    last_layer_output = NULL;
    int last_layer_size = 0;
    for(size_t i = 0; i < (size_t) model.layers_num; i++)
    {
        if(actual_layer == NULL)
        {
            actual_layer = model.initial_layer;
            last_layer_output = malloc(sizeof(double) * actual_layer->height);
            last_layer_size = actual_layer->height;
            for(size_t j = 0; j < (size_t) actual_layer->height; j++)
            {
                last_layer_output[j] = dotProd(last_layer_size, actual_layer->neurons[j].weights, input);
            }
        }
        else
        {
            actual_layer = actual_layer->next;
            actual_layer_output = malloc(sizeof(double) * actual_layer->height);
            for(size_t j = 0; j < (size_t) actual_layer->height; j++)
            {
                actual_layer_output[j] = dotProd(last_layer_size, actual_layer->neurons[j].weights, last_layer_output);
            }
            free(last_layer_output);
            last_layer_output = actual_layer_output;
            last_layer_size = actual_layer->height; 
        }
    }
    return last_layer_output;
}

network genDenseNetwork(int layers_num, int layer_size, int input_size, int output_size)
{
    network model;
    model.initial_layer = NULL;
    model.final_layer = NULL;
    model.input_size = input_size;
    model.output_size = output_size;
    model.layers_num = layers_num;
    layer *l;
    for(size_t i = 0; i < (size_t) layers_num; i++)
    {
        // Caso da primeira camada:
        if(i == 0)
        {
            l = malloc(sizeof(layer));
            model.initial_layer = l;
            if(i != (layers_num - 1))
                l->height = layer_size;
            else
                l->height = output_size;
            l->neurons = malloc(sizeof(l->height) * sizeof(perceptron));
            l->previous = NULL;
            l->next = NULL;
            for(size_t j = 0; j < (size_t) l->height; j++)
            {
                l->neurons[j].input_size = input_size;
                l->neurons[j].b = 0;
                l->neurons[j].weights = malloc(sizeof(l->neurons[j].input_size) * sizeof(double));
                for(size_t k = 0; k < (size_t) l->neurons[j].input_size; k++)
                    l->neurons[j].weights[k] = 1;
                l->neurons[j].input = NULL;
            }
            continue;
        }
        else if(i != (layers_num - 1))
        {
            l->next = malloc(sizeof(layer));
            l->next->next = NULL;
            l->next->previous = l;
            l = l->next; 
            l->height = layer_size;
            l->neurons = malloc(sizeof(layer_size) * sizeof(perceptron));
            for(size_t j = 0; j < (size_t) l->height; j++)
            {
                l->neurons[j].input_size = l->previous[0].height;
                l->neurons[j].b = 0;
                l->neurons[j].weights = malloc(l->neurons[j].input_size * sizeof(double));
                l->neurons[j].input = malloc(l->neurons[j].input_size * sizeof(perceptron *));
                for(size_t k = 0; k < (size_t) l->neurons[j].input_size; k++)
                {
                    l->neurons[j].weights[k] = 1;
                    l->neurons[j].input[k] = &l->previous->neurons[k];
                }
            }
            continue;
        }
        else
        {
            l->next = malloc(sizeof(layer));
            l->next->next = NULL;
            l->next->previous = l;
            l = l->next; 
            model.final_layer = l;
            // A última camada deve ter o mesmo número de neurônios que o output:
            l->height = output_size;
            l->neurons = malloc(output_size * sizeof(perceptron));
            for(size_t j = 0; j < (size_t) l->height; j++)
            {
                l->neurons[j].input_size = l->previous[0].height;
                l->neurons[j].b = 0;
                l->neurons[j].weights = malloc(l->neurons[j].input_size * sizeof(double));
                l->neurons[j].input = malloc(l->neurons[j].input_size * sizeof(perceptron *));
                for(size_t k = 0; k < (size_t) l->neurons[j].input_size; k++)
                {
                    l->neurons[j].weights[k] = 1;
                    l->neurons[j].input[k] = &l->previous->neurons[k];
                }
            }
            continue;
        }
    }
    if(model.final_layer == NULL) model.final_layer = model.initial_layer;
    return model;
}

int clampi(int k, int low, int high)
{
    assert(low < high);
    if(k < low) k = low;
    if(k > high) k = high;
    return k;
}

double triangleArea(double xa, double ya, double xb, double yb, double xc, double yc)
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
    height = clampi(height, 1, input_height - y0);
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
    // printf("teste = %s\n", (1 == 1) ? "true" : "false");
    // printf("teste = %d\n", (bool) (-10000));
    double **data;
    int data_size, input_size, output_size;
    data = readData("data.txt", &data_size, &input_size, &output_size);
    // for(size_t i = 0; i < (size_t) data_size; i++)
    // {
    //     for(size_t j = 0; j < (size_t) (input_size + output_size); j++)
    //         printf("%lf ", data[i][j]);
    //     printf("\n");
    // }
    
    double input_test[] = {2, 2};

    int n = TRAINING_TIMES;
    int layers_num = 1;
    int layers_size = 1;
    network model = genDenseNetwork(layers_num, layers_size, input_size, output_size);
    printf("cost function value before %i trains: %lf\n", n, costDenseNetwork(model, data_size, data));
    printf("evaluate before %i trainings: %lf %lf\n\n", n, evaluateDenseInput(model, input_test)[0], evaluateDenseInput(model, input_test)[1]);
    for(size_t i = 0; i < (size_t) n; i++) trainDenseNetwork(model, data_size, data);
    printf("cost function value after %i trains: %lf\n", n, costDenseNetwork(model, data_size, data));
    printf("evaluate after %i trainings: %lf %lf\n\n", n, evaluateDenseInput(model, input_test)[0], evaluateDenseInput(model, input_test)[1]);

    printDenseNetwork(model);
}