#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ASCII_CARACTERES 256

struct huffmantree
{
    char caractere;
    int grau;
    struct huffmantree* esq;
    struct huffmantree* dir;
};

void contador_de_graus(int* frequencias) {
    FILE* arquivo_amostra = fopen("amostra.txt", "r");
    if (arquivo_amostra==NULL) {
        perror("nao abriu o arquivo"); 
        exit(1);
    }

    char caractere;
    while ((caractere=fgetc(arquivo_amostra)) != EOF) {
        frequencias[(unsigned char)caractere]++; //intervalo de 0 a 255 pra englobar a tabela ascii inteira
        //aqui eu acho que dava pra usar signed char mas preferi o unsigned
    }

    fclose(arquivo_amostra);
}

struct huffmantree* criarnode(char caractere, int grau) {
    struct huffmantree* node=(struct huffmantree*)malloc(sizeof(struct huffmantree));
    node->caractere = caractere;
    node->grau = grau;
    node->esq = NULL;
    node->dir = NULL;
    return node;
}

void ordenar_nos(struct huffmantree* nodes[], int tamanho) {
    int i, j;
//escolhi o bubble pra implementar aqui
    for (i=0; i<tamanho-1;i++) {
        for (j = 0; j < tamanho - i - 1; j++) {
// ordena pelo grau
            if (nodes[j]->grau > nodes[j+1]->grau) {
                struct huffmantree* temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
            }
// se tiver mesmo grau ordena alfabeticamente
            else if (nodes[j]->grau==nodes[j+1]->grau && 
                     nodes[j]->caractere>nodes[j+1]->caractere) {
                struct huffmantree* temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
            }
        }
    }
}
struct huffmantree* juntar_menores(struct huffmantree* esq, struct huffmantree* dir) {
    struct huffmantree* node = (struct huffmantree*)malloc(sizeof(struct huffmantree));
    node->caractere = '\0'; // recomendação de aula, caractere para ele aparecer na primeira posição de mesmo grau
    node->grau = esq->grau+dir->grau;
    node->esq = esq;
    node->dir = dir;
    return node;
}

void construir_arvore_huffman(struct huffmantree* nodes[], int* tamanho) {
    int i;
    while (*tamanho>1) { // ordena ate a raiz
        ordenar_nos(nodes, *tamanho);
//como esta ordenado, os elementos em 0 e 1 são os menores, podemos juntar eles
        struct huffmantree* esq = nodes[0];
        struct huffmantree* dir = nodes[1];
        struct huffmantree* node_pai = juntar_menores(esq, dir);

// remover os 2 combinados da lista e deslocar o retante pra esquerda
        for (i=2; i<*tamanho; i++) {
            nodes[i-2] = nodes[i];
        }
        (*tamanho)-=2; //manter o tamanho correto da lista a cada combinação

        int pos = 0;
        while (pos<*tamanho && nodes[pos]->grau<node_pai->grau) {
            pos++; // acha onde o node pai deve ser inserido
        }

//inserção do node_pai
        for (i=*tamanho; i>pos; i--) {
            nodes[i] = nodes[i-1]; // move tudo a partir de pos para a direita
        }

        nodes[pos] = node_pai;
        (*tamanho)++;
    }
}

