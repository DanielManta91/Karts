#include <stdio.h>
#include <stdlib.h>
#include "Carro.h"

pcarro lerCarrosFichTexto(int *tam, char *filename){
    FILE * f;
    pcarro carros;

    f = fopen(filename, "r");

    if(f == NULL){
        printf("\n|ERRO| Nao foi possivel abrir o ficheiro %s para leitura\n", filename);
        return NULL;
    }

    (*tam)=0;

    int id, potencia, avarias;
    int nr_scans;

    do{
        nr_scans = fscanf(f, " %d %d %d", &id, &potencia, &avarias);                   //descobrir o nr de estruturas
        if(id < 0){
            printf("\n|ERRO| ID do carro com ID:%d tem de ser um valor inteiro positivo\n", id);
            fclose(f);
            return NULL;
        }
        if(potencia < 0){
            printf("\n|ERRO| Potencia no carro com ID:%d tem de ser um valor inteiro positivo\n", id);
            fclose(f);
            return NULL;
        }
        if(avarias < 0){
            printf("\n|ERRO| Avarias no carro com ID:%d tem de ser um valor inteiro positivo\n", id);
            fclose(f);
            return NULL;
        }

        if(nr_scans == 3)
            (*tam)+=1;
    } while(nr_scans == 3);

    if((*tam) == 0){
        printf("\n|ERRO| O ficheiro %s esta vazio\n", filename);
        fclose(f);
        return NULL;
    }

    if(nr_scans > 0 && nr_scans < 3){
        printf("\n|ERRO| O ficheiro %s nao tem o nr adequado de atributos para os carros\n", filename);
        fclose(f);
        return NULL;
    }


    carros = malloc(sizeof(carro) * (*tam) );

    if(carros == NULL){
        printf("\n|ERRO| Falha na alocacao de memoria para os carros\n");
        fclose(f);
        return NULL;
    }

    fseek(f, 0, SEEK_SET);

    int i;
    int id_temp[(*tam)];
    for(i=0; i<(*tam); i++){
        fscanf(f, " %d %d %d", &carros[i].id, &carros[i].potencia, &carros[i].corridas_avaria);
        id_temp[i] = carros[i].id;
        //printf("\nCarro: %d %d %d", carros[i].id, carros[i].potencia, carros[i].corridas_avaria);
    }

    int k;
    for(i=0; i<(*tam); i++){
        for(k=i+1; k<(*tam); k++)
            if(id_temp[i]==carros[k].id){
                printf("\n|ERRO| Dois carros possuem IDs repetidos! O valor deve ser unico entre todos os carros\n");
                free(carros);
                fclose(f);
                return NULL;
            }
    }

    fclose(f);

    return carros;
}



int escreverCarrosFichTexto(pcarro carros, int tam, char *filename){
    if(carros == NULL || tam == 0){
        printf("\n|ERRO| Array de carros encontra-se vazio\n");
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
        fprintf(f, "%d %d %d", carros[i].id, carros[i].potencia, carros[i].corridas_avaria);
        fprintf(f, "\n\n");
    }

    fclose(f);

    return 0;
}



void mostraCarros(carro carros[], int nr_carros){
    int i;
    printf("\nCARROS EM MEMORIA:");
    for(i=0; i<nr_carros; i++)
        printf("\nCarro: %d  Potencia(%d) KO(%d)", carros[i].id, carros[i].potencia, carros[i].corridas_avaria);
}
