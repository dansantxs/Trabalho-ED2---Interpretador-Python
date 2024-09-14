#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct pLinha {
    char token[50];
    struct pLinha *prox;
}; typedef struct pLinha Lin;

struct pPrograma {
    Lin *linha;
    struct pPrograma *ant, *prox;
}; typedef struct pPrograma Prog;


Lin* novaLinha(char* valor) {
    Lin* nova = (Lin*)malloc(sizeof(Lin));
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
    Prog* novo = (Prog*)malloc(sizeof(Prog));
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

    while (linha[fim] != ' ' && linha[fim] != '\t' && linha[fim] != '\n' && linha[fim] != '(' && linha[fim] != '\0') {
        fim++;
    }

    strncpy(token, linha, fim - inicio);
    token[fim - inicio] = '\0';

    if (strcmp(token, "def") == 0 || strcmp(token, "print") == 0) {
        extrairAntesDoParenteses = 1;
    }

    inicio = 0;
    fim = 0;

    while (fim <= tamanho) {
        if (linha[fim] == ' ' || linha[fim] == '\t' || linha[fim] == '\n' || linha[fim] == '\0') {
            if (fim > inicio) {
                strncpy(token, &linha[inicio], fim - inicio);
                token[fim - inicio] = '\0';
                adicionarToken(linhaTokens, token);
            }
            inicio = fim + 1;
        } else if (linha[fim] == '(') {
            if (extrairAntesDoParenteses && fim > inicio) {
                strncpy(token, &linha[inicio], fim - inicio);
                token[fim - inicio] = '\0';
                adicionarToken(linhaTokens, token);
            }
            token[0] = '(';
            token[1] = '\0';
            adicionarToken(linhaTokens, token);
            inicio = fim + 1;
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

void imprimirPrograma(Prog* programa) {
    Prog* progAtual = programa;
    while (progAtual != NULL) {
        Lin* linhaAtual = progAtual->linha;
        while (linhaAtual != NULL) {
            printf("%s ", linhaAtual->token);
            linhaAtual = linhaAtual->prox;
        }
        printf("\n");
        progAtual = progAtual->prox;
    }
}


int main(void) {
    Prog *p = NULL;
    char caminhoArquivo[50];
    strcpy(caminhoArquivo, "C://Trabalho//main.py");
    
    lerArquivoPython(&p, caminhoArquivo);
    
    imprimirPrograma(p);
    
    return 0;
}

