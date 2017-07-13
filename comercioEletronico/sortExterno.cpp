#include "sortExterno.hpp"

/*
*Funcao para criar min-heap de um vetor de produtos
*@param  vector<produto*> *v:      vetor de produtos
*        int ind            :      indice a ser comparado
*        int tam            :      tamanho do vetor
*        int opcao          :      parametro a ser comparado
*@return -
*********************************************************/
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

/*
*Funcao para ordenar um vetor de produtos por HeapSort (crescente)
*@param  vector<produto*> *v:      vetor de produtos
*        int tam            :      tamanho do vetor
*        int opcao          :      parametro a ser comparado
*@return -
*********************************************************/
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

/*
*Funcao para criar max-heap de um vetor de produotos
*@param  vector<produto*> *v:      vetor de produtos
*        int ind            :      indice a ser comparado
*        int tam            :      tamanho do vetor
*        int opcao          :      parametro a ser comparado
*@return -
*********************************************************/
void heapfyD(vector<produto*> *v, int ind, int tam, int opcao){
    int menor = ind;
    int esq = 2 * ind + 1;
    int dir = 2 * ind + 2;
    string s1;
    string s2;

    if(opcao == 1){ ///nome
        if(esq < tam && (*v)[ind]->nome > (*v)[esq]->nome)
            menor = esq;
        if(dir < tam && (*v)[menor]->nome > (*v)[dir]->nome)
            menor = dir;
    } else if (opcao == 2){ ///categoria
        if(esq < tam && (*v)[ind]->categoria > (*v)[esq]->categoria)
            menor = esq;
        if(dir < tam && (*v)[menor]->categoria > (*v)[dir]->categoria)
            menor = dir;
    } else { ///preco
        if(esq < tam && (*v)[ind]->preco > (*v)[esq]->preco)
            menor = esq;
        if(dir < tam && (*v)[menor]->preco > (*v)[dir]->preco)
            menor = dir;
    }


    if(menor != ind){
        produto* aux = (*v)[ind];
        (*v)[ind] = (*v)[menor];
        (*v)[menor] = aux;
        heapfyD(v, menor, tam, opcao);
    }
}

/*
*Funcao para ordenar um vetor de produtos por HeapSort (decrescente)
*@param  vector<produto*> *v:      vetor de produtos
*        int tam            :      tamanho do vetor
*        int opcao          :      parametro a ser comparado
*@return -
*********************************************************/
void heapSortD(vector<produto*> *v, int tam, int opcao){
    for(int i = tam/2; i >=0; i--){
        heapfyD(v, i, tam, opcao);
    }

    while(tam > 0){
        produto* aux = (*v)[0];
        (*v)[0] = (*v)[tam-1];
        (*v)[tam-1] = aux;
        heapfyD(v, 0, tam-1, opcao);
        tam--;
    }
}

/*
*Funcao para separar a base de dados em arquivos e ordená-los internamente
*@param  string inputFile   :      caminho para a base de dados
*        int numDados       :      numero maximo de dados para entrar em cada arquivo
*        int opcao          :      parametro a ser comparado
*        int ordem          :      ordem de ordenacao (crescente ou decrescente)
*@return int - numero de arquivos criados
*********************************************************/
int ordenaArquivosTemporarios(string inputFile, int numDados, int opcao, int ordem){
    //abre arquivo de entrada
    ifstream inFile;
    inFile.open(inputFile.c_str());

    ///array de structs
    vector<produto*> prods;
    int arquivoAtual = 0;

    string nome, categoria, descricao, preco, tamanho, enter;
    int numeroRegistros = 0;
    char quebra = (char) '\n'; // quebra de linha
    getline(inFile, tamanho);
    numeroRegistros = atoi(tamanho.c_str());

    //calcula o numero de particoes a serem feitas
    float div = (float)numeroRegistros/(float)numDados;
    int numParticoes = ceil(div);

    ofstream* outFiles = new ofstream[numParticoes];

    //cria o numero de arquivos estipulado com
    for(int i = 0; i < numParticoes; i++) {
        ostringstream convert;
        convert << i;
        outFiles[i].open(convert.str().c_str());
    }

    int i;
    while(!inFile.eof() && arquivoAtual < numParticoes){
        //cout << arquivoAtual << endl;
        i = 0;
        while(i < numDados && i < numeroRegistros){//getline(inFile, nome, ';')){
            if(inFile.eof()){
                break;
            }

            getline(inFile, nome, ';');
            getline(inFile, categoria, ';');
            getline(inFile, descricao, ';');
            getline(inFile, preco, ';');
            getline(inFile, enter, quebra); // a ideia é ler o enter no final da linha para que não seja transportado para a string

            produto* leitura = new produto;
            leitura->categoria = categoria;
            leitura->nome = nome;
            leitura->preco = strtof(preco.c_str(), 0);
            leitura->descricao = descricao;
            prods.push_back(leitura);
            i++;
        }

        if(ordem == 1){
            heapSort(&prods, prods.size(), opcao);
        } else {
            heapSortD(&prods, prods.size(), opcao);
        }


        for(unsigned int i = 0; i < prods.size(); i++){
            outFiles[arquivoAtual] << prods[i]->nome << ";" << prods[i]->categoria << ";" << prods[i]->descricao << ";" << prods[i]->preco << ";";
            delete prods[i];
            prods[i] = NULL;
            if(i+1 != prods.size()) outFiles[arquivoAtual] << endl;
        }
        prods.clear();
        arquivoAtual++;
    }

    for (int i = 0; i < numParticoes; i++){
        outFiles[i].close();
    }
    inFile.close();
    return numParticoes;
}

