// inilib.cpp : 定义静态库的函数。
//

#include<string>
#include<fstream>
#include<unordered_map>
#include<memory.h>
using IniConfig = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;
#ifdef __cplusplus
extern "C" {
#endif

	 bool LoadIniFromFile(const char* path, uintptr_t** parser);
	 bool WriteIniToFile(const char* path, uintptr_t* parser);
	 bool DeleteIni(uintptr_t* parser);
	 bool GetKeyVal(uintptr_t* parser, const char* section, const char* key, char* valbuffer, size_t bufferSize);
	 bool AddKeyVal(uintptr_t* parser, const char* section, const char* key, const char* val);
	 bool AddSection(uintptr_t* parser, const char* section);
	 bool removeSection(uintptr_t* parser, const char* section);
	 bool removeKey(uintptr_t* parser, const char* section, char* key);
         bool createEmptyIni(const char* path);
#ifdef __cplusplus
}
#endif
class INI_Parser
{

public:
	IniConfig* Cini;
	bool LoadINI(const std::string& path);
	bool writeINI(const std::string& path);
	bool get(const std::string& section, const std::string& key, std::string& val);
	bool add(const std::string& section, const std::string& key, const std::string& val);
	bool addSection(const std::string& section);
	bool removeSection(const std::string& section);
	bool removeKey(const std::string& section, const std::string& key);
	~INI_Parser()
	{
		if (Cini)
			delete Cini;
	}


};

bool createEmptyIni(const char *path)
{
   std::ofstream file(path);
   if(file.is_open())
   {
       file.close();
       return 1;
   }
   return 0;

}
bool LoadIniFromFile(const char* path, uintptr_t** parser)
{
	INI_Parser* ini = new INI_Parser;
	if (ini->LoadINI(path))
	{
		*parser = reinterpret_cast<uintptr_t*>(ini);
		return 1;

	}
	else
	{
		delete ini;
		parser = 0;
		return 0;
	}


}
bool WriteIniToFile(const char* path, uintptr_t* parser)
{
	if (!parser) return 0;
	INI_Parser* ini = (INI_Parser*)parser;
	return ini->writeINI(path);
}
bool DeleteIni(uintptr_t* parser)
{
	if (parser)
	{
		delete parser;
		return 1;
	}
	return 0;

}
bool GetKeyVal(uintptr_t* parser, const char* section, const char* key, char* valbuffer, size_t bufferSize)
{

	if (!parser)
		return 0;
	INI_Parser* ini = (INI_Parser*)parser;
	std::string buf;
	if (!ini->get(section, key, buf))
		return 0;
	if (bufferSize < buf.size())
		return 0;
	memcpy(valbuffer, buf.c_str(), buf.size());
	return 1;
}
bool AddKeyVal(uintptr_t* parser, const char* section, const char* key, const char* val)
{
	if (!parser)
		return 0;
	INI_Parser* ini = (INI_Parser*)parser;
	return ini->add(section, key, val);
}
bool AddSection(uintptr_t* parser, const char* section)
{
	if (!parser)
		return 0;
	INI_Parser* ini = (INI_Parser*)parser;
	return  ini->addSection(section);


}
bool removeSection(uintptr_t* parser, const char* section)
{
	if (!parser)
		return 0;
	INI_Parser* ini = (INI_Parser*)parser;
	return ini->removeSection(section);


}
bool removeKey(uintptr_t* parser, const char* section, char* key)
{
	if (!parser)
		return 0;
	INI_Parser* ini = (INI_Parser*)parser;
	return ini->removeKey(section, key);

}
std::string trim(std::string& str)
{
	if (str.empty())
		return str;
	std::string copy(str);
	copy.erase(0, copy.find_first_not_of(" "));
	copy.erase(copy.find_last_not_of(" ") + 1);
	return copy;

}

bool INI_Parser::LoadINI(const std::string& path) {

	std::ifstream ini(path);
	if (!ini.is_open())
	{

		return 0;
	}
	Cini = new IniConfig();
	std::string line;
	std::string currentSection = " ";
	std::unordered_map<std::string, std::string> sectionPairs;
	while (std::getline(ini, line)) {
		line = trim(line);
		if (line.empty())//skip empty row 
			continue;
		int i = line.find_first_not_of(' ');
		if (line[i] == '#' || line[i] == ';')// don't care
			continue;
		if (line.find('[') != -1)//section 
		{
			int begin = line.find_first_of('[');
			int end = line.find_last_of(']');
			if (end != -1)
			{
				if (currentSection != " ")
					Cini->insert({ currentSection,sectionPairs });
				sectionPairs.clear();
				currentSection = line.substr(begin + 1, end - begin - 1);
			}
			continue;
		}
		int spt = line.find_first_of('=');
		int end = line.find_first_of("#;");//find if has comment
		std::string key = line.substr(0, spt);
		std::string val = line.substr(spt + 1, end == -1 ? line.size() : end - spt - 1);
		sectionPairs.insert(std::make_pair(trim(key), trim(val)));
	}
	if (currentSection != " ")
		Cini->insert({ currentSection,sectionPairs });
	ini.close();

	return 1;

}

/*
* if section has not existed ,then add a new section
* if key has existed , val override the previous value
*/
bool INI_Parser::add(const std::string& section, const std::string& key, const std::string& val)
{
	auto ret = Cini->find(section);
	if (ret == Cini->end())
	{
		addSection(section);
		ret = Cini->find(section);
	}
	auto& sectionPairs = ret->second;
	sectionPairs[key] = val;
	return 1;
}
bool INI_Parser::removeSection(const std::string& section)
{
	auto ret = Cini->find(section);
	if (ret == Cini->end())
		return 0;
	Cini->erase(section);
	return 1;
}
bool INI_Parser::addSection(const std::string& section)
{
	auto ret = Cini->find(section);
	if (ret != Cini->end())
		return 0;
	std::unordered_map<std::string, std::string> noPair;
	Cini->insert({ section,noPair });
	return 1;
}
bool INI_Parser::removeKey(const std::string& section, const std::string& key)
{

	auto ret = Cini->find(section);
	if (ret == Cini->end())
		return 0;
	auto& sectionPairs = ret->second;
	auto ret1 = sectionPairs.find(key);
	if (ret1 == sectionPairs.end())
		return 0;
	sectionPairs.erase(key);
	return 1;
}
/*
 if section don't exist then  return 0
 if key don't exist val will be set to "" and return 1
*/
bool INI_Parser::get(const std::string& section, const std::string& key, std::string& val)
{
	auto ret = Cini->find(section);
	if (ret == Cini->end())
		return 0;
	auto& sectionPairs = ret->second;
	val = sectionPairs[key];
	return 1;
}
bool INI_Parser::writeINI(const std::string& path)
{
	std::ofstream file(path, std::ostream::out);
	if (!file.is_open())
	{
		return 0;
	}
	for (auto s = Cini->begin(); s != Cini->end(); s++)
	{

		file << '[' << s->first << ']' << std::endl;

		for (auto& p : s->second)
		{
			file << p.first << "=" << p.second << std::endl;
		}

	}
	file.close();
	return 1;

}

