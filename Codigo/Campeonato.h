#ifndef CAMPEONATO_H
#define CAMPEONATO_H
#include "Carro.h"
#include "Piloto.h"
#include "Equipa.h"

typedef struct p pontuacao, *ppontuacao;

struct p{
    float pontos;
    int corridas_feitas;
    piloto pilot;
    ppontuacao prox;
};

int campeonato(piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros);

ppontuacao criaPontuacoes(piloto pilotos[], int nr_pilotos);

void mostraPontuacoes(ppontuacao primeira);

void actualizaPontuacoes(ppontuacao pontuacoes, pequipa primeira);

void actualizaInfoCampeonato(ppontuacao primeira, pequipa equipas, piloto pilotos[], int nr_pilotos);

ppontuacao ordenaPontuacoes(ppontuacao primeira);

int comparaIdades(piloto p1, piloto p2);

void eliminaPontuacoes(ppontuacao primeira);

int guardaCampeonato(ppontuacao primeira, int provas_restantes, int nr_pilotos);

ppontuacao recuperaCampeonato(int *provas_restantes);

#endif // CAMPEONATO_H
