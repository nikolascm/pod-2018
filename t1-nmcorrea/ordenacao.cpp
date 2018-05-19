#include "ordenacao.hpp"

void Arquivo::abreArquivo (char * nome) {
    meuArquivo.open(nome,ios::in);
    if(!meuArquivo.is_open()) {
        cout << "Arquivo nao pode ser aberto" << endl;
        exit(0);
    }
}
void Arquivo::ordenacaoExterna() {
    int contador = 1;
    int numArqs = criaArqsOrdenados();
    float pontoMedio = numArqs*2;
    while(pontoMedio > 1) { 
        for(int i=1; i<=pontoMedio; i+=2) {
            abreParticoes(i);
            escreveNaParticao(contador);
        }
        contador = 1;
        pontoMedio = ceil(pontoMedio/2.0);
    }
    char oldName[] = "text_1.txt"; 
    char newName[] = "txt_ordenado.txt"; 
    rename(oldName,newName);
    removeArquivosTmps();
}
int Arquivo::criaArqsOrdenados() {
    while(!meuArquivo.eof()) {
        extraiPalavras();
        numeroPalavras++;
        if((calculaTamanhoEmBytes(palavras)+ buffer.length()) < N) {
            palavras.push_back(buffer);
            buffer.clear();
        } else if((calculaTamanhoEmBytes(palavras)+ buffer.length()) > N) {
            ordenacaoInterna(palavras);
            palavras.push_back(buffer);
            buffer.clear();
            numeroArquivos++;
        } else if((calculaTamanhoEmBytes(palavras)+ buffer.length()) == N) {
            palavras.push_back(buffer);
            buffer.clear();
            ordenacaoInterna(palavras);
            numeroArquivos++;
        }
    }
    if(!palavras.empty()) {
        ordenacaoInterna(palavras);
        palavras.clear();
        numeroArquivos++;
    }
    meuArquivo.close();
    cout << "Número de caracteres: " << numeroCaracteres << endl;
    cout << "Número de palavras: " << numeroPalavras << endl;
    cout << "Número de arquivos: " << numeroArquivos << endl;
    return numeroArquivos;
}
void Arquivo::ordenacaoInterna(vector <string> v) {
    static int index = 1;
    sort(v.begin(), v.end());
    ofstream arquivoSaida("text_"+to_string(index)+".txt");  
    for(string &str : v)
        arquivoSaida << str << endl;
    index++;
    palavras.clear();
    arquivoSaida.close();
}
void Arquivo::extraiPalavras() {
    locale loc;
    char caractere;
    meuArquivo.get(caractere);
    while(!isspace(caractere,loc) && isalpha(caractere,loc)) {
        caractere = tolower(caractere,loc);
        buffer.push_back(caractere);
        meuArquivo.get(caractere);
        numeroCaracteres++;
    } 
}
void Arquivo::abreParticoes(int index) {
    arquivoTemporario.open("temp.txt");
    primeiraEntrada.open("text_"+to_string(index)+".txt");
    segundaEntrada.open("text_"+to_string(index+1)+".txt");
    comparaPalavras();
    primeiraEntrada.close();
    segundaEntrada.close();
    arquivoTemporario.close();
}
void Arquivo::comparaPalavras() {
    string palavra_1, palavra_2;
    if(primeiraEntrada.is_open()) primeiraEntrada >> palavra_1;
    if(segundaEntrada.is_open()) segundaEntrada >> palavra_2;

    while(verificaEstadoArquivo(primeiraEntrada) &&
          verificaEstadoArquivo(segundaEntrada)) {         
        if(palavra_1.compare(palavra_2)<0) {
            arquivoTemporario << palavra_1;
            primeiraEntrada >> palavra_1;
        } else {
            arquivoTemporario << palavra_2;
            segundaEntrada >> palavra_2;                        
        }
        arquivoTemporario << ' ';
    }    
    while(verificaEstadoArquivo(primeiraEntrada)) {
        arquivoTemporario << palavra_1 << ' ';
        primeiraEntrada >> palavra_1;
    }
    while(verificaEstadoArquivo(segundaEntrada)) {
        arquivoTemporario << palavra_2 << ' ';
        segundaEntrada >> palavra_2;
    }
}
bool Arquivo::verificaEstadoArquivo(ifstream &f) {
    if(f.is_open() && !f.eof()) return true;
    else return false; 
}
void Arquivo::escreveNaParticao(int &index) {
    string nome = "text_"+to_string(index)+".txt";
    ifstream arquivoTemporario("temp.txt");
    ofstream arquivoDeEscrita(nome);
    while(!arquivoTemporario.eof()) {   
        char caractere;
        arquivoTemporario.get(caractere);
        arquivoDeEscrita << caractere;
    }
    index++;
    remove("temp.txt");
    arquivoDeEscrita.close();
    arquivoTemporario.close(); 
}
void Arquivo::removeArquivosTmps() {
    for(int i=1; i<=numeroArquivos; i++)
    remove(("text_"+to_string(i)+".txt").c_str());
}
int Arquivo::calculaTamanhoEmBytes(vector <string> vetor) {
    int total = 0;
    for(string str : vetor)
        total += str.size();
    return total;
}
int main(int argc, char **nome) {
    Arquivo principal;
    principal.abreArquivo(nome[1]);
    principal.ordenacaoExterna();
    return 0;
}