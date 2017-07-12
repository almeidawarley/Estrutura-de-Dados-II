#include "Trie.h"

Trie::Trie()
{
    raiz = NULL;
}

Trie::~Trie()
{
    //dtor
}

/*
*Função para buscar uma palavra na Trie
*@param  string palavra:    palavra que será procurada na Trie
*@return bool:              retorna true caso a palavra seja encontrada e false caso contrário
*/
bool Trie::buscarPalavra(std::string palavra){
    int tam = palavra.size();
    int i=0;
    TrieNo* aux = raiz;
    TrieNo* anterior = NULL;
    //Busco o maior prefixo até chegar no NULL
    while(aux!=NULL && i<tam){
        anterior = aux;
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
    if(i==tam && anterior->getEhChave()) return true;
    else return false;
}

/*
*Função para inserir uma palavra na Trie
*@param string palavra: palavra que será inserida na Trie
        int indice:     local do produto cuja chave está sendo inserida na Trie
*@return -
*/
void Trie::inserirPalavra(std::string palavra, int indice){
    int tam = palavra.size();
    int i=0;
    bool direita = false,desceu = false; //Indicadores dos caminhos a cada iteração do while
    TrieNo* aux = raiz;
    TrieNo* anterior = NULL;
    //Busco o maior prefixo até chegar no NULL
    while(aux!=NULL && i<tam){
        anterior = aux;
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

    //Se inseriu a palavra coloco que o anterior ao anterior é fim de chave, adicionando a posição do produto no vetor de indices do nó
    if(i == tam){
        anterior->setChave(indice);
        return;
    }


    //Cria o nó inicial para continuar a inserir a partir do NULL
    TrieNo* novo = new TrieNo(palavra[i]);
    i++;

    //Conectando o anterior ao novo nó criado
    if(anterior!=NULL && !desceu){
        if(direita) anterior->setDir(novo);
        else anterior->setEsq(novo);
    }
    if(anterior!=NULL && desceu){
        anterior->setMeio(novo);
    }
    //Caso a Trie esteja vazia inicialmente colocamos o novo nó como raiz
    if(raiz == aux){
        raiz = novo;
    }
    aux = novo;

    //Insiro o restante da palavra criando os níveis inferiores
    while(i<tam){
        anterior = aux;
        TrieNo* novo = new TrieNo(palavra[i]);
        aux->setMeio(novo);
        i++;
        aux = aux->getMeio();
    }
    aux->setChave(indice);

}

/*
*Função auxliar da impressão da Trie, imprime recursivamente a partir de um nó
*@param TrieNo* no:       nó que contém a nova letra a ser inserida na palavra atual
        string atual:     palavra atual que será impressa quando chegar em uma chave
*@return -
*/
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

/*
*Função de impressão da Trie, apenas imprime recursivamente a partir da raiz
*@param -
*@return -
*/
void Trie::imprimeTrie(){
    imprimeRecursivo(raiz);
}


/*
*Função auxliar do completaPalavra, insere as palavras com o prefixo atual no vector palavras
*@param TrieNo* no:             nó que contém a nova letra a ser inserida na palavra atual
        string atual:           palavra atual que será inserida quando chegar em uma chave
        vector<string> *atual:  ponteiro para um vector que irá acumular as palavras encontradas
*@return -
*/
void completaRecursivo(TrieNo* no,std::string atual,std::vector<std::string> *palavras){
    if(no == NULL || palavras->size() >= 8){
        return;
    }
    completaRecursivo(no->getDir(),atual,palavras);
    completaRecursivo(no->getEsq(),atual,palavras);
    atual = atual + no->getLetra();
    if(no->getEhChave()) palavras->push_back(atual);
    completaRecursivo(no->getMeio(),atual,palavras);
}

/*
*Função de auto-completar que completa uma dada palavra ou o maior prefixo que é prefixo de alguma chave da trie
*@param string palavra:  palavra que será completada pela trie (ou será utilizada para buscar uma sugestão)
*@return vector<string>: armazena as sugestões de chaves da trie possíveis que completa um dado prefixo
*/
std::vector<std::string> Trie::completaPalavra(std::string palavra){
    int tam = palavra.size();
    int i=0;
    TrieNo *aux = raiz;
    TrieNo *prev = aux;
    std::vector<std::string> palavras;

    //Essa linha faz o auto-completar sugerir o próprio prefixo caso ele já seja uma chave
    if(buscarPalavra(palavra)) palavras.push_back(palavra);

    while(aux!=NULL && i<tam){
        if(palavra[i]==aux->getLetra()){
            // Atualiza o ponteiro "prev" apenas quando encontrar uma letra
            aux = aux->getMeio();
            prev = aux;
            i++;
        }else{
            if(palavra[i]>aux->getLetra()){
                aux = aux->getDir();
            }else{
                aux = aux->getEsq();
            }
        }
    }
    if (aux == NULL) {
        completaRecursivo(prev, palavra.substr(0, i), &palavras);
    } else {
        completaRecursivo(aux, palavra, &palavras);
    }
    return palavras;

}

/*
*Função que recupera as posições(indices) de produtos com a chave encontrada na Trie
*@param string palavra:  chave que será buscada cujos produtos serão retornados
*@return vector<int>:    armazena as posições dos produtos encontradas na chave buscada
*/
std::vector<int> Trie::recuperaIndices(std::string palavra){
    int tam = palavra.size();
    int i=0;
    TrieNo* aux = raiz;
    TrieNo* anterior = NULL;
    //Considero aqui que a palavra já está na Trie como chave
    while(aux!=NULL && i<tam){
        anterior = aux;
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
    return anterior->getIndices();
}
