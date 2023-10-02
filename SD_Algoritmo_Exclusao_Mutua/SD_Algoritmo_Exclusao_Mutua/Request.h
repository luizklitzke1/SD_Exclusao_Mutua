#pragma once
#include <memory>

class CProcesso;
class CRequest
{
public:
	CProcesso* GetProcesso() const;
	void SetProcesso(CProcesso* pProcesso);

	int GetIdRecurso() const;
	void SetIdRecurso(int iRecurso);
	
private:
	CProcesso* m_pProcesso;
	int m_IdRecurso ;
};

