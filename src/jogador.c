#include "../include/utils.h"
#include "../include/jogador.h"
#include "../include/inimigo.h"
#include "../include/npc.h"
#include "../include/estruturas.h"

Jogador jogador = {TELA_LARGURA / 2 - 25, TELA_ALTURA / 2 - 25, 100, 100};
Inimigo inimigo;

bool iniciaJogador(Jogador *jogador, int selecao)
{
    jogador->vida = 1;
    jogador->pontos = 0;
    jogador->movDireita = false;
    jogador->viradoParaEsquerda = 0;
    jogador->movEsquerda = false;
    jogador->pulando = true;
    jogador->x = TELA_LARGURA / 2 - jogador->w;
    jogador->scrollX = 0;
    jogador->y = TELA_ALTURA / 2 - jogador->h;
    jogador->velocidadeY = 0;
    jogador->resgatando = false;
    jogador->imune = false;
    const char *personagem;

    switch (selecao)
    {
    case 0:
        personagem = "Foxy";
        jogador->num_idle = 4;
        jogador->num_run = 6;
        jogador->num_jump = 2;
        jogador->velocidade_movimento = 1000;
        jogador->forca_salto = -80;

        break;
    case 1:
        personagem = "Squirrel";
        jogador->num_idle = 8;
        jogador->num_run = 6;
        jogador->num_jump = 4;
        jogador->velocidade_movimento = 1000;
        jogador->forca_salto = -60;

        break;
    default:
        printf("Seleção de personagem inválida.\n");
        return false;
    }

    char caminho[256];

    idle = (SDL_Texture **)malloc(jogador->num_idle * sizeof(SDL_Texture *));
    run = (SDL_Texture **)malloc(jogador->num_run * sizeof(SDL_Texture *));
    jump = (SDL_Texture **)malloc(jogador->num_jump * sizeof(SDL_Texture *));

    if (!idle || !run || !jump)
    {
        printf("Erro na alocação de memória para as texturas\n");
        return false;
    }

    // Carregando texturas idle
    for (int i = 0; i < jogador->num_idle; i++)
    {
        snprintf(caminho, sizeof(caminho), "assets/img/Characters/Players/%s/Sprites/idle/player-idle-%d.png", personagem, i + 1);
        idle[i] = IMG_LoadTexture(renderizador, caminho);
        if (idle[i] == NULL)
        {
            printf("Erro ao carregar textura idle %d para %s: %s\n", i + 1, personagem, IMG_GetError());
            return false;
        }
    }

    // Carregando texturas run
    for (int i = 0; i < jogador->num_run; i++)
    {
        snprintf(caminho, sizeof(caminho), "assets/img/Characters/Players/%s/Sprites/run/player-run-%d.png", personagem, i + 1);
        run[i] = IMG_LoadTexture(renderizador, caminho);
        if (run[i] == NULL)
        {
            printf("Erro ao carregar textura run %d para %s: %s\n", i + 1, personagem, IMG_GetError());
            return false;
        }
    }

    // Carregando texturas jump
    for (int i = 0; i < jogador->num_jump; i++)
    {
        snprintf(caminho, sizeof(caminho), "assets/img/Characters/Players/%s/Sprites/jump/player-jump-%d.png", personagem, i + 1);
        jump[i] = IMG_LoadTexture(renderizador, caminho);
        if (jump[i] == NULL)
        {
            printf("Erro ao carregar textura jump %d para %s: %s\n", i + 1, personagem, IMG_GetError());
            return false;
        }
    }

    return true;
}

void atualizaJogador(Jogador *jogador)
{
    // Movimento horizontal
    if (jogador->movDireita)
    {
        jogador->x += jogador->velocidade_movimento * deltaTime;
        jogador->viradoParaEsquerda = 0;
    }
    if (jogador->movEsquerda)
    {
        jogador->x -= jogador->velocidade_movimento * deltaTime;
        jogador->viradoParaEsquerda = 1;
    }
    jogador->scrollX = -jogador->x + TELA_LARGURA / 2 - jogador->w;
    if (jogador->scrollX > 0)
    {
        jogador->scrollX = 0;
    }

    // Pulo e queda
    if (jogador->pulando || !jogador->nochao)
    {
        jogador->y += jogador->velocidadeY;
        jogador->velocidadeY += GRAVIDADE;

        if (jogador->y >= TELA_ALTURA - jogador->h - 50)
        {
            jogador->y = TELA_ALTURA - jogador->h - 50;
            jogador->pulando = false;
            jogador->nochao = true;
            jogador->velocidadeY = 0;
        }
    }
}

void desenhaJogador(Jogador *jogador, SDL_Texture **idle, SDL_Texture **run, SDL_Texture **jump)
{
    SDL_Rect rectJogador = {jogador->x + jogador->scrollX, jogador->y, jogador->w, jogador->h};

    SDL_Texture *texturaAtual;
    if (jogador->pulando || !jogador->nochao)
    {
        texturaAtual = (jogador->velocidadeY > 0) ? jump[1] : jump[0];
    }
    else if (jogador->movDireita || jogador->movEsquerda)
    {
        texturaAtual = run[SDL_GetTicks() / 150 % 6];
    }
    else
    {
        texturaAtual = idle[SDL_GetTicks() / 300 % 4];
    }

    jogador->viradoParaEsquerda ? SDL_RenderCopyEx(renderizador, texturaAtual, NULL, &rectJogador, 0, NULL, SDL_FLIP_HORIZONTAL) : SDL_RenderCopy(renderizador, texturaAtual, NULL, &rectJogador);
}

void liberaJogador(Jogador *jogador)
{
    for (int i = 0; i < jogador->num_idle; i++)
    {
        SDL_DestroyTexture(idle[i]);
    }
    free(idle);

    for (int i = 0; i < jogador->num_run; i++)
    {
        SDL_DestroyTexture(run[i]);
    }
    free(run);

    for (int i = 0; i < jogador->num_jump; i++)
    {
        SDL_DestroyTexture(jump[i]);
    }
    free(jump);
}
