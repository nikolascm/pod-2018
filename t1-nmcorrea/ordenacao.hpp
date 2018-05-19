// Linha para compilação: g++ -std=c++11 ordenacao.cpp -o ordenacao
// Execução: ./ordenacao "exemplo_entrada.txt"

#include <vector>
#include <locale> 
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

struct Arquivo {
	string buffer;
	const int N = 200;
	ifstream meuArquivo;
	vector <string> palavras;

	int numeroArquivos = 0;
	int numeroPalavras = 0;
	int numeroCaracteres = 0;

	ofstream arquivoTemporario;
	ifstream primeiraEntrada;
	ifstream segundaEntrada;

	void extraiPalavras();
	void comparaPalavras();
	int criaArqsOrdenados();
	void ordenacaoExterna();
	void removeArquivosTmps();
	void abreArquivo(char*nome);
	void ordenacaoInterna(vector<string>v);
	void abreParticoes(int index);
	void escreveNaParticao(int &index);
	bool verificaEstadoArquivo(ifstream &f);
	int calculaTamanhoEmBytes(vector <string> vetor);
};