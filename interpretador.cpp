#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio2.h>

// Estruturas
struct pLinha {
    char token[50];
    struct pLinha *prox;
}; typedef struct pLinha Lin;

struct pPrograma {
    Lin *linha;
    struct pPrograma *ant, *prox;
}; typedef struct pPrograma Prog;

struct pilhaLilha {
    Lin *info;
    struct pilhaLilha *prox;
}; typedef struct pilhaLilha PilhaL;

// Funções - Pilha
void initPL(PilhaL **PL) {
    *PL = NULL;
}

void pushL(PilhaL **PL, Lin *x) {
    PilhaL *nova = (PilhaL*) malloc(sizeof(PilhaL));
    nova->info = x;
    nova->prox = *PL;
    *PL = nova;
}

char isEmptyPL(PilhaL *PL) {
    return PL == NULL;
}

void popL(PilhaL **PL, Lin **x) {
    PilhaL *aux;
    if (!isEmptyPL(*PL)) {
        aux = *PL;
        *x = (*PL)->info;
        *PL = (*PL)->prox;
        free(aux);
    } else {
        *x = NULL;
    }
}

// Funções
Lin* novaLinha(char* valor) {
    Lin* nova = (Lin*) malloc(sizeof(Lin));
    strcpy(nova->token, valor);
    nova->prox = NULL;
    return nova;
}

void adicionarToken(Lin** linha, char* valor) {
    Lin* novo = novaLinha(valor);
    if (*linha == NULL) {
        *linha = novo;
    } else {
        Lin* temp = *linha;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = novo;
    }
}

Prog* novoPrograma() {
    Prog* novo = (Prog*) malloc(sizeof(Prog));
    novo->linha = NULL;
    novo->ant = NULL;
    novo->prox = NULL;
    return novo;
}

void adicionarLinhaPrograma(Prog** programaAtual, Lin* novaLinha) {
    Prog* novoProg = novoPrograma();
    novoProg->linha = novaLinha;
    
    if (*programaAtual == NULL) {
        *programaAtual = novoProg;
    } else {
        (*programaAtual)->prox = novoProg;
        novoProg->ant = *programaAtual;
        *programaAtual = novoProg;
    }
}

void extrairTokens(char* linha, Lin** linhaTokens) {
    int inicio = 0, fim = 0, tamanho = strlen(linha);
    char token[50];
    int extrairAntesDoParenteses = 0; 
    int dentroParenteses = 0;

    while (fim <= tamanho) {
        if (linha[fim] == ' ' || linha[fim] == '\t' || linha[fim] == '\n' || linha[fim] == '\0') {
            if (fim > inicio) {
                strncpy(token, &linha[inicio], fim - inicio);
                token[fim - inicio] = '\0';
                adicionarToken(linhaTokens, token);
            }
            inicio = fim + 1;
        } else if (linha[fim] == '(') {
            strncpy(token, &linha[inicio], fim - inicio);
            token[fim - inicio] = '\0';
            adicionarToken(linhaTokens, token);
            token[0] = '(';
            token[1] = '\0';
            adicionarToken(linhaTokens, token);
            inicio = fim + 1;
            dentroParenteses = 1;
        } else if (linha[fim] == ')') {
            if (fim > inicio) {
                strncpy(token, &linha[inicio], fim - inicio);
                token[fim - inicio] = '\0';
                adicionarToken(linhaTokens, token);
            }
            token[0] = ')';
            token[1] = '\0';
            adicionarToken(linhaTokens, token);
            inicio = fim + 1;
            dentroParenteses = 0;
        } else if (linha[fim] == ',') {
            strncpy(token, &linha[inicio], fim - inicio);
            token[fim - inicio] = '\0';
            adicionarToken(linhaTokens, token);
            token[0] = ',';
            token[1] = '\0';
            adicionarToken(linhaTokens, token);
            inicio = fim + 1;
        }
        fim++;
    }
}

void lerArquivoPython(Prog** p, char* caminhoArquivo) {
    FILE* arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Prog* programa = NULL;
    Prog* programaAtual = NULL;

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Lin* nova = NULL;
        extrairTokens(linha, &nova);
        adicionarLinhaPrograma(&programaAtual, nova);
        if (programa == NULL) {
            programa = programaAtual;
        }
    }

    fclose(arquivo);
    *p = programa;
}

void imprimirPrograma(Prog* programa, Prog* linhaExec) {
    system("cls");
    Prog* progAtual = programa;
    while (progAtual != NULL) {
        Lin* linhaAtual = progAtual->linha;
        if (progAtual == linhaExec) {
            textbackground(CYAN);
            textcolor(BLACK);
        } else {
            textbackground(BLACK);
            textcolor(WHITE);
        }
        while (linhaAtual != NULL) {
            printf("%s ", linhaAtual->token);
            linhaAtual = linhaAtual->prox;
        }
        printf("\n");
        progAtual = progAtual->prox;
    }
}

Prog* encontrarPrimeiraLinhaExecutavel(Prog* programa) {
    Prog* progAtual = programa;
    int fimDef = 1;

    while (progAtual != NULL) {
        Lin* linhaAtual = progAtual->linha;

        if (linhaAtual != NULL) {
            if (strcmp(linhaAtual->token, "def") == 0)
                fimDef = 0;
            else if (strcmp(linhaAtual->token, "fim-def") == 0)
                fimDef = 1;
            else if (fimDef == 1)
                return progAtual;
        }

        progAtual = progAtual->prox;
    }

    return NULL;
}

Prog* encontrarFuncao(Prog* programa, char* nomeFuncao) {
    Prog* progAtual = programa;
    while (progAtual != NULL) {
        Lin* linhaAtual = progAtual->linha;
        if (linhaAtual != NULL && strcmp(linhaAtual->token, "def") == 0) {
            linhaAtual = linhaAtual->prox; 
            if (linhaAtual != NULL && strcmp(linhaAtual->token, nomeFuncao) == 0) {
                return progAtual;
            }
        }
        progAtual = progAtual->prox;
    }
    return NULL; 
}

int main(void) {
    Prog *p = NULL, *linhaExec;
    char tecla;
    char caminhoArquivo[50];
    
    while (1) {
        if (kbhit()) {
            tecla = getch();
            
            if (tecla == 65) { // F7 - Abrir arquivo
                //printf("Digite o caminho do arquivo: ");
                //scanf("%s", caminhoArquivo);
                strcpy(caminhoArquivo, "C://Trabalho//main.py");
                lerArquivoPython(&p, caminhoArquivo);
                linhaExec = encontrarPrimeiraLinhaExecutavel(p);
                imprimirPrograma(p, linhaExec);
            }
            
            if (tecla == 66) { // F8 - Executar passo a passo
                while (linhaExec != NULL && tecla != 27) { // ESC
                    if (kbhit()) {
                        tecla = getch();
                        if (tecla == 13) { // ENTER
                            linhaExec = linhaExec->prox;
                            imprimirPrograma(p, linhaExec);
                        }
                    }
                }
            }
            
            if (tecla == 27) // ESC
                return 0;
        }
    }
}
