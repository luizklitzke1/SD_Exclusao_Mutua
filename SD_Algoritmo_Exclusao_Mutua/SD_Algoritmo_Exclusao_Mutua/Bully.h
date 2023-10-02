#pragma once
#include <vector>
#include "Processo.h"

class CBully
{
public:
     CBully();
    ~CBully();

    void main();
    std::shared_ptr<CProcesso> GetRandomProcess();
    void doElection(std::shared_ptr<CProcesso> process);

public:
    bool isInElection;
    std::shared_ptr<CProcesso> m_pCoordenador;

    std::vector<std::shared_ptr<CProcesso>> m_listProcessos;
    std::vector<int> m_listIdRecursos;
};