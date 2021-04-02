//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/						/*Update Interval*/			/*Update Signal*/
	{"",		"dbdiskusage",					120,						0},
	{"",		"dbvolume",						0,							10},
	{"",		"dbmemusage",					10,							0},
	{"",		"dbcpuusage",					10,							0},
	{"",		"dbcputemp",					5,							0},
	{"", 		"date +'%I:%M '",				60,							0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = "  |  ";
static unsigned int delimLen = 5;
