#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio2.h>
#include <windows.h>

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

struct pVariavel {
    char nome[50];
    char valor[50];
    int status; //1 = variável global - 2 = variável local
    struct pVariavel *prox;
}; typedef struct pVariavel Variavel;

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
        } else if (linha[fim] == '\"') {
            strncpy(token, &linha[inicio], fim - inicio);
            token[fim - inicio] = '\0';
            adicionarToken(linhaTokens, token);
            token[0] = '"';
            token[1] = '\0';
            adicionarToken(linhaTokens, token);
            inicio = fim + 1;
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
        } else if (linha[fim] == '!') {
            strncpy(token, &linha[inicio], fim - inicio);
            token[fim - inicio] = '\0';
            adicionarToken(linhaTokens, token);
            token[0] = '!';
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
    nova->prox = NULL;

    if (*funcoes == NULL) {
        *funcoes = nova;
    } else {
        Funcoes *aux = *funcoes;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = nova;
    }
}

void lerArquivoPython(Prog** p, char* caminhoArquivo, Funcoes** funcoes) {
    FILE* arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
	
	*funcoes = NULL;
    Prog* programa = NULL;
    Prog* programaAtual = NULL;

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Lin* nova = NULL;
        extrairTokens(linha, &nova);
        if(nova != NULL){
        	adicionarLinhaPrograma(&programaAtual, nova);
        	if (programa == NULL) {
	            programa = programaAtual;
	        }
	
	        if (nova != NULL && strcmp(nova->token, "def") == 0 && nova->prox != NULL) {
	            adicionarFuncao(&(*funcoes), nova->prox->token, programaAtual);
	        }
        }
    }

    fclose(arquivo);
    *p = programa;
}

void exibirPrograma(Prog* programa, Prog* linhaExec) {
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
    int fimDef = -1;

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

Prog* encontrarFuncao(Funcoes *funcoes, char* nomeFuncao) {
    Funcoes *atual = funcoes;
    
    while (atual != NULL) {
        if (strcmp(atual->nome, nomeFuncao) == 0) {
            return atual->endereco; 
        }
        atual = atual->prox;
    }

    return NULL;
}

Prog* ehChamadaDeFuncao(Lin *linha, Funcoes *funcoes) {
	Prog *enderecoFuncao;
    while (linha != NULL) {
        if (linha->prox != NULL && strcmp(linha->prox->token, "(") == 0) {
	        enderecoFuncao = encontrarFuncao(funcoes, linha->token);
	        if(enderecoFuncao != NULL)
				return enderecoFuncao;
	    }
	    
	    linha = linha->prox;
    }
    
    return NULL;
}

void armazenarConteudoPrint(Lin *linha) {
    FILE *arquivoSaida = fopen("saida.txt", "a");
    if (arquivoSaida == NULL) {
        printf("Erro ao abrir arquivo de saída.\n");
        return;
    }

    int dentroAspas = 0;
    while (linha != NULL) {    	
        if (strcmp(linha->token, "\"") == 0) {
            dentroAspas = !dentroAspas; 
        } else if (dentroAspas == 1) {
            fprintf(arquivoSaida, "%s ", linha->token);
        } 
        linha = linha->prox;
    }
    fprintf(arquivoSaida, "\n");
    fclose(arquivoSaida);
}

void ehPrint(Lin *linha){
	if (strcmp(linha->token, "print") == 0) 
		armazenarConteudoPrint(linha->prox);
}

void exibirConteudoPrint() {
    system("cls"); 
    FILE *arquivoSaida = fopen("saida.txt", "r");
    if (arquivoSaida  == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivoSaida)) {
        printf("%s", linha);
    }

    fclose(arquivoSaida);

    printf("\nPressione qualquer tecla para continuar...");
    getch();
}

void exibirVariaveis(Variavel *variaveis) {
    system("cls");
    
    if (variaveis == NULL) {
        printf("Nenhuma variavel declarada.\n");
    } else {
    	while (variaveis != NULL) {
	        printf("%s = %s\n", variaveis->nome, variaveis->valor);
        	variaveis = variaveis->prox;
	    }
    }
    
    printf("\nPressione qualquer tecla para continuar...");
    getch();
}

void adicionarVariavel(Variavel **variaveis, char *nome, char *valor, int status) {
    Variavel *nova = (Variavel*) malloc(sizeof(Variavel));
    strcpy(nova->nome, nome);
    strcpy(nova->valor, valor);
    nova->status = status;
    nova->prox = NULL;

    if (*variaveis == NULL) {
        *variaveis = nova;
    } else {
        Variavel *aux = *variaveis;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = nova;
    }
}

