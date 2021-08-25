#include "CMpqManager.h"

CMpqManager::CMpqManager()
{

}

CMpqManager::~CMpqManager()
{
	for (const auto& mpq : m_mpqList)
		SFileCloseArchive(mpq);

	m_mpqList.~vector();
}

BOOL CMpqManager::loadMpq(std::string mpqName, DWORD priority, DWORD flags)
{
	HANDLE mpq;
	BOOL result = SFileOpenArchive(mpqName.c_str(), !priority ? m_mpqList.size() + 13 : priority, flags, &mpq);
	
	m_mpqList.push_back(mpq);

	return result;
}