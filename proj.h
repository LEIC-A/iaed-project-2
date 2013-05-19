#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================ */
/* ===================== Constantes =============================== */
/* ================================================================ */
   
#define MAXLINHA 1026
#define MAXPALAVRA 1025
#define NUMSEP 9
#define ESTIMATIVAPALAVRASLINHA 255

/* Constantes para o tipo bool definido abaixo */
#define FALSE 0
#define TRUE 1

/* ================================================================ */
/* ===================== Estruturas de Dados ====================== */
/* ================================================================ */

/* O tipo bool sera apenas um numero inteiro (0 = Falso, 1 = Verdadeiro) */
typedef char bool;
/* Prototipo de uma estrutura (necessario pois as estruturas referenciam-se mutuamente) */
typedef struct PALAVRA palavra; 

/* ----- Estrutura Ocorrencia ------------------------------------- */
/* ---------------------------------------------------------------- */

typedef struct OCORRENCIA {
        int linha;
	palavra * palavra_anterior;
	palavra * palavra_seguinte;
        struct OCORRENCIA * next;
} ocorrencia;
 
/* ----- Estrutura Palavra ---------------------------------------- */
/* ---------------------------------------------------------------- */

struct PALAVRA {
        char* valor;
        int n_ocorrencias;
        bool esquecida;
	ocorrencia * ultima_ocorrencia; /* Apenas para otimizacao */
        ocorrencia * ocorrencias;
};

/* ----- Estrutura Node ------------------------------------------- */
/* ---------------------------------------------------------------- */

/* Funciona como uma lista de estruturas palavra */
typedef struct NODE {
	palavra* item;
	struct NODE* next;
} node;

/* ================================================================ */
/* ===================== Variaveis externas ======================= */
/* ================================================================ */

extern char** tabela_linhas;			/* Tabela com as strings originais */
extern int n_linhas;				/* Numero de linhas de texto a serem lidas */
extern node** hash_table;			/* Hash Table */
extern unsigned long hash_table_size;		/* Tamanho da Hash Table */
extern int n_palavras;				/* Numero de palavras diferentes */
extern unsigned long last_hashed;		/* Ultimo hash calculado */
extern int last_strlen;				/* Tamanho da ultima string que sofreu a operacao de hash */
extern node** vector_nodes_palavra;		/* Vetor com ponteiros para todas as estruturas node (que contem palavras) */

/* ================================================================ */
/* ===================== Prototipos de Funcoes ==================== */
/* ================================================================ */

/* ----- Comandos (cmds.c) ---------------------------------------- */
/* ---------------------------------------------------------------- */

void esquece(char* palavra);				/* Esquece uma palavra (comando f) */
palavra* procura_palavra(char* palavra);		/* Procura uma palavra e devolve um ponteiro para esta */
void mostra_texto();					/* Mostra as linhas originais (comando s) */
void procura_1p(char* palavra);				/* Procura por uma palavra e imprime as linhas em que se encontra (comando l) */
void procura_2p(char* palavra1, char* palavra2);	/* Procura por duas palavras consecutivas e imprime as linhas em que se encontram (comando w) */
int cstring_cmp(const void *a, const void *b);		/* Funcao que compara strings usada na funcao qsort() (quicksort) */
void lista_palavras();					/* Lista todas as palavras por ordem lexicografica e mostra o numero de ocorrencia (comando h) */
void imprime_linha(int linha);				/* Imprime uma linha no formato: ("%d %s", numero da linha, conteudo da linha) */

/* ----- Parsing (parse.c) ---------------------------------------- */
/* ---------------------------------------------------------------- */

void parse_line(char *line, int n_line);		/* Faz parse de uma linha */
void handle_word(char* word, int linha);		/* Faz parse de uma palavra */
void nova_ocorrencia(palavra* p_actual, int linha);	/* Gera uma nova ocorrencia de uma palavra */
palavra* nova_palavra(char* word, int linha);		/* Gera uma nova palavra */
void hash_table_insert(palavra* word);			/* Insere uma palavra na hash table */

/* ----- Funcoes Gerais (funcs.c) --------------------------------- */
/* ---------------------------------------------------------------- */

int tolower(int c);					/* Transforma uma letra em minuscula */
void tolower_string(char* str);				/* Transforma todas as letras de uma string em minusculas */
unsigned long hash(char *str);				/* Faz o hash de uma string */
void free_memory();					/* Liberta a memoria no final do programa */
void init_hash_table();					/* Inicia a hash table com NULL */
node** cria_vector_nodes_palavra();			/* Cria um vetor de ponteiros para todas as estruturas node */











