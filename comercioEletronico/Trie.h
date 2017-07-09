#ifndef TRIE_H
#define TRIE_H
#include"TrieNo.h"
#include<string>
#include<vector>
#include<iostream>
#include<stdlib.h>

class Trie
{
    public:
        Trie();
        ~Trie();
        bool buscarPalavra(std::string palavra);
        void inserirPalavra(std::string palavra, int indice);
        std::vector<std::string> completaPalavra(std::string palavra);
        std::vector<int> recuperaIndices(std::string palavra);
        void imprimeTrie();
    private:
        TrieNo* raiz;
        int numChaves;
};

#endif // TRIE_H
