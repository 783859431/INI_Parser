#ifndef _INI_PARSE
#define _INI_PARSE
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
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
#endif 

