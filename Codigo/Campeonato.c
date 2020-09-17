#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Campeonato.h"
#include "Corrida.h"



int campeonato(piloto pilotos[], int nr_pilotos, carro carros[], int nr_carros){
    ppontuacao pontuacoes = NULL;
    pequipa equipas = NULL;
    int nr_provas;
    int menu, sair=0, i;
    int ok;

    FILE * f;

    f = fopen("campeonatosave.dat", "rb");
    if(f == NULL){
        do{
            printf("\n\nCONFIGURACAO DE CAMPEONATO:");
            printf("\nIndique o numero de provas desejadas, um inteiro entre 3 e 8: ");
            scanf(" %d", &nr_provas);
        } while(nr_provas < 3 || nr_provas > 8);

        pontuacoes = criaPontuacoes(pilotos, nr_pilotos);
        if(pontuacoes == NULL)
            return -1;
    }
    else{
        pontuacoes = recuperaCampeonato(&nr_provas);
        if(pontuacoes == NULL)
            return -1;
    }

    i=0;
    do{
        printf("\n\nMENU CAMPEONATO:");
        printf("\n0 - Sair");
        printf("\n1 - Ver Pontuacao");
        printf("\n2 - Iniciar Provas");
        printf("\n3 - Gravar Campeonato");
        printf("\nOpcao: ");
        scanf(" %d", &menu);
        switch(menu){
            case 0: sair = 1;
                    break;
            case 1: mostraPontuacoes(pontuacoes);
                    break;
            case 2: if(i<nr_provas){
                        printf("\n\nCAMPEONATO:");
                        printf("\nNumero de Provas Restantes: %d", nr_provas - i);

                        equipas = corrida(equipas, pilotos, nr_pilotos, carros, nr_carros);
                        if(equipas == NULL){
                            return -1;
                        }

                        actualizaInfoCampeonato(pontuacoes, equipas, pilotos, nr_pilotos);

                        actualizaPontuacoes(pontuacoes, equipas);

                        //pontuacoes = ordenaPontuacoes(pontuacoes);

                        eliminaEquipas(equipas);
                        equipas = NULL;
                        i++;
                    }
                    break;
            case 3: ok = guardaCampeonato(pontuacoes, nr_provas - i, nr_pilotos);
                    if(ok != 0){
                        return -1;
                    }
                    break;
            default:
                    printf("\nIntroduza uma opcao valida!");
        }
    } while(sair == 0);

    eliminaPontuacoes(pontuacoes);

    return 0;
}



ppontuacao criaPontuacoes(piloto pilotos[], int nr_pilotos){
    ppontuacao nova, anterior, primeira;
    int i;

    for(i=0; i<nr_pilotos; i++){

        nova = malloc(sizeof(pontuacao));

        if(nova == NULL){
            printf("\n|ERRO| Falha na alocacao de memoria para pontuacoes do campeonato\n");
            return NULL;
        }
        if(i==0)
            primeira = nova;

        else
            anterior->prox = nova;

        nova->pilot.id = pilotos[i].id;
        strcpy(nova->pilot.nome, pilotos[i].nome);
        nova->pilot.dia = pilotos[i].dia;
        nova->pilot.mes = pilotos[i].mes;
        nova->pilot.ano = pilotos[i].ano;
        nova->pilot.peso = pilotos[i].peso;
        nova->pilot.xp = pilotos[i].xp;
        nova->pilot.corridas_impedido = pilotos[i].corridas_impedido;
        nova->pontos = 0.0;
        nova->corridas_feitas = 0;
        nova->prox = NULL;

        anterior = nova;
    }

    return primeira;
}



void eliminaPontuacoes(ppontuacao primeira){
    ppontuacao aux;

    while(primeira != NULL){
        aux = primeira;
        primeira = primeira->prox;
        free(aux);
    }
}



void mostraPontuacoes(ppontuacao primeira){
    int i = 1;
    printf("\nPONTUACAO DO CAMPEONATO:");
    while(primeira != NULL){
        printf("\n%d: %-20s Id(%d) Pontos:%.1f  Corridas Completadas: %d", i, primeira->pilot.nome, primeira->pilot.id, primeira->pontos, primeira->corridas_feitas);
        i++;
        primeira = primeira->prox;
    }
}



void actualizaInfoCampeonato(ppontuacao primeira, pequipa equipas, piloto pilotos[], int nr_pilotos){
    if(primeira == NULL){
        printf("\n|ERRO| Nao existe lista de pontuacoes\n");
        return;
    }

    int i;
    for(i=0; i<nr_pilotos; i++){
        while(primeira->pilot.id != pilotos[i].id)
            primeira = primeira->prox;

        if(verificaIDPiloto(equipas, primeira->pilot.id) == 0){      //verificar se participou na corrida
            primeira->corridas_feitas ++;
            primeira->pilot.corridas_impedido = pilotos[i].corridas_impedido;
            primeira->pilot.xp = pilotos[i].xp;
        }
    }
}



