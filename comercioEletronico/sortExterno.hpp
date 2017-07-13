#ifndef SORTEXTERNO_HPP_INCLUDED
#define SORTEXTERNO_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef struct Heap{
    string produto;
    int indice;
}Heap;

typedef struct sProduto{
    string nome;
    string categoria;
    string descricao;
    float preco;
}produto;

void heapfy(vector<produto*> *v, int ind, int tam, int opcao);
void heapSort(vector<produto*> *v, int tam, int opcao);
void heapfyD(vector<produto*> *v, int ind, int tam, int opcao);
void heapSortD(vector<produto*> *v, int tam, int opcao);

int ordenaArquivosTemporarios(string inputFile, unsigned int numDados, int opcao, int ordem);
vector<string> splitString(const string& s, const char& c);
void heapfyArquivo(Heap** v, int ind, int tam, int opcao);
void heapfyArquivoD(Heap** v, int ind, int tam, int opcao);
void mergeArquivos(string outputFile, int numDados, int numParticoes, int opcao, int ordem) ;
void sortExterno(string input, string output, int numDados, int opcao, int ordem);




#endif // SORTEXTERNO_HPP_INCLUDED
