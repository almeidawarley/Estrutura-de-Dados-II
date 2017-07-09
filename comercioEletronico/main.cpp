#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
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
    int tamanho;
    base >> tamanho;
    produtos->reserve(tamanho);
    string nome, categoria, descricao;
    float preco;
    register int indice = 0;
    while(base >> nome >> categoria >> descricao >> preco){
        produto * leitura = new produto;
        leitura->categoria = categoria;
        leitura->nome = nome;
        leitura->preco = preco;
        leitura->descricao = descricao;
        produtos->push_back(leitura);
        iCategorias->inserirPalavra(categoria, indice);
        iProdutos->inserirPalavra(nome, indice);
        indice++;
    }
}

/*
*Funcao para imprimir a base atual de produtos
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
*@return -
*********************************************************/
void imprimeBase(vector<produto*> produtos){
    for(unsigned int i = 0; i < produtos.size(); i++){
        cout << produtos[i]->nome << " | " << produtos[i]->categoria << " | " << produtos[i]->preco << endl;
    }
}

//irá salvar a base novamente e desalocar os ponteiros para produtos do vector
void finaliza(vector<produto*> produtos, string caminho){
    ofstream saida(caminho.c_str());
    saida << produtos.size() << endl;
    for(unsigned int i = 0; i < produtos.size(); i++){
        saida << produtos[i]->nome << " " << produtos[i]->categoria << " " << produtos[i]->descricao << " " << produtos[i]->preco << endl;
        delete produtos[i];
        produtos[i] = NULL;
    }
}

void cadastrarProduto(vector<produto*> *produtos, Trie *iCategorias, Trie *iProdutos){
    string nome, categoria, descricao;
    float preco;
    produto * aCadastrar = new produto;
    cout << "CADASTRANDO PRODUTO..." << endl;
    cout << "Digite o nome do produto a ser cadastrado: "; cin >> aCadastrar->nome;
    cout << "Digite a categoria do produto a ser cadastrado: "; cin >> aCadastrar->categoria;
    cout << "Digite a descricao do produto a ser cadastrado: "; cin >> aCadastrar->descricao;
    cout << "Digite o preco do produto a ser cadastrado: "; cin >> aCadastrar->preco;
    iCategorias->inserirPalavra(aCadastrar->categoria, produtos->size());
    iProdutos->inserirPalavra(aCadastrar->nome, produtos->size());
    produtos->push_back(aCadastrar);
}

void buscarPorNome(vector<produto*> *produtos, Trie *iProdutos){
    cout << "BUSCANDO POR NOME..." << endl;
    string nome;
    cout << "***************************************************" << endl;
    cout << "Digite o nome do produto que deseja procurar (digite '.' para finalizar o nome): "<<endl;
    //cin >> nome;
    //!Ideia: Mas só funciona no windows
    char c = getch(); //Algum equivalente no linux? getchar() espera o enter para ler as letras :/
    if (system("CLS")) system("clear"); //Funciona no windows e no linux (mas não parece uma boa ideia)
    while(c!='.'){
        cout << "Digite o nome do produto que deseja procurar (digite '.' para finalizar o nome): "<<endl;
        nome = nome+c;
        cout<<nome<<endl;
        cout<<"Sugestoes:"<<endl;
        vector<string> sugestoes = iProdutos->completaPalavra(nome);
        for(unsigned int i = 0; i < sugestoes.size(); i++)
            cout << i+1 << "] " << sugestoes[i] << endl;
        c = getch();
        if (system("CLS")) system("clear");
    }
    //! Fim da ideia. Se não der substituir isso por cin>>nome (que está comentado ali em cima)


    if(iProdutos->buscarPalavra(nome)){
        vector<int> indices = iProdutos->recuperaIndices(nome);
        cout << "Produtos encontrados! Informacoes: " << endl;
        for(int k = 0; k < indices.size(); k++){
            cout << " | Nome: " << (*produtos)[indices[k]]->nome << endl;
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
void buscarPorCategoria(vector<produto*> *produtos, Trie *iCategorias){
    cout << "BUSCANDO POR CATEGORIA" << endl;
    string categoria;
    cout << "***************************************************" << endl;
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

void relatorio(vector<produto*> *produtos){
    cout << "RELATORIO..." << endl;
}

int main(){
    //wObdYSEvWRBLzZ02h67Y

    int opcao = 0;
    string caminho;
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
        cout << "***************************************************" << endl << endl;
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
        cout << "***************************************************" << endl << endl;
        switch(opcao){
            case 2: finaliza(produtos, caminho); return 0;
            default: break;
        }
    }
    finaliza(produtos, caminho);
    return 0;
}
