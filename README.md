  # 🛠️ Ferramenta de Reconhecimento de Palavras por Autômatos

  Este projeto é uma ferramenta desenvolvida em C++ que permite ler a descrição de um autômato finito (AFD, AFND ou AFND com ε-transições), identificar seu tipo automaticamente, e testar uma lista de palavras, verificando se são aceitas pelo autômato. A ferramenta também mede o tempo de execução para cada palavra testada.

  ---

  ## ⚙️ Como funciona

  ### Entrada
  A ferramenta recebe três arquivos pela linha de comando:

  ```bash
  ./ferramenta <automato.aut> <teste.in> <saida.out>
  ```

  - `automato.aut`: Arquivo JSON que descreve o autômato (estados, transições, inicial e finais).
  - `teste.in`: Arquivo texto contendo uma lista de palavras no formato `palavra;resultado_esperado`, uma por linha.
  - `saida.out`: Arquivo onde será salvo o resultado dos testes, com o tempo de execução.

  ### Saída
  O arquivo de saída gerado tem o seguinte formato:

  ```
  palavra;resultado_esperado;resultado_obtido;tempo_execucao_ns
  ```

  Exemplo:

  ```
  aab;1;1;1320
  ab;0;1;980
  ba;1;0;1421
  ```

  ---

  ## 🧠 Tipos de autômatos suportados

  - **AFD**: Autômato Finito Determinístico.
  - **AFND**: Autômato Finito Não Determinístico.
  - **AFND_E**: Autômato Finito Não Determinístico com Transições-ε (epsilon).

  A ferramenta detecta automaticamente o tipo de autômato com base nas transições.

  ---

  ## 📋 Exemplo de uso

  1. Compile o programa:

  ```bash
  g++ -o ferramenta main.cpp
  ```

  2. Execute com os arquivos:

  ```bash
  ./ferramenta afnd_e.aut testes.in resultado.out
  ```

  3. Verifique o conteúdo do `resultado.out`.

  ---

  ## 📁 Exemplo de arquivos

  ### `afnd_e.aut`

  ```json
  {
    "initial": 0,
    "final": [2],
    "transitions": [
      { "from": "0", "read": "a", "to": "1" },
      { "from": "1", "read": null, "to": "2" },
      { "from": "2", "read": "b", "to": "2" }
    ]
  }
  ```

  ### `testes.in`

  ```
  ab;1
  a;0
  abb;1
  ```

  ---

  ## 🧪 Sobre o projeto

  Esta ferramenta foi desenvolvida como parte da disciplina **Teoria da Computação** (2025) com o objetivo de automatizar testes de palavras em diferentes tipos de autômatos.

  ---

  ## 👨‍💻 Autor

  Trabalho acadêmico – Universidade / Curso de Computação – 2025.
