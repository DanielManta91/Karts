/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Volta.c
 * Author: Daniel
 *
 * Created on 22 de Maio de 2019, 17:33
 */

#include <stdio.h>
#include <stdlib.h>

#include "Volta.h"


pvolta adicionaVolta(pvolta v){
    pvolta nova;

    nova = malloc(sizeof(volta));

    if(nova == NULL){
        printf("\n|ERRO| Falha na alocacao de memoria para uma nova volta\n");
        return v;
    }

    nova->tempo = 0;
    nova->prox = NULL;

    if(v == NULL){
        return nova;
    }

    pvolta aux;
    aux = v;

    while(aux->prox != NULL)
        aux = aux->prox;

    aux->prox = nova;

    return v;
}



void eliminaVoltas(pvolta primeira){
    pvolta aux;

    while(primeira != NULL){
        aux = primeira;
        primeira = primeira->prox;
        free(aux);
    }
}
