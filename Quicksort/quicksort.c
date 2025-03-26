#include <stdio.h>
#include <stdlib.h>
struct tlist {
    int valor;
    struct tlist* next;
};

struct tqueue {
    int valor;
    struct tqueue* next;
};

////////////////////////// 0 /////////////////////////
    //cabeçalho das funções
void build_list(int* vet, int num, struct tlist** head);
void list_to_vector_of_nodes(struct tlist* head, struct tlist** vet, int num);
int particiona(struct tlist** vet, int inicio,int fim);
void quicksort(struct tlist**, int inicio, int fim);
void vector_to_queue(struct tlist** vet, struct tqueue** head, int num);
void print_node_vector(struct tlist** vet, int num);
void print_list(struct tlist* head);
void print_queue(struct tqueue* head);

////////////////////////// 1 /////////////////////////
    // 1- transformar o vetor de tamanho NUM_ELEM em lista
void build_list(int* vet, int num, struct tlist** head) {
    if (vet == NULL|| num==0){
        /*quanto ao num==0, eu encontrava problemas quando usava somente a condição de 
        vet/head==NULL e isso me deu uma dor de cabeça absurda, acredito que foi porque
        posicionei alguns else que acabei retirando quando fui tornar o código mais
        legível. por questão de isso ter resolvido o meu problema, decidi deixar aqui
        como uma verificação a mais (talvez inútil, agora que não tem mais os else)*/
        printf("Vetor vazio!\n");
        return;
    }else{
        *head = (struct tlist*)malloc(sizeof(struct tlist));
        (*head)->valor = vet[0];
        (*head)->next = NULL;
        int i = 1;
        struct tlist* aux = *head; // marca o fim da lista pra manter a ordem do vetor original
        while (i < num) {
            struct tlist* new_Node = (struct tlist*)malloc(sizeof(struct tlist));
            new_Node->valor = vet[i];
            new_Node->next = NULL;
            aux->next = new_Node;
            aux = new_Node;
            i++;
        }
    }
}

////////////////////////// 2 /////////////////////////
    //2- transformar a lista em um vetor de nodes com next==NULL
void list_to_vector_of_nodes(struct tlist* head, struct tlist** vet, int num) {
    if(head==NULL|| num==0){
        printf("Lista Vazia!\n");
        return;
    }
    struct tlist* current = head;
    int i = 0;
    while (i < num) { //inicializa vet[i] com o node tlist
        vet[i] = current;
        current = current->next;
        vet[i]->next = NULL; //atualiza o campo next para NULL, quebrando o link da lista encadeada e tornando os nodes independentes
        i++;
        }
}

////////////////////////// 3 /////////////////////////
void quicksort (struct tlist** vet, int inicio, int fim){
    if(inicio<fim){ //particiona ate que so tenha um elemento
        int PosPivo = particiona(vet,inicio,fim); //obtem a posicao do pivo
        quicksort(vet,inicio,PosPivo-1); //ordena a esquerda do pivo
        quicksort(vet,PosPivo+1,fim); //ordena a direita do pivo
    }
}
int particiona(struct tlist** vet,int inicio, int fim){
    if (vet==NULL||fim==0){
        return -1;
    }
    int PosPivo=fim; //pega o ultimo elemento como pivo
    int k=inicio,i; //inicializa k para controlar a posicao do vetor onde pode haver troca
    for(i=inicio;i<fim;i++){
        if(vet[i]->valor <= vet[PosPivo]->valor){
            //trocar vet[i] com vet[k]
            struct tlist* aux = vet[i];
            vet[i]=vet[k];
            vet[k]=aux;
            k++; // move k para a posiçao seguinte pois k-1 acabou de ser ordenado em relaçao ao pivo
        }
    }
    /*aqui para baixo nos slides tinha uma comparacao se a[k]>a[posPivo], eu tinha colocado o if aqui
    mas reparei que k já está na posição correta para trocar com o vetor quando eu saio do laço acima.
    eu não sei se isso causa problema em algum contexto porque testei alguns e todos funcionaram, mas 
    como o if não se mostrou estritamente necessário e não pareceu influenciar no funcionamento do código
    eu resolvi removê-lo.
    */
    struct tlist* aux = vet[k];
    vet[k]=vet[PosPivo];
    vet[PosPivo]=aux;
    return k;
}
////////////////////////// 4 /////////////////////////
    //4- transformar o vetor de nodes ordenado como fila
void vector_to_queue(struct tlist** vet, struct tqueue** head, int num) {
    if(vet==NULL||num==0){
        return;
    }
    int i;
    // alocação do primeiro node
    if (*head == NULL) {
        *head = (struct tqueue*)malloc(sizeof(struct tqueue));
        (*head)->valor = vet[0]->valor;
        (*head)->next = NULL;
    } struct tqueue* aux=*head;
    // alocação dos nodes seguintes
    for (i = 1; i < num; i++) {
        aux->next = (struct tqueue*)malloc(sizeof(struct tqueue)); // tava dando problema nessa linha, esqueci de alocar espaço
        aux=aux->next;
        aux->valor=vet[i]->valor;
        aux->next=NULL;
        }
}

///////////////////////// 6 /////////////////////////
    //funções auxiliares, output para facilitar a visualização do algoritmo
void print_node_vector(struct tlist** vet, int num) {
    if(vet==NULL||num==0){
        return;
    }
    int i;
    printf("Vetor de nodes: ");
    for (i = 0; i < num; i++) {
        printf("%d  ", vet[i]->valor);
    }
    printf("\n");
}

void print_list(struct tlist* head) {
    if(head==NULL){
        return;
    }
    printf("Lista: ");
    struct tlist* current = head;
    while (current != NULL) {
        printf("%d ", current->valor);
        current = current->next;
    }
    printf("\n");
}

    //5- apresentacao da fila
void print_queue(struct tqueue* head) {
    if(head==NULL){
        return;
    }
    printf("Fila: ");
    struct tqueue* current = head;
    while (current != NULL) {
        printf("%d ", current->valor);
        current = current->next;
    }
    printf("\n");
}

///////////////////////// 7 /////////////////////////
    //implementação das funções acima no main

int main() {
    int vet[] = {};
    int NUM_ELEM = sizeof(vet) / sizeof(vet[0]);

    // 1. criar lista
    struct tlist* head = NULL;
    build_list(vet, NUM_ELEM, &head);
    print_list(head);

    // 2. converter lista para vetor de nós
    struct tlist* node_vector[NUM_ELEM];
    list_to_vector_of_nodes(head, node_vector, NUM_ELEM);
    print_node_vector(node_vector, NUM_ELEM);

    // 3. ordenar vetor de nós
    quicksort(node_vector,0,NUM_ELEM-1);
    print_node_vector(node_vector, NUM_ELEM);

    // 4. converter vetor de nós ordenado em fila
    struct tqueue* head_queue = NULL;
    vector_to_queue(node_vector, &head_queue, NUM_ELEM);

    // 5. imprimir fila
    print_queue(head_queue);

    return 0;
}
