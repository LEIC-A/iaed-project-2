#include "proj.h"

/* Variaveis globais modificadas na funcao de hash */

/* Tem como objectivo a optimizacao do codigo */

unsigned long last_hashed;
int last_strlen;

/* ================================================================ */
/* ===================== Funcoes Gerais =========================== */
/* ================================================================ */

/* Funcao de hash de strings conhecida como djb2 Hash Function */
unsigned long hash(char *str)
{
	/* O valor 5381 foi escolhido pelo criador da funcao porque obtinha os melhores resultados */
	unsigned long hash = 5381;
	int c;
	int str_lenght = 0;

	while ((c = *str++)){
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		str_lenght++;
	}
	
	/* Guardar valores globais */
	last_strlen = str_lenght;
	last_hashed = hash % hash_table_size;
	
	return last_hashed;
}

/* Funcao que converte toda uma string em minusculas */
void tolower_string(char* str)
{
	int i;
	for(i = 0; str[i]; i++)
		str[i] = tolower(str[i]);
}

/* Funcao que inicializa a hash table, percorrendo-a e colocando todos os seus valores a NULL */
void init_hash_table()  
{
	unsigned long i;
	for(i=0; i<hash_table_size; i++)
		hash_table[i] = NULL;
}

/* Funcao que liberta o espaco guardado em memoria */
void free_memory()
{
	int i;
	ocorrencia *occur, *temp_occur;
	/* Liberta a tabela de linhas originais */
	for(i=0; i<n_linhas; i++)
		free(tabela_linhas[i]);
	free(tabela_linhas);
	
	/* Liberta todas as estruturas node e as estruturas a esta associadas */
	for(i=0; i < n_palavras; i++){
		free(vector_nodes_palavra[i]->item->valor); /* Liberta a string (palavra) */
		for(occur=vector_nodes_palavra[i]->item->ocorrencias; occur != NULL; occur=temp_occur){
			temp_occur = occur->next;
			free(occur);
		}
		free(vector_nodes_palavra[i]->item);
		free(vector_nodes_palavra[i]);
	}
	
	/* Liberta as estruturas globais */
	free(vector_nodes_palavra);
	free(hash_table);
			
	return;
}


/* Funcao que cria o vetor de ponteiros para todas as palvras */
node** cria_vector_nodes_palavra()
{
	int i,j;
	node **vector;
	node* word_node;
	
	vector = (node**) malloc( sizeof(node*) * n_palavras );
	
	for(i=0, j=0; i < hash_table_size; i++){
		if(hash_table[i] != NULL){
			for(word_node=hash_table[i]; word_node; word_node=word_node->next) {
				vector[j++] = word_node;
			}
		}
	}
	return vector;
}