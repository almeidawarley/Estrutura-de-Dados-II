#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include "sortExterno.hpp"
#include "Trie.h"

using namespace std;

/*
*Funcao para limpar a tela no Windows e no Linux
*@param -
*@return -
*********************************************************/
void clear(){
    if(system("CLS")) system("clear");
}

/*
*Funcao para preencher uma string com caracteres vazios
*@param  string palavra:    string que sera preenchida
*@param  int tamanho:       tamanho da string de retorno
*@param  char caracter:     caracter que sera usado para preencher a string
*@return string:            string do tamanho especificado preenchida com caracteres vazios
*********************************************************/
string padString(string palavra, int tamanho, char caracter = '\0')
{
    // A string precisa ser menor ou igual ao tamanho final
    if (palavra.size() < tamanho)
        return palavra + std::string(tamanho - palavra.size(), caracter);
    else
        return palavra;
}

/*
*Funcao para ler a base de um caminho informado por parametro
*@param  Trie *iCategorias:             TRIE para indexacao das categorias
         Trie *iProdutos:               TRIE para indexacao dos produtos
         string caminho:                caminho do arquivo da base
*@return -
*********************************************************/
int lerBase(Trie *iCategorias, Trie *iProdutos, string caminho){
    ifstream base(caminho.c_str());
    string nome, categoria, descricao, preco, tamanho, enter;
    int numeroRegistros = 0;
    double meta = 0;
    char quebra = (char) '\n';
    getline(base, tamanho);
    numeroRegistros = atoi(tamanho.c_str());
    register int indice = 1;
    cout << "Lendo registros de " << caminho << ": 000%";
    while(getline(base, nome, ';')){
        getline(base, categoria, ';');
        getline(base, descricao, ';');
        getline(base, preco, ';');
        getline(base, enter, quebra);
        iCategorias->inserirPalavra(categoria.substr(0,categoria.find('\0')), indice);
        iProdutos->inserirPalavra(nome.substr(0,nome.find('\0')) , indice);
        if((float)indice/numeroRegistros > meta){
            printf("\b\b\b\b%3.f%%", meta * 100);
            meta += 0.1;
        }
        indice++;
    }
    cout <<"\b\b\b\b100%"<< endl;
    return numeroRegistros;
}

/*
*Funcao para cadastrar novo produto na base
*@param  Trie *iCategorias:             TRIE para indexacao das categorias
         Trie *iProdutos:               TRIE para indexacao dos produtos
         string caminho:                caminho do arquivo da base
         int *numeroRegistros:          ponteiro para inteiro que contem número de registros da base
*@return -
*********************************************************/
void cadastrarProduto(Trie *iCategorias, Trie *iProdutos, string caminho, int *numeroRegistros){
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

    (*numeroRegistros) += 1;
    iCategorias->inserirPalavra(aCadastrar->categoria,(*numeroRegistros));
    iProdutos->inserirPalavra(aCadastrar->nome,(*numeroRegistros));
    fstream base(caminho.c_str(), ios::app);
    base << padString(aCadastrar->nome, 50) << ";" << padString(aCadastrar->categoria, 20) << ";" << padString(aCadastrar->descricao, 500) << ";";
    base << setw(7) << setfill('0') << setprecision(3) << aCadastrar->preco << ";" << endl;

    base.close();
    fstream registro(caminho.c_str());
    registro << (*numeroRegistros) << endl;
    registro.close();
}

/*
*Funcao para recuperar as informações de um produto no arquivo a partir do índice (linha)
*@param  string caminho:  caminho para o arquivo que contém a base
         int linha:       linha do arquivo onde estão as informações
*@return produto: retorna uma struct com os dados obtidos da linha fornecida
*********************************************************/
string getProduto(string caminho, int linha){
    ifstream base(caminho.c_str());
    string linhaDescartavel;
    string linhaProduto;
    base>>linhaDescartavel;
    int pular = linhaDescartavel.size()+2; //descartar também \n e \r
    getline(base.seekg(pular+583*(linha-1)), linhaProduto, '\n'); //onde 583 é a quantidade de caracteres em cada linha de produto
    produto resultado;
    return linhaProduto;
}

/*
*Funcao para imprimir o relatorio do resultado da busca
*@param  string caminho:        caminho do arquivo da base
*@return -
*********************************************************/
void imprimeBuscaNaoOrdenada(string caminho){
    ifstream base;
    base.open(caminho);
    string nome, categoria, descricao, preco,tamanho,enter;
    char quebra = (char) '\n';
    getline(base,tamanho,quebra);
    while(getline(base, nome, ';')){
        getline(base, categoria, ';');
        getline(base, descricao, ';');
        getline(base, preco, ';');
        getline(base, enter, quebra);
        cout << " > Nome: " << nome << endl;
        cout << " | Categoria: " << categoria << endl;
        cout << " | Descricao: " << descricao << endl;
        cout << " | Preco: " << strtof(preco.c_str(),0) << endl;
    }
}

