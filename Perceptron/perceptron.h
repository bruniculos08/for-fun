#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#define EPSILON 0.01
// Para a função de custo: 0 -> mean of abs value differences, 1 -> mean of squared differences 
#define COST_FUNCTION 0
#define TRAINING_TIMES 50000

typedef struct Perceptron perceptron;

struct Perceptron
{
    // Os inputs de um "perceptron" são os outputs dos "perceptrons" da camanda...
    // ... anterior ligados e ele:
    perceptron **input;
    int input_size;
    // O número de pesos é igual ao número de inputs:
    double *weights;
    // Constante bias:
    double b;
};

typedef struct Layer layer;

struct Layer
{
    // Uma camada é um vetor de "perceptrons":
    perceptron *neurons;
    int height;
    // As camadas serão colocadas em uma lista encadeada e portanto são necessárias...
    // ... as seguintes informações:
    layer *previous;
    layer *next;
};

typedef struct Network
{
    int input_size;
    // Uma rede tem um número de outputs que diz portanto o número de perceptrons em sua última camada:
    int output_size;
    int layers_num;
    // Camada inicial:
    layer *initial_layer;
    // Camada final:
    layer *final_layer;
} network;

// Função para "imprimir" rede neural densa:
void printDenseNetwork(network model);
// Função para gerar vetor de dados:
double **readData(char *file_name, int *data_size, int *input_size, int *output_size);
// Função para multiplicar vetores de doubles:
double dotProd(int dim, double *v1, double *v2);
// Função para gerar rede com o máximo de perceptrons e conexões entre as camadas:
network genDenseNetwork(int layers_num, int layer_size, int input_size, int output_size);
// Função para calcular vetor de output de uma rede neural densa (i.e., todo neurônio da camada i recebe todos os outputs da camada i-1)...
// com base em um vetor de input:
double *evaluateDenseInput(network model, double *input); 
// Função de custo (erro):
double costDenseNetwork(network model, int data_size, double **data);
// Função de treino:
void train(network model, int data_size, double **data);
// Função para colocar k no intervalo [low,high]:
int clampi(int k, int low, int high);
// Função para retornar 1 ou -1 de acordo com o sinal de x:
double signal(double x);
// Função para calcular área de um triângulo de vértices (xa,ya), (xb,yb) e (xc,yc):
double triangleArea(double xa, double ya, double xb, double yb, double xc, double yc);
// Função para verificar se um ponto (xp,yp) está contido no triângulo de vértices (xa,ya), (xb,yb) e (xc,yc):
bool pointInsideTriangle(double xp, double yp, double xa, double ya, double xb, double yb, double xc, double yc);
// Desenha em uma matriz "input" um retângulo de dimensões width e height:
void generateRect(double **input, int input_width, int input_height, double value, int x0, int y0, int width, int height);