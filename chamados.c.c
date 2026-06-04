#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQ "chamados.txt"
#define TAM_NOME 80
#define TAM_SETOR 50
#define TAM_DESC 120

#define LARGURA_CONTEUDO 50
#define LARGURA_TELA 150

// Cores ANSI para interface visual mais agradável
#define RESET "\x1b[0m"
#define BOLD "\x1b[1m"
#define DIM "\x1b[2m"
#define CYAN "\x1b[36m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define MAGENTA "\x1b[35m"
#define BLUE "\x1b[34m"
#define GRAY "\x1b[90m"
#define WHITE "\x1b[37m"

typedef struct Chamado {
    int id;
    char nome[TAM_NOME];
    char setor[TAM_SETOR];
    char descricao[TAM_DESC];
    int prioridade;
    struct Chamado *prox;
} Chamado;

typedef struct Fila {
    Chamado *inicio;
    Chamado *fim;
    int tamanho;
} Fila;

// Limpa a tela dependendo do sistema operacional
void limpar_tela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Limpa o buffer de entrada para evitar problemas com scanf/fgets
void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Lê uma string com validação e remove quebra de linha
void ler_string(const char *msg, char *dest, int tam) {
    printf("%s", msg);
    if (fgets(dest, tam, stdin) != NULL) {
        dest[strcspn(dest, "\n")] = '\0';
    } else {
        dest[0] = '\0';
    }
}

