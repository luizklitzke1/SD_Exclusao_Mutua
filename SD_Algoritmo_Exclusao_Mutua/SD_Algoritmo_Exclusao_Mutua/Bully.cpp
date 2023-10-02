#include <thread>
#include <random>
#include "Bully.h"
#include "ThreadHelper.h"
#include "windows.h"

CBully::CBully()
{
    isInElection = false;

    //Cria alguns recursos para teste
    CRecurso recursoDummy1;
    recursoDummy1.SetId(1);
    recursoDummy1.SetIsInUse(false);
    m_listRecursos.push_back(std::make_shared<CRecurso>(recursoDummy1));

    //CRecurso recursoDummy2;
    //recursoDummy2.SetId(31);
    //recursoDummy2.SetIsInUse(false);
    //m_listRecursos.push_back(std::make_shared<CRecurso>(recursoDummy2));
    //
    //CRecurso recursoDummy3;
    //recursoDummy3.SetId(55);
    //recursoDummy3.SetIsInUse(false);
    //m_listRecursos.push_back(std::make_shared<CRecurso>(recursoDummy3));
}

CBully::~CBully()
{
}

void CBully::main()
{
    SetConsoleCP      (1252);
    SetConsoleOutputCP(1252);

    printf("ALGORITMO DE EXCLUSÃO MÚTUA - CENTRALIZADO\n");
    printf("Juntamente com: Algoritmo de eleição - Bully\n\n");

    std::thread th40(CThreadHelper::CreateNewProcess   , std::ref(*this)); // a cada 40 segundos um novo processo deve ser criado (ID randômico)
    std::thread th60(CThreadHelper::InactiveCoordenador, std::ref(*this)); // a cada 1 minuto o coordenador morre

    th40.join();
    th60.join();
}

std::shared_ptr<CProcesso> CBully::GetRandomProcess()
{
    if (m_listProcessos.size() <= 0 || (m_listProcessos.size() == 1 && m_listProcessos.at(0)->GetIsActive() == false))
        return std::shared_ptr<CProcesso>(nullptr);

    while (true)
    {
        std::random_device rd;
        std::mt19937       gen(rd());

        std::uniform_int_distribution<> dis(0, m_listProcessos.size() - 1);

        if (m_listProcessos[dis(gen)]->GetIsActive())
            return m_listProcessos[dis(gen)];
    }
}

void CBully::doElection(std::shared_ptr<CProcesso> process)
{
    if (process == nullptr || (m_pCoordenador != nullptr && m_pCoordenador->GetIsActive()))
        return;
    
    isInElection = true;
    std::shared_ptr<CProcesso> newCoordenador = process;
    bool callNewElection = false;

    for (auto& itProcess : m_listProcessos)
    {
        if (itProcess->GetIsActive() && itProcess->GetId() > newCoordenador->GetId())
        {
            newCoordenador = itProcess;
            callNewElection = true;
            break;
        }
    }

    if (newCoordenador->GetIsActive() == false)
        callNewElection = true;

    if (callNewElection)
        return doElection(newCoordenador);

    if (m_pCoordenador != nullptr)
        m_pCoordenador->SetIsCoordenador(false);

    m_pCoordenador = newCoordenador;
    m_pCoordenador->SetIsCoordenador(true);
    m_pCoordenador->SetListaRecursos(&m_listRecursos);
    printf("Novo coordenador: %d\n\n", m_pCoordenador->GetId());

    for (auto& itProcess : m_listProcessos)
        itProcess->SetCoordenador(m_pCoordenador);

    isInElection = false;
}