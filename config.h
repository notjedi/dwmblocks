//Modify this file to change what commands output to your statusbar, and recompile using the make command.
/* 💰   🤑  */
static const Block blocks[] = {
	/*Command*/						/*Update Interval*/			/*Update Signal*/
	{"dbspotify",					5,						    7},
	{"dbprice ada 💰",		         60,					     6},
	/* {"dbnettraf",					1,						    8}, */
	{"dbdiskusage",					300,						9},
	{"dbvolume",					0,							10},
	{"dbmemusage",					10,							11},
	{"dbcpuusage",					10,							12}, /* costly, so i run it only every 10 secs */
	{"dbcputemp",					5,							13},
	{"dbdate",						60,							14},
};

/* sets delimeter between status commands. NULL character ('\0') means no delimeter. */
static char delim[] = "  ";
static unsigned int delimLen = 5;
