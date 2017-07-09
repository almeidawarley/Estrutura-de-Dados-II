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


//Setds:
void TrieNo::setEsq(TrieNo* no){esq = no;}
void TrieNo::setDir(TrieNo* no){dir = no;}
void TrieNo::setMeio(TrieNo* no){meio = no;}
void TrieNo::setChave(bool chave){ehChave = chave;}

//Gets:
TrieNo* TrieNo::getEsq(){return esq;}
TrieNo* TrieNo::getDir(){return dir;}
TrieNo* TrieNo::getMeio(){return meio;}
char TrieNo::getLetra(){return letra;}
bool TrieNo::getEhChave(){return ehChave;}