// Pausa aguardando o usuário pressionar ENTER
void pausar(void) {
    int c;
    centralizar();
    printf(GRAY "Pressione ENTER para continuar..." RESET);
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Imprime espaços para centralizar
void centralizar(void) {
    int i;
    int espacos = (LARGURA_TELA - LARGURA_CONTEUDO) / 2;
    i = 0;
    for (i; i < espacos; i++) {
        printf(" ");
    }
}

// Retorna string da prioridade
const char* get_prioridade_str(int pri) {
    if (pri == 1) return "BAIXA";
    if (pri == 2) return "MEDIA";
    if (pri == 3) return "ALTA";
    return "DESC";
}

// Retorna cor da prioridade
const char* get_prioridade_cor(int pri) {
    if (pri == 1) return BLUE;
    if (pri == 2) return YELLOW;
    if (pri == 3) return RED;
    return GRAY;
}

// Inicializa a fila vazia
void iniciar_fila(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
}

// Cria um novo chamado na memória
Chamado *criar_chamado(int id, const char *nome, const char *setor, const char *descricao, int prioridade) {
    Chamado *no = malloc(sizeof(Chamado));
    if (no == NULL) {
    	centralizar();
        printf(RED BOLD "[ERRO] Memoria insuficiente para criar chamado.\n" RESET);
        exit(1);
    }
    
	no->id = id;
    strncpy(no->nome, nome, TAM_NOME - 1);
	no->nome[TAM_NOME - 1] = '\0';
    
    strncpy(no->setor, setor, TAM_SETOR - 1);
	no->setor[TAM_SETOR - 1] = '\0';
    
    strncpy(no->descricao, descricao, TAM_DESC - 1);
	no->descricao[TAM_DESC - 1] = '\0';
    
    no->prioridade = prioridade;
    no->prox = NULL;
    return no;
}

// Adiciona chamado no final da fila (com ordenação por prioridade)
void enfileirar(Fila *f, Chamado *no) {
    no->prox = NULL;
    
    // Se fila vazia ou prioridade MAIOR que o primeiro (3 > 1)
    if (f->inicio == NULL || no->prioridade > f->inicio->prioridade) {
        no->prox = f->inicio;
        f->inicio = no;
        if (f->fim == NULL) f->fim = no;
        f->tamanho++;
        return;
    }
    
    // Procurar posição correta (prioridade maior vem antes)
    Chamado *atual = f->inicio;
    while (atual->prox != NULL && atual->prox->prioridade >= no->prioridade) {
        atual = atual->prox;
    }
    
    no->prox = atual->prox;
    atual->prox = no;
    
    if (no->prox == NULL) {
        f->fim = no;
    }
    
    f->tamanho++;
}

// Remove e retorna o primeiro chamado da fila
Chamado *desenfileirar(Fila *f) {
    if (f->inicio == NULL){
		return NULL;
	}
    Chamado *no = f->inicio;
    f->inicio = no->prox;
    
	if (f->inicio == NULL){
		f->fim = NULL;
	}
	
    no->prox = NULL;
    f->tamanho--;
    
	return no;
}

// Calcula o próximo ID disponível (um acima do maior existente)
int proximo_id(Fila *f) {
    int maior = 0;
    
	Chamado *no = f->inicio;
    
	while (no != NULL) {
        if (no->id > maior){
			maior = no->id;
		}
        no = no->prox;
    }
    return maior + 1;
}

// Imprime o cabeçalho da aplicação centralizado
void cabecalho(void) {
    printf("\n");
    centralizar();
    printf(CYAN BOLD "+------------------------------------------------+" RESET "\n");
    centralizar();
    printf(CYAN BOLD "|        SISTEMA DE CHAMADOS DE SUPORTE          |" RESET "\n");
    centralizar();
    printf(CYAN BOLD "+------------------------------------------------+" RESET "\n");
    centralizar();
    printf(GRAY "+------------------------------------------------+" RESET "\n");
    centralizar();
    printf(DIM "|	Fila encadeada + persistencia em arquivo   |\n" RESET);
    centralizar();
    printf(GRAY "+------------------------------------------------+" RESET "\n");
    printf("\n");
}

// Imprime o menu com descrições mais claras centralizado
void menu(Fila *f) {
    centralizar();
    printf(BOLD "MENU PRINCIPAL" RESET "\n\n");
    centralizar();
    printf(BOLD "[1]" RESET "  Abrir chamado     ->   Criar novo chamado de suporte\n");
    centralizar();
    printf(BOLD "[2]" RESET "  Listar chamados   ->   Ver todos os pendentes\n");
    centralizar();
    printf(BOLD "[3]" RESET "  Listar todos      ->   Ver pendentes + atendidos\n");
    centralizar();
    printf(BOLD "[4]" RESET "  Atender proximo   ->   Atender o primeiro da fila\n");
    centralizar();
    printf(BOLD "[5]" RESET "  Buscar por ID     ->   Encontrar chamado pelo numero\n");
    centralizar();
    printf(BOLD "[6]" RESET "  Salvar dados      ->   Salvar no arquivo\n");
    centralizar();
    printf(BOLD "[7]" RESET "  Sair              ->   Encerrar o programa\n\n");
    
    if (f->tamanho > 0) {
        centralizar();
        printf(GREEN "%d chamado(s) pendente(s)\n" RESET, f->tamanho);
    } else {
        centralizar();
        printf(RED "Nenhum chamado pendente\n" RESET);
    }
    
    printf("\n");
    centralizar();
    printf(YELLOW BOLD "Escolha uma opcao [1-7]: " RESET);
    
}

// Imprime uma linha separadora
void separador(void) {
    centralizar();
    printf(GRAY "---------------------------------------------------------\n" RESET);
}

// Lista todos os chamados com layout alinhado
void listar(Fila *f) {
    Chamado *no = f->inicio;
	printf("\n");
    if (no == NULL) {
        centralizar();
        printf(RED BOLD "[INFO] Nenhum chamado pendente.\n" RESET);
        return;
    }
    
    centralizar();
    printf(MAGENTA BOLD "	    === CHAMADOS PENDENTES (%d) ===\n" RESET, f->tamanho);
    separador();
    
    centralizar();
    printf(BOLD "%-4s | %-20s | %-12s | %-6s | %s \n" RESET, "ID", "NOME", "SETOR", "PRIOR", "DESCRICAO");
    separador();
    
    while (no != NULL) {
        char desc_truncada[TAM_DESC];
        strncpy(desc_truncada, no->descricao, 34);
        desc_truncada[34] = '\0';
        if (strlen(no->descricao) > 34) {
            strcat(desc_truncada, "...");
        }
        
        const char *cor_pri = get_prioridade_cor(no->prioridade);
        centralizar();
        printf(GREEN "%-4d" RESET " | %-20s | %-12s | %s%-6s%s | %s\n", 
               no->id, no->nome, no->setor, cor_pri, get_prioridade_str(no->prioridade), RESET, desc_truncada);
        no = no->prox;
    }
    
    separador();
}

// Lista TODOS os chamados (pendentes + atendidos)
void listar_todos(Fila *f) {
    FILE *arq_atendidos;
    char linha[300];
    int id;
    char nome[TAM_NOME], setor[TAM_SETOR], descricao[TAM_DESC];
    int prioridade;
    Chamado *no;
    int total_atendidos = 0;
    int total_pendentes = f->tamanho;
    
    printf("\n");
    centralizar();
    printf(MAGENTA BOLD "	      === CHAMADOS PENDENTES (%d) ===\n" RESET, total_pendentes);
    separador();
    
    centralizar();
    printf(BOLD "%-4s | %-20s | %-12s | %-6s | %s \n" RESET, "ID", "NOME", "SETOR", "PRIOR", "DESCRICAO");
    separador();
    
    no = f->inicio;
    while (no != NULL) {
        char desc_truncada[TAM_DESC];
        strncpy(desc_truncada, no->descricao, 34);
        desc_truncada[34] = '\0';
        if (strlen(no->descricao) > 34) {
            strcat(desc_truncada, "...");
        }
        
        const char *cor_pri = get_prioridade_cor(no->prioridade);
        centralizar();
        printf(GREEN "%-4d" RESET " | %-20s | %-12s | %s%-6s%s | %s\n", 
               no->id, no->nome, no->setor, cor_pri, get_prioridade_str(no->prioridade), RESET, desc_truncada);
        no = no->prox;
    }
    
    separador();
    
    printf("\n");
    centralizar();
    printf(YELLOW BOLD "	         === CHAMADOS ATENDIDOS ===\n" RESET);
    separador();
    
    arq_atendidos = fopen("atendidos.txt", "r");
    if (arq_atendidos != NULL) {
        while (fgets(linha, sizeof(linha), arq_atendidos) != NULL) {
            linha[strcspn(linha, "\n")] = '\0';
            if (sscanf(linha, "%d;%79[^;];%49[^;];%119[^;];%d", &id, nome, setor, descricao, &prioridade) == 5) {
                char desc_truncada[TAM_DESC];
                strncpy(desc_truncada, descricao, 34);
                desc_truncada[34] = '\0';
                if (strlen(descricao) > 34) {
                    strcat(desc_truncada, "...");
                }
                
                const char *cor_pri = get_prioridade_cor(prioridade);
                centralizar();
                printf(YELLOW "%-4d" RESET " | %-20s | %-12s | %s%-6s%s | %s\n", 
                       id, nome, setor, cor_pri, get_prioridade_str(prioridade), RESET, desc_truncada);
                total_atendidos++;
            }
        }
        fclose(arq_atendidos);
    } else {
        centralizar();
        printf(GRAY "       Nenhum chamado atendido registrado.\n" RESET);
    }
    
    separador();
    
    centralizar();
    printf(DIM "       TOTAL: %d pendente(s) + %d atendido(s) = %d chamado(s)\n" RESET, 
           total_pendentes, total_atendidos, total_pendentes + total_atendidos);
}

// Abre um novo chamado com validação
void abrir_chamado(Fila *f) {
    char nome[TAM_NOME], setor[TAM_SETOR], descricao[TAM_DESC];
    int prioridade;
    
    printf("\n");
    centralizar();
    printf(CYAN BOLD "	    	 === ABRIR CHAMADO ===\n" RESET);
    separador();
    
    centralizar();
    printf("Nome do solicitante: ");
    ler_string("", nome, TAM_NOME);
    
    if (strlen(nome) == 0) {
        centralizar();
        printf(RED "[ERRO] Nome não pode estar vazio.\n" RESET);
        pausar();
        return;
    }
    
    centralizar();
    printf("Setor: ");
    ler_string("", setor, TAM_SETOR);
    
    if (strlen(setor) == 0) {
        centralizar();
        printf(RED "[ERRO] Setor não pode estar vazio.\n" RESET);
        pausar();
        return;
    }
    
    centralizar();
    printf("Descricao: ");
    ler_string("", descricao, TAM_DESC);
    
    if (strlen(descricao) == 0) {
        centralizar();
        printf(RED "[ERRO] Descrição não pode estar vazia.\n" RESET);
        pausar();
        return;
    }
    
    centralizar();
    printf("Prioridade (1-Baixa, 2-Media, 3-Alta): ");
    if (scanf("%d", &prioridade) != 1) {
        prioridade = 2;
    }
    if (prioridade < 1 || prioridade > 3) {
        prioridade = 2;
    }
    limpar_buffer();
    
    enfileirar(f, criar_chamado(proximo_id(f), nome, setor, descricao, prioridade));
    
    centralizar();
    printf(GREEN BOLD "[SUCESSO] Chamado #%d aberto com sucesso!\n" RESET, proximo_id(f) - 1);
}

// Atende o próximo chamado da fila
void atender_proximo(Fila *f) {
    Chamado *no = desenfileirar(f);
    
    if (no == NULL) {
        centralizar();
        printf(RED BOLD "[INFO] Fila vazia. Nenhum chamado para atender.\n" RESET);
        return;
    }
    printf("\n");
    centralizar();
    printf(YELLOW BOLD "		=== ATENDENDO CHAMADO ===\n" RESET);
    separador();
    centralizar();
    printf(BOLD "ID:" RESET " %d\n", no->id);
    centralizar();
    printf(BOLD "Nome:" RESET " %s\n", no->nome);
    centralizar();
    printf(BOLD "Setor:" RESET " %s\n", no->setor);
    centralizar();
    printf(BOLD "Descricao:" RESET " %s\n", no->descricao);
    centralizar();
    const char *cor_pri = get_prioridade_cor(no->prioridade);
    printf(BOLD "Prioridade:" RESET " %s%s%s\n", cor_pri, get_prioridade_str(no->prioridade), RESET);
    separador();
    
    FILE *arq_atendidos = fopen("atendidos.txt", "a");
    if (arq_atendidos != NULL) {
        fprintf(arq_atendidos, "%d;%s;%s;%s;%d\n", no->id, no->nome, no->setor, no->descricao, no->prioridade);
        fclose(arq_atendidos);
    }
    
    free(no);
    
    centralizar();
    printf(GREEN BOLD "[SUCESSO] Chamado atendido e concluído!\n" RESET);
}

// Busca chamado por ID com feedback claro
void buscar_por_id(Fila *f) {
    int id;
    Chamado *no;
    
    printf("\n");
    centralizar();
    printf(CYAN BOLD "		=== BUSCAR POR ID ===\n" RESET);
    separador();
    centralizar();
    printf("Digite o ID do chamado: ");
    
    if (scanf("%d", &id) != 1) {
        centralizar();
        printf(RED "[ERRO] ID inválido. Digite um número inteiro.\n" RESET);
        limpar_buffer();
        pausar();
        return;
    }
    limpar_buffer();
    
    no = f->inicio;
    while (no != NULL) {
        if (no->id == id) {
            centralizar();
            printf(GREEN BOLD "[ENCONTRADO] Chamado encontrado!\n" RESET);
            separador();
            centralizar();
            printf(BOLD "ID:" RESET " %d\n", no->id);
            centralizar();
            printf(BOLD "Nome:" RESET " %s\n", no->nome);
            centralizar();
            printf(BOLD "Setor:" RESET " %s\n", no->setor);
            centralizar();
            printf(BOLD "Descricao:" RESET " %s\n", no->descricao);
            centralizar();
            const char *cor_pri = get_prioridade_cor(no->prioridade);
            printf(BOLD "Prioridade:" RESET " %s%s%s\n", cor_pri, get_prioridade_str(no->prioridade), RESET);
            separador();
            pausar();
            return;
        }
        no = no->prox;
    }
    
    centralizar();
    printf(RED BOLD "[INFO] Chamado #%d não encontrado.\n" RESET, id);
}

// Salva todos os chamados no arquivo
void salvar(Fila *f, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "w");
    Chamado *no;
    
    if (arq == NULL) {
        centralizar();
        printf(RED BOLD "[ERRO] Não foi possível abrir o arquivo '%s' para salvar.\n" RESET, nome_arquivo);
        return;
    }
    
    no = f->inicio;
    while (no != NULL) {
        fprintf(arq, "%d;%s;%s;%s;%d\n", no->id, no->nome, no->setor, no->descricao, no->prioridade);
        no = no->prox;
    }
    
    fclose(arq);
    centralizar();
    printf(GREEN BOLD "[SUCESSO] %d chamado(s) salvo(s) em '%s'.\n" RESET, f->tamanho, nome_arquivo);
}

