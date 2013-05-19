#include "proj.h"

/* Variaveis Globais e Constantes */

const char separators[] = { ' ','\t',',',';','.','?','!','"','\n' };

palavra* palavra_ant;			/* Estas variaveis seriam utilizadas e/ou modificadas em varias funcoes, */
palavra* palavra_act;			/* por isso tornaram-se variaveis globais para simplificacao do codigo */
ocorrencia* ultima_ocorrencia;
ocorrencia* ocurrencia_actual;

int n_palavras = 0;

/* ================================================================ */
/* ===================== Funcoes de Parsing ======================= */
/* ================================================================ */

/* ---------------------------------------------------------------- */
/* ----- Funcao de parsing de uma linha --------------------------- */
/* ================================================================ */

/* Faz o split de uma linha em palavras e insere cada palavra na `hash_table` */
void parse_line(char *line, int n_line)
{
	int i, j, k;
	char buffer[MAXLINHA];

	/* Reset nas variaveis globais. No inicio do parsing da linha nao devem ter valor pois as linhas sao independentes */
	palavra_ant = NULL;
	palavra_act = NULL;
	ultima_ocorrencia = NULL;
	ocurrencia_actual = NULL;
	
	for(i = 0, k = 0; line[i] ; i++, k++) {
		buffer[k] = tolower(line[i]);
		for(j = 0; j < NUMSEP; j++) {
			if(line[i] == separators[j]) {
				if(k) {
					buffer[k] = '\0';
					handle_word(buffer, n_line);
				}
				k = -1;
			}
		}
	}
}

/* ---------------------------------------------------------------- */
/* ----- Funcao de parsing de uma palavra ------------------------- */
/* ================================================================ */

void handle_word(char* word, int linha){
	
	/* A funcao procura_palavra (usada abaixo) realiza um hash() da palavra. Essa funcao de hash foi modificada
	   de forma a contar o numero de caracteres da palavra (resultado da funcao strlen) que guarda numa variavel 
	   global bem como o hash calculado, que tambem fica numa variavel global. Isto evita usar estas duas operacoes 
	   mais tarde para tratar uma palavra pois esses valores estao calculados e guardados */
	   
	if( ( palavra_act = procura_palavra(word) ) ){
		/* A palavra ja apareceu no texto */
		nova_ocorrencia(palavra_act, linha);
	} else {
		/* A palavra nunca tinha aparecido no texto */
		palavra_act = nova_palavra(word, linha);
	}
	
	if(ultima_ocorrencia){
		/* Definir a `palavra_seguinte` da ocorrencia anterior como a palavra atual */
		ultima_ocorrencia->palavra_seguinte = palavra_act;
	}
	
	/* Atualizacao de variaveis */
	/* A variavel 'ocorrencia_actual' e atualizada dentro das funcoes acima usadas */
	palavra_ant = palavra_act;
	ultima_ocorrencia = ocurrencia_actual;
}

/* ---------------------------------------------------------------- */
/* ----- Funcao que gera uma nova ocorrencia de uma palavra  ------ */
/* ================================================================ */

void nova_ocorrencia(palavra* p_actual, int linha){
	
	/* Declaracoes */
	ocorrencia* occur;
	
	/* Execucao */
	/* Gera a nova ocorrencia */
	occur = (ocorrencia*) malloc( sizeof(ocorrencia) );
	occur->linha = linha;
	occur->palavra_anterior = palavra_ant;
	occur->palavra_seguinte = NULL;
	occur->next = NULL;
	
	/* Atualiza o contador de ocorrencias e coloca a ocorrencia no fim da lista */
	p_actual->n_ocorrencias++;
	
	if( p_actual->ultima_ocorrencia != NULL ) { 
		/* Se existe alguma ocorrencia na lista, insere-se no fim da lista */
		p_actual->ultima_ocorrencia->next = occur;
	} else {
		/* Nao ha uma lista de ocorrencias e a ocorrencia e inserida directamente */
		p_actual->ocorrencias = occur;
	}
	
	/* Atualiza a ultima ocorrencia */
	p_actual->ultima_ocorrencia = occur;
	
	/* Altera o valor da variavel global */
	ocurrencia_actual = occur;
}

/* ---------------------------------------------------------------- */
/* ----- Funcao que gera uma nova palavra ------------------------- */
/* ================================================================ */

palavra* nova_palavra(char* word, int linha){
	
	/* Declaracoes */
	palavra* new_word;
	char* new_string;
	
	/* Execucao */
	
	new_word = (palavra*) malloc( sizeof(palavra) );
	new_string = (char*) malloc( sizeof(char) * ( last_strlen + 1 ) );
	strcpy(new_string, word);
	
	new_word->valor = new_string;
	new_word->esquecida = FALSE;
	new_word->n_ocorrencias = 0;
	new_word->ultima_ocorrencia = NULL;
	new_word->ocorrencias = NULL;
	
	nova_ocorrencia(new_word, linha);
	
	/* Colocar a palavra na hash table */
	hash_table_insert(new_word);
	
	/* Incrementa o contador global de palavras (validas) global */
	n_palavras++;
	
	return new_word;
}

/* ---------------------------------------------------------------- */
/* ----- Funcao que insere uma palavra na hash table -------------- */
/* ================================================================ */

void hash_table_insert(palavra* word){
	
	/* Declaracoes */
	unsigned long hashed;
	node* hash_node;
	node* new_node;
	
	/* Execucao */
	
	new_node = (node*) malloc( sizeof(node) );
	new_node->item = word;
	
	hashed = last_hashed; /* Ultimo hash calculado pela funcao hash() ; Variavel global */
	
	/* Como a ordem de insercao nao e relevante, as palavras serao inseridas no inicio da lista em caso de colisao */
	
	if((hash_node = hash_table[hashed])){
		/* Se houver uma colisao */
		new_node->next = hash_node;
		hash_table[hashed] = new_node;
	} else {
		/* Se nao houver colisao */
		new_node->next = NULL;
		hash_table[hashed] = new_node;
	}
}