void actualizaPontuacoes(ppontuacao pontuacoes, pequipa primeira){
    pequipa aux;
    pvolta actual;
    int i, z, k, id, tempo, pts;
    int soma, volta_actual;
    int max_carros, total_voltas;
    float extra;

    max_carros = total_voltas = 0;
    aux = primeira;
    actual = aux->prim_volta;
    //Deduzir as voltas e nr de pilotos
    while(actual != NULL){
        total_voltas++;
        actual = actual->prox;
    }
    while(aux != NULL){
        max_carros++;
        aux = aux->prox;
    }

    //array temporário para guardar ids e tempos de forma ordenada
    int ids[3][max_carros];         //Linha 0: id | Linha 1: tempo
    float pontos[max_carros];           //array temporário para pontos acumulados
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
            pontos[i] = 0.0;
            i++;

            aux = aux->prox;
        }

        //Ordenar array temporário por tempo mais baixo
        for(i=0; i<max_carros; i++)
            for(k=i+1; k<max_carros; k++)
                if((ids[1][i] > ids[1][k] && ids[1][k] != 0) || ids[1][i] == 0){
                    tempo = ids[1][i];
                    id = ids[0][i];
                    pts = pontos[i];

                    ids[1][i] = ids[1][k];
                    ids[0][i] = ids[0][k];
                    pontos[i] = pontos[k];

                    ids[1][k] = tempo;
                    ids[0][k] = id;
                    pontos[k] = pts;
                }

        //actualiza a experiência acumulada por volta no array temporário
        pontos[0] += 0.5;                       //quem acabou em primeiro recebe 0.5
        for(i=0; i<max_carros; i++){
            if(ids[1][i] == 0)              //se o tempo for 0, então houve um acidente
                pontos[i] = 0.0;
        }
    }

    //Pontuações para os 5 primeiros classificados
    extra=5.0;
    for(i=0; i<max_carros || extra == 0.0; i++){
        pontos[i] += extra;
        extra = extra - 1.0;
    }

    //actualizar as pontuações do campeonato com os valores finais de experiência ganha
    while(pontuacoes != NULL){
        for(i=0; i<max_carros; i++)
            if(pontuacoes->pilot.id == ids[0][i]){
                pontuacoes->pontos += pontos[i];
            }
        pontuacoes = pontuacoes->prox;
    }
}


//NÃO FUNCIONAL E NÃO UTILIZADO
ppontuacao ordenaPontuacoes(ppontuacao primeira){
    ppontuacao actual, aux;

    actual = primeira;
    while(actual != NULL){
        aux = actual->prox;
        while(aux != NULL){

            if(actual->pontos < aux->pontos
            || (actual->pontos == aux->pontos && actual->corridas_feitas > aux->corridas_feitas)   //quem tiver menos corridas feitas e a mesma pontuação, tem vantagem
            || (actual->pontos == aux->pontos && actual->corridas_feitas == aux->corridas_feitas && comparaIdades(actual->pilot, aux->pilot) == 1) ){   //quem for mais velho tem vantagem

                actual->prox = aux->prox;
                aux->prox = actual;

                if(actual == primeira){
                    primeira = aux;
                    actual = primeira;
                }
            }

            aux = aux->prox;
        }

        actual = actual->prox;
    }

    return primeira;
}



int comparaIdades(piloto p1, piloto p2){
    if(p1.ano > p2.ano || (p1.ano == p2.ano && p1.mes > p2.mes) ||  (p1.ano == p2.ano && p1.mes == p2.mes && p1.dia > p2.dia) )
        return 1;       //se o p2 for o mais velho, retorna 1
    if(p1.ano < p2.ano || (p1.ano == p2.ano && p1.mes < p2.mes) ||  (p1.ano == p2.ano && p1.mes == p2.mes && p1.dia < p2.dia) )
        return -1;      //se o p2 for o mais novo, retorna -1
    else
        return 0;       //se tiverem exactamente a mesma idade, retorna 0
}



int guardaCampeonato(ppontuacao primeira, int provas_restantes, int nr_pilotos){
    FILE * f;

    f = fopen("campeonatosave.dat", "wb");

    if(f == NULL){
        printf("\n|ERRO| Falha na abertura do ficheiro campeonatosave.dat para escrita\n");
        return -1;
    }

    fwrite(&nr_pilotos, 1, sizeof(int), f);
    fwrite(&provas_restantes, 1, sizeof(int), f);

    while(primeira != NULL){
        fwrite(primeira, 1, sizeof(pontuacao), f);
        primeira = primeira->prox;
    }

    fclose(f);

    return 0;
}



ppontuacao recuperaCampeonato(int *provas_restantes){
    FILE *f;
    int i, nr_ler;
    ppontuacao primeira = NULL, nova = NULL, anterior = NULL;

    f = fopen("campeonatosave.dat", "rb");

    if(f == NULL){
        printf("\n|ERRO| Falha na abertura do ficheiro campeonatosave.dat para leitura\n");
        return NULL;
    }

    fread(&nr_ler, 1, sizeof(int), f);
    fread(provas_restantes, 1, sizeof(int), f);

    for(i=0; i<nr_ler; i++){
        nova = malloc(sizeof(pontuacao));
        if(nova == NULL){
            printf("\n|ERRO| Nao foi possivel alocar memoria para pontuacoes\n");
            return NULL;
        }

        fread(nova, 1, sizeof(pontuacao), f);
        nova->prox = NULL;

        if(i==0){
            primeira = nova;
            anterior = nova;
        }
        else{
            anterior->prox = nova;
            anterior = nova;
        }
    }

    fclose(f);

    if(remove("campeonatosave.dat") != 0){
        printf("\n|ERRO| Nao foi possivel eliminar o ficheiro campeonatosave.dat\n");
    }

    return primeira;
}
