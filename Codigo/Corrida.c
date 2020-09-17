#include <stdio.h>
#include <stdlib.h>
#include "Corrida.h"
#include "Equipa.h"
#include "Volta.h"
#include "utils.h"

pequipa corrida(pequipa primeira, piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros){
    int total_voltas, comp_pista, max_carros;
    int menu, sair = 0,i;
    int volta_escolhida;
    pequipa nova = NULL;

    do{
        printf("\nIndique o numero de voltas desejado, um inteiro entre %d e %d: ", MIN_VOLTAS, MAX_VOLTAS);
        scanf(" %d", &total_voltas);

    } while(total_voltas < MIN_VOLTAS || total_voltas > MAX_VOLTAS);

    do{
        printf("\nIndique o comprimento de pista desejado, um inteiro entre %d e %d: ", MIN_COMP_PISTA, MAX_COMP_PISTA);
        scanf(" %d", &comp_pista);

    } while(comp_pista < MIN_COMP_PISTA || comp_pista > MAX_COMP_PISTA);

    do{
        printf("\nIndique o numero de carros desejado para a corrida, um inteiro superior a 0: ");
        scanf(" %d", &max_carros);

    } while(max_carros <= 0);


    primeira = criaEquipa(primeira, &max_carros, total_voltas, pilotos, nr_pilotos, carros, nr_carros);
    if(primeira == NULL)
        return NULL;

    for(i=1; i<max_carros; i++){
        nova = criaEquipa(primeira, &max_carros, total_voltas, pilotos, nr_pilotos, carros, nr_carros);
        if(nova == NULL)
            return NULL;

        primeira = adicionaEquipa(primeira, nova);
        if(primeira == NULL)
            return NULL;
    }

    reduzImpedimentos(pilotos, nr_pilotos, carros, nr_carros);

    mostraEquipas(primeira);
    printf("\n");
    mostraNaoEscolhidos(primeira, pilotos, nr_pilotos, carros, nr_carros);

    fazCorrida(primeira, comp_pista);

    actualizaAcidentes(primeira, pilotos, nr_pilotos, carros, nr_carros);

    actualizaXP(primeira, pilotos, nr_pilotos, max_carros, total_voltas);


    printf("\n\nCLASSIFICACOES:");
    for(i=1; i<=total_voltas; i++){
        printf("\nVolta %d:", i);
        mostraClassificacao(primeira, i, max_carros);
        espera(5);
    }

    printf("\n\nCLASSIFICACAO FINAL:");
    mostraClassificacao(primeira, total_voltas, max_carros);

    do{
        printf("\n\nMENU CORRIDA:");
        printf("\n0 - Sair");
        printf("\n1 - Rever Classificacoes Globais");
        printf("\n2 - Rever Classificacao de uma volta");
        printf("\nOpcao: ");
        scanf(" %d", &menu);
        switch(menu){
            case 0: sair = 1;
                    break;
            case 1: printf("\n\nCLASSIFICACOES GLOBAIS:");
                    for(i=1; i<=total_voltas; i++){
                        printf("\nVolta %d:", i);
                        mostraClassificacao(primeira, i, max_carros);
                    }
                    break;
            case 2: do{
                        printf("\n\nEscolha uma volta entre 1 e %d: ", total_voltas);
                        scanf(" %d", &volta_escolhida);
                    } while(volta_escolhida < 1 || volta_escolhida > total_voltas);
                    printf("\nCLASSIFICACAO NA VOLTA %d:", volta_escolhida);
                    mostraClassificacao(primeira, volta_escolhida, max_carros);
                    break;
            default:
                    printf("\nIntroduza uma opcao valida!");
        }
    } while(sair == 0);

    return primeira;
}



void fazCorrida(pequipa primeira, int comp_pista){
    int idade;
    int dia, mes, ano, hora, minuto, seg;
    pvolta anterior;
    pvolta actual;

    obtemData(&dia, &mes, &ano, &hora, &minuto, &seg);

    while(primeira != NULL){
        actual = primeira->prim_volta;
        anterior = actual;
        while(actual != NULL){
            if(actual == primeira->prim_volta){
                if(probEvento(PROB_ACIDENTE) == 0){
                idade = ano - primeira->pilot.ano;
                actual->tempo = calculaSegundos(idade, primeira->pilot.peso, primeira->pilot.xp, primeira->car.potencia, comp_pista);
                }
                else{
                    actual->tempo = 0;
                    anterior = actual;
                }
            }
            else{
                if(probEvento(PROB_ACIDENTE) == 0 && anterior->tempo > 0){
                    idade = ano - primeira->pilot.ano;
                    actual->tempo = calculaSegundos(idade, primeira->pilot.peso, primeira->pilot.xp, primeira->car.potencia, comp_pista);
                }
                else{
                    actual->tempo = 0;
                    anterior = actual;

                }
            }
            actual = actual->prox;
        }
        primeira = primeira->prox;
    }
}



void mostraTempos(pequipa primeira){
    int i;
    pvolta aux;
    printf("\n\nTEMPOS GLOBAIS:");
    while(primeira != NULL){
        printf("\nPiloto: %s    Carro: %d ", primeira->pilot.nome, primeira->car.id);
        i = 1;
        aux = primeira->prim_volta;
        while(aux != NULL){
            printf(" Volta %d: %d s", i, aux->tempo);
            i++;
            aux = aux->prox;
        }
        primeira = primeira->prox;
    }
}



