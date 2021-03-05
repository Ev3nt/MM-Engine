#include "CEngine.h"

#include <document.h>
#include <istreamwrapper.h>
#include <ostreamwrapper.h>
#include <prettywriter.h>
#include <fstream>
#include <string>
#include <vector>

inline LPSTR strcopy(LPCSTR str)
{
	size_t size = strlen(str) + 1;
	char* retval = new char[size];
	strcpy_s(retval, size, str);

	return retval;
}

CEngine::CEngine(): mpqmanager(new CMpqManager)
{
	mpqmanager->OpenMpq("MM Engine.mpq");

	m_Data["ModName"] = (DWORD)strcopy("MM Engine");
	m_Data["ModIcon"] = (DWORD)strcopy("MMEngine.ico");
	m_Data["Logo"] = NULL;
	m_Data["Info"] = (DWORD)strcopy("© (2021) The Mod Makers");
	m_Data["Enable"] = true;
	m_Data["ModVersion"] = (DWORD)strcopy("MM Engine - Version 2.0.3 (Dev Build)");
	m_Data["Priority"] = 9;
	m_Data["Skins"] = (DWORD)(new std::vector<std::string>{ "Human", "Orc", "Undead", "NightElf" });
	m_Data["Keys"] = (DWORD)(new std::vector<std::string>{ "HUMAN", "ORC", "UNDEAD", "NIGHT_ELF" });

	std::ifstream json("MM Engine.json");

	if (!json)
	{
		std::ofstream json("MM Engine.json");
		
		rapidjson::Document doc;

		doc.SetObject();

		rapidjson::OStreamWrapper ostream(json);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(ostream);
		doc.Accept(writer);

		json.close();
	}

	json.close();

	json.open("MM Engine.json");

	rapidjson::Document doc;
	rapidjson::IStreamWrapper istream(json);
	doc.ParseStream(istream);
	json.close();

	{
		if (!doc.IsObject())
		{
			MessageBox(NULL, "MM Engine.json повреждён или имеет синтаксические ошибки.", "Ошибка", MB_ICONERROR);

			return;
		}

		if (doc.HasMember("ShowMpqMessage"))
		{
			if (doc["ShowMpqMessage"].IsBool())
				m_Data["ShowMpqMessage"] = doc["ShowMpqMessage"].GetBool();
		}
		else
			doc.AddMember("ShowMpqMessage", false, doc.GetAllocator());


		std::ofstream json("MM Engine.json");

		rapidjson::OStreamWrapper ostream(json);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(ostream);
		doc.Accept(writer);

		json.close();
	}
}

CEngine::~CEngine()
{

}

void CEngine::LoadMod(LPCSTR lpModName)
{
	if (!lpModName)
		return;

	m_Data["Mod"] = (DWORD)strcopy(lpModName);

	std::string path = ".\\Mods\\" + (std::string)lpModName + "\\";

	std::ifstream manifest(path + "Manifest.json");

	if (!manifest)
		return;

	rapidjson::Document doc;
	rapidjson::IStreamWrapper istream(manifest);
	doc.ParseStream(istream);
	manifest.close();

	if (!doc.IsObject())
	{
		MessageBox(NULL, "Manifest.json повреждён или имеет синтаксические ошибки.", "Ошибка", MB_ICONERROR);

		return;
	}

	if (doc.HasMember("ModName") && doc["ModName"].IsString())
	{
		delete[] (LPSTR)m_Data["ModName"];
		m_Data["ModName"] = (DWORD)strcopy(doc["ModName"].GetString());
	}

	if (doc.HasMember("ModIcon") && doc["ModIcon"].IsString())
	{
		delete[] (LPSTR)m_Data["ModIcon"];
		m_Data["ModIcon"] = (DWORD)strcopy((path + std::string(doc["ModIcon"].GetString()) + ".ico").c_str());
	}

	if (doc.HasMember("Preview") && doc["Preview"].IsObject())
	{
		if (doc["Preview"].HasMember("Logo") && doc["Preview"]["Logo"].IsString())
			m_Data["Logo"] = (DWORD)strcopy(doc["Preview"]["Logo"].GetString());

		if (doc["Preview"].HasMember("Info") && doc["Preview"]["Info"].IsString())
		{
			delete[] (LPSTR)m_Data["Info"];
			m_Data["Info"] = (DWORD)strcopy(doc["Preview"]["Info"].GetString());
		}

		if (doc["Preview"].HasMember("Enable") && doc["Preview"]["Enable"].IsBool())
			m_Data["Enable"] = doc["Preview"]["Enable"].GetBool();
	}

	if (doc.HasMember("ModVersion") && doc["ModVersion"].IsString())
	{
		delete[] (LPSTR)m_Data["ModVersion"];
		m_Data["ModVersion"] = (DWORD)strcopy(doc["ModVersion"].GetString());
	}

	if (doc.HasMember("Mpqs") && doc["Mpqs"].IsArray())
	{
		for (size_t it = 0; it < doc["Mpqs"].MemberCount(); it++)
		{
			rapidjson::Value mpq = doc["Mpqs"][it].GetObjectA();

			if (mpq.HasMember("Name") && mpq["Name"].IsString())
			{
				std::string filename = mpq["Name"].GetString();
				
				if (!mpqmanager->OpenMpq((path + "Mpqs\\" + filename + ".mpq").c_str(), (mpq.HasMember("Priority") && mpq["Priority"].IsInt()) ? mpq["Priority"].GetInt() : 0) && m_Data["ShowMpqMessage"])
					MessageBox(NULL, ("В моде " + std::string((LPCSTR)m_Data["Mod"]) + " отсутствует " + filename + ".mpq").c_str(), "Внимание", MB_ICONWARNING);
			}
		}
	}

	if (doc.HasMember("Races") && doc["Races"].IsArray())
	{
		std::vector<std::string>& races = (*(std::vector<std::string>*)m_Data["Skins"]);
		std::vector<std::string>& raceskeys = (*(std::vector<std::string>*)m_Data["Keys"]);

		for (auto r : races)
			r.~basic_string();

		for (auto r : raceskeys)
			r.~basic_string();

		std::vector<std::string>().swap(races);
		std::vector<std::string>().swap(raceskeys);

		for (size_t it = 0; it < doc["Races"].MemberCount(); it++)
		{
			rapidjson::Value race = doc["Races"][it].GetObjectA();

			if (race.HasMember("Skin") && race["Skin"].IsString())
			{

				races.push_back(race["Skin"].GetString());
				raceskeys.push_back(race.HasMember("Key") && race["Key"].IsString() ? race["Key"].GetString() : "UNKNOWN");
			}
		}
	}
}

DWORD CEngine::GetData(LPCSTR lpKey)
{
	for (const auto& item : m_Data)
		if (!_strcmpi(item.first, lpKey))
			return item.second;

	return NULL;
}