void alterarVariavel(Variavel **variaveis, char *nome, char *valor) {
    Variavel *aux = *variaveis;
    
    while (aux != NULL && strcmp(aux->nome, nome) != 0) {
        aux = aux->prox;
    }
    
    if (aux != NULL) {
        strcpy(aux->valor, valor);
    }
}

char* buscarVariavel(Variavel *variaveis, char *nome) {
    Variavel *aux = variaveis;
        
    while (aux != NULL) {  
        if (strcmp(aux->nome, nome) == 0) {
            return aux->valor;
        }
        aux = aux->prox;
    }

    return NULL;
}

void ehVariavel(Lin *linha, Variavel **variaveis, int status) {
    Lin *aux;
    char *valor, *atual;
    
    while (linha != NULL) {
        if (linha->prox != NULL && strcmp(linha->prox->token, "=") == 0) {
            aux = linha;  
            linha = linha->prox->prox;
            
            valor = buscarVariavel(*variaveis, linha->token);
            if (valor == NULL) {
                valor = linha->token;
            }
			
			atual = buscarVariavel(*variaveis, aux->token);
			if (atual == NULL) {
                adicionarVariavel(&(*variaveis), aux->token, valor, status);
            } else {
                alterarVariavel(&(*variaveis), aux->token, valor);
			}
        }
        
        linha = linha->prox;
    }
}

void excluirVariaveisLocais(Variavel **variaveis) {
    Variavel *atual = *variaveis, *ant = NULL, *aux;
    
    while (atual != NULL) {
        if (atual->status == 2) {
            if (ant == NULL) {
                *variaveis = atual->prox;
            } else {
                ant->prox = atual->prox;
            }
            Variavel *aux = atual;
            atual = atual->prox;
            free(aux);
        } else {
            ant = atual;
            atual = atual->prox;
        }
    }
}

int main(void) {
	remove("saida.txt");
	
    char tecla;
    char caminhoArquivo[50], nomeFuncao[50];
    
    PilhaP *pilhaExec;
    initPP(&pilhaExec);
    
    Prog *p = NULL, *linhaExec, *enderecoFuncao;
    Funcoes *funcoes; 
    Variavel *variaveis; 
    
    while (1) {
        if (kbhit()) {
            tecla = getch();
			            
            if (tecla == 65) { // F7 - Abrir arquivo
                strcpy(caminhoArquivo, "C://Users//Daniel dos Santos//Documents//GitHub//Trabalho-ED2---Interpretador-Python//main.py");
                lerArquivoPython(&p, caminhoArquivo, &funcoes); 
                linhaExec = encontrarPrimeiraLinhaExecutavel(p);
                exibirPrograma(p, linhaExec);
            }
            
            if (tecla == 66) { // F8 - Executar passo a passo
            	variaveis = NULL;
            	
                while (linhaExec != NULL && tecla != 27) { // ESC
                    if (kbhit()) {
                        tecla = getch();
                        if (tecla == 13) { // ENTER								
                        	enderecoFuncao = ehChamadaDeFuncao(linhaExec->linha, funcoes);
							if (enderecoFuncao != NULL) {
							    pushP(&pilhaExec, linhaExec);
							    linhaExec = enderecoFuncao;
							    exibirPrograma(p, linhaExec);
							
							    while (linhaExec != NULL && strcmp(linhaExec->linha->token, "fim-def") != 0) {
							        if (kbhit()) {
							            tecla = getch();
							            if (tecla == 13) { // ENTER
											ehVariavel(linhaExec->linha, &variaveis, 2);
							            	ehPrint(linhaExec->linha);
							            
							                linhaExec = linhaExec->prox;
							                exibirPrograma(p, linhaExec);
							            }
                        
				                        if (tecla == 67) { // F9 - Exibir variáveis e seus valores
											exibirVariaveis(variaveis);
											exibirPrograma(p, linhaExec);
							            }
				                        
				                        if (tecla == 68) { // F10 - Exibir print dos resultados
											exibirConteudoPrint();
											exibirPrograma(p, linhaExec);
							            }
							        }
							    }
								
								excluirVariaveisLocais(&variaveis);
							    popP(&pilhaExec, &linhaExec); 
							}							
							
							ehVariavel(linhaExec->linha, &variaveis, 1);
							ehPrint(linhaExec->linha);							
                            
                            linhaExec = linhaExec->prox;
                            exibirPrograma(p, linhaExec);
                        }
                        
                        if (tecla == 67) { // F9 - Exibir variáveis e seus valores
							exibirVariaveis(variaveis);
							exibirPrograma(p, linhaExec);
			            }
                        
                        if (tecla == 68) { // F10 - Exibir print dos resultados
							exibirConteudoPrint();
							exibirPrograma(p, linhaExec);
			            }
                    }
                }
            }
            
            if (tecla == 27) // ESC
                return 0;
        }
    }
}
