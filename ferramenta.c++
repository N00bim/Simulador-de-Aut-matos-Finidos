#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <set>
#include "json.hpp" 

using namespace std;
using json = nlohmann::json;

struct Transicao {
    int from;
    string read;
    int to;
};


bool lerAutomato(const string& nomeArquivo, int& estadoInicial, vector<int>& estadosFinais, vector<Transicao>& transicoes) {
    ifstream file(nomeArquivo);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return false;
    }

    json j;
    file >> j;
    estadoInicial = j["initial"];
    estadosFinais = j["final"].get<vector<int>>();

    for (const auto& tr : j["transitions"]) {
        Transicao t;
        t.from = stoi(tr["from"].get<string>());
        t.to = stoi(tr["to"].get<string>());
        t.read = tr["read"].is_null() ? "" : tr["read"].get<string>();
        transicoes.push_back(t);
    }

    return true;
}

string verificarAutomato(vector<Transicao>& transicoes){
    for(Transicao t : transicoes){
        if(t.read == ""){
            return "AFND_E";
        }
    }
    map<pair<int, string>, set<int>> transicaoMap;

    for (Transicao t : transicoes) {
        auto chave = make_pair(t.from, t.read);
        transicaoMap[chave].insert(t.to);
    }

    for (auto it = transicaoMap.begin(); it != transicaoMap.end(); ++it) {
        const set<int>& destinos = it->second;
    
        if (destinos.size() > 1) {
            return "AFND";
        }
    }
    
    return "AFD";
}

bool executaAFD(const string& palavra, int estadoInicial, const vector<int>& finais, const vector<Transicao>& transicoes) {
    int estado = estadoInicial;
    for (char c : palavra) {
        string simbolo(1, c);
        bool transicaoValida = false;
        for (const Transicao& t : transicoes) {
            if (t.from == estado && t.read == simbolo) {
                estado = t.to;
                transicaoValida = true;
                break;
            }
        }
        if (!transicaoValida) return false;
    }
    return find(finais.begin(), finais.end(), estado) != finais.end();
}

bool executaAFND(const string& palavra, int estadoInicial, const vector<int>& finais, const vector<Transicao>& transicoes) {
    set<int> estadosAtuais = {estadoInicial};
    for (char c : palavra) {
        string simbolo(1, c);
        set<int> proximos;
        for (int estado : estadosAtuais) {
            for (const Transicao& t : transicoes) {
                if (t.from == estado && t.read == simbolo) {
                    proximos.insert(t.to);
                }
            }
        }
        estadosAtuais = proximos;
        if (estadosAtuais.empty()) return false;
    }
    for (int estado : estadosAtuais) {
        if (find(finais.begin(), finais.end(), estado) != finais.end()) {
            return true;
        }
    }
    return false;
}

set<int> epsilonFecho(int estado, const vector<Transicao>& transicoes) {
    set<int> fecho = {estado};
    vector<int> pilha = {estado};
    while (!pilha.empty()) {
        int atual = pilha.back();
        pilha.pop_back();
        for (const Transicao& t : transicoes) {
            if (t.from == atual && t.read == "" && fecho.find(t.to) == fecho.end()) {
                fecho.insert(t.to);
                pilha.push_back(t.to);
            }
        }
    }
    return fecho;
}

bool executaAFND_E(const string& palavra, int estadoInicial, const vector<int>& finais, const vector<Transicao>& transicoes) {
    set<int> estadosAtuais = epsilonFecho(estadoInicial, transicoes);
    for (char c : palavra) {
        string simbolo(1, c);
        set<int> proximos;
        for (int estado : estadosAtuais) {
            for (const Transicao& t : transicoes) {
                if (t.from == estado && t.read == simbolo) {
                    set<int> fechoDestino = epsilonFecho(t.to, transicoes);
                    proximos.insert(fechoDestino.begin(), fechoDestino.end());
                }
            }
        }
        estadosAtuais = proximos;
        if (estadosAtuais.empty()) return false;
    }
    for (int estado : estadosAtuais) {
        if (find(finais.begin(), finais.end(), estado) != finais.end()) {
            return true;
        }
    }
    return false;
}

void processarCSV(const string& nomeCSV, const string& nomeSaida,
                  const string& tipo, int estadoInicial,
                  const vector<int>& estadosFinais,
                  const vector<Transicao>& transicoes) {
    ifstream entrada(nomeCSV);
    ofstream saida(nomeSaida);
    if (!entrada.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV de entrada." << endl;
        return;
    }
    if (!saida.is_open()) {
        cerr << "Erro ao criar o arquivo de saída." << endl;
        return;
    }

    string linha;
    while (getline(entrada, linha)) {
        size_t separador = linha.find(';');
        if (separador == string::npos) continue;

        string palavra = linha.substr(0, separador);
        string esperadoStr = linha.substr(separador + 1);

        auto inicio = chrono::high_resolution_clock::now();

        bool obtido = false;
        if (tipo == "AFD") obtido = executaAFD(palavra, estadoInicial, estadosFinais, transicoes);
        else if (tipo == "AFND") obtido = executaAFND(palavra, estadoInicial, estadosFinais, transicoes);
        else if (tipo == "AFND_E") obtido = executaAFND_E(palavra, estadoInicial, estadosFinais, transicoes);

        auto fim = chrono::high_resolution_clock::now();
        auto duracao = chrono::duration_cast<chrono::nanoseconds>(fim - inicio).count();

        saida << palavra << ';' << esperadoStr << ';' << (obtido ? "1" : "0") << ';' << duracao << endl;
    }

    entrada.close();
    saida.close();
}   

int main(int argc, char* argv[]) {
    if (argc != 4) {
        return 1;
    }

    string arquivoAutomato = argv[1];
    string arquivoEntrada = argv[2];
    string arquivoSaida = argv[3];

    int estadoInicial;
    vector<int> estadosFinais;
    vector<Transicao> transicoes;

    if (!lerAutomato(arquivoAutomato, estadoInicial, estadosFinais, transicoes)) {
        return 1;
    }

    string tipo = verificarAutomato(transicoes);
    processarCSV(arquivoEntrada, arquivoSaida, tipo, estadoInicial, estadosFinais, transicoes);

    return 0;
}