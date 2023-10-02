#include <thread>
#include <random>
#include <format>
#include <functional>
#include "Processo.h"
#include "Bully.h"

const int MIN_TIME_REQUEST = 10;
const int MAX_TIME_REQUEST = 25;

const int MIN_TIME_PROCESS = 5;
const int MAX_TIME_PROCESS = 15;

CProcesso::CProcesso(int id) 
    : m_listRecursos(nullptr)
{
    m_iId = id;
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
    while (GetIsActive())
    {
        //os processos tentam consumir o(s) recurso(s) num intervalo de 10 a 25 segundos
        int iSegundosEspera = rand() % (MAX_TIME_REQUEST - MIN_TIME_REQUEST + 1) + MIN_TIME_REQUEST;
        std::this_thread::sleep_for(std::chrono::seconds(iSegundosEspera));

        if (GetIsActive() == false)
            break;

        CRequest request;
        request.SetProcesso(this);
        request.SetRecurso(GetCoordenador()->GetRecursoAleatorio());

        std::string sMensagemRequest = std::format("[REQUEST] Processo {}, requisitou o recurso {} apos {} segundos\n", GetId(), request.GetRecurso()->GetId(), iSegundosEspera);
        printf(sMensagemRequest.c_str());
        
        std::thread thRequest(&CProcesso::RequisitaRecurso, GetCoordenador(), request);
        thRequest.detach();
    }
}

void CProcesso::ConsomeRecurso(std::shared_ptr<CRecurso> recurso)
{
    int iSegundosConsumo = rand() % (MAX_TIME_PROCESS - MIN_TIME_PROCESS + 1) + MIN_TIME_PROCESS;
    //o tempo de processamento de um recurso é de 5 a 15 segundos
    std::this_thread::sleep_for(std::chrono::seconds(iSegundosConsumo));

    CRequest requestLiberacao;
    requestLiberacao.SetProcesso (this);
    requestLiberacao.SetRecurso(recurso);

    std::string sMensagemRequestLiberacao = std::format("[CONSUMIDO] Recurso {}, consumido pelo processo {} durante {} segundos\n", recurso->GetId(), GetId(), iSegundosConsumo);
    printf(sMensagemRequestLiberacao.c_str());

    GetCoordenador()->LiberaRecurso(requestLiberacao);
}

void CProcesso::SetListaRecursos(std::vector<std::shared_ptr<CRecurso>>* listaRecursos)
{
    m_listRecursos = listaRecursos;
}

void CProcesso::RequisitaRecurso(const CRequest& request)
{
    if (GetIsCoordenador() == false)
        return;

    if (m_mapFilasRecursos[request.GetRecurso()->GetId()].size() || request.GetRecurso()->GetIsInUse())
    {
        m_mapFilasRecursos[request.GetRecurso()->GetId()].push(request);

        std::string sAvisoFila = std::format("[FILA] Adicionada request do processo {} na fila do recurso {}\n\n", request.GetProcesso()->GetId(), request.GetRecurso()->GetId());
        printf(sAvisoFila.c_str());

        return;
    }

    request.GetRecurso()->SetIsInUse(true);
    request.GetProcesso()->ConsomeRecurso(request.GetRecurso());

    return;
}

void CProcesso::LiberaRecurso(const CRequest& request)
{
    if (GetIsCoordenador() == false)
        return;

    request.GetRecurso()->SetIsInUse(false);

    if (m_mapFilasRecursos.find(request.GetRecurso()->GetId()) == m_mapFilasRecursos.end())
        return; // Nunca teve a fila

    auto& itFilaRecurso = m_mapFilasRecursos[request.GetRecurso()->GetId()];

    if (itFilaRecurso.size() == 0)
        return; //Ninguem na fila

    while (itFilaRecurso.size()) // buscar um que não esteja inativo para evitar softlock
    {
        CRequest requestNaFila = itFilaRecurso.front();
        itFilaRecurso.pop();

        if (requestNaFila.GetProcesso()->GetIsActive() == false)
            continue;

        std::string sAvisoFila = std::format("[LIBERADO] Liberado consumo do Recurso {} para o processo {}\n\n", request.GetRecurso()->GetId(), request.GetProcesso()->GetId());
        printf(sAvisoFila.c_str());

        std::thread threadConsome(&CProcesso::ConsomeRecurso, requestNaFila.GetProcesso(), request.GetRecurso());
        threadConsome.join();

        break;
    }
}

std::shared_ptr<CRecurso> CProcesso::GetRecursoAleatorio()
{
    if (GetIsCoordenador() == false)
        return 0;

    if (m_listRecursos->size() == 0)
        return 0;

    std::random_device rd;
    std::mt19937       gen(rd());

    std::uniform_int_distribution<> dis(0, m_listRecursos->size() - 1);
    return m_listRecursos->at(dis(gen));
}