/*
*Funcao para imprimir o relatorio do resultado da busca de forma ordenada por preco
*@param  string caminho:        caminho do arquivo da base
         int ordem     :        ordem de ordenação (1: crescente |2: decrescente)
*@return -
*********************************************************/
void imprimeBuscaOrdenada(string caminho, int ordem){
    sortExterno(caminho, "buscaOrdenada.txt", 500000, 3, ordem);
    ifstream base;
    base.open("buscaOrdenada.txt");
    string nome, categoria, descricao, preco, tamanho, enter;
    char quebra = (char) '\n';
    while(getline(base, nome, ';')){
        getline(base, categoria, ';');
        getline(base, descricao, ';');
        getline(base, preco, ';');
        getline(base, enter, quebra);
        cout << " > Nome: " << nome << endl;
        cout << " | Categoria: " << categoria << endl;
        cout << " | Descricao: " << descricao << endl;
        cout << " | Preco: " << strtof(preco.c_str(),0) << endl;
    }
}

/*
*Funcao para buscar produto na base pelo nome
*@param  string caminho                 caminho do arquivo da base
         Trie *iProdutos:               TRIE para indexacao dos produtos
*@return -
*********************************************************/
void buscarPorNome(string caminho, Trie *iProdutos){
    cout << "BUSCANDO POR NOME..." << endl;
    string nome;
    cout << "Digite o nome do produto que deseja procurar (digite '.' para finalizar o nome): "<<endl;
    char c = getch();
    clear();
    while(c!='\r'){
        cout << "Digite o nome do produto que deseja procurar: "<<endl;
        if (c == '\b') {
            // Remove o ultimo caracter da string se a tecla pressionada for backspace
            if (nome.size() > 0)
                nome.erase(nome.end() - 1);
        } else {
            nome = nome+c;
        }
        cout<<nome<<endl;

        if (nome.size() > 0) {
            cout<<"Sugestoes:"<<endl;
            vector<string> sugestoes = iProdutos->completaPalavra(nome);
            for(unsigned int i = 0; i < sugestoes.size(); i++)
                cout << i+1 << "] " << sugestoes[i] << endl;
        }
        c = getch();
        clear();
    }
    if (nome.size() > 0) {
        if(iProdutos->buscarPalavra(nome)){
            vector<int> indices = iProdutos->recuperaIndices(nome);
            ofstream auxArquivo;
            auxArquivo.open("resultadoBusca.txt");
            auxArquivo<<indices.size()<<endl;
            for(int i = 0;i<indices.size();i++){
                auxArquivo<<getProduto(caminho,indices[i])<<endl;
            }
            auxArquivo.close();
            if(indices.size()>1){
                cout<< "Varios produtos encontrados! Voce deseja ordena-los por preco? [1 para sim e 0 para nao]"<<endl;
                bool ordenar;
                cin>>ordenar;
                if(ordenar){
                    cout<<"Deseja ordenar crescente[1] ou decrescentemente[2]?"<<endl;
                    int ordem;
                    cin>>ordem;
                    imprimeBuscaOrdenada("resultadoBusca.txt",ordem);
                }else{
                    imprimeBuscaNaoOrdenada("resultadoBusca.txt");
                }

            }else{
                cout << "Produtos encontrados! Informacoes: " << endl;
                imprimeBuscaNaoOrdenada("resultadoBusca.txt");
            }
        }else{
            cout << "Produto nao encontrado. Que tal: " << endl;
            vector<string> retorno = iProdutos->completaPalavra(nome);
            for(unsigned int i = 0; i < retorno.size(); i++)
                cout << i+1 << "] " << retorno[i] <<endl;
        }
    }
}



