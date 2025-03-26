#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define M 23 //numero primo settado no pdf 

struct tlist
{
    int valor;
    struct tlist* next;
};

struct tlist* criar_node(int key){
    //função só pra criar os nodes da lista
    struct tlist* node=(struct tlist*)malloc(sizeof(struct tlist));
    node->valor=key; //valor do K
    node->next=NULL;
    return node;
}

int obtem_indice (struct tlist* node){
    /*preferi deixar uma função especialmente pro k % M por questão de  didática.
    depois de finalizar a função tabela hash eu reparei que poderia só ter deixado
    lá dentro, mas como eu fiz sem pseudo codigo acabou ficando daqui pra baixo a
    linha de raciocinio pra montar a tabela hash, preferi deixar assim mesmo achando
    menos prático*/
    int index;
    index = (node->valor) % M;
    return index;
}
void tabela_hash(struct tlist*node,struct tlist* vet[],int index){
    if(vet[index]==NULL){
        vet[index]=node; //se não tiver nenhum node no indice obtido, ele é colocado no começo da lista
    } else{
        struct tlist* aux=vet[index];
        while(aux->next!= NULL){
            aux = aux->next; //alcança o final da lista encadeada do indice obtido
        }
        aux->next=node; // posiciona um node de mesmo indice ja ocupado na tabela
    }
}
void imprimir_tabela_hash(struct tlist* vet[]) {
    int i;
    for (i = 0; i < M; i++) {
        if (vet[i] == NULL){
            continue; //coloquei isso pra pular os indices 'vazios'
        } else {
            struct tlist* aux = vet[i];
            printf("%d - ", i); // index
            while (aux != NULL) {
                printf("%d", aux->valor); // imprime os valores k
                aux = aux->next;
                if (aux != NULL) {
                    printf(" -> "); // setinha pra ficar formatado no output conforme o arquivo de texto do trabalho
                }
            }
            printf(" -> NULL \n"); // sempre fecha a lista com "-> NULL" pra demonstrar o fim
        }
    }
}
int main(){
    struct tlist* vet[M]={NULL};
    char linha[69420];
    int key,index;
    FILE *arquivo;
    arquivo = fopen("casos_de_teste.txt", "r"); //abrindo aqui o arquivo de texto em modo de leitura (r)
    if (arquivo==NULL){
        perror("arquivo vazio"); // coloquei o perror (que eu descobri agora que existe) pra descobrir porque meu arquivo não estava abrindo
    } else {
        //printf("oi\n"); tinha colocado essa linha aqui pra saber se consegui corrigir o erro de o arquivo sempre estar NULL.
        //resolvi deixar porque achei divertido
        if(fgets(linha,sizeof(linha),arquivo)!=NULL){ //verifica se é possivel ler uma linha do arquivo
            char *token=strtok(linha,";"); //; como ponto de quebra de linha
            while(token!=NULL){
                key=atoi(token); //usando a funçao ascii to integer pra transformar o char obtido em um numero inteiro
                struct tlist* newNode = criar_node(key);
                index=obtem_indice(newNode);
                tabela_hash(newNode,vet,index); //insere o node criado na função criar_node em uma posição index da tabela hash
                token=strtok(NULL,";"); //pula pro próximo número
            }
            imprimir_tabela_hash(vet); //imprime a tabela hash
        }
    }
    fclose(arquivo); //fecha o casos_de_teste.txt
    return 0;
}