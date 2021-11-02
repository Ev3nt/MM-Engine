#include "Manifest.h"
#include "Variables.h"
#include <document.h>
#include <istreamwrapper.h>
#include <ostreamwrapper.h>
#include <prettywriter.h>
#include <fstream>
#include <Windows.h>

#define setString(key, value) (engine->getData<std::string*>(key)->~basic_string(), engine->setData(key, new std::string(value)))
#define setOther(key, value) (engine->setData(key, value))

void readMainManifest()
{
	std::ifstream json("MM Engine.json");

	if (!json)
	{
		std::ofstream json("MM Engine.json");

		rapidjson::Document document;

		document.SetObject();

		rapidjson::OStreamWrapper ostream(json);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(ostream);
		document.Accept(writer);

		json.close();
	}

	json.close();

	json.open("MM Engine.json");

	rapidjson::Document document;
	rapidjson::IStreamWrapper istream(json);
	document.ParseStream(istream);
	json.close();

	{
		if (!document.IsObject())
		{
			MessageBox(NULL, "MM Engine.json повреждён или имеет синтаксические ошибки.", "Ошибка", MB_ICONERROR);

			return;
		}

		if (document.HasMember("ShowMpqMessage"))
		{
			if (document["ShowMpqMessage"].IsBool())
			{
				engine->setData("ShowMpqMessage", document["ShowMpqMessage"].GetBool());
			}
		}
		else
		{
			document.AddMember("ShowMpqMessage", true, document.GetAllocator());
		}


		std::ofstream json("MM Engine.json");

		rapidjson::OStreamWrapper ostream(json);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(ostream);
		document.Accept(writer);

		json.close();
	}
}

void readModManifest()
{
	std::string path = ".\\Mods\\" + engine->getModName() + "\\";

	std::ifstream manifest(path + "Manifest.json");

	if (!manifest)
	{
		return;
	}

	rapidjson::Document document;
	rapidjson::IStreamWrapper istream(manifest);
	document.ParseStream(istream);
	manifest.close();

	if (!document.IsObject())
	{
		MessageBox(NULL, "Manifest.json is corrupt or contains syntax error.", "Error", MB_ICONERROR);

		return;
	}

	if (document.HasMember("ModName") && document["ModName"].IsString())
	{
		setString("ModName", document["ModName"].GetString());
	}

	if (document.HasMember("ModIcon") && document["ModIcon"].IsString())
	{
		setString("ModIcon", document["ModIcon"].GetString());
	}
	
	if (document.HasMember("Preview") && document["Preview"].IsObject())
	{
		if (document["Preview"].HasMember("Bitmap") && document["Preview"]["Bitmap"].IsString())
		{
			setString("Bitmap", document["Preview"]["Bitmap"].GetString());
		}
	
		if (document["Preview"].HasMember("Info") && document["Preview"]["Info"].IsString())
		{
			setString("Info", document["Preview"]["Info"].GetString());
		}
	
		if (document["Preview"].HasMember("Enable") && document["Preview"]["Enable"].IsBool())
		{
			setOther("Enable", document["Preview"]["Enable"].GetBool());
		}
	}
	
	if (document.HasMember("ModVersion") && document["ModVersion"].IsString())
	{
		setString("ModVersion", document["ModVersion"].GetString());
	}

	if (document.HasMember("Mpqs") && document["Mpqs"].IsArray())
	{
		for (size_t it = 0; it < document["Mpqs"].MemberCount(); it++)
		{
			rapidjson::Value mpq = document["Mpqs"][it].GetObjectA();
	
			if (mpq.HasMember("Name") && mpq["Name"].IsString())
			{
				std::string filename = mpq["Name"].GetString();
	
				if (!engine->getMpqManager().loadMpq((path + "Mpqs\\" + filename + ".mpq").c_str(),
					(mpq.HasMember("Priority") && mpq["Priority"].IsInt()) ? mpq["Priority"].GetInt() : 0) && engine->getData<bool>("ShowMpqMessage"))
				{
					MessageBox(NULL, (engine->getModName() + " doesn't contain " + filename + ".mpq").c_str(), "Warning", MB_ICONWARNING);
				}
			}
		}
	}
	 
	if (document.HasMember("Races") && document["Races"].IsArray())
	{
		std::vector<std::string>& races = *(engine->getData<std::vector<std::string>*>("Skins"));
		std::vector<std::string>& racesKeys = *(engine->getData<std::vector<std::string>*>("Keys"));
	
		for (auto race : races)
			race.~basic_string();
	
		for (auto key : racesKeys)
			key.~basic_string();
	
		std::vector<std::string>().swap(races);
		std::vector<std::string>().swap(racesKeys);
	
		for (size_t it = 0; it < document["Races"].MemberCount(); it++)
		{
			rapidjson::Value race = document["Races"][it].GetObjectA();
	
			if (race.HasMember("Skin") && race["Skin"].IsString())
			{
				races.push_back(race["Skin"].GetString());
				racesKeys.push_back(race.HasMember("Key") && race["Key"].IsString() ? race["Key"].GetString() : "UNKNOWN");
			}
		}
	}
	
	if (document.HasMember("Bins") && document["Bins"].IsArray())
	{
		for (size_t it = 0; it < document["Bins"].MemberCount(); it++)
		{
			if (document["Bins"][it].IsString())
			{
				LoadLibrary((path + "Bins\\" + document["Bins"][it].GetString() + ".dll").c_str());
			}
		}
	}
}

//----------------------------------------------------------------

void readManifests()
{
	readMainManifest();
	readModManifest();
}