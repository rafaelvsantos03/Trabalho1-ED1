#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definições de structs
typedef struct unidade
{
    int tipo; // 1-Explorador, 2-Soldado
    int x, y;
    struct unidade *prox;
} Unidade;

typedef struct hUnidade
{
    Unidade *inicio;
    Unidade *final;
} HUnidade;

typedef struct Edificio
{
    int tipo; // 1-Edifício de Recursos, 2-Campo de Treinamento, 3-Laboratório de Pesquisa
    int x, y;
    struct Edificio *prox;
} Edificio;

typedef struct hEdificio
{
    Edificio *inicio;
    Edificio *final;
} HEdificio;

typedef struct alianca
{
    char nome[15];
    struct alianca *prox;
} Alianca;

typedef struct hAlianca
{
    Alianca *inicio;
    Alianca *final;
} HAlianca;

typedef struct Faccao
{
    char nome[15];
    int pontosRecursos;
    int pontosPoder;
    int x, y;
    HUnidade *proxUnidade;
    HEdificio *proxEdificio;
    HAlianca *proxAlianca;
    struct Faccao *prox;
} Faccao;

typedef struct hFaccao
{
    Faccao *inicio;
    Faccao *final;
} HFaccao;

typedef struct Mapa
{
    int **terreno;
    int largura, altura;
} Mapa;

typedef enum
{
    TERRENO_PLANO = 1,
    TERRENO_FLORESTA = 2,
    TERRENO_MONTANHA = 3
} TerrenoTipo;

// Prototipos das funções
Mapa *inicializarMapa(int largura, int altura);
void gerarMapa(Mapa *mapa);
void liberarMapa(Mapa *mapa);
void imprimeMapa(Mapa *mapa);
void gerarMapa(Mapa *mapa);
TerrenoTipo mapaObterTerreno(Mapa *mapa, int x, int y);

Faccao *alocaFaccao(char *nome);
HFaccao *criaFaccao();
void adicionarFaccao(HFaccao *hFaccao, char *nome);
void faccaoConstruirEdificio(Faccao *faccao, int tipo, int x, int y);
void adicionarUnidadeAFaccao(Faccao *faccao, int tipo, int x, int y);
void faccaoColetaRecursos(Faccao *faccao, const char *nomeFaccao, const int qtdePts);
Faccao *buscarFaccao(HFaccao *faccoes, const char *nome);
void faccaoAtk(Faccao *atacante, Faccao *defensora);
void desalocaFaccao(HFaccao *hFaccao, char *nome);
void faccaoAdicionarAlianca(HFaccao *hfaccao, const char *nomeFac1, const char *nomeFac2);
void faccaoWin(HFaccao *faccoes);
void faccaoMoveUnidade(Faccao *faccao, int novoX, int novoY);

Unidade *alocarUnidade(int tipo, int x, int y);
HUnidade *criaUnidade();
void adicionarUnidade(HUnidade *hunidade, int tipo, int x, int y);
void liberarUnidades(HUnidade *hunidade);
void removerUnidade(HUnidade *hUnidade, Unidade *unidade);

Edificio *alocarEdificio(int tipo, int x, int y);
void adicionarEdificio(Faccao *faccao, int tipo, int x, int y);
void desalocaEdificio(Edificio *edificio);
void juntarEdificio(HEdificio *ed1, HEdificio *ed2);

HAlianca *criaHAlianca();
void adicionarAlianca(HFaccao *hfaccao, char *nome);

