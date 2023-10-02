#include "ThreadHelper.h"
#include <thread>
#include <chrono>
#include <stdio.h>
#include <string>
#include <format>
#include <stdlib.h> 

void CThreadHelper::CreateNewProcess(CBully& bully)
{
	srand(time(NULL)); // nitialize random seed

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(40));
		printf("\n40 segundos - CRIAÇÃO DO NOVO PROCESSO\n");

		int Id = rand() % INT_MAX + 1; // in the range 1 to 2147483647

		//(ID randômico)
		while (std::find_if(bully.m_listProcessos.begin(), bully.m_listProcessos.end(), [&](const std::shared_ptr<CProcesso>& processo)
			{
				return processo->GetId() == Id;
			}) != bully.m_listProcessos.end()
		);
		{
			Id = rand() % LONG_MAX + 1; // in the range 1 to 2147483647
		}

		std::shared_ptr<CProcesso> newProcess = std::make_shared<CProcesso>(Id);
		if (bully.m_pCoordenador != nullptr)
            newProcess->SetCoordenador(bully.m_pCoordenador);

		bully.m_listProcessos.push_back(newProcess);

		std::string showProcesses;

		for (size_t i = 0; i < bully.m_listProcessos.size(); ++i)
		{
			auto& process = bully.m_listProcessos[i];
			showProcesses.append(std::format("{}{}", process->GetId(), process->GetIsActive() ? "(A)" : "(I)"));

			if (i != bully.m_listProcessos.size() - 1)
				showProcesses.append(", ");
		}

		printf("Processos: %s\n\n", showProcesses.c_str());

		bully.doElection(newProcess);

		std::thread thRequestRecurso(&CProcesso::MandaRequestRecurso, newProcess);
		thRequestRecurso.detach();
	}
}

void CThreadHelper::InactiveCoordenador(CBully& bully)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(60));
		printf("\n60 segundos - INATIVAR COORDENADOR\n");

		if (bully.m_pCoordenador == nullptr)
		{
			printf("Nenhum coordenador definido!\n\n");
			continue;
		}
		
		bully.m_pCoordenador->SetIsActive(false);
		printf("Coordenador %d inativo!\n", bully.m_pCoordenador->GetId());
		printf("Fila de processos limpa!\n\n");

		bully.doElection(bully.GetRandomProcess());
	}
}
