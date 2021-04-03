//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Command*/						/*Update Interval*/			/*Update Signal*/
	{"dbdiskusage",					120,						15},
	{"dbvolume",					0,							10},
	{"dbmemusage",					10,							11},
	{"dbcpuusage",					10,							12},
	{"dbcputemp",					5,							13},
	{"dbdate",						60,							14},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = "  |  ";
static unsigned int delimLen = 5;
