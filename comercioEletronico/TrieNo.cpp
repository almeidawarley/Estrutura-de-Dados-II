#include "TrieNo.h"
#include<stdlib.h>

TrieNo::TrieNo(char letra)
{
    this->letra = letra;
    esq = NULL;
    dir = NULL;
    meio = NULL;
    ehChave = false;
}

TrieNo::~TrieNo()
{
    //dtor
}


/*
*Função para adicionar um indice no Nó e considerá-lo um nó-chave
*@param  int indice: linha do arquivo onde se localiza o produto identificado pela chave
*@return -
*/
void TrieNo::setChave(int indice){
    ehChave = true;
    indices.push_back(indice);
}

//Sets:
void TrieNo::setEsq(TrieNo* no){esq = no;}
void TrieNo::setDir(TrieNo* no){dir = no;}
void TrieNo::setMeio(TrieNo* no){meio = no;}


//Gets:
TrieNo* TrieNo::getEsq(){return esq;}
TrieNo* TrieNo::getDir(){return dir;}
TrieNo* TrieNo::getMeio(){return meio;}
char TrieNo::getLetra(){return letra;}
bool TrieNo::getEhChave(){return ehChave;}
std::vector<int> TrieNo::getIndices(){return indices;}
