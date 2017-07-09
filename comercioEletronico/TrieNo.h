#ifndef TRIENO_H
#define TRIENO_H
#include <vector>

class TrieNo
{
    public:
        TrieNo(char letra);
        ~TrieNo();

        //Sets:
        void setEsq(TrieNo* no);
        void setDir(TrieNo* no);
        void setMeio(TrieNo* no);
        void setChave(bool chave, int indice);

        //Gets
        TrieNo* getEsq();
        TrieNo* getDir();
        TrieNo* getMeio();
        char getLetra();
        bool getEhChave();
        std::vector<int> getIndices();
    private:
        TrieNo *esq;
        TrieNo *dir;
        TrieNo *meio;
        char letra;
        bool ehChave;
        std::vector<int> indices;


};

#endif // TRIENO_H
