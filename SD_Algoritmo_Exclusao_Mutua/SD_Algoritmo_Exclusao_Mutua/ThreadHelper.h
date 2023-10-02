#pragma once
#include "Bully.h"

class CThreadHelper
{
public:
    static void InactiveCoordenador(CBully& bully); // a cada 1 minuto o coordenador morre
    static void CreateNewProcess   (CBully& bully); // a cada 40 segundos um novo processo deve ser criado (ID randômico)
};
