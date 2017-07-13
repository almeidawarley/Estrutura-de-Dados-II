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
#include "Trie.h"

using namespace std;

/*
*Struct para armazenamento computacional dos produtos
*********************************************************/
typedef struct sProduto{
    string nome;
    string categoria;
    string descricao;
    float preco;
}produto;

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
*Funcao para ler a base de um caminho informado por par�metro
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         Trie *iCategorias:             TRIE para indexa��o das categorias
         Trie *iProdutos:               TRIE para indexa��o dos produtos
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
        iCategorias->inserirPalavra(categoria, indice);
        iProdutos->inserirPalavra(nome, indice);
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
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         Trie *iCategorias:             TRIE para indexa��o das categorias
         Trie *iProdutos:               TRIE para indexa��o dos produtos
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
    ofstream base(caminho.c_str(), ios::app);
    base << padString(aCadastrar->nome, 50) << ";" << padString(aCadastrar->categoria, 20) << ";" << padString(aCadastrar->descricao, 500) << ";";
    base << setw(7) << setfill('0') << setprecision(3) << aCadastrar->preco << ";" << endl;
}

/*
*Funcao para recuperar as informações de um produto no arquivo a partir do índice (linha)
*@param  string caminho:  caminho para o arquivo que contém a base
         int linha:       linha do arquivo onde estão as informações
*@return produto: retorna uma struct com os dados obtidos da linha fornecida
*********************************************************/
produto getProduto(string caminho, int linha){
    ifstream base(caminho.c_str());
    string linhaDescartavel;
    string linhaProduto;
    base>>linhaDescartavel;
    int pular = linhaDescartavel.size()+2; //descartar também \n e \r
    getline(base.seekg(pular+583*(linha-1)), linhaProduto, '\n'); //onde 583 é a quantidade de caracteres em cada linha de produto
    produto resultado;
    replace(linhaProduto.begin(), linhaProduto.end(), ';', ' ');
    stringstream leitura(linhaProduto);
    leitura>>resultado.nome>>resultado.categoria>>resultado.descricao>>resultado.preco;
    return resultado;
}

/*
*Funcao para buscar produto na base pelo nome
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         Trie *iProdutos:               TRIE para indexa��o dos produtos
*@return -
*********************************************************/
void buscarPorNome(string caminho, Trie *iProdutos){
    cout << "BUSCANDO POR NOME..." << endl;
    string nome;
    cout << "Digite o nome do produto que deseja procurar (digite '.' para finalizar o nome): "<<endl;
    //cin >> nome;
    //!Ideia: Mas s� funciona no windows
    char c = getch(); //Algum equivalente no linux? getchar() espera o enter para ler as letras :/
    clear(); //Funciona no windows e no linux (mas n�o parece uma boa ideia)
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
    //! Fim da ideia. Se n�o der substituir isso por cin>>nome (que est� comentado ali em cima)

    if (nome.size() > 0) {
        if(iProdutos->buscarPalavra(nome)){
            vector<int> indices = iProdutos->recuperaIndices(nome);
            cout << "Produtos encontrados! Informacoes: " << endl;
            for(int k = 0; k < indices.size(); k++){
                produto resultado = getProduto(caminho,indices[k]);
                cout << " > Nome: " << resultado.nome << endl;
                cout << " | Categoria: " << resultado.categoria << endl;
                cout << " | Descricao: " << resultado.descricao << endl;
                cout << " | Preco: " << resultado.preco << endl;
            }
        }else{
            cout << "Produto nao encontrado. Que tal: " << endl;
            vector<string> retorno = iProdutos->completaPalavra(nome);
            for(unsigned int i = 0; i < retorno.size(); i++)
                cout << i+1 << "] " << retorno[i] << endl;
        }
    }
}

/*
*Funcao para buscar produto na base pela categoria
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         Trie *iCategorias:             TRIE para indexa��o das categorias
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
            cout << "Produtos encontrados! Informacoes: " << endl;
            for(int k = 0; k < indices.size(); k++){
                produto resultado = getProduto(caminho,indices[k]);
                cout << " > Nome: " << resultado.nome << endl;
                cout << " | Categoria: " << resultado.categoria << endl;
                cout << " | Descricao: " << resultado.descricao << endl;
                cout << " | Preco: " << resultado.preco << endl;
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
*Funcao para desalocar os produtos e atualizar a base conforme caminho informado por par�metro
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
         string caminho:                caminho do arquivo da base
*@return -
*********************************************************/
void finaliza(string caminho, int numeroRegistros){
    fstream base(caminho.c_str());
    base << numeroRegistros << endl;

}

