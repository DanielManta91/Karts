/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Carro.h
 * Author: Daniel
 *
 * Created on 12 de Maio de 2019, 20:57
 */

#ifndef CARRO_H
#define CARRO_H

typedef struct{
    int id;
    int potencia;
    int corridas_avaria;
} carro, *pcarro;

pcarro lerCarrosFichTexto(int *tam, char *filename);

int escreverCarrosFichTexto(pcarro carros, int tam, char *filename);

void mostraCarros(carro carros[], int nr_carros);

#endif /* CARRO_H */

