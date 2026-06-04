# Sistema de Chamados de Suporte

Sistema de gerenciamento de chamados de suporte desenvolvido em **C**, utilizando **fila encadeada** e persistencia em arquivo. O programa permite abrir, listar, atender, buscar e salvar chamados de forma pratica com uma interface textual colorida no terminal.

> **Disciplina:** DSA (Estrutura de Dados e Algoritimos)  
> **Objetivo:** Praticar implementacao de filas encadeadas, alocacao dinamica e manipulacao de arquivos em C.

---

## Funcionalidades

- Abrir novos chamados com nome, setor e descricao
- Listar chamados pendentes
- **Listar todos os chamados** (pendentes + atendidos)
- Atender o proximo chamado da fila (FIFO)
- Buscar chamado por ID
- Salvar dados em arquivo
- Carregar dados automaticamente ao iniciar
- Confirmar salvamento antes de sair
- Interface com cores ANSI e conteudo centralizado

---

## Estrutura de Dados

O sistema utiliza uma **fila encadeada dinamica** onde cada chamado possui:

```c
typedef struct Chamado {
    int id;
    char nome[TAM_NOME];
    char setor[TAM_SETOR];
    char descricao[TAM_DESC];
    struct Chamado *prox;
} Chamado;
```

```c
typedef struct Fila {
    Chamado *inicio;
    Chamado *fim;
    int tamanho;
} Fila;
```

---

## Arquivos Gerados

| Arquivo | Descricao |
|---------|-----------|
| chamados.txt | Chamados pendentes |
| atendidos.txt | Chamados ja atendidos |

Formato do arquivo (separado por ;):
id;nome;setor;descricao

---

## Menu do Sistema

[1] Abrir chamado     ->   Criar novo chamado de suporte
[2] Listar chamados   ->   Ver todos os pendentes
[3] Listar todos      ->   Ver pendentes + atendidos
[4] Atender proximo   ->   Atender o primeiro da fila
[5] Buscar por ID     ->   Encontrar chamado pelo numero
[6] Salvar dados      ->   Salvar no arquivo
[7] Sair              ->   Encerrar o programa (com confirmacao)

---

## Como Compilar

### GCC (Linux/Mac/MinGW)
gcc main.c -o sistema_chamados

### Dev-C++
1. Abra o arquivo .c no Dev-C++
2. Clique em Executar -> Compilar e Executar

---

## Como Executar

### Windows
sistema_chamados.exe

### Linux / Mac
./sistema_chamados

---

## Configuracao do Terminal (Dev-C++)

Para visualizar o conteudo centralizado corretamente:

1. Compile e execute o programa
2. Na janela do console, clique com botao direito na barra de titulo
3. Selecione Propriedades -> Layout
4. Mude Largura do buffer para 150
5. Mude Largura da janela para 150
6. Clique em OK

---

## Exemplo de Uso

1. Execute o programa
2. Digite 1 para abrir um chamado
3. Informe nome, setor e descricao
4. Digite 2 para listar pendentes ou 3 para listar todos
5. Digite 4 para atender o proximo chamado
6. Digite 5 para buscar por ID
7. Digite 6 para salvar manualmente
8. Digite 7 para sair (pergunta se deseja salvar)

---

## Logica do Sistema

### Fila FIFO
O atendimento segue First In, First Out - o primeiro chamado que entra e o primeiro a ser atendido.

### Persistencia
- Chamados sao salvos em chamados.txt
- Atendidos sao movidos para atendidos.txt
- Dados sao carregados automaticamente ao iniciar

### ID Automatico
O sistema calcula o proximo ID disponivel automaticamente (maior ID + 1).

---

## Funcoes Principais

- iniciar_fila() - Inicializa a fila vazia
- criar_chamado() - Aloca memoria para novo chamado
- enfileirar() - Adiciona chamado ao final da fila
- desenfileirar() - Remove e retorna o primeiro chamado
- proximo_id() - Calcula proximo ID disponivel
- abrir_chamado() - Coleta dados e cadastra novo chamado
- listar() - Exibe chamados pendentes
- listar_todos() - Exibe pendentes + atendidos
- atender_proximo() - Atende e move para atendidos.txt
- buscar_por_id() - Busca em pendentes e atendidos
- salvar() - Grava chamados no arquivo
- carregar() - Recupera chamados do arquivo
- liberar() - Libera memoria alocada
- centralizar() - Imprime espacos para centralizar texto

---

## Tecnologias Utilizadas

- Linguagem C
- Alocacao dinamica (malloc / free)
- Manipulacao de arquivos (fopen, fprintf, fgets, sscanf)
- Estrutura de dados: fila encadeada
- Codigos ANSI para cores no terminal

---

## Melhorias Futuras

- Editar chamados existentes
- Excluir chamados especificos por ID
- Ordenacao por setor ou prioridade
- Validacao mais robusta de entradas
- Separacao do codigo em multiplos arquivos (.h e .c)
- Interface com busca por nome/setor

---

## Autor

Desenvolvido por **Vitor Pierre e Kawane Santos**

---

## Licenca

Projeto de uso academico e educacional para a disciplina de **DSA (Estrutura de Dados e Algoritmos)**.
