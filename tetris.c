#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================================
// CONFIGURAÇÕES GERAIS
// ============================================================
#define TAM_FILA 5
#define TAM_PILHA 3

// 🧩 Nível Novato: Estruturas Base
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L', 'J', 'Z', 'S'
    int id;
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int total;
} FilaCircular;

// 🧠 Nível Aventureiro: Estrutura da Pilha
typedef struct {
    Peca itens[TAM_PILHA];
    int topo; // Índice do próximo espaço livre
} PilhaReserva;

// Variável global para IDs sequenciais
int global_id = 1;

// ============================================================
// FUNÇÕES AUXILIARES (Geração de Peças)
// ============================================================
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    p.tipo = tipos[rand() % 7];
    p.id = global_id++;
    return p;
}

// ============================================================
// IMPLEMENTAÇÃO NÍVEL NOVATO (FILA CIRCULAR)
// ============================================================
void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = 0;
    f->total = 0;
    // Já inicia enchendo a fila
    for(int i = 0; i < TAM_FILA; i++) {
        f->itens[f->tras] = gerarPeca();
        f->tras = (f->tras + 1) % TAM_FILA;
        f->total++;
    }
}

int filaVazia(FilaCircular *f) { return (f->total == 0); }
int filaCheia(FilaCircular *f) { return (f->total == TAM_FILA); }

void enqueue(FilaCircular *f, Peca p) {
    if (filaCheia(f)) return;
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % TAM_FILA; // Lógica circular
    f->total++;
}

Peca dequeue(FilaCircular *f) {
    Peca p = f->itens[f->frente]; // Salva para retornar
    f->frente = (f->frente + 1) % TAM_FILA; // Lógica circular
    f->total--;
    return p;
}

void mostrarFila(FilaCircular *f) {
    printf("\n[ FILA PROXIMAS (%d/%d) ] ", f->total, TAM_FILA);
    if (filaVazia(f)) { printf("Vazia"); }
    else {
        int count = 0;
        int i = f->frente;
        printf("Frente -> ");
        while (count < f->total) {
            printf("[%c-%02d] ", f->itens[i].tipo, f->itens[i].id);
            i = (i + 1) % TAM_FILA;
            count++;
        }
    }
    printf("\n");
}

// ============================================================
// IMPLEMENTAÇÃO NÍVEL AVENTUREIRO (PILHA)
// ============================================================
void inicializarPilha(PilhaReserva *p) {
    p->topo = 0;
}

int pilhaVazia(PilhaReserva *p) { return (p->topo == 0); }
int pilhaCheia(PilhaReserva *p) { return (p->topo == TAM_PILHA); }

int push(PilhaReserva *p, Peca item) {
    if (pilhaCheia(p)) return 0; // Falha
    p->itens[p->topo] = item;
    p->topo++;
    return 1; // Sucesso
}

Peca pop(PilhaReserva *p) {
    p->topo--;
    return p->itens[p->topo];
}

void mostrarPilha(PilhaReserva *p) {
    printf("[ PILHA RESERVA (%d/%d) ] ", p->topo, TAM_PILHA);
    if (pilhaVazia(p)) { printf("Vazia"); }
    else {
        printf("Base -> ");
        for (int i = 0; i < p->topo; i++) {
            printf("[%c-%02d] ", p->itens[i].tipo, p->itens[i].id);
        }
        printf("<- Topo");
    }
    printf("\n");
}

// ============================================================
// IMPLEMENTAÇÃO NÍVEL MESTRE (INTEGRAÇÃO ESTRATÉGICA)
// ============================================================

// Opção 4: Trocar frente da fila com topo da pilha
void trocarFrenteComTopo(FilaCircular *f, PilhaReserva *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n>>> Erro: Precisa ter pecas na Fila e na Pilha para trocar!\n");
        return;
    }

    // Acessa diretamente os índices
    int idxFila = f->frente;
    int idxPilha = p->topo - 1;

    Peca temp = f->itens[idxFila];
    f->itens[idxFila] = p->itens[idxPilha];
    p->itens[idxPilha] = temp;

    printf("\n>>> TROCA REALIZADA: Frente da Fila <-> Topo da Pilha.\n");
}

// Opção 5: Trocar os 3 primeiros da fila com a pilha completa
void trocarBloco(FilaCircular *f, PilhaReserva *p) {
    if (p->topo != 3 || f->total < 3) {
        printf("\n>>> Erro: A Pilha deve estar CHEIA (3) e Fila ter ao menos 3 pecas.\n");
        return;
    }

    // Realiza a troca dos 3 elementos
    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % TAM_FILA; // Calcula índice circular
        int idxPilha = i; // Base (0), meio (1), topo-1 (2)

        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[idxPilha];
        p->itens[idxPilha] = temp;
    }
    printf("\n>>> TROCA DE BLOCO (3 pecas) REALIZADA!\n");
}

// ============================================================
// PROGRAMA PRINCIPAL
// ============================================================
int main() {
    srand(time(NULL)); // Semente aleatória

    FilaCircular fila;
    PilhaReserva pilha;
    int opcao;
    Peca pAux;

    // Inicializações
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        printf("\n========================================\n");
        printf("       DESAFIO TETRIS STACK - MENU      \n");
        printf("========================================\n");
        mostrarPilha(&pilha);
        mostrarFila(&fila);
        printf("----------------------------------------\n");
        
        // Menu Unificado
        printf("1 - Jogar peca da frente (Dequeue)\n"); // Novato
        printf("2 - Reservar peca da frente (Fila -> Pilha)\n"); // Aventureiro
        printf("3 - Usar peca da reserva (Pop Pilha)\n"); // Aventureiro
        printf("4 - Trocar Frente Fila <-> Topo Pilha\n"); // Mestre
        printf("5 - Trocar 3 da Fila <-> 3 da Pilha\n"); // Mestre
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar
                pAux = dequeue(&fila);
                printf("\n>>> Voce JOGOU a peca [%c-%d]!\n", pAux.tipo, pAux.id);
                // Regra Novato: Repor peça
                enqueue(&fila, gerarPeca());
                break;

            case 2: // Reservar (Fila -> Pilha)
                if (pilhaCheia(&pilha)) {
                    printf("\n>>> Pilha Cheia! Nao e possivel reservar.\n");
                } else {
                    pAux = dequeue(&fila); // Tira da fila
                    push(&pilha, pAux);    // Põe na pilha
                    printf("\n>>> Peca [%c-%d] enviada para a RESERVA.\n", pAux.tipo, pAux.id);
                    // Regra Aventureiro: Fila sempre cheia
                    enqueue(&fila, gerarPeca());
                }
                break;

            case 3: // Usar Reserva (Pop)
                if (pilhaVazia(&pilha)) {
                    printf("\n>>> Pilha Vazia! Nada para usar.\n");
                } else {
                    pAux = pop(&pilha);
                    printf("\n>>> Voce usou a peca da RESERVA: [%c-%d]!\n", pAux.tipo, pAux.id);
                }
                break;

            case 4: // Troca Simples (Mestre)
                trocarFrenteComTopo(&fila, &pilha);
                break;

            case 5: // Troca em Bloco (Mestre)
                trocarBloco(&fila, &pilha);
                break;

            case 0:
                printf("\nSaindo do jogo...\n");
                break;

            default:
                printf("\nOpcao Invalida!\n");
        }

    } while (opcao != 0);

}

