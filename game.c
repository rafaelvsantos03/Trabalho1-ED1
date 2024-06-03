#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Terrenos
#define PLANICIE 1
#define MONTANHA 2
#define FLORESTA 3

//Edificios
#define RECURSO 1
#define CAMPOTREINAMENTO 2
#define LABPESQUISA 3

//Unidades
#define SOLDADO 1
#define EXPLORADOR 2

typedef struct terreno{
  int recurso;
  int tipo; //1- planicie, 2- montanha, 3- floresta
}TTerreno;
typedef struct unidade{
  int x;
  int y;
  int tipo; //1- soldado, 2-explorador
  struct unidade *prox;
}TUnidade;

typedef struct edificio{
  int x;
  int y;
  int tipo; // 1-recurso, 2 campo de treinamento, 3 lab pesquisa
  struct edificio *prox;
}TEdificio;

typedef struct alianca{
  char nome[15];
  struct alianca *prox;
}TAlianca;

typedef struct faccao{
  char nome[15];
  int precurso;
  int ppoder;
  struct faccao *prox;
  TUnidade *proxu;
  TEdificio *proxe;
  TAlianca *proxa;
}TFaccao;

void inicializarMapa(TTerreno ***mapa, int r, int c){
  *mapa = (TTerreno**)malloc(r * sizeof(TTerreno*));
  if(*mapa == NULL){
    printf("Erro ao alocar memória\n");
    exit(EXIT_FAILURE);
  }
  for(int i = 0; i < r; i++){
    (*mapa)[i] = (TTerreno*)malloc(c * sizeof(TTerreno));
    if((*mapa)[i] == NULL){
      printf("Erro ao alocar memória\n");
      exit(EXIT_FAILURE);
    }
    for(int j = 0; j < c; j++){
      (*mapa)[i][j].tipo = rand() % 3 + 1;
      (*mapa)[i][j].recurso = 0;
    }
  }
}