/*
*Funcao para buscar produto na base pela categoria
*@param  string caminho                 caminho do arquivo da base
         Trie *iCategorias:             TRIE para indexacao das categorias
*@return -
*********************************************************/
void buscarPorCategoria(string caminho, Trie *iCategorias){
    cout << "BUSCANDO POR CATEGORIA" << endl;
    string categoria;
    cout << "Digite o nome do produto que deseja procurar (digite '.' para finalizar o nome): "<<endl;
    char c = getch();
    clear();
    while(c!='\r'){
        cout << "Digite o nome do produto que deseja procurar: "<<endl;
        if (c == '\b') {
            // Remove o ultimo caracter da string se a tecla pressionada for backspace
            if (categoria.size() > 0)
                categoria.erase(categoria.end() - 1);
        } else {
            categoria = categoria + c;
        }
        cout << categoria <<endl;

        if (categoria.size() > 0) {
            cout << "Sugestoes:" << endl;
            vector<string> sugestoes = iCategorias->completaPalavra(categoria);
            for(unsigned int i = 0; i < sugestoes.size(); i++)
                cout << i+1 << "] " << sugestoes[i] << endl;
        }
        c = getch();
        clear();
    }

    if (categoria.size() > 0) {
        if(iCategorias->buscarPalavra(categoria)){
            vector<int> indices = iCategorias->recuperaIndices(categoria);
            ofstream auxArquivo;
            auxArquivo.open("resultadoBusca.txt");
            auxArquivo<<indices.size()<<endl;
            for(int i = 0;i<indices.size();i++){
                auxArquivo<<getProduto(caminho,indices[i])<<endl;
            }
            auxArquivo.close();
            if(indices.size()>1){
                cout<< "Varios produtos encontrados! Voce deseja ordena-los por preco? [1 para sim e 0 para nao]"<<endl;
                bool ordenar;
                cin>>ordenar;
                if(ordenar){
                    cout<<"Deseja ordenar crescente[1] ou decrescentemente[2]?"<<endl;
                    int ordem;
                    cin>>ordem;
                    imprimeBuscaOrdenada("resultadoBusca.txt",ordem);
                }else{
                    imprimeBuscaNaoOrdenada("resultadoBusca.txt");
                }
            }
        }else{
            cout << "Produto nao encontrado. Que tal: " << endl;
            vector<string> retorno = iCategorias->completaPalavra(categoria);
            for(unsigned int i = 0; i < retorno.size(); i++)
                cout << i+1 << "] " << retorno[i] << endl;
        }
    }
}

/*
*Funcao para imprimir o relatorio de todos os produtos na base de dados de forma ordenada
*@param  string caminho:        caminho do arquivo da base
         int ordem     :        ordem de ordenação (1: crescente |2: decrescente)
         int opcao     :        parametro a ser ordenado (1:nome |2:categoria |3:preco)
*@return -
*********************************************************/
void imprimeRelatorio(string caminho, int ordem, int opcao){ // 1-crescente -  2-decrescente
    vector<produto*> aux;

    sortExterno(caminho, "baseOrdenada.txt", 500000, opcao, ordem);
    ifstream base;
    base.open("baseOrdenada.txt");
    string nome, categoria, descricao, preco, tamanho, enter;
    char quebra = (char) '\n';

    while(getline(base, nome, ';')){
        getline(base, categoria, ';');
        getline(base, descricao, ';');
        getline(base, preco, ';');
        getline(base, enter, quebra);
        cout << " > Nome: " << nome << endl;
        cout << " | Categoria: " << categoria << endl;
        cout << " | Descricao: " << descricao.substr(0,25) << endl;
        cout << " | Preco: " << strtof(preco.c_str(),0) << endl << endl;

    }

}

/*
*Funcao para gerar relatorio de todos os produtos na base
*@param  string caminho:    caminho do arquivo da base
*@return -
*********************************************************/
void relatorio(string caminho){
    cout << "RELATORIO..." << endl;

    int opcao = 0;
    cout << "Escolha uma opcao abaixo: " << endl;
    cout << "1) ordenado por nome" << endl;
    cout << "2) ordenado por categoria" << endl;
    cout << "3) ordenado por preco" << endl;
    cout << "4) salvar e sair" << endl;
    cout << "Digite sua escolha: ";
    cin >> opcao;
    cout << "***************************************************" << endl << endl;

    if( opcao == 4 ) return;


    int ordem = 0;
    cout << "Escolha uma opcao abaixo: " << endl;
    cout << "1) ordenado crescente" << endl;
    cout << "2) ordenado decrescente" << endl;
    cin >> ordem;

    imprimeRelatorio(caminho, ordem, opcao);
}

int main(){
    std::cout << std::setprecision(2) << std::fixed;
    int opcao = 0;
    string caminho;
    cout << "***************************************************" << endl;
    cout << "Informe o arquivo de origem dos registros: "; cin >> caminho;
    vector<produto*> produtos;
    Trie iCategorias, iProdutos;
    int numeroRegistros = lerBase(&iCategorias, &iProdutos, caminho);
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
            case 1: cadastrarProduto(&iCategorias, &iProdutos, caminho, &numeroRegistros); break;
            case 2: buscarPorNome(caminho, &iProdutos); break;
            case 3: buscarPorCategoria(caminho, &iCategorias); break;
            case 4: relatorio(caminho); break;
            case 5: return 0;
            default: cout << "Opcao invalida, tente novamente" << endl;
        }
        cout << "***************************************************" << endl;
        cout << "Deseja selecionar outra opcao?" << endl;
        cout << "1) sim\t\t2) nao" << endl;
        cout << "Digite sua escolha: ";
        cin >> opcao;
        cout << "***************************************************" << endl;
        switch(opcao){
            case 2: return 0;
            default: break;
        }
        clear();
    }
    return 0;
}
