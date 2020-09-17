/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Volta.h
 * Author: Daniel
 *
 * Created on 22 de Maio de 2019, 17:33
 */

#ifndef VOLTA_H
#define VOLTA_H

typedef struct v volta, *pvolta;

struct v{
    int tempo;
    pvolta prox;
};

pvolta adicionaVolta(pvolta primeira);

void eliminaVoltas(pvolta primeira);

#endif /* VOLTA_H */

