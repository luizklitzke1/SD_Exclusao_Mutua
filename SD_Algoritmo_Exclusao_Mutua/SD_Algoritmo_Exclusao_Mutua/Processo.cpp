#include <thread>
#include <random>
#include <format>
#include <functional>
#include "Processo.h"

const int MIN_TIME_REQUEST = 10;
const int MAX_TIME_REQUEST = 25;

const int MIN_TIME_PROCESS = 5 ;
const int MAX_TIME_PROCESS = 15;

CProcesso::CProcesso(int id) 
    : m_listIdRecursos(nullptr)
{
    m_iId = id;

    std::thread thRequestRecurso(&CProcesso::MandaRequestRecurso, std::ref(*this));
    thRequestRecurso.join();
}

CProcesso::~CProcesso()
{
}

int CProcesso::GetId()
{
    return m_iId;
}

bool CProcesso::GetIsActive()
{
    return m_bIsActive;
}

void CProcesso::SetIsActive(bool isActive)
{
    m_bIsActive = isActive;
}

bool CProcesso::GetIsCoordenador()
{
    return m_bIsCoordenador;
}

void CProcesso::SetIsCoordenador(bool isCoordenador)
{
    m_bIsCoordenador = isCoordenador;
}

std::shared_ptr<CProcesso> CProcesso::GetCoordenador()
{
    return m_pCoordenador;
}

void CProcesso::SetCoordenador(std::shared_ptr<CProcesso> coordenador)
{
    m_pCoordenador = coordenador;
}

void CProcesso::MandaRequestRecurso()
{
    while (true)
    {
        //os processos tentam consumir o(s) recurso(s) num intervalo de 10 a 25 segundos
        std::this_thread::sleep_for(std::chrono::seconds(rand() % (MAX_TIME_REQUEST - MIN_TIME_REQUEST + 1) + MIN_TIME_REQUEST));

        CRequest request;
        request.SetProcesso(this);
        request.SetIdRecurso(GetCoordenador()->GetIdRecursoAleatorio());
        
        GetCoordenador()->RequisitaRecurso(request);
    }
}

void CProcesso::ConsomeRecurso(int iRecurso)
{
    int iSegundosConsumo = rand() % (MAX_TIME_PROCESS - MIN_TIME_PROCESS + 1) + MIN_TIME_PROCESS;
    //o tempo de processamento de um recurso é de 5 a 15 segundos
    std::this_thread::sleep_for(std::chrono::seconds(iSegundosConsumo));

    CRequest requestLiberacao;
    requestLiberacao.SetProcesso (this);
    requestLiberacao.SetIdRecurso(iRecurso);

    std::string sMensagemRequestLiberacao = std::format("Recurso {}, consumido pelo processo {} durante {} segundos\n", iRecurso, GetId(), iSegundosConsumo);
    printf(sMensagemRequestLiberacao.c_str());

    GetCoordenador()->LiberaRecurso(requestLiberacao);
}

void CProcesso::SetListaRecursos(std::vector<int>* listaRecursos)
{
    m_listIdRecursos = listaRecursos;
}

void CProcesso::RequisitaRecurso(const CRequest& request)
{
    if (GetIsCoordenador() == false)
        return;

    std::string sMensagemRequest = std::format("Requisição do recursos de Id {}, feita pelo processo de Id {}\n", request.GetIdRecurso(), request.GetProcesso()->GetId());
    printf(sMensagemRequest.c_str());

    if (m_mapFilasRecursos[request.GetIdRecurso()].size())
    {
        m_mapFilasRecursos[request.GetIdRecurso()].push(request);

        std::string sAvisoFila = std::format("Adicionada request do processo {} na fila do recurso {}\n", request.GetProcesso()->GetId(), request.GetIdRecurso());
        printf(sAvisoFila.c_str());

        return;
    }

    request.GetProcesso()->ConsomeRecurso(request.GetIdRecurso());

    return;
}

void CProcesso::LiberaRecurso(const CRequest& request)
{
    if (GetIsCoordenador() == false)
        return;

    if (m_mapFilasRecursos.find(request.GetIdRecurso()) == m_mapFilasRecursos.end())
        return; // Nunca teve a fila

    auto& itFilaRecurso = m_mapFilasRecursos[request.GetIdRecurso()];

    if (itFilaRecurso.size() == 0)
        return; //Ninguem na fila

    while (itFilaRecurso.size()) // buscar um que não esteja inativo para evitar softlock
    {
        CRequest requestNaFila = itFilaRecurso.front();
        itFilaRecurso.pop();

        if (requestNaFila.GetProcesso()->GetIsActive() == false)
            continue;

        //todo
        std::thread threadConsome(&CProcesso::ConsomeRecurso, requestNaFila.GetProcesso(), request.GetIdRecurso());
        threadConsome.join();

        break;
    }
}

int CProcesso::GetIdRecursoAleatorio()
{
    if (GetIsCoordenador() == false)
        return 0;

    if (m_listIdRecursos->size() == 0)
        return 0;

    std::random_device rd;
    std::mt19937       gen(rd());

    std::uniform_int_distribution<> dis(0, m_listIdRecursos->size() - 1);
    return m_listIdRecursos->at(dis(gen));
}
