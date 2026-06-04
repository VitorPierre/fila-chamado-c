# Sistema de Chamados de Suporte

Sistema de gerenciamento de chamados de suporte desenvolvido em **C**, utilizando **fila encadeada com prioridade** e persistencia em arquivo. O programa permite abrir, listar, atender, buscar e salvar chamados de forma pratica com uma interface textual colorida no terminal.

> **Disciplina:** DSA (Estrutura de Dados e Algoritmos)  
> **Autores:** Vitor Pierre e Kawane Santos  
> **Objetivo:** Praticar implementacao de filas encadeadas, alocacao dinamica, manipulacao de arquivos e sistemas de prioridade em C.

---

## Funcionalidades

- ✅ Abrir novos chamados com nome, setor, descricao e **prioridade**
- ✅ Listar chamados pendentes (com coluna de prioridade colorida)
- ✅ **Listar todos os chamados** (pendentes + atendidos)
- ✅ Atender chamado por **prioridade** (ALTA → MEDIA → BAIXA)
- ✅ Buscar chamado por ID
- ✅ Salvar dados em arquivo
- ✅ Carregar dados automaticamente ao iniciar
- ✅ Confirmar salvamento antes de sair
- ✅ Interface com cores ANSI e conteudo centralizado

---

## Sistema de Prioridade

O sistema prioriza chamados conforme a urgencia:

| Prioridade | Valor | Cor | Ordem de Atendimento |
|------------|-------|-----|---------------------|
| **ALTA** | 3 | 🔴 Vermelho | ⭐ Primeiro |
| **MEDIA** | 2 | 🟡 Amarelo | Meio |
| **BAIXA** | 1 | 🔵 Azul | Por ultimo |

**Exemplo:** Se entrar 1 BAIXA, 1 ALTA e 1 MEDIA, a fila fica:
```
1. ALTA (3)   ← atende primeiro
2. MEDIA (2)  ← atende segundo  
3. BAIXA (1)  ← atende por ultimo
```

---

## Estrutura de Dados

O sistema utiliza uma **fila encadeada dinamica com prioridade** onde cada chamado possui:

```c
typedef struct Chamado {
    int id;
    char nome[TAM_NOME];
    char setor[TAM_SETOR];
    char descricao[TAM_DESC];
    int prioridade;  // 1=BAIXA, 2=MEDIA, 3=ALTA
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
| `chamados.txt` | Chamados pendentes |
| `atendidos.txt` | Chamados ja atendidos |

Formato do arquivo (separado por `;`):
```
id;nome;setor;descricao;prioridade
```

---

## Menu do Sistema

```
[1] Abrir chamado     ->   Criar novo chamado de suporte
[2] Listar chamados   ->   Ver todos os pendentes
[3] Listar todos      ->   Ver pendentes + atendidos
[4] Atender proximo   ->   Atender por prioridade (ALTA primeiro)
[5] Buscar por ID     ->   Encontrar chamado pelo numero
[6] Salvar dados      ->   Salvar no arquivo
[7] Sair              ->   Encerrar o programa (com confirmacao)
```

---

## Como Compilar

### GCC (Linux/Mac/MinGW)
```bash
gcc chamados.c -o sistema_chamados
```

### Dev-C++
1. Abra o arquivo `.c` no Dev-C++
2. Clique em **Executar** → **Compilar e Executar**

---

## Como Executar

### Windows
```bash
sistema_chamados.exe
```

### Linux / Mac
```bash
./sistema_chamados
```

---

## Configuracao do Terminal (Dev-C++)

Para visualizar o conteudo centralizado corretamente:

1. Compile e execute o programa
2. Na janela do console, **clique com botao direito** na barra de titulo
3. Selecione **Propriedades** → **Layout**
4. Mude **Largura do buffer** para `150`
5. Mude **Largura da janela** para `150`
6. Clique em **OK**

---

## Exemplo de Uso

1. Execute o programa
2. Digite `1` para abrir um chamado
3. Informe nome, setor, descricao
4. Escolha prioridade: `1` (BAIXA), `2` (MEDIA) ou `3` (ALTA)
5. Digite `2` para listar pendentes ou `3` para listar todos
6. Digite `4` para atender o chamado de maior prioridade
7. Digite `5` para buscar por ID
8. Digite `6` para salvar manualmente
9. Digite `7` para sair (pergunta se deseja salvar)

---

## Logica do Sistema

### Fila com Prioridade
O atendimento segue **prioridade decrescente** (3 → 2 → 1). Chamados de ALTA prioridade sao atendidos primeiro, independentemente de quando entraram na fila.

### Persistencia
- Chamados sao salvos em `chamados.txt`
- Atendidos sao movidos para `atendidos.txt`
- Dados sao carregados automaticamente ao iniciar

### ID Automatico
O sistema calcula o proximo ID disponivel automaticamente (maior ID + 1).

### Ordenacao Automatica
Ao enfileirar, o sistema posiciona o chamado na ordem correta de prioridade automaticamente.

---

## Funcoes Principais

| Funcao | Descricao |
|--------|-----------|
| `iniciar_fila()` | Inicializa a fila vazia |
| `criar_chamado()` | Aloca memoria para novo chamado |
| `enfileirar()` | Adiciona chamado na ordem de prioridade |
| `desenfileirar()` | Remove e retorna o chamado de maior prioridade |
| `proximo_id()` | Calcula proximo ID disponivel |
| `abrir_chamado()` | Coleta dados e cadastra novo chamado com prioridade |
| `listar()` | Exibe chamados pendentes com prioridade colorida |
| `listar_todos()` | Exibe pendentes + atendidos |
| `atender_proximo()` | Atende chamado de maior prioridade |
| `buscar_por_id()` | Busca em pendentes e atendidos |
| `salvar()` | Grava chamados no arquivo |
| `carregar()` | Recupera chamados do arquivo |
| `liberar()` | Libera memoria alocada |
| `centralizar()` | Imprime espacos para centralizar texto |
| `get_prioridade_str()` | Retorna string da prioridade (BAIXA/MEDIA/ALTA) |
| `get_prioridade_cor()` | Retorna cor conforme prioridade |

---

## Tecnologias Utilizadas

- Linguagem C
- Alocacao dinamica (`malloc` / `free`)
- Manipulacao de arquivos (`fopen`, `fprintf`, `fgets`, `sscanf`)
- Estrutura de dados: fila encadeada com prioridade
- Codigos ANSI para cores no terminal

---

## Melhorias Futuras

- [ ] Editar chamados existentes
- [ ] Excluir chamados especificos por ID
- [ ] Ordenacao por setor alem da prioridade
- [ ] Validacao mais robusta de entradas
- [ ] Separacao do codigo em multiplos arquivos (`.h` e `.c`)
- [ ] Interface com busca por nome/setor
- [ ] Estatisticas de atendimentos

---

## Autor

Desenvolvido por **Vitor Pierre e Kawane Santos**

---

## Licenca

Projeto de uso academico e educacional para a disciplina de **DSA (Estrutura de Dados e Algoritmos)**.
