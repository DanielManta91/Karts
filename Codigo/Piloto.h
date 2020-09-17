/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Piloto.h
 * Author: Daniel
 *
 * Created on 12 de Maio de 2019, 20:57
 */

#ifndef PILOTO_H
#define PILOTO_H

typedef struct{
    char nome[100];
    int id;
    int dia, mes, ano;      //data de nascimento
    int peso;
    float xp;
    int corridas_impedido;
} piloto, *ppiloto;

ppiloto lerPilotosFichTexto(int *tam, char *filename);

int escreverPilotosFichTexto(ppiloto pilotos, int tam, char *filename);

void mostraPilotos(piloto pilotos[], int nr_pilotos);

int disciplinaPiloto(int id, int nr_corridas, piloto pilotos[], int nr_pilotos);

#endif /* PILOTO_H */

