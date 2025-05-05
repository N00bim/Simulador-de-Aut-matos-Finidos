# Objetivo da ferramenta
O projeto tem como objetivo desenvolver uma ferramenta capaz de **simular autômatos finitos**, utilizando um arquivo `.aut` que representa o **diagrama de transições do autômato**. A ferramenta testa o funcionamento do autômato com base em um arquivo de entrada no formato **CSV**, e gera um **arquivo de saída** também em formato CSV, contendo os resultados das simulações.

O programa é projetado para ser **totalmente operável via linha de comando**.

Para este projeto, a linguagem escolhida para o desenvolvimento da ferramenta foi o **C++**.

---
## Funcionamento da Ferramenta

A seguir estão descritos os principais passos para o funcionamento da ferramenta:

### Leitura do Arquivo `.aut` (JSON)

O arquivo `.aut` possui formato **JSON** e define:

- Estado inicial
- Conjunto de estados finais
- Lista de transições (estado de origem, símbolo lido, estado de destino)

**Exemplo:**
```json
{
  "initial": 0,
  "final": [2],
  "transitions": [
    {"from": "0", "read": "a", "to": "1"},
    {"from": "1", "read": "b", "to": "2"}
  ]
}
```

A leitura é feita com o auxílio da biblioteca [`nlohmann/json`](https://github.com/nlohmann/json).

**Comando para incluir a bilioteca**:
```cpp
#include "json.hpp"
using json = nlohmann::json;
```
> Necessita que o arquivo `json.hpp` esteja no **mesmo diretorio** que o `.c++` do codigo fonte.

**A estrutura interna utiliza:**
```c++
 struct Transicao {
    int from;
    string read;
    int to;
 };
```
---
### Identificação do Tipo de Autômato Finito

Inicialmente, devemos identificar o tipo de Autômato Finito:

- **AFD** (Determinístico)
- **AFND** (Não determinístico)
- **AFND_ε** (Não determinístico com transições vazias
---
**Critérios:**
1. Transição com símbolo vazio (`""`) → **AFND_ε**
2. Múltiplas transições para (estado, símbolo) → **AFND**
3. Caso contrário → **AFD**

---
1. **Verificar AFND_ε**
```c++
for (Transicao t : transicoes) {
 if (t.read == "") {
     return "AFND_E";
 }
}
```
2. **Verificar AFND**
```c++
map<pair<int, string>, set<int>> transicaoMap;

for (Transicao t : transicoes) {
    auto chave = make_pair(t.from, t.read);
    transicaoMap[chave].insert(t.to);
}

for (auto& it : transicaoMap) {
    if (it.second.size() > 1) {
        return "AFND";
    }
}
```
3. **Caso não se enquadre nas condições acima → AFD**

---
### Entrada e Saída (CSV)

A ferramenta utiliza as bibliotecas padrão `fstream` do **C++** para os arquivos de entrada e saída. O processo é realizado da seguinte forma:

- **Entrada**:  
  Um arquivo `.in` contendo as palavras a serem testadas no formato:
```
palavra;resultado_esperado
```

- **Saída**:  
  Os resultados das simulações são salvos em um arquivo `.out`, também no formato CSV, com o seguinte formato:
```
palavra;resultado_esperado;resultado_obtido;tempo_de_execucao_em_nanosegundos
```
---
### Compilação e Execução

```bash
g++ -o ferramenta.exe nome_do_codigo.c++
./ferramenta.exe automato.aut entrada.in saida.out
```