/*
*Funcao auxiliar para separar uma strings em um separador
*@param  const string& s    :      string a ser separada
*        const char& c      :      separador usado para quebrar as strings
*@return vector<string> - vector de strings obtidas apos a separacao
*********************************************************/
vector<string> splitString(const string& s, const char& c) {
	istringstream ss(s);
	string auxStr;

	vector<string> v;
	while(getline(ss, auxStr, c)) {
		v.push_back(auxStr);
	}
	return v;
}

/*
*Funcao para criar min-heap de nós de uma heap lidos dos arquivos
*@param  Heap** v           :      ponteiro para o vetor de nós da heap
*        int ind            :      indice a ser comparado
*        int tam            :      tamanho do vetor
*        int opcao          :      parametro a ser comparado
*@return -
*********************************************************/
void heapfyArquivo(Heap** v, int ind, int tam, int opcao){
    int maior = ind;
    int esq = 2 * ind + 1;
    int dir = 2 * ind + 2;

    vector<string> auxVInd, auxVEsq, auxVDir;
    auxVInd = splitString((*v)[ind].produto, ';');

    if(esq < tam){
        auxVEsq = splitString((*v)[esq].produto, ';');
    }

    if(dir < tam){
        auxVDir = splitString((*v)[dir].produto, ';');
    }

    bool maiorEsq = false;

    if(opcao == 1){ ///nome
        if(esq < tam && auxVInd[0] > auxVEsq[0]){
            maior = esq;
            maiorEsq = true;
        }
        if(dir < tam){
            if(maiorEsq){
                if(auxVEsq[0] > auxVDir[0]){
                    maior = dir;
                }
            } else {
                if(auxVInd[0] > auxVDir[0]){
                    maior = dir;
                }
            }
        }
    } else if (opcao == 2){ ///categoria
        if(esq < tam && auxVInd[1] > auxVEsq[1]){
            maior = esq;
            maiorEsq = true;
        }
        if(dir < tam){
            if(maiorEsq){
                if(auxVEsq[1] > auxVDir[1]){
                    maior = dir;
                }
            } else {
                if(auxVInd[1] > auxVDir[1]){
                    maior = dir;
                }
            }
        }
    } else { ///preco

        if(esq < tam && strtof(auxVInd[3].c_str(),0) > strtof(auxVEsq[3].c_str(),0)){
            maior = esq;
            maiorEsq = true;
        }
        if(dir < tam){
            if(maiorEsq){
                if(strtof(auxVEsq[3].c_str(),0) > strtof(auxVDir[3].c_str(),0)){
                    maior = dir;
                }
            } else {
                if(strtof(auxVInd[3].c_str(),0) > strtof(auxVDir[3].c_str(),0)){
                    maior = dir;
                }
            }
        }
    }


    if(maior != ind){
        Heap aux = (*v)[ind];
        (*v)[ind] = (*v)[maior];
        (*v)[maior] = aux;
        heapfyArquivo(v, maior, tam, opcao);
    }
}

