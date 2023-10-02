#include "Request.h"

CProcesso* CRequest::GetProcesso() const
{
	return m_pProcesso;
}

void CRequest::SetProcesso(CProcesso* pProcesso)
{
	m_pProcesso = pProcesso;
}

int CRequest::GetIdRecurso() const
{
	return m_IdRecurso;
}

void CRequest::SetIdRecurso(int iRecurso)
{
	m_IdRecurso = iRecurso;
}
