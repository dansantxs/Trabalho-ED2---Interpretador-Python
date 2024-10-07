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

struct pFuncoes {
    char nome[50];
    Prog *endereco;
    struct pFuncoes *prox;
}; typedef struct pFuncoes Funcoes;

struct pilhaProg {
    Prog *info;
    struct pilhaProg *prox;
}; typedef struct pilhaProg PilhaP;

// Funções - Pilha
void initPP(PilhaP **PP) {
    *PP = NULL;
}

void pushP(PilhaP **PP, Prog *x) {
    PilhaP *nova = (PilhaP*) malloc(sizeof(PilhaP));
    nova->info = x;
    nova->prox = *PP;
    *PP = nova;
}

char isEmptyPP(PilhaP *PP) {
    return PP == NULL;
}

void popP(PilhaP **PP, Prog **x) {
    PilhaP *aux;
    if (!isEmptyPP(*PP)) {
        aux = *PP;
        *x = (*PP)->info;
        *PP = (*PP)->prox;
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
        /*} else if (linha[fim] == '"') {
            strncpy(token, &linha[inicio], fim - inicio);
            token[fim - inicio] = '\0';
            adicionarToken(linhaTokens, token);
            token[0] = '"';
            token[1] = '\0';
            adicionarToken(linhaTokens, token);
            inicio = fim + 1;*/
        } else if (linha[fim] == '.') {
            strncpy(token, &linha[inicio], fim - inicio);
            token[fim - inicio] = '\0';
            adicionarToken(linhaTokens, token);
            token[0] = '.';
            token[1] = '\0';
            adicionarToken(linhaTokens, token);
            inicio = fim + 1;
        } else if (linha[fim] == ':') {
            strncpy(token, &linha[inicio], fim - inicio);
            token[fim - inicio] = '\0';
            adicionarToken(linhaTokens, token);
            token[0] = ':';
            token[1] = '\0';
            adicionarToken(linhaTokens, token);
            inicio = fim + 1;
        }
        fim++;
    }
}

void adicionarFuncao(Funcoes **funcoes, char *nomeFuncao, Prog *enderecoFuncao) {
    Funcoes *nova = (Funcoes*) malloc(sizeof(Funcoes));
    strcpy(nova->nome, nomeFuncao);
    nova->endereco = enderecoFuncao;
    nova->prox = *funcoes;
    *funcoes = nova;
}

void lerArquivoPython(Prog** p, char* caminhoArquivo, Funcoes** funcoes) {
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

        if (nova != NULL && strcmp(nova->token, "def") == 0 && nova->prox != NULL) {
            adicionarFuncao(funcoes, nova->prox->token, programaAtual);
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

int ehChamadaDeFuncao(Lin *linha, char nomeFuncao[50]) {
    while(linha != NULL){
    	if(strcmp(linha->token, "print") == 0 && strcmp(linha->prox->token, "(") == 1){
    		strcpy(nomeFuncao, linha->token);
    		return 1;
		}
    	
    	linha = linha->prox;
	}
	
    return 0;
}

int main(void) {
    Prog *p = NULL, *linhaExec, *funcaoInicio;
    PilhaP *pilhaExec;
    Funcoes *funcoes = NULL; 
    char tecla;
    char caminhoArquivo[50], nomeFuncao[50];
    
    initPP(&pilhaExec);  // Inicializa a pilha de execução
    
    while (1) {
        if (kbhit()) {
            tecla = getch();
            
            if (tecla == 65) { // F7 - Abrir arquivo
                strcpy(caminhoArquivo, "C://Users//Daniel dos Santos//Documents//GitHub//Trabalho-ED2---Interpretador-Python//main.py");
                lerArquivoPython(&p, caminhoArquivo, &funcoes); 
                linhaExec = encontrarPrimeiraLinhaExecutavel(p);
                imprimirPrograma(p, linhaExec);
            }
            
            if (tecla == 66) { // F8 - Executar passo a passo
                while (linhaExec != NULL && tecla != 27) { // ESC
                    if (kbhit()) {
                        tecla = getch();
                        if (tecla == 13) { // ENTER
                            if (ehChamadaDeFuncao(linhaExec->linha, nomeFuncao)) {
                            	printf("oi");
                                pushP(&pilhaExec, linhaExec);
                                
                                linhaExec = encontrarFuncao(p, nomeFuncao);
                                imprimirPrograma(p, linhaExec);

                                while (linhaExec != NULL && strcmp(linhaExec->linha->token, "fim-def") != 0) {
                                    if (kbhit()) {
                                        tecla = getch();
                                        if (tecla == 13) { // ENTER
                                            linhaExec = linhaExec->prox;
                                            imprimirPrograma(p, linhaExec);
                                        }
                                    }
                                }
                                
                                popP(&pilhaExec, &linhaExec);
                            }
                            
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