TFaccao* inicializarFaccao() {
    TFaccao *faccaoA = (TFaccao*)malloc(sizeof(TFaccao));
    TFaccao *faccaoB = (TFaccao*)malloc(sizeof(TFaccao));
    if(faccaoA == NULL || faccaoB == NULL){
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
    strcpy(faccaoA->nome, "Faccao A");
    strcpy(faccaoB->nome, "Faccao B");
    faccaoA->precurso = 100;
    faccaoB->precurso = 100;
    faccaoA->ppoder = 0;
    faccaoB->ppoder = 0;
    faccaoA->proxu = NULL;
    faccaoB->proxu = NULL;
    faccaoA->proxe = NULL;
    faccaoB->proxe = NULL;
    faccaoA->proxa = NULL;
    faccaoB->proxa = NULL;
    faccaoA->prox = faccaoB;
    faccaoB->prox = NULL;
    return faccaoA;
}
void adicionarUnidade(TFaccao *faccao, int x, int y, int tipo){
  TUnidade *nova = (TUnidade*)malloc(sizeof(TUnidade));
  if(nova == NULL){
    printf("Erro ao alocar memoria\n");
    exit(EXIT_FAILURE);
  }
  nova->x = x;
  nova->y = y;
  nova->tipo = tipo;
  nova->prox = faccao->proxu;
  faccao->proxu = nova;
}
void removerUnidade(TFaccao *faccao, int x, int y){
  TUnidade *aux = faccao->proxu;
  TUnidade *ant = NULL;
  while(aux != NULL && (aux->x != x || aux->y != y)){
    ant = aux;
    aux = aux->prox;
  }
  if(aux == NULL){
    printf("Unidade nao encontrada\n");
    return;
  }
  if(ant == NULL){
    faccao->proxu = aux->prox;
  }else{
    ant->prox = aux->prox;
  }
  free(aux);
}
void adicionarEdificio(TFaccao *faccao, int x, int y, int tipo){
  TEdificio *nova = (TEdificio*)malloc(sizeof(TEdificio));
  if(nova == NULL){
    printf("Erro ao alocar memoria\n");
    exit(EXIT_FAILURE);
  }
  nova->x = x;
  nova->y = y;
  nova->tipo = tipo;
  nova->prox = faccao->proxe;
  faccao->proxe = nova;
}
void removerEdificio(TFaccao *faccao, int x, int y){
  TEdificio *aux = faccao->proxe;
  TEdificio *ant = NULL;
  while(aux != NULL && (aux->x != x || aux->y != y)){
    ant = aux;
    aux = aux->prox;
  }
  if(aux == NULL){
    printf("Edificio nao encontrado\n");
    return;
  }
  if(ant == NULL){
    faccao->proxe = aux->prox;
  }else{
    ant->prox = aux->prox;
  }
  free(aux);
}
void moverUnidade(TUnidade *unidade, int x, int y){
  unidade->x = x;
  unidade->y = y;
}
void coletarRecurso(TTerreno **mapa, TFaccao *faccao, int x, int y, int mapa_linha, int mapa_coluna) {
    if (x < 0 || y < 0 || x >= mapa_linha || y >= mapa_coluna){
        printf("Posição inválida\n");
        return;
    }
    if(mapa[x][y].recurso == 0){
        printf("Não há recurso nessa posição\n");
        return;
    }
    faccao->precurso += mapa[x][y].recurso;
    mapa[x][y].recurso = 0;
}

void construirEdificio(TEdificio **mapa, int x, int y, TFaccao *faccao, int tipo, int mapa_linha, int mapa_coluna){
  if(x < 0 || y < 0 || x >= mapa_linha || y >= mapa_coluna){
    printf("Posicao invalida\n");
    return;
  }
  TEdificio *atualTerreno = faccao -> proxe;
  while(atualTerreno != NULL){
    if(atualTerreno->x == x && atualTerreno->y == y){
      printf("Ja existe um edificio nessa posicao\n");
      return;
    }
    atualTerreno = atualTerreno-> prox;
  }
  int custo = 0;
  switch(tipo){
    case RECURSO:
      custo = 10;
      break;
    case CAMPOTREINAMENTO:
      custo = 20;
      break;
    case LABPESQUISA:
      custo = 30;
      break;
    default:
      printf("Tipo de edificio invalido\n");
      return;
  }
  if (faccao->precurso < custo){
    printf("Recursos insuficientes\n");
    return;
  }
  TEdificio *novo = (TEdificio*)malloc(sizeof(TEdificio));
  if(novo == NULL){
    printf("Erro ao alocar memoria\n");
    exit(EXIT_FAILURE);
  }
  novo->x = x;
  novo->y = y;
  novo->tipo = tipo;
  novo->prox = faccao->proxe;
  faccao->proxe = novo;
  faccao->precurso -= custo;
  printf("Edificio construido com sucesso\n");
}
void Combate(TUnidade *atk, TFaccao *atkf, TUnidade *def, TFaccao *deff, TTerreno *tipo) {
    printf("Combate entre %s e %s em (%d, %d).\n", atkf->nome, deff->nome, def->x, def->y);

    if (atk->tipo != def->tipo) {
        printf("As unidades estão em terrenos diferentes. O defensor tem vantagem defensiva.\n");
    }

    int attacker_damage = atk->tipo == SOLDADO ? 10 : 5;
    if (tipo->tipo == MONTANHA && atk->tipo != MONTANHA) {
        attacker_damage -= 2;
    } else if (tipo->tipo == FLORESTA && atk->tipo != FLORESTA) {
        attacker_damage += 1;
    }

    int defender_defense = def->tipo == SOLDADO ? 8 : 3; 
    if (tipo->tipo == MONTANHA && def->tipo != MONTANHA) {
        defender_defense += 2;
    } else if (tipo->tipo == FLORESTA && def->tipo != FLORESTA) {
        defender_defense -= 1;
    }

    if (attacker_damage > defender_defense) {
        printf("%s venceu o combate.\n", atkf->nome);
        removerUnidade(deff, def->x, def->y);
    } else {
        printf("%s venceu o combate.\n", deff->nome);
        removerUnidade(atkf, atk->x, atk->y);
    }
}
void aumentaPoder(TFaccao *faccao){
    int poderUnidade = 1;
    int poderEdificio = 2;
    int poderTotal = 0;
    TUnidade *auxu = faccao->proxu;
    while(auxu != NULL){
        poderTotal += poderUnidade;
        auxu = auxu->prox;
    }
    TEdificio *auxe = faccao->proxe;
    while(auxe != NULL){
        poderTotal += poderEdificio;
        auxe = auxe->prox;
    }
    faccao->ppoder = poderTotal;
}

void formaAlianca(TFaccao *faccao1, TFaccao *faccao2){
    TAlianca *nova = faccao1->proxa;
    while(nova != NULL){
        if(nova->prox == faccao2){ // Verifica se o próximo elemento da aliança aponta para faccao2
            printf("Aliança já existente\n");
            return;
        }
        nova = nova->prox;
    }
    TAlianca *novaAlianca = (TAlianca*)malloc(sizeof(TAlianca));
    if(novaAlianca == NULL){
        printf("Erro ao alocar memória para a aliança.\n");
        return;
    }
    novaAlianca->prox = faccao2; // Armazena o ponteiro para a faccao2
    novaAlianca->prox = faccao1->proxa;
    faccao1->proxa = novaAlianca;

    printf("Aliança formada entre %s e %s.\n", faccao1->nome, faccao2->nome);
}
