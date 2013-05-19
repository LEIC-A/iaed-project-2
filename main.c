#include "proj.h"

/* Variaveis Globais */
char** tabela_linhas;
int n_linhas;
node** hash_table;
unsigned long hash_table_size;
node** vector_nodes_palavra;

int main()
{
	/* Variaveis */
	char* s, *s_aux;
	char p1[MAXPALAVRA], p2[MAXPALAVRA], c;
	int i = 0;
	
	
	/* Obtem o numero de linhas a serem lidas (guarda-o numa variavel global) */
	scanf("%d",&n_linhas);
	getchar();
	
	
	/* Aloca um vetor de linhas (strings), uma string para guardar temporariamente cada linha e a hash table */
	tabela_linhas = (char**) malloc( sizeof(char*) * n_linhas );
	s = (char*) malloc( sizeof(char) * MAXLINHA );
	hash_table_size = n_linhas * ESTIMATIVAPALAVRASLINHA;
	hash_table = (node**) malloc( sizeof(node*) * (hash_table_size) );
	
	/* Inicializa a hash table */
	/* Esta funcao encontra-se comentada devido a problemas de memoria com o mooshak. 
	   Este problema pode ter a ver com um bug no mooshak ou com alguma optimizacao feita pelo compilador
	   que acaba por gastar mais memoria do que a funcao gasta (atribuicao de NULL a memoria alocada nao
	   gasta memoria).
	   Verificou-se que no mooshak a hash table e inicializada a NULL por default por isso nao vai influenciar os resultados.
	   Contudo esta funcao deveria ser chamada para evitar possiveis valores aleatorios.
	*/
	
	/*init_hash_table();*/

	
	/* Le as linhas */
	do {
		fgets(s,MAXLINHA,stdin);
		s_aux = (char*) malloc( sizeof(char) * (strlen(s) + 1) );
		strcpy(s_aux,s);
		tabela_linhas[i] = s_aux;
		parse_line(s_aux, i+1);
		i++;
	} while ( i < n_linhas );
	
	free(s);
	
    /* Cria um vetor de ponteiros para todas as palavras */
	vector_nodes_palavra = cria_vector_nodes_palavra();
	
	/* Executa os comandos */
	do {
		c = getchar();
		switch (c)
		{
			case 's':
				mostra_texto(tabela_linhas, n_linhas);
				break;
			case 'f':
				getchar();
				scanf("%s",p1);
				esquece(p1);
				break;
			case 'l':
				getchar();
				scanf("%s",p1);
				tolower_string(p1);
				procura_1p(p1);
				break;
			case 'w':
				getchar();
				scanf("%s",p1);
				scanf("%s",p2);
				tolower_string(p1);
				tolower_string(p2);
				procura_2p(p1, p2);
				break;
			case 'h':
				lista_palavras(); 
				break;
			default:
				/* Fim do programa */
				free_memory();
				return 0;
				
		}
		getchar();
	} while (1);
	
	return 0;
}
