#include "Request.h"

CProcesso* CRequest::GetProcesso() const
{
	return m_pProcesso;
}

void CRequest::SetProcesso(CProcesso* pProcesso)
{
	m_pProcesso = pProcesso;
}

std::shared_ptr<CRecurso> CRequest::GetRecurso() const
{
	return m_pRecurso;
}

void CRequest::SetRecurso(std::shared_ptr<CRecurso> recurso)
{
	m_pRecurso = recurso;
}
