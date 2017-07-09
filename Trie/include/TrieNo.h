#ifndef TRIENO_H
#define TRIENO_H

class TrieNo
{
    public:
        TrieNo(char letra);
        ~TrieNo();

        //Sets:
        void setEsq(TrieNo* no);
        void setDir(TrieNo* no);
        void setMeio(TrieNo* no);
        void setChave(bool chave);

        //Gets
        TrieNo* getEsq();
        TrieNo* getDir();
        TrieNo* getMeio();
        char getLetra();
        bool getEhChave();
    private:
        TrieNo *esq;
        TrieNo *dir;
        TrieNo *meio;
        char letra;
        bool ehChave;


};

#endif // TRIENO_H
