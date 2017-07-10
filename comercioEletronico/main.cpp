#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include "Trie.h"

//!Não sei se funciona no linux
#include<conio.h>

using namespace std;

//Struct para armazenamento computacional do produto
typedef struct sProduto{
    string nome;
    string categoria;
    string descricao;
    float preco;
}produto;

//joguei numa funcao porque, caso decidamos fazer outra coisa, dá pra alterar facilmente
void clearScreen(){
    if(system("CLS")) system("clear");
}


/*
*Funcao para imprimir a base atual de produtos
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
*@return -
*********************************************************/
void imprimeBase(vector<produto*> *produtos){
    for(unsigned int i = 0; i < produtos->size(); i++){
        cout << (*produtos)[i]->nome << " | " << (*produtos)[i]->categoria << " | " << (*produtos)[i]->descricao << " | " << (*produtos)[i]->preco << endl;
    }
}

/*
*Funcao para ler a base de um caminho informado por parâmetro
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         Trie *iCategorias:             TRIE para indexação das categorias
         Trie *iProdutos:               TRIE para indexação dos produtos
         string caminho:                caminho do arquivo da base
*@return -
*********************************************************/
void lerBase(vector<produto*> *produtos, Trie *iCategorias, Trie *iProdutos, string caminho){
    ifstream base(caminho.c_str());
    string nome, categoria, descricao, preco, tamanho, enter;
    int numeroRegistros = 0;
    double meta = 0;
    char quebra = (char) 10; // quebra de linha
    getline(base, tamanho, ';');
    getline(base, enter, quebra); // a ideia é ler o enter no final da linha para que não seja transportado para a string
    numeroRegistros = atoi(tamanho.c_str());
    produtos->reserve(numeroRegistros);
    register int indice = 0;
    cout << "Lendo registros de " << caminho << ": 000%";
    while(getline(base, nome, ';')){
        getline(base, categoria, ';');
        getline(base, descricao, ';');
        getline(base, preco, ';');
        getline(base, enter, quebra);
        produto * leitura = new produto;
        leitura->categoria = categoria;
        leitura->nome = nome;
        leitura->preco = strtof(preco.c_str(), 0);
        leitura->descricao = descricao;
        produtos->push_back(leitura);
        iCategorias->inserirPalavra(categoria, indice);
        iProdutos->inserirPalavra(nome, indice);
        indice++;
        if((float)indice/numeroRegistros > meta){
            printf("\b\b\b\b%3.f%%", meta * 100);
            meta += 0.1;
        }
    }
    cout <<"\b\b\b\b100%"<< endl;
    //imprimeBase(produtos);
}

/*
*Funcao para desalocar os produtos e atualizar a base conforme caminho informado por parâmetro
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         string caminho:                caminho do arquivo da base
*@return -
*********************************************************/
void finaliza(vector<produto*> produtos, string caminho){
    ofstream saida(caminho.c_str());
    double meta = 0;
    saida << produtos.size() << endl;
    cout << "Atualizando registros em " << caminho << ": 000%";
    for(unsigned int i = 0; i < produtos.size(); i++){
        saida << produtos[i]->nome << ";" << produtos[i]->categoria << ";" << produtos[i]->descricao << ";" << produtos[i]->preco << ";";
        delete produtos[i];
        produtos[i] = NULL;
        if(i+1 != produtos.size()) saida << endl;
        if((float)(produtos.size()-i)/produtos.size() > meta){
            printf("\b\b\b\b%3.f%%", meta * 100);
            meta += 0.1;
        }
    }
    cout <<"\b\b\b\b100%"<< endl;
}

/*
*Funcao para cadastrar novo produto na base
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         Trie *iCategorias:             TRIE para indexação das categorias
         Trie *iProdutos:               TRIE para indexação dos produtos
*@return -
*********************************************************/
void cadastrarProduto(vector<produto*> *produtos, Trie *iCategorias, Trie *iProdutos){
    string preco;
    produto * aCadastrar = new produto;
    cout << "CADASTRANDO PRODUTO..." << endl;
    char quebra = (char) 10;
    cin.ignore();
    cout << "Digite o nome do produto a ser cadastrado: ";
    getline(cin, aCadastrar->nome, quebra);
    cout << "Digite a categoria do produto a ser cadastrado: ";
    getline(cin, aCadastrar->categoria, quebra);
    cout << "Digite a descricao do produto a ser cadastrado: ";
    getline(cin, aCadastrar->descricao, quebra);
    cout << "Digite o preco do produto a ser cadastrado: ";
    getline(cin, preco, quebra);
    aCadastrar->preco = strtof(preco.c_str(), 0);
    iCategorias->inserirPalavra(aCadastrar->categoria, produtos->size());
    iProdutos->inserirPalavra(aCadastrar->nome, produtos->size());
    produtos->push_back(aCadastrar);
}

