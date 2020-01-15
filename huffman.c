#include <stdio.h>
#include <stdlib.h>

struct No{
No *esq;
No *dir;
No *prox;
int freq;
char letra;
char *code;

};typedef struct No ListaHuffman;
}

char *carregaArquivo(char *nomeArquivo);
int *contarFrequencia(char *texto);
ListaHuffman *insereOrdenado (int, int);
ListaHuffman *converterListaToArvore(ListaHuffman *);
void gerarCodigos(ListaHuffman *);

int main(){

char *nomeArquivo, *texto;
int vetor[256];//criar ponteiro com callloc
ListaHuffman *lista;

nomeArquivo = lerString();
texto = carregaArquivo(nomeArquivo);
vetor = contarFrequencia(texto);

for(int i=0; i<256; i++){
if(vetor[i] > 0){
lista = insereOrdenado(vetor[i], i);
}
}

lista = converterListaToArvore(lista);
gerarCodigos(lista);


}
char *carregaArquivo(char *nomeArquivo){

}

char *lerString(){
char c, *str = NULL;
int i = 0, j = 1;
setbuf(stdin, NULL); //limpa o buffer de entrada
str = (char *)malloc(10*sizeof(char)+1);//aloca 11 espaços
printf("Insira a expressao aritmetica:");
while ((c = getchar()) != '\n' && c != EOF && c != '\0' ){//se n for nulo nem o fim
if (j == 10){
str = (char *) realloc (str, strlen(str)+10*sizeof(char)+1);//realoca comk +11 espaços
}
str[i] = c;
i++;
j++;
}
str[i] = '\0';
return str;
}
