#include "CMpqManager.h"

CMpqManager::CMpqManager()
{

}

CMpqManager::~CMpqManager()
{
	for (const auto& mpq : m_Mpqs)
		SFileCloseArchive(mpq);

	std::vector<HANDLE>().swap(m_Mpqs);
}

bool CMpqManager::OpenMpq(LPCSTR lpArchiveName, DWORD dwPriority, DWORD dwFlags)
{
	HANDLE mpq;
	if (!SFileOpenArchive(lpArchiveName, (dwPriority ? dwPriority : m_Mpqs.size() + 9), dwFlags, &mpq))
		return false;

	m_Mpqs.push_back(mpq);

	return true;
}