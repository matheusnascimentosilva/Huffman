/*
*Projeto de Estrutura de Dados
*@autores: Matheus Nascimento Silva
* Criação de funções adaptadas por:
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//Definição do tipo de dados 'byte'
// 'unsigned char': É o tipo que consegue gravar no intervalo que vai de 0 a 255 bytes
typedef unsigned char byte;

typedef struct nodeArvore
{
    int frequencia;
    byte c;
    struct nodeArvore *esquerda;
    struct nodeArvore *direita;
} nodeArvore;

typedef struct nodeLista
{
    nodeArvore *n;
    struct nodeLista *proximo;
} nodeLista;

typedef struct lista
{
    nodeLista *head;
    int elementos;
} lista;

nodeLista *novoNodeLista(nodeArvore *noArv)
{
    // Alocando memória
    nodeLista *novo;
    if ( (novo = malloc(sizeof(*novo))) == NULL ) return NULL;

    // Adiciona a árvore ao nó
    novo->n = noArv;

    // Faz o campo próximo apontar para NULL
    novo->proximo = NULL;

    return novo;
}
nodeArvore *novoNodeArvore(byte c, int frequencia, nodeArvore *esquerda, nodeArvore *direita)
{
    // Aloca memória
    nodeArvore *novo;

    if ( ( novo = malloc(sizeof(*novo)) ) == NULL ) return NULL;

    // Atribui na árvore os valores passados como parâmetro
    novo->c = c;
    novo->frequencia = frequencia;
    novo->esquerda = esquerda;
    novo->direita = direita;

    return novo;
}
void insereLista(nodeLista *n, lista *l)
{
    // Se a lista passada como parâmetro não tem um nó no início (vazia), insira o nó no início
    if (!l->head)
    {
        l->head = n;
    }

    // Se o campo 'frequência' do 'nó' parâmetro for menor que o campo 'frequência' do primeiro item (head)
    // da lista, incluir o novo nó como head, e colocar o head antigo como next desse novo

    else if (n->n->frequencia < l->head->n->frequencia)
    {
        n->proximo = l->head;
        l->head = n;
    }
    else
    {
        // nó auxiliar que inicia apontando para o segundo nó da lista (head->proximo)
        nodeLista *aux = l->head->proximo;
        // nó auxiliar que inicia apontando para o primeiro nó da lista
        nodeLista *aux2 = l->head;

        // Laço que percorre a lista e insere o nó na posição certa de acordo com sua frequência.
        //
        // Se sabe que aux começa apontando para o segundo item da lista e aux2 apontando para o primeiro.
        // Sendo assim, os ponteiros seguirão mudando de posição enquanto aux não for o fim da lista,
        // e enquanto a frequência do nó apontado por aux for menor ou igual a frequência do 'nó' parâmetro.

        while (aux && aux->n->frequencia <= n->n->frequencia)
        {
            aux2 = aux;
            aux = aux2->proximo;
        }

        // Se insere o nó na posição certa.
        aux2->proximo = n;
        n->proximo = aux;
    }

    // Incrementa quantidade de elementos
    l->elementos++;
}
nodeArvore *popMinLista(lista *l)
{

    // Ponteiro auxilar que aponta para o primeiro nó da lista
    nodeLista *aux = l->head;

    // Ponteiro auxiliar que aponta para a árvore contida em aux (árvore do primeiro nó da lista)
    nodeArvore *aux2 = aux->n;

    // Aponta o 'head' da lista para o segundo elemento dela
    l->head = aux->proximo;

    // Libera o ponteiro aux
    free(aux);
    aux = NULL;

    // Decrementa a quantidade de elementos
    l->elementos--;

    return aux2;
}
void contarFrequencia(FILE *entrada, unsigned int *listaBytes)
{

    byte c;

    /***
    *
    * fread( array/bloco de memoria , tamanho de cada elemento, quantos elementos, arquivo de entrada )
    * fread retorna a quantidade de blocos lidos com sucesso
    *
    * Faz a leitura de 1 bloco de tamanho 1 byte a partir do arquivo 'entrada'
    * e salva no espaco de memoria de 'c'.
    *
    * Converte esse byte num valor decimal, acessa o bucket correspondente e incrementa o valor (frequência).
    *
    ***/

    while (fread(&c, 1, 1, entrada) >= 1)
    {
        listaBytes[(byte)c]++;
    }
    rewind(entrada); // "rebobina o arquivo"

}
bool pegaCodigo(nodeArvore *n, byte c, char *buffer, int tamanho)
{

    // Caso base da recursão:
    // Se o nó for folha e o seu valor for o buscado, colocar o caractere terminal no buffer e encerrar

    if (!(n->esquerda || n->direita) && n->c == c)
    {
        buffer[tamanho] = '\0';
        return true;
    }
    else
    {
        bool encontrado = false;

        // Se existir um nó à esquerda
        if (n->esquerda)
        {
            // Adicione '0' no bucket do buffer correspondente ao 'tamanho' nodeAtual
            buffer[tamanho] = '0';

            // fazer recursão no nó esquerdo
            encontrado = pegaCodigo(n->esquerda, c, buffer, tamanho + 1);
        }

        if (!encontrado && n->direita)
        {
            buffer[tamanho] = '1';
            encontrado = pegaCodigo(n->direita, c, buffer, tamanho + 1);
        }
        if (!encontrado)
        {
            buffer[tamanho] = '\0';
        }
        return encontrado;
    }
}
nodeArvore *converteListaToArvore(unsigned *listaBytes)
{
    // Lista com head apontando pra NULL e com campo 'elementos' valendo 0;
    lista l = {NULL, 0};

    // Popula usando a array 'listaBytes' (que contém as frequências) uma lista encadeada de nós.
    // Cada nó contém uma árvore.
    for (int i = 0; i < 256; i++)
    {
        if (listaBytes[i]) // Se existe ocorrência do byte
        {
            // Insere na lista 'l' um nó referente ao byte i e sua respectiva frequência (guardada em listaBytes[i]).
            // Faz os nós esquerdo e direito das árvores apontarem para NULL;
            insereLista(novoNodeLista(novoNodeArvore(i, listaBytes[i], NULL, NULL)), &l);
        }
    }

    while (l.elementos > 1) // Enquanto o número de elementos da lista for maior que 1
    {
        // Nó esquerdo chama a função popMinLista() que vai na lista e pega a árvore fixada no primeiro nó
        // (que é a que contém a menor frequência)
        nodeArvore *nodeEsquerdo = popMinLista(&l);

        // Pega o outro nó que tem menor frequência
        nodeArvore *nodeDireito = popMinLista(&l);

        // Preenche com '#' o campo de caractere do nó da árvore.
        // Preenche o campo 'frequência' com a soma das frequências de 'nodeEsquerdo' e 'nodeDireito'.
        // Aponta o nó esquerdo para nodeEsquerdo e o nó direito para nodeDireito
        nodeArvore *soma = novoNodeArvore(
                               '#',
                               nodeEsquerdo->frequencia + nodeDireito->frequencia, nodeEsquerdo, nodeDireito
                           );

        // Reinsere o nó 'soma' na lista l
        insereLista(novoNodeLista(soma), &l);
    }

    return popMinLista(&l);
}
void LiberaMemoriaHuffman(nodeArvore *n)
{
    // Caso base da recursão, enquanto o nó não for NULL
    if (!n) return;
    else
    {
        nodeArvore *esquerda = n->esquerda;
        nodeArvore *direita = n->direita;
        free(n);
        LiberaMemoriaHuffman(esquerda);
        LiberaMemoriaHuffman(direita);
    }
}
int gerarCodigos(FILE *entrada, int posicao, byte *aux )
{
    (posicao % 8 == 0) ? fread(aux, 1, 1, entrada) : NULL == NULL ;

    // Exclamação dupla converte para '1' (inteiro) se não for 0. Caso contrário, deixa como está (0)
    // Joga '1' na casa binária 'posicao' e vê se "bate" com o byte salvo em *aux do momento
    // Isso é usado para percorrer a árvore (esquerda e direita)
    return !!((*aux) & (1 << (posicao % 8)));
}
void MensagemDeErro()
{
    printf("Arquivo nao encontrado\n");
    exit(0);
}

int main(){

return 0;
}