/*
*Funcao para buscar produto na base pelo nome
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         Trie *iProdutos:               TRIE para indexação dos produtos
*@return -
*********************************************************/
void buscarPorNome(vector<produto*> *produtos, Trie *iProdutos){
    cout << "BUSCANDO POR NOME..." << endl;
    string nome;
    cout << "Digite o nome do produto que deseja procurar (digite '.' para finalizar o nome): "<<endl;
    //cin >> nome;
    //!Ideia: Mas só funciona no windows
    char c = getch(); //Algum equivalente no linux? getchar() espera o enter para ler as letras :/
    clearScreen(); //Funciona no windows e no linux (mas não parece uma boa ideia)
    while(c!='.'){
        cout << "Digite o nome do produto que deseja procurar (digite '.' para finalizar o nome): "<<endl;
        nome = nome+c;
        cout<<nome<<endl;
        cout<<"Sugestoes:"<<endl;
        vector<string> sugestoes = iProdutos->completaPalavra(nome);
        for(unsigned int i = 0; i < sugestoes.size(); i++)
            cout << i+1 << "] " << sugestoes[i] << endl;
        c = getch();
        clearScreen();
    }
    //! Fim da ideia. Se não der substituir isso por cin>>nome (que está comentado ali em cima)


    if(iProdutos->buscarPalavra(nome)){
        vector<int> indices = iProdutos->recuperaIndices(nome);
        cout << "Produtos encontrados! Informacoes: " << endl;
        for(int k = 0; k < indices.size(); k++){
            cout << " > Nome: " << (*produtos)[indices[k]]->nome << endl;
            cout << " | Categoria: " << (*produtos)[indices[k]]->categoria << endl;
            cout << " | Descricao: " << (*produtos)[indices[k]]->descricao << endl;
            cout << " | Preco: " << (*produtos)[indices[k]]->preco << endl;
        }
    }else{
        cout << "Produto nao encontrado. Que tal: " << endl;
        vector<string> retorno = iProdutos->completaPalavra(nome);
        for(unsigned int i = 0; i < retorno.size(); i++)
            cout << i+1 << "] " << retorno[i] << endl;
    }

}

/*
*Funcao para buscar produto na base pela categoria
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         Trie *iCategorias:             TRIE para indexação das categorias
*@return -
*********************************************************/
void buscarPorCategoria(vector<produto*> *produtos, Trie *iCategorias){
    /// TODO: fazer o mesmo que for produzido no buscar por nome
    cout << "BUSCANDO POR CATEGORIA" << endl;
    string categoria;
    cout << "Digite a categoria que deseja procurar: "; cin >> categoria;
    if(iCategorias->buscarPalavra(categoria)){
        cout << "Produto encontrado! Informacoes: " << endl;
    }else{
        cout << "Produto nao encontrado. Que tal: " << endl;
        vector<string> retorno = iCategorias->completaPalavra(categoria);
        for(unsigned int i = 0; i < retorno.size(); i++)
            cout << i+1 << "] " << retorno[i] << endl;

    }
}

/*
*Funcao para gerar relatório de todos os produtos na base
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
*@return -
*********************************************************/
void relatorio(vector<produto*> *produtos){
    cout << "RELATORIO..." << endl;
    /// TODO: receber do usuário sobre qual característica deseja ordenar
    /// e implementar sort (mergeSort ou heapSort) que leva em consideração a característica selecionada
}

int main(){
    int opcao = 0;
    string caminho;
    cout << "***************************************************" << endl;
    cout << "Informe o arquivo de origem dos registros: "; cin >> caminho;
    vector<produto*> produtos;
    Trie iCategorias, iProdutos;
    lerBase(&produtos, &iCategorias, &iProdutos, caminho);
    while(opcao!=5){
        cout << "***************************************************" << endl;
        cout << "Bem-vindo ao Toraku, sistema de comercio eletronico" << endl;
        cout << "Escolha uma opcao abaixo: " << endl;
        cout << "1) cadastrar novo produto" << endl;
        cout << "2) buscar produto por nome" << endl;
        cout << "3) buscar produto por categoria" << endl;
        cout << "4) relatorio dos produtos" << endl;
        cout << "5) sair" << endl;
        cout << "Digite sua escolha: ";
        cin >> opcao;
        cout << "***************************************************" << endl;
        switch(opcao){
            case 1: cadastrarProduto(&produtos, &iCategorias, &iProdutos); break;
            case 2: buscarPorNome(&produtos, &iProdutos); break;
            case 3: buscarPorCategoria(&produtos, &iCategorias); break;
            case 4: relatorio(&produtos); break;
            case 5: finaliza(produtos, caminho); return 0;
            default: cout << "Opcao invalida, tente novamente" << endl;
        }
        cout << "***************************************************" << endl;
        cout << "Deseja selecionar outra opcao?" << endl;
        cout << "1) sim\t\t2) nao" << endl;
        cout << "Digite sua escolha: ";
        cin >> opcao;
        cout << "***************************************************" << endl;
        switch(opcao){
            case 2: finaliza(produtos, caminho); return 0;
            default: break;
        }
        clearScreen();
    }
    finaliza(produtos, caminho);
    return 0;
}
