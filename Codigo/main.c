/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: Daniel
 *
 * Created on 11 de Maio de 2019, 12:39
 */

#include <stdio.h>
#include <stdlib.h>
#include "Piloto.h"
#include "Carro.h"
#include "Equipa.h"
#include "Corrida.h"
#include "Campeonato.h"
/*
 *
 */
int main(int argc, char** argv) {
    ppiloto pilotos = NULL;
    int nr_pilotos;
    pcarro carros = NULL;
    int nr_carros;
    int menu, sair = 0;
    int id, nr_corridas;

    pequipa equipas = NULL;

    int ok;

    pilotos = lerPilotosFichTexto(&nr_pilotos, "pilotos.txt");

    carros = lerCarrosFichTexto(&nr_carros, "carros.txt");

    do{
        printf("\n\nMENU INICIAL: ");
        printf("\n0 - Sair");
        printf("\n1 - Visualizar Pilotos");
        printf("\n2 - Visualizar Carros");
        printf("\n3 - Corrida de Treino");
        printf("\n4 - Campeonato");
        printf("\n5 - Disciplinar Piloto");
        printf("\nOpcao: ");
        scanf(" %d", &menu);
        switch(menu){
            case 0: sair = 1;
                    break;
            case 1: mostraPilotos(pilotos, nr_pilotos);
                    break;
            case 2: mostraCarros(carros, nr_carros);
                    break;
            case 3: equipas = corrida(equipas, pilotos, nr_pilotos, carros, nr_carros);
                    if(equipas == NULL){
                        free(pilotos);
                        free(carros);
                        return -1;
                    }
                    eliminaEquipas(equipas);
                    equipas = NULL;
                    break;
            case 4: ok = campeonato(pilotos, nr_pilotos, carros, nr_carros);
                    if(ok != 0){
                        free(pilotos);
                        free(carros);
                        return -1;
                    }
                    break;
            case 5: do{
                        printf("\nIndique o ID do Piloto a punir: ");
                        scanf(" %d", &id);
                        printf("\nIndique o numero de corridas a penalizar: ");
                        scanf(" %d", &nr_corridas);
                    } while(disciplinaPiloto(id, nr_corridas, pilotos, nr_pilotos) != 0);
                    break;
            default:
                    printf("\nIntroduza uma opcao valida!");
        }
    } while(sair == 0);


    ok = escreverCarrosFichTexto(carros, nr_carros, "carros.txt");
    if(ok != 0){
        free(pilotos);
        free(carros);
        return -1;
    }

    ok = escreverPilotosFichTexto(pilotos, nr_pilotos, "pilotos.txt");
    if(ok != 0){
        free(pilotos);
        free(carros);
        return -1;
    }

    free(pilotos);
    free(carros);

    return (EXIT_SUCCESS);
}