void gerar_codificacao(struct huffmantree* raiz, char** codigos, char* codigo_atual, int profundidade) {
//utilizei a recursividade aqui pra salvar alguns 'checkpoints' e facilitar a visualização e criação da tabela de codigos huffman
    if (raiz->esq == NULL && raiz->dir == NULL) {
// chegou em uma folha -> atribui o código gerado
        codigo_atual[profundidade] = '\0';
        /*usei strdup porque uso codigo_atual durante toda a recursao, tentei usar uma lógica
        que não envolvia o strdup e todos os códigos acabavam sendo o ultimo estado do codigo_atual*/
        codigos[(unsigned char)raiz->caractere] = strdup(codigo_atual); 
        return;
    }

// 0 para a esquerda
    codigo_atual[profundidade] = '0';
    gerar_codificacao(raiz->esq, codigos, codigo_atual, profundidade + 1);

// 1 para a direita
    codigo_atual[profundidade] = '1';
    gerar_codificacao(raiz->dir, codigos, codigo_atual, profundidade + 1);
}
void gravar_codificacao(char* codigos[]){
    FILE* arquivo_codificado = fopen("codificado.txt", "w");
    if (arquivo_codificado == NULL) {
        perror("nao abriu o arquivo");
        exit(1);
    }

    FILE* arquivo_amostra = fopen("amostra.txt", "r");
    if (arquivo_amostra == NULL) {
        perror("nao abriu o arquivo");
        exit(1);
    }

    char caractere;
    //pega cada caractere e coloca a versão codificada encontrada na variavel char codigos e escreve no arquivo codificado.txt
    while ((caractere = fgetc(arquivo_amostra)) != EOF) {
        fputs(codigos[(unsigned char)caractere], arquivo_codificado);
    }

    fclose(arquivo_amostra);
    fclose(arquivo_codificado);

}
void decodificar_codigo(struct huffmantree* nodes[]){
    char caractere;
    FILE* arquivo_decodificado = fopen("decodificado.txt", "w");
    if (arquivo_decodificado == NULL) {
        perror("nao abriu o arquivo");
        exit(1);
    }
    FILE* arquivo_codificado = fopen("codificado.txt", "r");
    if (arquivo_codificado == NULL) {
        perror("nao abriu o arquivo");
        exit(1);
    }

    struct huffmantree* raiz = nodes[0];
    struct huffmantree* atual = raiz;
    while ((caractere = fgetc(arquivo_codificado)) != EOF) {
        if (caractere == '0') {
            atual = atual->esq; //busca os caracteres à esquerda
        } else if (caractere == '1') {
            atual = atual->dir; //busca os caracteres à direita
        }

        if (atual->esq == NULL && atual->dir == NULL) {
            fputc(atual->caractere, arquivo_decodificado); //encontra o nó folha e consequentemente o caractere
            atual = raiz; // retorna pra raiz, recomeçando o processo até que não hajam mais caracteres
        }
    }

    fclose(arquivo_codificado);
    fclose(arquivo_decodificado);
}

void liberar_arvore(struct huffmantree* raiz) {
    if (raiz == NULL) return;

    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);

    free(raiz); //limpa a arvore
}

int main() {
    /*tentei adicionar uma ideia do file ser NULL como referencia pra sobrescrever os
    codificado.txt e codificado.txt para ficarem vazios caso o amostra.txt também estivesse
    e todo o codigo do main abaixo entraria no 'else', pra só executar no caso de ter
    caracteres no amostra mas eu realmente não sabia direito como fazer isso, tentei
    mas não funcionou como esperado então o código não trata esse caso, não tem
    mensagem de aviso quando o amostra.txt tá vazio, o código só não imprime nada*/
    int freq[ASCII_CARACTERES] = {0};
    contador_de_graus(freq);
    struct huffmantree* nodes[ASCII_CARACTERES];
    int tamanho = 0, i;
// cria um nó pros caracteres com frequencia maior que 0
    for (i = 0; i < ASCII_CARACTERES; i++) {
        if (freq[i] > 0) {
            nodes[tamanho++] = criarnode((char)i, freq[i]);
        }
    }
    construir_arvore_huffman(nodes, &tamanho);
//codigos de huffman
    char* codigos[ASCII_CARACTERES] = {NULL};
    char codigo_atual[ASCII_CARACTERES];
    gerar_codificacao(nodes[0], codigos, codigo_atual, 0);
// tabela de codificacao de huffman
    printf("tabela de codificacao:\n");
    for (i = 0; i < ASCII_CARACTERES; i++) {
        if (freq[i]>0) {
            printf("%c: %s\n",(char)i,codigos[i]);
        }
    }
    gravar_codificacao(codigos);
    decodificar_codigo(nodes);

//liberacao de memoria
    for (i = 0; i < ASCII_CARACTERES; i++) {
        if (codigos[i] != NULL) {
            free(codigos[i]);
        }
    }
    liberar_arvore(nodes[0]);
    return 0;
}