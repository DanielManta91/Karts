/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Equipa.h"
#include "Volta.h"
#include "utils.h"

pequipa criaEquipa(pequipa primeira, int *max_carros, int nr_voltas, piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros){
    pequipa nova;
    pequipa aux;
    int i, k;
    int i_piloto;
    int i_carro;

    nova = malloc(sizeof(equipa));

    if(nova == NULL){
        printf("\n|ERRO| Falha na alocacao de memoria para uma nova equipa\n");
        return NULL;
    }

    //ciclo para descobrir quantos dos pilotos estão aptos
    int max_p = 0;
    for(i=0; i<nr_pilotos; i++){
        if(pilotos[i].corridas_impedido == 0)
            max_p++;
    }

    if(max_p == 0){
        printf("\n|ERRO| Nao existem pilotos aptos a competir\n");
        return NULL;
    }

    if(max_p < *max_carros){
        printf("\n|ERRO| Nao existem pilotos suficientes para o nr de carros desejado\n");
        do{
            printf("\nIndique o numero de carros desejado para a corrida, um inteiro superior a 0 e inferior ou igual a %d: ", max_p);
            scanf(" %d", max_carros);

        } while(*max_carros <= 0 || *max_carros > max_p);
    }
    //Array temporário para pilotos aptos
    piloto p[max_p];

    //copiar pilotos aptos
    for(i=0, k=0; i<nr_pilotos && k<max_p; i++){
        if(pilotos[i].corridas_impedido == 0){
            strcpy(p[k].nome, pilotos[i].nome);
            p[k].id = pilotos[i].id;
            p[k].peso = pilotos[i].peso;
            p[k].xp = pilotos[i].xp;
            k++;
        }
    }

    //ciclo para descobrir quantos dos carros estão prontos
    int max_c = 0;
    for(i=0; i<nr_carros; i++){
        if(carros[i].corridas_avaria == 0)
            max_c++;
    }

    if(max_c == 0){
        printf("\n|Erro| Nao existem carros prontos para competir\n");
        return NULL;
    }

    if(max_c < *max_carros){
        printf("\n|ERRO| Nao existem carros suficientes para o nr de carros desejado\n");
        do{
            printf("\nIndique o numero de carros desejado para a corrida, um inteiro superior a 0 e inferior ou igual a %d: ", max_c);
            scanf(" %d", max_carros);

        } while(*max_carros <= 0 || *max_carros > max_c);
    }
    //Array temporário para carros prontos
    carro c[max_c];

    //copiar carros prontos
    for(i=0, k=0; i<nr_carros && k<max_c; i++){
        if(carros[i].corridas_avaria == 0){
            c[k].id = carros[i].id;
            c[k].potencia = carros[i].potencia;
            k++;
        }
    }

    initRandom();

    if(primeira == NULL){       //Se for a primeira equipa a ser criada, gera valores aleatórios para usar como índices, 1 vez
        i_piloto = intUniformRnd(0, max_p - 1);
        i_carro = intUniformRnd(0, max_c - 1);
    }
    else{                    //não sendo a primeira equipa, é preciso verificar que não se repetem membros
        aux = primeira;
        i_piloto = intUniformRnd(0, max_p - 1);
        while(aux != NULL){
            if(p[i_piloto].id == aux->pilot.id){
                i_piloto = intUniformRnd(0, max_p - 1);
                aux = primeira;                       //volta ao início da lista
            }
            else
                aux = aux->prox;
        }

        aux = primeira;                               //volta ao início da lista
        i_carro = intUniformRnd(0, max_c - 1);
        while(aux != NULL){
            if(c[i_carro].id == aux->car.id){
                i_carro = intUniformRnd(0, max_c - 1);
                aux = primeira;
            }
            else
                aux = aux->prox;
        }
    }

    strcpy(nova->pilot.nome, p[i_piloto].nome);
    nova->pilot.id = p[i_piloto].id;
    nova->pilot.peso = p[i_piloto].peso;
    nova->pilot.xp = p[i_piloto].xp;

    nova->car.id = c[i_carro].id;
    nova->car.potencia = c[i_carro].potencia;
    nova->prim_volta = NULL;

    for(i=0; i<nr_voltas; i++)
        nova->prim_volta = adicionaVolta(nova->prim_volta);

    nova->prox = NULL;

    return nova;
}



pequipa adicionaEquipa(pequipa primeira, pequipa nova){
    pequipa aux;
    aux = primeira;

    while(aux->prox != NULL)
        aux = aux->prox;

    aux->prox = nova;

    return primeira;
}



void eliminaEquipas(pequipa primeira){
    pequipa aux;

    while(primeira != NULL){
        aux = primeira;
        primeira = primeira->prox;
        eliminaVoltas(aux->prim_volta);
        free(aux);
    }
}



void mostraEquipas(pequipa primeira){
    printf("\nEQUIPAS ACTUAIS:");
    while(primeira != NULL){
        printf("\nPiloto: %s    Carro: %d", primeira->pilot.nome, primeira->car.id);
        primeira = primeira->prox;
    }
}



void mostraNaoEscolhidos(pequipa primeira, piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros){
    int i;

    for(i=0; i<nr_pilotos; i++){
        if(verificaIDPiloto(primeira, pilotos[i].id) != 0){
            if(pilotos[i].corridas_impedido > 0)
                printf("\nPiloto %s de ID %d nao escolhido. Motivo: Impedido por %d corrida(s)", pilotos[i].nome, pilotos[i].id, pilotos[i].corridas_impedido);
            else
                printf("\nPiloto %s de ID %d nao escolhido. Motivo: Azar", pilotos[i].nome, pilotos[i].id);
        }
    }

    for(i=0; i<nr_carros; i++){
        if(verificaIDCarro(primeira, carros[i].id) != 0){
            if(carros[i].corridas_avaria != 0)
                printf("\nCarro de ID %d nao escolhido. Motivo: Avariado por %d corrida(s)", carros[i].id, carros[i].corridas_avaria);
            else
                printf("\nCarro de ID %d nao escolhido. Motivo: Azar", carros[i].id);
        }
    }
}



int verificaIDPiloto(pequipa primeira, int id){       //verifica se existe uma equipa com este ID de piloto
    while(primeira != NULL){
        if(primeira->pilot.id == id)
            return 0;                           //se já existe, retorna 0

        primeira = primeira->prox;
    }

    return 1;                                   //se não existe, retorna 1
}



int verificaIDCarro(pequipa primeira, int id){       //verifica se existe uma equipa com este ID de carro
    while(primeira != NULL){
        if(primeira->car.id == id)
            return 0;                           //se já existe, retorna 0

        primeira = primeira->prox;
    }

    return 1;                                   //se não existe, retorna 1
}