int main()
{
    HFaccao *hfaccao = criaFaccao();      // Cria a lista de facções
    Mapa *mapa = inicializarMapa(10, 10); // Cria um mapa 10x10
    gerarMapa(mapa);                      // Gera um mapa aleatório

    // Inicializa algumas facções para o jogo
    char nome1[9], nome2[9], nome3[9];
    printf("Informe 3 nomes faccoes: ");
    scanf("%s", &nome1);
    scanf("%s", &nome2);
    scanf("%s", &nome3);

    adicionarFaccao(hfaccao, nome1);
    adicionarFaccao(hfaccao, nome2);
    adicionarFaccao(hfaccao, nome3);

    // Loop principal do jogo
    int escolha = 1;
    while (escolha != 4)
    {
        printf("Escolha uma acao:\n");
        printf("1. Mover unidade\n");
        printf("2. Construir edificio\n");
        printf("3. Atacar faccao\n");
        printf("4. Sair do jogo\n");
        printf("-> ");
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
        {
            char nomeFaccao[15];
            int novoX, novoY;
            printf("Digite o nome da faccao, X e Y: ");
            scanf("%s %d %d", nomeFaccao, &novoX, &novoY);
            Faccao *faccao = buscarFaccao(hfaccao, nomeFaccao);
            if (faccao)
            {
                faccaoMoveUnidade(faccao, novoX, novoY);
                faccaoColetaRecursos(hfaccao, nome1, 10);
            }
            break;
        }
        case 2:
        {
            char nomeFaccao[15];
            int tipo, x, y;
            printf("Digite o nome da faccao, tipo de edificio, X e Y: ");
            scanf("%s %d %d %d", nomeFaccao, &tipo, &x, &y);
            Faccao *faccao = buscarFaccao(hfaccao, nomeFaccao);
            if (faccao)
            {
                faccaoConstruirEdificio(faccao, tipo, x, y);
            }
            break;
        }
        case 3:
        {
            char nomeAtacante[15], nomeDefensora[15];
            int x, y;
            printf("Digite o nome da faccao atacante, nome da faccao defensora, X e Y: ");
            scanf("%s %s %d %d", nomeAtacante, nomeDefensora, &x, &y);
            Faccao *atacante = buscarFaccao(hfaccao, nomeAtacante);
            Faccao *defensora = buscarFaccao(hfaccao, nomeDefensora);
            if (atacante && defensora)
            {
                faccaoAtk(atacante, defensora);
            }
            break;
        }
        case 4:
        {
            printf("Saindo do jogo...\n");
            liberarMapa(mapa);
            desalocaFaccao(hfaccao, nome1);
            desalocaFaccao(hfaccao, nome2);
            desalocaFaccao(hfaccao, nome3);
        } break;
        
        default:
        {
            printf("Escolha inválida. Tente novamente.\n");
        }
        }
    }
    return 0;
}

// Funções de alocação e manipulação de structs

Faccao *alocaFaccao(char *nome)
{
    Faccao *nova = (Faccao *)malloc(sizeof(Faccao));
    if (nova)
    {
        strcpy(nova->nome, nome);
        nova->pontosRecursos = 0;
        nova->pontosPoder = 0;
        nova->x = 0;
        nova->y = 0;
        nova->proxUnidade = NULL;
        nova->proxEdificio = NULL;
        nova->proxAlianca = NULL;
        nova->prox = NULL;
    }
    return nova;
}

HFaccao *criaFaccao()
{
    HFaccao *nova = (HFaccao *)malloc(sizeof(HFaccao));
    if (nova)
    {
        nova->inicio = NULL;
        nova->final = NULL;
    }
    return nova;
}

void adicionarFaccao(HFaccao *hFaccao, char *nome)
{
    Faccao *nova = alocaFaccao(nome);
    if (nova)
    {
        if (hFaccao->inicio == NULL)
        {
            hFaccao->inicio = nova;
            hFaccao->final = nova;
        }
        else
        {
            hFaccao->final->prox = nova;
            hFaccao->final = nova;
        }
    }
}

// Funções relacionadas ao mapa
// mapa
Mapa *inicializarMapa(int largura, int altura)
{
    // Verificação de alocação de memória omitida para brevidade
    Mapa *mapa = (Mapa *)malloc(sizeof(Mapa));
    mapa->largura = largura;
    mapa->altura = altura;
    mapa->terreno = (int **)malloc(largura * sizeof(int *));
    for (int i = 0; i < largura; i++)
    {
        mapa->terreno[i] = (int *)malloc(altura * sizeof(int));
    }
    return mapa;
}

// Função para liberar a memória do mapa
void liberarMapa(Mapa *mapa)
{
    for (int i = 0; i < mapa->largura; i++)
    {
        free(mapa->terreno[i]);
    }
    free(mapa->terreno);
    free(mapa);
}

