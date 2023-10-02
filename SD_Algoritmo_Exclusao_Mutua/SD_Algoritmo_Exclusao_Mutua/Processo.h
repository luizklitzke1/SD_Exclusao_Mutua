#pragma once
#include "Request.h"
#include <memory>
#include <queue>
#include <unordered_map>

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
    void ConsomeRecurso(int iRecurso);

    void SetListaRecursos(std::vector<int>* listaRecursos);

    //Funções para coordenador
    void RequisitaRecurso(const CRequest& request);
    void LiberaRecurso   (const CRequest& request);

    int GetIdRecursoAleatorio();

private:
    unsigned long m_iId = 0;

    bool m_bIsActive      = true;
    bool m_bIsCoordenador = false;

    std::shared_ptr<CProcesso> m_pCoordenador = nullptr;

    //Dados para coordenador
    std::vector<int>* m_listIdRecursos;
    std::unordered_map<int, std::queue<CRequest>> m_mapFilasRecursos;
};
