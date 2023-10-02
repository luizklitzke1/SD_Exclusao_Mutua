#pragma once
class CRecurso
{
public:
	int GetId() const;
	void SetId(int id);

	bool GetIsInUse() const;
	void SetIsInUse(bool bIsInUse);

private:
	int m_iId = 0;
	bool m_bIsInUse = false;
};