// Carrega chamados do arquivo
void carregar(Fila *f, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "r");
    char linha[300];
    int id;
    char nome[TAM_NOME], setor[TAM_SETOR], descricao[TAM_DESC];
    int prioridade;
    
    if (arq == NULL) {
        return;
    }
    
    int carregados = 0;
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        if (sscanf(linha, "%d;%79[^;];%49[^;];%119[^;];%d", &id, nome, setor, descricao, &prioridade) == 5) {
            enfileirar(f, criar_chamado(id, nome, setor, descricao, prioridade));
            carregados++;
        }
    }
    
    fclose(arq);
    
    if (carregados > 0) {
        centralizar();
        printf(GREEN DIM "\n[INFO] %d chamado(s) carregado(s) do arquivo.\n" RESET, carregados);
    }
}

// Libera toda a memória da fila
void liberar(Fila *f) {
    Chamado *atual = f->inicio;
    while (atual != NULL) {
    	Chamado *proximo = atual->prox;
        free(atual);
        atual = proximo;
        
    }
    free(f);
}

int main(void) {
    Fila fila;
    int op;
    
    iniciar_fila(&fila);
    carregar(&fila, ARQ);
    
    do {
        limpar_tela();
        cabecalho();
        menu(&fila);
        
        if (scanf("%d", &op) != 1) {
            centralizar();
            printf(RED "[ERRO] Opção inválida. Digite um número entre 1 e 7.\n" RESET);
            limpar_buffer();
            pausar();
            continue;
        }
        limpar_buffer();
        
        switch (op) {
            case 1: 
                abrir_chamado(&fila); 
                break;
            case 2: 
                listar(&fila); 
                break;
            case 3: 
                listar_todos(&fila); 
                break;
            case 4: 
                atender_proximo(&fila); 
                break;
            case 5: 
                buscar_por_id(&fila); 
                break;
            case 6: 
                salvar(&fila, ARQ); 
                centralizar();
                printf(GRAY BOLD "[SAINDO] Encerrando sistema...\n" RESET);
                exit(1);
			case 7:
			    centralizar();
			    printf(YELLOW BOLD "[CONFIRMAR] Deseja salvar os dados antes de sair? [S/N]:" RESET);
			    
				char resp;
			    resp = getchar();
			    
			    if (resp == 's' || resp == 'S') {
			        salvar(&fila, ARQ);
			    }
			    
			    centralizar();
			    printf(GRAY BOLD "[SAINDO] Encerrando sistema...\n" RESET);
			    liberar(&fila);
			    return 0;
				            
			default: 
			    centralizar();
			    printf(RED BOLD "[ERRO] Opção inválida. Digite um número entre 1 e 7.\n" RESET);
		}
        
        if (op != 7) pausar();
    } while (op != 7);
    
    liberar(&fila);
    return 0;
}