void mostraClassificacao(pequipa primeira, int n_volta, int max_carros){
    pequipa aux;
    pvolta actual;
    int i, k, x1, x0;
    int volta_actual, soma;

    //array temporário para guardar ids e tempos de forma ordenada
    int ids[2][max_carros];         //Linha 0: id | Linha 1: tempo

    aux = primeira;
    i=0;
    while(aux != NULL){
        actual = aux->prim_volta;
        soma = 0;
        volta_actual = 0;
        while(actual != NULL && volta_actual < n_volta){
            if(actual->tempo == 0){
                soma = 0;
            }
            else{
                soma += actual->tempo;
            }
            actual = actual->prox;
            volta_actual++;
        }
        ids[0][i] = aux->pilot.id;
        ids[1][i] = soma;
        i++;

        aux = aux->prox;
    }

    //Ordenar array temporário por tempo mais baixo
    for(i=0; i<max_carros; i++)
        for(k=i+1; k<max_carros; k++)
            if((ids[1][i] > ids[1][k] && ids[1][k] != 0) || ids[1][i] == 0){
                x1 = ids[1][i];
                x0 = ids[0][i];

                ids[1][i] = ids[1][k];
                ids[0][i] = ids[0][k];

                ids[1][k] = x1;
                ids[0][k] = x0;
            }

    int pos;
    pos = 0;

    for(i=0; i<max_carros; i++){
        aux = primeira;
        pos++;
        while(aux->pilot.id != ids[0][i])
            aux = aux->prox;

        printf("\n%d-%s (Id:%d) / Carro %d:", pos, aux->pilot.nome, aux->pilot.id, aux->car.id);

        actual = aux->prim_volta;
        volta_actual = 0;
        while(aux != NULL && volta_actual < n_volta){
            if(actual == aux->prim_volta)
                printf(" %d ", actual->tempo);
            else
                printf("+ %d ", actual->tempo);
            actual = actual->prox;
            volta_actual++;
        }
        printf("= %d segundos", ids[1][i]);
    }
}



void reduzImpedimentos(piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros){
    int i;
    for(i=0; i<nr_pilotos; i++)
        if(pilotos[i].corridas_impedido > 0)
            pilotos[i].corridas_impedido--;

    for(i=0; i<nr_carros; i++)
        if(carros[i].corridas_avaria > 0)
            carros[i].corridas_avaria--;
}



void actualizaAcidentes(pequipa primeira, piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros){
    pequipa aux;
    pvolta actual;
    int i;
    int temp;

    aux = primeira;

    while(aux != NULL){
        temp = 0;
        actual = aux->prim_volta;
        while(actual != NULL){
            if(actual->tempo == 0){
                temp = 1;
                break;
            }
            actual = actual->prox;
        }

        if(temp == 1){
            for(i=0; i<nr_pilotos; i++){
                if(pilotos[i].id == aux->pilot.id)
                    pilotos[i].corridas_impedido += 2;
            }

            for(i=0; i<nr_carros; i++){
                if(carros[i].id == aux->car.id)
                    carros[i].corridas_avaria += 1;
            }
        }

        aux = aux->prox;
    }

}



void actualizaXP(pequipa primeira, piloto pilotos[], int nr_pilotos, int max_carros, int total_voltas){
    pequipa aux;
    pvolta actual;
    int i, z, k, id, tempo, exp;
    int soma, volta_actual;

    //array temporário para guardar ids e tempos de forma ordenada
    int ids[3][max_carros];         //Linha 0: id | Linha 1: tempo
    float xp[max_carros];           //array temporário para experiência acumulada

    for(z=0; z<total_voltas; z++){

        aux = primeira;
        i=0;
        while(aux != NULL){         //ciclo de equipas
            actual = aux->prim_volta;
            volta_actual = 0;
            soma = 0;
            while(actual != NULL && volta_actual <= total_voltas){      //ciclo de voltas de cada equipa
                if(actual->tempo == 0){
                    soma = 0;
                }
                else{
                    soma += actual->tempo;
                }
                actual = actual->prox;
                volta_actual++;
            }
            ids[0][i] = aux->pilot.id;
            ids[1][i] = soma;
            xp[i] = 0.0;
            i++;

            aux = aux->prox;
        }

        //Ordenar array temporário por tempo mais baixo
        for(i=0; i<max_carros; i++)
            for(k=i+1; k<max_carros; k++)
                if((ids[1][i] > ids[1][k] && ids[1][k] != 0) || ids[1][i] == 0){
                    tempo = ids[1][i];
                    id = ids[0][i];
                    exp = xp[i];

                    ids[1][i] = ids[1][k];
                    ids[0][i] = ids[0][k];
                    xp[i] = xp[k];

                    ids[1][k] = tempo;
                    ids[0][k] = id;
                    xp[k] = exp;
                }

        //actualiza a experiência acumulada por volta no array temporário
        xp[0] += 0.5;                       //quem acabou em primeiro recebe 0.5
        for(i=0; i<max_carros; i++){
            if(ids[1][i] == 0)              //se o tempo for 0, então houve um acidente
                xp[i] = -1.0;
        }
    }

    //actualizar o array dinâmico de pilotos com os valores finais de experiência ganha
    for(i=0; i<nr_pilotos; i++){
        for(k=0; k<max_carros; k++)
            if(pilotos[i].id == ids[0][k]){
                pilotos[i].xp += xp[k];

                if(pilotos[i].xp < 0.0)
                    pilotos[i].xp = 0.0;
            }
    }

}
