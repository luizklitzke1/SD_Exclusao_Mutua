#include "Recurso.h"

int CRecurso::GetId() const
{
	return m_iId;
}

void CRecurso::SetId(int id)
{
	m_iId = id;
}

bool CRecurso::GetIsInUse() const
{
	return m_bIsInUse;
}

void CRecurso::SetIsInUse(bool bIsInUse)
{
	m_bIsInUse = bIsInUse;
}