void heapfy(vector<produto*> *v, int ind, int tam, int opcao){
    int maior = ind;
    int esq = 2 * ind + 1;
    int dir = 2 * ind + 2;
    string s1;
    string s2;

    if(opcao == 1){ ///nome
        if(esq < tam && (*v)[ind]->nome < (*v)[esq]->nome)
            maior = esq;
        if(dir < tam && (*v)[maior]->nome < (*v)[dir]->nome)
            maior = dir;
    } else if (opcao == 2){ ///categoria
        if(esq < tam && (*v)[ind]->categoria < (*v)[esq]->categoria)
            maior = esq;
        if(dir < tam && (*v)[maior]->categoria < (*v)[dir]->categoria)
            maior = dir;
    } else { ///CONFERIR COMPARAC�ES ENTRE FLOATS
        if(esq < tam && (*v)[ind]->preco < (*v)[esq]->preco)
            maior = esq;
        if(dir < tam && (*v)[maior]->preco < (*v)[dir]->preco)
            maior = dir;
    }


    if(maior != ind){
        produto* aux = (*v)[ind];
        (*v)[ind] = (*v)[maior];
        (*v)[maior] = aux;
        heapfy(v, maior, tam, opcao);
    }
}


void heapSort(vector<produto*> *v, int tam, int opcao){
    for(int i = tam/2; i >=0; i--){
        heapfy(v, i, tam, opcao);
    }
    while(tam > 0){
        produto* aux = (*v)[0];
        (*v)[0] = (*v)[tam-1];
        (*v)[tam-1] = aux;
        heapfy(v, 0, tam-1, opcao);
        tam--;
    }
}


void imprimeRelatorio(vector<produto*> *produtos, int ordem, int opcao){ // 1-crescente -  2-decrescente
    vector<produto*> aux;

    for(int i = 0; i < produtos->size(); i++){
        aux.push_back((*produtos)[i]);
    }

    heapSort(&aux, aux.size(), opcao);

    if(ordem == 1){
        for(int i = 0; i < aux.size(); i++){
            ///imprime as informa��es
            cout << aux[i]->nome << " " << aux[i]->categoria << " " << aux[i]->preco << endl;
        }
    } else {
        for(int i = aux.size()-1; i>=0; i--){
            ///imprime as informa��es
            cout << aux[i]->nome << " " << aux[i]->categoria << " " << aux[i]->preco << endl;
        }
    }

}

/*
*Funcao para gerar relat�rio de todos os produtos na base
*@param  vector<produto*> *produtos:    ponteiro para o vetor de produtos
*@return -
*********************************************************/
void relatorio(vector<produto*> *produtos){
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
    //while(opcao2 != 1 && opcao2 != 2){
    cout << "Escolha uma opcao abaixo: " << endl;
    cout << "1) ordenado crescente" << endl;
    cout << "2) ordenado decrescente" << endl;
    cin >> ordem;
    //}

    imprimeRelatorio(produtos, ordem, opcao);


//    switch(opcao){
//        case 1: imprimeRelatorio(produtos, opcao2, opcao); break;
//        case 2: relatorioCategoria(produtos, opcao2, opcao); break;
//       // case 3: relatorioPreco(produtos, opcao2); break;
//        case 4: return;
//        default: cout << "Opcao invalida" << endl; break;
//    }


}

int main(){
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
            case 4: relatorio(&produtos); break;
            case 5: finaliza(caminho, numeroRegistros); return 0;
            default: cout << "Opcao invalida, tente novamente" << endl;
        }
        cout << "***************************************************" << endl;
        cout << "Deseja selecionar outra opcao?" << endl;
        cout << "1) sim\t\t2) nao" << endl;
        cout << "Digite sua escolha: ";
        cin >> opcao;
        cout << "***************************************************" << endl;
        switch(opcao){
            case 2: finaliza(caminho, numeroRegistros); return 0;
            default: break;
        }
        clear();
    }
    finaliza(caminho, numeroRegistros);
    return 0;
}
