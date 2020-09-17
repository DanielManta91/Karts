/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Corrida.h
 * Author: Daniel
 *
 * Created on 26 de Maio de 2019, 18:47
 */

#ifndef CORRIDA_H
#define CORRIDA_H
#include "Piloto.h"
#include "Carro.h"
#include "Equipa.h"

#define MIN_VOLTAS 5
#define MAX_VOLTAS 10
#define MIN_COMP_PISTA 500
#define MAX_COMP_PISTA 1000
#define PROB_ACIDENTE 0.05      //valor entre 0 e 1

pequipa corrida(pequipa primeira, piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros);

void fazCorrida(pequipa primeira, int comp_pista);

void mostraTempos(pequipa primeira);

void mostraClassificacao(pequipa primeira, int n_volta, int max_carros);

void reduzImpedimentos(piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros);

void actualizaAcidentes(pequipa primeira, piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros);

void actualizaXP(pequipa primeira, piloto pilotos[], int nr_pilotos, int max_carros, int total_voltas);

#endif /* CORRIDA_H */

