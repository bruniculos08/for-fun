#include <stdio.h>
#include <string.h>
#include <math.h>

unsigned long long GCD(unsigned long long a, unsigned long long b){
    unsigned long long aux;
    while(b != 0){
        aux = b;
        b = a % b;
        a = aux;
    }
    return a;
}

unsigned long long Coprime(unsigned long long phi){
    for (unsigned long long i = 2; i < phi; i++){
        if(GCD(phi, i) == 1) return i;
    }
    return 1;
}

unsigned long long InverseModulo(unsigned long long a, unsigned long long phi){
    for (unsigned long long i = 1; i < phi; i++){
        if((a*i)%phi == 1) return i;
    }
    return a;
}

unsigned long long PowInt(unsigned long long x, unsigned long long exp){
    if(exp == 0) return 1;
    return x * PowInt(x, exp-1);
}

// Criptografia RSA
// p = 41
// q = 47
// N = 1927
// (p-1)(q-1) = 1840
int main(void){
    unsigned long long p, q, N, phi, s, r;
    p = 11;
    q = 5;
    N = p*q;
    phi = (p-1)*(q-1);
    
    s = Coprime(phi);
    r = InverseModulo(s, phi);

    unsigned long long mensagem = 5;
    unsigned long long mensagem_publicada = PowInt(mensagem, s) % N;
    unsigned long long mensagem_recebida = PowInt(mensagem_publicada, r) % N;

    printf("mensagem publicada = %llu\n", mensagem_publicada);
    printf("mensagem recebida = %llu\n", mensagem_recebida);
    return 0;
}

