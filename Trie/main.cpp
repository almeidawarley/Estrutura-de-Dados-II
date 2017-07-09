#include <iostream>
#include<vector>
#include<string>
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
    cout<<nova->buscarPalavra("fator")<<" "<<nova->buscarPalavra("fatos")<<endl;
    std::vector<std::string> palavras = nova->completaPalavra("tes");
    for(int i = 0;i<palavras.size();i++){
        cout<<palavras[i]<<endl;
    }

    return 0;
}
