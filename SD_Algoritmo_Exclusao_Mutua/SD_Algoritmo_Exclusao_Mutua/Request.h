#pragma once
#include <memory>

class CProcesso;
class CRecurso;

class CRequest
{
public:
	CProcesso* GetProcesso() const;
	void SetProcesso(CProcesso* pProcesso);

	std::shared_ptr<CRecurso> GetRecurso() const;
	void SetRecurso(std::shared_ptr<CRecurso> recurso);
	
private:
	CProcesso* m_pProcesso;
	std::shared_ptr<CRecurso> m_pRecurso ;
};