void gerarMapa(Mapa *mapa)
{
    srand(time(NULL));

    for (int i = 0; i < mapa->largura; i++)
    {
        for (int j = 0; j < mapa->altura; j++)
        {
            int tipo = TERRENO_PLANO + (rand() % 3); // Gera um número entre TERRENO_PLANO e TERRENO_MONTANHA
            mapa->terreno[i][j] = tipo;
        }
    }
}

TerrenoTipo mapaObterTerreno(Mapa *mapa, int x, int y)
{
    if (x >= 0 && x < mapa->largura && y >= 0 && y < mapa->altura)
    {
        return (TerrenoTipo)mapa->terreno[x][y];
    }
    else
    {
        return TERRENO_MONTANHA; // Retorna montanha para posições inválidas
    }
}

void imprimeMapa(Mapa *mapa)
{
    for (int i = 0; i < mapa->largura; i++)
    {
        for (int j = 0; j < mapa->altura; j++)
        {
            switch (mapa->terreno[i][j])
            {
            case TERRENO_PLANO:
                printf("P ");
                break;
            case TERRENO_FLORESTA:
                printf("F ");
                break;
            case TERRENO_MONTANHA:
                printf("M ");
                break;
            default:
                printf("? ");
                break;
            }
        }
        printf("\n");
    }
}

// Funções relacionadas a edifícios
Edificio *alocarEdificio(int tipo, int x, int y)
{
    Edificio *novo = (Edificio *)malloc(sizeof(Edificio));
    if (novo)
    {
        novo->tipo = tipo;
        novo->x = x;
        novo->y = y;
        novo->prox = NULL;
    }
    return novo;
}

void adicionarEdificio(Faccao *faccao, int tipo, int x, int y)
{
    if (faccao != NULL && faccao->proxEdificio != NULL)
    {
        Edificio *novo = alocarEdificio(tipo, x, y);
        if (novo != NULL)
        {
            novo->prox = faccao->proxEdificio->inicio;
            faccao->proxEdificio->inicio = novo;
        }
    }
}

void desalocaEdificio(Edificio *edificio)
{
    if (edificio != NULL)
    {
        free(edificio);
    }
}

void juntarEdificio(HEdificio *ed1, HEdificio *ed2)
{
    Edificio *aux = ed2->inicio;
    while (aux)
    {
        adicionarEdificio(NULL, aux->tipo, aux->x, aux->y); // Parâmetros da função corrigidos
        aux = aux->prox;
    }
}

// Funções relacionadas a alianças
HAlianca *criaHAlianca()
{
    HAlianca *nova = (HAlianca *)malloc(sizeof(HAlianca));
    if (nova)
    {
        nova->inicio = NULL;
        nova->final = NULL;
    }
    return nova;
}

void adicionarAlianca(HFaccao *hfaccao, char *nome)
{
    Alianca *novo = (Alianca *)malloc(sizeof(Alianca));
    if (novo)
    {
        strcpy(novo->nome, nome);
        novo->prox = NULL;
        if (hfaccao->inicio == NULL)
        {
            hfaccao->inicio = novo;
            hfaccao->final = novo;
        }
        else
        {
            hfaccao->final->prox = novo;
            hfaccao->final = novo;
        }
    }
}

// Funções de manipulação de facções
void faccaoConstruirEdificio(Faccao *faccao, int tipo, int x, int y)
{
    adicionarEdificio(faccao, tipo, x, y);
}

void adicionarUnidadeAFaccao(Faccao *faccao, int tipo, int x, int y)
{
    if (faccao != NULL && faccao->proxUnidade != NULL)
    {
        Unidade *nova = (Unidade *)malloc(sizeof(Unidade));
        if (nova != NULL)
        {
            nova->tipo = tipo;
            nova->x = x;
            nova->y = y;
            nova->prox = faccao->proxUnidade->inicio;
            faccao->proxUnidade->inicio = nova;
        }
    }
}

void faccaoColetaRecursos(Faccao *faccao, const char *nomeFaccao, const int qtdePts)
{
    if (faccao != NULL && strcmp(faccao->nome, nomeFaccao) == 0)
    {
        faccao->pontosRecursos += qtdePts;
    }
}

