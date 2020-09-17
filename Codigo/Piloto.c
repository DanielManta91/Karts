/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include "Piloto.h"

ppiloto lerPilotosFichTexto(int *tam, char *filename){
    FILE * f;
    ppiloto pilotos;

    f = fopen(filename, "r");

    if(f == NULL){
        printf("\n|ERRO| Nao foi possivel abrir o ficheiro %s para leitura\n", filename);
        return NULL;
    }

    (*tam)=0;

    char nome[200];
    int id, dia, mes, ano, peso, impedido;
    float xp;
    int nr_scans;

    while(fscanf(f, " %200[^\n]", nome) == 1 &&                     //descobrir o nr de estruturas
          (nr_scans = fscanf(f, " %d %d %d %d %d %f %d", &id, &dia, &mes, &ano, &peso, &xp, &impedido)) ){
        if(id < 0){
            printf("\n|ERRO| ID de %s tem de ser um valor inteiro positivo\n", nome);
            fclose(f);
            return NULL;
        }
        if(dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900){
            printf("\n|ERRO| Data de Nascimento de %s tem de ser no formato: dd mm aa\n", nome);
            fclose(f);
            return NULL;
        }
        if(peso < 1){
            printf("\n|ERRO| Peso de %s tem de ser um valor inteiro positivo\n", nome);
            fclose(f);
            return NULL;
        }
        if(xp < 0.0){
            printf("\n|ERRO| Experience de %s tem de ser um valor real maior ou igual que 0.0\n", nome);
            fclose(f);
            return NULL;
        }
        if(impedido < 0){
            printf("\n|ERRO| O nr de corridas impedido de %s tem de ser um valor inteiro positivo\n", nome);
            fclose(f);
            return NULL;
        }

        (*tam)+=1;
    }

    if((*tam) == 0){
        printf("\n|ERRO| O ficheiro %s esta vazio\n", filename);
        fclose(f);
        return NULL;
    }

    if(nr_scans > 0 && nr_scans < 7){
        printf("\n|ERRO| O ficheiro %s nao tem o nr adequado de atributos para os pilotos\n", filename);
        fclose(f);
        return NULL;
    }


    pilotos = malloc(sizeof(piloto) * (*tam) );

    if(pilotos == NULL){
        printf("\n|ERRO| Falha na alocacao de memoria para os pilotos\n");
        fclose(f);
        return NULL;
    }

    fseek(f, 0, SEEK_SET);

    int i;
    int id_temp[(*tam)];
    for(i=0; i<(*tam); i++){
        fscanf(f, " %200[^\n]", pilotos[i].nome);
        fscanf(f, " %d %d %d %d %d %f %d", &pilotos[i].id, &pilotos[i].dia, &pilotos[i].mes, &pilotos[i].ano, &pilotos[i].peso, &pilotos[i].xp, &pilotos[i].corridas_impedido);
        id_temp[i] = pilotos[i].id;
        //printf("\n%s %d %d %d %d %d %.1f %d", pilotos[i].nome, pilotos[i].id, pilotos[i].dia, pilotos[i].mes, pilotos[i].ano, pilotos[i].peso, pilotos[i].xp, pilotos[i].corridas_impedido);
    }

    int k;
    for(i=0; i<(*tam); i++){
        for(k=i+1; k<(*tam); k++)
            if(id_temp[i]==pilotos[k].id){
                printf("\n|ERRO| %s e %s possuem IDs repetidos! O valor deve ser unico entre todos os pilotos\n", pilotos[i].nome, pilotos[k].nome);
                free(pilotos);
                fclose(f);
                return NULL;
            }
    }

    fclose(f);

    return pilotos;
}



int escreverPilotosFichTexto(ppiloto pilotos, int tam, char *filename){
    if(pilotos == NULL || tam == 0){
        printf("\n|ERRO| Array de pilotos encontra-se vazio\n");
        return -1;
    }

    FILE * f;

    f = fopen(filename, "w");

    if(f == NULL){
        printf("\n|ERRO| Nao foi possivel abrir o ficheiro %s para escrita\n", filename);
        return -1;
    }

    int i;
    for(i=0; i<tam; i++){
        fprintf(f, "%s", pilotos[i].nome);
        fprintf(f, "\n");
        fprintf(f, "%d %d %d %d %d %.1f %d", pilotos[i].id, pilotos[i].dia, pilotos[i].mes, pilotos[i].ano, pilotos[i].peso, pilotos[i].xp, pilotos[i].corridas_impedido);
        fprintf(f, "\n\n");
    }

    fclose(f);

    return 0;
}



void mostraPilotos(piloto pilotos[], int nr_pilotos){
    int i;
    printf("\nPILOTOS EM MEMORIA:");
    for(i=0; i<nr_pilotos; i++)
        printf("\n%s    Id(%d) Data: %d de %d de %d Peso(%d) Exp(%.1f) KO(%d)", pilotos[i].nome, pilotos[i].id, pilotos[i].dia, pilotos[i].mes, pilotos[i].ano, pilotos[i].peso, pilotos[i].xp, pilotos[i].corridas_impedido);
}



int disciplinaPiloto(int id, int nr_corridas, piloto pilotos[], int nr_pilotos){
    int i;

    if(nr_corridas < 1 || nr_corridas > 3){
        printf("\nNumero de corridas a penalizar invalido, tem de ser um inteiro entre 1 e 3!");
        return 1;
    }

    for(i=0; i<nr_pilotos; i++)
        if(pilotos[i].id == id){
            pilotos[i].corridas_impedido += nr_corridas;
            return 0;
        }

    printf("\nID de piloto escolhido invalido!");
    return 1;
}
