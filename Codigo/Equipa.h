/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Equipa.h
 * Author: Daniel
 *
 * Created on 22 de Maio de 2019, 18:41
 */

#ifndef EQUIPA_H
#define EQUIPA_H
#include "Piloto.h"
#include "Carro.h"
#include "Volta.h"

typedef struct e equipa, *pequipa;

struct e{
    piloto pilot;
    carro car;
    pvolta prim_volta;
    pequipa prox;
};

pequipa criaEquipa(pequipa primeira, int *max_carros, int nr_voltas, piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros);

pequipa adicionaEquipa(pequipa primeira, pequipa nova);

void eliminaEquipas(pequipa primeira);

void mostraEquipas(pequipa primeira);

void mostraNaoEscolhidos(pequipa primeira, piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros);

int verificaIDPiloto(pequipa primeira, int id);

int verificaIDCarro(pequipa primeira, int id);

#endif /* EQUIPA_H */