/*
*Funcao para criar max-heap de nós de uma heap lidos dos arquivos
*@param  Heap** v           :      ponteiro para o vetor de nós da heap
*        int ind            :      indice a ser comparado
*        int tam            :      tamanho do vetor
*        int opcao          :      parametro a ser comparado
*@return -
*********************************************************/
void heapfyArquivoD(Heap** v, int ind, int tam, int opcao){
    int maior = ind;
    int esq = 2 * ind + 1;
    int dir = 2 * ind + 2;

    vector<string> auxVInd, auxVEsq, auxVDir;
    auxVInd = splitString((*v)[ind].produto, ';');

    if(esq < tam){
        auxVEsq = splitString((*v)[esq].produto, ';');
    }

    if(dir < tam){
        auxVDir = splitString((*v)[dir].produto, ';');
    }

    bool maiorEsq = false;

    if(opcao == 1){ ///nome
        if(esq < tam && auxVInd[0] < auxVEsq[0]){
            maior = esq;
            maiorEsq = true;
        }
        if(dir < tam){
            if(maiorEsq){
                if(auxVEsq[0] < auxVDir[0]){
                    maior = dir;
                }
            } else {
                if(auxVInd[0] < auxVDir[0]){
                    maior = dir;
                }
            }
        }
    } else if (opcao == 2){ ///categoria
        if(esq < tam && auxVInd[1] < auxVEsq[1]){
            maior = esq;
            maiorEsq = true;
        }
        if(dir < tam){
            if(maiorEsq){
                if(auxVEsq[1] < auxVDir[1]){
                    maior = dir;
                }
            } else {
                if(auxVInd[1] < auxVDir[1]){
                    maior = dir;
                }
            }
        }
    } else { ///preco

        if(esq < tam && strtof(auxVInd[3].c_str(),0) < strtof(auxVEsq[3].c_str(),0)){
            maior = esq;
            maiorEsq = true;
        }
        if(dir < tam){
            if(maiorEsq){
                if(strtof(auxVEsq[3].c_str(),0) < strtof(auxVDir[3].c_str(),0)){
                    maior = dir;
                }
            } else {
                if(strtof(auxVInd[3].c_str(),0) < strtof(auxVDir[3].c_str(),0)){
                    maior = dir;
                }
            }
        }
    }


    if(maior != ind){
        Heap aux = (*v)[ind];
        (*v)[ind] = (*v)[maior];
        (*v)[maior] = aux;
        heapfyArquivoD(v, maior, tam, opcao);
    }
}

/*
*Funcao para unir os arquivos criados pela funcao de ordenar a base de forma ordenada
*@param  string outputFile  :      caminho para o arquivo de saida ordenado
*        int numDados       :      numero maximo de dados em cada arquivo separado
*        int numParticoes   :      numero de arquivos a serem lidos
*        int opcao          :      parametro a ser comparado
*        int ordem          :      ordem de ordenacao (crescente ou decrescente)
*@return -
*********************************************************/
void mergeArquivos(string outputFile, int numDados, int numParticoes, int opcao, int ordem) {
    ifstream* in = new ifstream[numParticoes];
    for (int i = 0; i < numParticoes; i++){
        ostringstream convert;
        convert << i;
        in[i].open(convert.str().c_str());
    }

    ofstream out;
    out.open(outputFile.c_str());

    Heap* auxHeap = new Heap[numParticoes];
    string strProd, enter;
    char quebra = (char) '\n';
    int i;

    for (i = 0; i < numParticoes; i++){
        getline(in[i], strProd, '\n');
        auxHeap[i].produto = strProd;
        auxHeap[i].indice = i;
    }

    if(ordem == 1){
        for(int i = numParticoes/2; i >=0; i--){
            heapfyArquivo(&auxHeap, i, numParticoes, opcao);
        }
    } else {
        for(int i = numParticoes/2; i >=0; i--){
            heapfyArquivoD(&auxHeap, i, numParticoes, opcao);
        }
    }


    int count = 0;
    int tam = numParticoes;

    while (count < tam){

        Heap aux = auxHeap[0];
        out << aux.produto;
        out << '\n';

        int ind = aux.indice;

        if(!getline(in[ind], strProd, '\n')){
            auxHeap[0] = auxHeap[tam-1];
            auxHeap[tam-1] = aux;
            tam--;
            count++;
        } else {
            auxHeap[0].produto = strProd;
        }
        if(ordem == 1)
            heapfyArquivo(&auxHeap, 0, tam, opcao);
        else
            heapfyArquivoD(&auxHeap, 0, tam, opcao);
    }

    for (int i = 0; i < numParticoes; i++){
        in[i].close();
    }
    out.close();
}

/*
*Funcao para unir os arquivos criados pela funcao de ordenar a base de forma ordenada
*@param  string input       :      caminho para o arquivo de entrada (base)
*        string output      :      caminho para o arquivo de saida ordenado
*        int numDados       :      numero maximo de dados por arquivo a ser separado
*        int opcao          :      parametro a ser comparado
*        int ordem          :      ordem de ordenacao (crescente ou decrescente)
*@return -
*********************************************************/
void sortExterno(string input, string output, int numDados, int opcao, int ordem){
    int numPart = ordenaArquivosTemporarios(input, numDados, opcao, ordem);
    //cout << "Sorting..." << endl;
    mergeArquivos(output, numDados, numPart, opcao, ordem);
}
