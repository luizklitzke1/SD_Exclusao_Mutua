#pragma once
#include "Request.h"
#include <memory>
#include <queue>
#include <unordered_map>

class CBully;
class CProcesso
{
public:
    CProcesso(int id);
    ~CProcesso();

    int GetId();

    bool GetIsActive();
    void SetIsActive(bool isActive);

    bool GetIsCoordenador();
    void SetIsCoordenador(bool isCoordenador);

    std::shared_ptr<CProcesso> GetCoordenador();
    void SetCoordenador(std::shared_ptr<CProcesso> coordenador);

    void MandaRequestRecurso();
    void ConsomeRecurso(std::shared_ptr<CRecurso> recurso);

    void SetListaRecursos(std::vector<std::shared_ptr<CRecurso>>* listaRecursos);

    //Funções para coordenador
    void RequisitaRecurso(const CRequest& request);
    void LiberaRecurso   (const CRequest& request);

    std::shared_ptr<CRecurso> GetRecursoAleatorio();

private:
    unsigned long m_iId = 0;

    bool m_bIsActive      = true;
    bool m_bIsCoordenador = false;

    CBully* m_pBully = nullptr;
    std::shared_ptr<CProcesso> m_pCoordenador = nullptr;

    //Dados para coordenador
    std::vector<std::shared_ptr<CRecurso>>* m_listRecursos;
    std::unordered_map<int, std::queue<CRequest>> m_mapFilasRecursos;
};
