#include "proj.h"

/* ================================================================ */
/* ===================== Funcoes Gerais =========================== */
/* ================================================================ */

/* ----- Esquece uma palavra -------------------------------------- */
/* ---------------------------------------------------------------- */

void esquece(char* string){
	palavra* word;
	word = procura_palavra(string);
	if(word != NULL){
		word->esquecida = TRUE;
	} 
}

/* ----- Procura uma palavra e devolve um ponteiro para esta ------ */
/* ---------------------------------------------------------------- */

palavra* procura_palavra(char* word){ /* Devolve NULL se nao existir */
	unsigned long hashed;
	node * hash_node;
	
	hashed = hash(word);
	hash_node = hash_table[hashed];
	
	while(hash_node){
		if( strcmp( hash_node->item->valor, word ) ==  0 ) { 
			/* se as strings forem iguais */
			return hash_node->item;
		}
		hash_node = hash_node->next;
	}
	return NULL;
}

/* ----- Mostra o texto original ---------------------------------- */
/* ---------------------------------------------------------------- */

void mostra_texto(){ 
	int i;
	for( i=0 ; i<n_linhas ; i++ ){
		printf("%s", tabela_linhas[i]);
	}
}

/* ----- Imprime uma linha ---------------------------------------- */
/* ---------------------------------------------------------------- */

void imprime_linha(int linha)
{
	printf("%d %s", linha, tabela_linhas[linha - 1]);
}

/* ----- Procura 1 palavra (comando l) ---------------------------- */
/* ---------------------------------------------------------------- */

void procura_1p(char* word){

	palavra* palavra_encontrada;
	ocorrencia* ocur_actual;
	int ultima_linha=0;
	
	palavra_encontrada = procura_palavra(word);
	
	if( (palavra_encontrada != NULL) && (palavra_encontrada->esquecida == FALSE) ){
		ocur_actual = palavra_encontrada->ocorrencias;
		do{
			if(ultima_linha != ocur_actual->linha){ /* Previne o output de linhas iguais */
				imprime_linha(ocur_actual->linha);
				ultima_linha = ocur_actual->linha;
			}
			ocur_actual = ocur_actual->next;
			
		} while(ocur_actual);
	}
	return;
}

/* ----- Procura 2 palavras (comando w) --------------------------- */
/* ---------------------------------------------------------------- */

void procura_2p(char* palavra1, char* palavra2){
	palavra* palavra_encontrada1;
	palavra* palavra_encontrada2;
	ocorrencia* ocur_actual;
	int ultima_linha=0;
	
	palavra_encontrada1 = procura_palavra(palavra1);
	palavra_encontrada2 = procura_palavra(palavra2);

	
	if(( palavra_encontrada1 != NULL ) && ( palavra_encontrada2 != NULL ) ){ 
		if( ( palavra_encontrada1->esquecida == FALSE ) && ( palavra_encontrada2->esquecida == FALSE ) ){ 
			ocur_actual = palavra_encontrada1->ocorrencias;
			do {
				if( ( (ocur_actual->palavra_anterior != NULL) && ( strcmp(palavra2, ocur_actual->palavra_anterior->valor) == 0 ) ) || 
				( (ocur_actual->palavra_seguinte != NULL) && ( strcmp(palavra2, ocur_actual->palavra_seguinte->valor) == 0 ) ) ) {
					if(ultima_linha != ocur_actual->linha){ /* Previne o output de linhas iguais */
						imprime_linha(ocur_actual->linha);
						ultima_linha = ocur_actual->linha;
					}
				}	
				ocur_actual = ocur_actual->next;
			} while(ocur_actual);
		}
	}
	return;
}

/* ----- Compara 2 strings (usada para o qsort) ------------------- */
/* ---------------------------------------------------------------- */

int cstring_cmp(const void *a, const void *b)
{
	node* ia = *(node**) a;
	node* ib = *(node**) b;
	return strcmp(ia->item->valor, ib->item->valor);
}

/* ----- Faz uma listagem das palavras (comando h) ---------------- */
/* ---------------------------------------------------------------- */

void lista_palavras()
{
	int i;
	static bool sorted = FALSE;
	
	if(sorted == FALSE){
		qsort(vector_nodes_palavra, (size_t) n_palavras, (size_t) sizeof(node*), cstring_cmp );
		sorted = TRUE;
	}
	
	for(i=0; i < n_palavras; i++){
		if(vector_nodes_palavra[i]->item->esquecida != TRUE)
			printf("%s %d\n", vector_nodes_palavra[i]->item->valor, vector_nodes_palavra[i]->item->n_ocorrencias);
	}
	
	return;
}