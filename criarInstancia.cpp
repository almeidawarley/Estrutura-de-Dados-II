#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <stdlib.h>
#include <iomanip>

using namespace std;


static const char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

void gen_random(string *nova, const int len) {
    nova->clear();
    for (int i = 0; i < len; ++i) {
        (*nova) += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
}

int main(int argc, char *argv[]){
    if(argc <= 2) {
        cout << "Numero de parametros insuficiente" << endl;
        cout << "Uso: programa <numero_instancias> <arquivo_saida>" << endl;
        return 0;
    }
    int tamanho = atoi(argv[1]);
    ofstream saida(argv[2]);
    string nome, categoria, descricao;
    float preco = 0, decimal = 0;
    saida << tamanho << endl;
    for(int i = 0; i < tamanho; i++){
        gen_random(&nome, 50);
        gen_random(&descricao, 500);
        gen_random(&categoria, 20);
        do{
            preco = (float)(rand()%1000);
            decimal = ((float)(rand()%100))/100.0;
        }while(preco < 100);
        preco += decimal;
        saida << nome << ";" << categoria << ";" << descricao << ";";
        saida << setw(7) << setfill('0') << setprecision(3) << preco << ";" << endl;
    }
}