Faccao *buscarFaccao(HFaccao *faccoes, const char *nome)
{
    Faccao *aux = faccoes->inicio;
    while (aux != NULL)
    {
        if (strcmp(aux->nome, nome) == 0)
        {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

void faccaoAtk(Faccao *atacante, Faccao *defensora)
{
    if (atacante != NULL && defensora != NULL)
    {
        defensora->pontosPoder -= 10; // Exemplo de lógica de ataque
    }
}

void desalocaFaccao(HFaccao *hFaccao, char *nome)
{
    Faccao *aux = hFaccao->inicio;
    Faccao *ant = NULL;
    while (aux != NULL)
    {
        if (strcmp(aux->nome, nome) == 0)
        {
            if (ant == NULL)
            {
                hFaccao->inicio = aux->prox;
            }
            else
            {
                ant->prox = aux->prox;
            }
            free(aux);
            return;
        }
        ant = aux;
        aux = aux->prox;
    }
}

void faccaoAdicionarAlianca(HFaccao *hfaccao, const char *nomeFac1, const char *nomeFac2)
{
    Faccao *fac1 = buscarFaccao(hfaccao, nomeFac1);
    Faccao *fac2 = buscarFaccao(hfaccao, nomeFac2);
    if (fac1 != NULL && fac2 != NULL)
    {
        Alianca *novaAlianca = (Alianca *)malloc(sizeof(Alianca));
        if (novaAlianca != NULL)
        {
            strcpy(novaAlianca->nome, nomeFac2);
            novaAlianca->prox = fac1->proxAlianca->inicio;
            fac1->proxAlianca->inicio = novaAlianca;
        }
    }
}

void faccaoWin(HFaccao *faccoes)
{
    Faccao *aux = faccoes->inicio;
    Faccao *vencedor = aux;
    while (aux != NULL)
    {
        if (aux->pontosRecursos > vencedor->pontosRecursos)
        {
            vencedor = aux;
        }
        aux = aux->prox;
    }
    printf("A faccao vencedora é: %s\n", vencedor->nome);
}

void faccaoMoveUnidade(Faccao *faccao, int novoX, int novoY)
{
    if (faccao != NULL && faccao->proxUnidade != NULL)
    {
        Unidade *unidade = faccao->proxUnidade->inicio;
        if (unidade != NULL)
        {
            unidade->x = novoX;
            unidade->y = novoY;
        }
    }
}

Unidade *alocarUnidade(int tipo, int x, int y)
{
    Unidade *nova = (Unidade *)malloc(sizeof(Unidade));
    if (nova)
    {
        nova->tipo = tipo;
        nova->x = x;
        nova->y = y;
        nova->prox = NULL;
        return nova;
    }
    printf("\nFalha ao alocar memoria");
    return NULL;
}

HUnidade *criaUnidade()
{
    HUnidade *nova = (HUnidade *)malloc(sizeof(HUnidade));
    if (nova)
    {
        nova->inicio = NULL;
        nova->final = NULL;
        return nova;
    }
    return NULL;
}

void adicionarUnidade(HUnidade *hunidade, int tipo, int x, int y)
{
    Unidade *nova = alocarUnidade(tipo, x, y);
    if (nova)
    {
        nova->prox = hunidade->inicio;
        hunidade->inicio = nova;
    }
}

void moverUnidade(Unidade *unidade, int novoX, int novoY)
{
    unidade->x = novoX;
    unidade->y = novoY;
}

void removerUnidade(HUnidade *hunidade, Unidade *unidade)
{
    if (hunidade == NULL || unidade == NULL)
    {
        return;
    }
    Unidade *anterior = NULL;
    Unidade *atual = hunidade->inicio;
    while (atual != NULL)
    {
        if (atual == unidade)
        {
            if (anterior == NULL)
            {
                hunidade->inicio = atual->prox;
            }
            else
            {
                anterior->prox = atual->prox;
            }
            if (atual == hunidade->final)
            {
                hunidade->final = anterior;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

void liberarUnidades(HUnidade *hunidade)
{
    if (hunidade == NULL)
    {
        return;
    }
    Unidade *atual = hunidade->inicio;
    Unidade *prox;
    while (atual != NULL)
    {
        prox = atual->prox; // Guarda a referência para o próximo elemento
        free(atual);        // Libera a unidade atual
        atual = prox;       // Avança para o próximo elemento
    }
    free(hunidade); // Libera a lista de unidades
}
