#include "Trie.h"

Trie::Trie()
{
    raiz = NULL;
}

Trie::~Trie()
{
    //dtor
}


bool Trie::buscarPalavra(std::string palavra){
    int tam = palavra.size();
    int i=0;
    TrieNo* aux = raiz;
    //Busco o maior prefixo até chegar no NULL
    while(aux!=NULL && i<tam){
        if(palavra[i]==aux->getLetra()){
            aux = aux->getMeio();
            i++;
        }else{
            if(palavra[i]>aux->getLetra()){
                aux = aux->getDir();
            }else{
                aux = aux->getEsq();
            }
        }
    }
    if(i==tam) return true;
    else return false;
}


void Trie::inserirPalavra(std::string palavra){
    int tam = palavra.size();
    int i=0;
    bool direita = false,desceu = false; //Indicadores dos caminhos tomados até o NULL
    TrieNo* aux = raiz;
    TrieNo* aux2 = NULL;
    //Busco o maior prefixo até chegar no NULL
    while(aux!=NULL && i<tam){
        aux2 = aux;
        if(palavra[i]==aux->getLetra()){
            aux = aux->getMeio();
            desceu = true;
            i++;
        }else{
            if(palavra[i]>aux->getLetra()){
                aux = aux->getDir();
                direita = true;
                desceu = false;
            }else{
                aux = aux->getEsq();
                direita = false;
                desceu = false;
            }
        }
    }

    //Se inseriu a palavra coloco que o anterior ao aux é fim de chave, e finalizo a função
    if(i == tam){
        aux2->setChave(true);
        return;
    }


    //Cria o nó inicial para continuar a inserir a partir do NULL
    TrieNo* novo = new TrieNo(palavra[i]);
    i++;

    //Corrigindo ponteiros em relação ao nó não nulo criado
    if(aux2!=NULL && !desceu){
        if(direita) aux2->setDir(novo);
        else aux2->setEsq(novo);
    }
    if(aux2!=NULL && desceu){
        aux2->setMeio(novo);
    }
    if(raiz == aux){
        raiz = novo;
    }
    aux = novo;

    //Insiro o restante da palavra criando os níveis inferiores
    while(i<tam){
        aux2 = aux;
        TrieNo* novo = new TrieNo(palavra[i]);
        aux->setMeio(novo);
        i++;
        aux = aux->getMeio();
    }
    aux->setChave(true);

}

void imprimeRecursivo(TrieNo* no,std::string atual = ""){
    if(no == NULL){
        return;
    }
    imprimeRecursivo(no->getDir(),atual);
    imprimeRecursivo(no->getEsq(),atual);
    atual = atual + no->getLetra();
    if(no->getEhChave()) std::cout<<atual<<std::endl;
    imprimeRecursivo(no->getMeio(),atual);
}

void Trie::imprimeTrie(){
    imprimeRecursivo(raiz);
}



void completaRecursivo(TrieNo* no,std::string atual,std::vector<std::string> *palavras){
    if(no == NULL){
        return;
    }
    completaRecursivo(no->getDir(),atual,palavras);
    completaRecursivo(no->getEsq(),atual,palavras);
    atual = atual + no->getLetra();
    if(no->getEhChave()) palavras->push_back(atual);
    completaRecursivo(no->getMeio(),atual,palavras);
}

std::vector<std::string> Trie::completaPalavra(std::string palavra){
    int tam = palavra.size();
    int i=0;
    TrieNo* aux = raiz;
    std::vector<std::string> palavras;
    while(aux!=NULL && i<tam){
        if(palavra[i]==aux->getLetra()){
            aux = aux->getMeio();
            i++;
        }else{
            if(palavra[i]>aux->getLetra()){
                aux = aux->getDir();
            }else{
                aux = aux->getEsq();
            }
        }
    }
    if(aux == NULL) return palavras;

    //!Essa linha aqui faz o auto-completar sugerir o próprio prefixo caso ele já seja uma chave
    if(buscarPalavra(palavra)) palavras.push_back(palavra);
    //!

    completaRecursivo(aux,palavra,&palavras);
    return palavras;

}

