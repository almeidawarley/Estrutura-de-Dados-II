#include <iostream>
#include<Trie.h>
using namespace std;

int main()
{
    Trie *nova = new Trie();
    nova->inserirPalavra("teste");
    nova->inserirPalavra("testa");
    nova->inserirPalavra("tes");
    nova->inserirPalavra("fato");
    nova->inserirPalavra("fator");
    nova->imprimeTrie();
    cout<<nova->buscarPalavra("fator")<<" "<<nova->buscarPalavra("fatos");
    return 0;
}
