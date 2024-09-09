#include "../ini.h"
#include <stdlib.h>

int main() 
{
	uintptr_t *pat = 0;
	uintptr_t* config = 0;
	char buf[15] = { 0 };
	createEmptyIni("config.ini");
	LoadIniFromFile("config.ini", &pat);
	AddKeyVal(pat, "ClientInfo", "clientIP",	"192.168.0.1");
	AddKeyVal(pat, "ClientInfo", "clientPort", "1234");
	AddKeyVal(pat, "ClientInfo", "MaxConnection", "100");
	AddKeyVal(pat, "ClientInfo", "usage", "152");
	AddKeyVal(pat, "ServerInfo", "clientIP", "192.168.0.1");
	AddKeyVal(pat, "ServerInfo", "clientPort", "1234");
	AddKeyVal(pat, "ServerInfo", "MaxConnection", "100");
	AddKeyVal(pat, "ServerInfo", "usage", "152");
	WriteIniToFile("hh.ini",pat);
	LoadIniFromFile("hh.ini", &config);

	GetKeyVal(config, "ClientInfo","clientIP",buf,15);
	printf("clientIP=%s\n", buf);
	DeleteIni(pat);
	DeleteIni(config);
	system("pause");
    return 0;
}
