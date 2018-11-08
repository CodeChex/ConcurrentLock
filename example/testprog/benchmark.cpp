// benchmark.cpp : Defines the class behaviors for the application.
//

#include "benchmark.h"
#include "benchmarkDlg.h"

#ifdef _WIN32
extern "C" 
{
	#include "getopt.h"
}
#else
   #include <ctype.h>
   #include <string.h>
   #define strcmpi(a,b) strcasecmp(a,b)
   #define strncmpi(a,b,c) strncasecmp(a,b,c)
   #define stricmp(a,b) strcasecmp(a,b)
   #define strnicmp(a,b,c) strncasecmp(a,b,c)
   #define strlwr(s) if(s) { int z; for(z=strlen(s)-1;z>=0;z--) s[z]=tolower(s[z]); }
   #define strupr(s) if(s) { int z; for(z=strlen(s)-1;z>=0;z--) s[z]=toupper(s[z]); }
#endif

CBenchmarkDlg CentralBooking;

/////////////////////////////////////////////////////////////////////////////
unsigned main (unsigned argc, char* argv[])
{
	unsigned rc = 0;

	// setup parameters

	// display statistics 
	// -D:interval,turns
	unsigned interval = 1;
	unsigned turns = 4;
	bool resetStats = false;

	// test lock type
	// -T:CONCUR,MUTEX,CRITSECT
	CentralBooking.m_nLockType = SELECT_NONE;

	// -R:n,msec,timeout
	CentralBooking.m_nRead = 10;
	CentralBooking.m_msecsR = 50;
	CentralBooking.m_timeoutR = 125;

	// -W:n,msec,timeout
	CentralBooking.m_nWrite = 2;
	CentralBooking.m_msecsW = 10;
	CentralBooking.m_timeoutW = 250;

	// really parse it
	int errflag = 0;
	int c;
	char* pstr;
	extern int optind;
	extern char *optarg;
	while ((c = getopt(argc, argv, "D:d:T:t:R:r:W:w:?")) != EOF)
	{
		switch (c) 
		{
			case 'T':	/* lock type */
			case 't':
				if ( !optarg )
				{
					errflag ++;
					fprintf(stderr, "error: no lock type specified\n");
				}
				else if ( stricmp(optarg,"concur") == 0)
				{
					CentralBooking.m_nLockType = SELECT_CONCUR;
				}
				else if ( stricmp(optarg,"mutex") == 0 )
				{
					CentralBooking.m_nLockType = SELECT_MUTEX;
				}
				else if ( stricmp(optarg,"critsect") == 0 )
				{
					CentralBooking.m_nLockType = SELECT_CRITSECT;
				}
				else
				{
					errflag ++;
					fprintf(stderr, "error: invalid lock type [%s] specified\n", optarg);
				}
				break;
			case 'D':	/* statistics display frequency */
			case 'd':
				if ( !optarg )
				{
					errflag ++;
					fprintf(stderr, "error: no display stats parameters specified\n");
				}
				else if ( pstr = strtok(optarg,",") )
				{
					interval = atoi(pstr);
					if ( pstr = strtok(NULL,",") )
					{
						turns = atoi(pstr);
						if ( pstr = strtok(NULL,",") )
						{
							strupr(pstr);
							resetStats = (*pstr == 'Y') ? true : false;
						}
					}
				}
				break;
			case 'R':	/* read thread settings */
			case 'r':
				if ( !optarg )
				{
					errflag ++;
					fprintf(stderr, "error: no READ thread parameters specified\n");
				}
				else if ( pstr = strtok(optarg,",") )
				{
					CentralBooking.m_nRead = atoi(pstr);
					if ( pstr = strtok(NULL,",") )
					{
						CentralBooking.m_msecsR = atoi(pstr);
						if ( pstr = strtok(NULL,",") )
						{
							CentralBooking.m_timeoutR = atoi(pstr);
						}
					}
				}
				break;
			case 'W':	/* read thread settings */
			case 'w':
				if ( !optarg )
				{
					errflag ++;
					fprintf(stderr, "error: no WRITE thread parameters specified\n");
				}
				else if ( pstr = strtok(optarg,",") )
				{
					CentralBooking.m_nWrite = atoi(pstr);
					if ( pstr = strtok(NULL,",") )
					{
						CentralBooking.m_msecsW = atoi(pstr);
						if ( pstr = strtok(NULL,",") )
						{
							CentralBooking.m_timeoutW = atoi(pstr);
						}
					}
				}
				break;
			case '?':
			default:
				errflag ++;
				break;
		}
	}
	if ( !(CentralBooking.m_nRead + CentralBooking.m_nWrite) )
	{
		errflag ++;
		fprintf(stderr, "error: no READ or WRITE threads specified\n");
	}
	if ( errflag || CentralBooking.m_nLockType == SELECT_NONE )
	{
		fprintf(stderr, "\nusage: %s\n", argv[0]);
		fprintf(stderr, "\tType of Lock : -T[CONCUR|MUTEX|CRITSECT]\n");
		fprintf(stderr, "\t\t *** required\n");
		fprintf(stderr, "\tDisplay Stats: -D[interval(sec)],[iterations],[reset]\n");
		fprintf(stderr, "\t\t defaults to interval(%u sec),iterations(%u), reset(%u) \n",
							interval, turns, resetStats ? _T('Y') : _T('N'));
		fprintf(stderr, "\tRead Threads : -R[num,cycle(ms),timeout(ms)] ...\n");
		fprintf(stderr, "\t\t defaults to num(%u), cycle(%u ms), timeout( %u ms)\n",
							CentralBooking.m_nRead,
							CentralBooking.m_msecsR,
							CentralBooking.m_timeoutR);
		fprintf(stderr, "\tWrite Threads : -W[num,cycle(ms),timeout(ms)] ...\n");
		fprintf(stderr, "\t\t defaults to num(%u), cycle(%u ms), timeout( %u ms)\n",
							CentralBooking.m_nWrite,
							CentralBooking.m_msecsW,
							CentralBooking.m_timeoutW);
		exit(2);
	}

	// regurgitate parameters
	_ftprintf(stderr,_T("\nLOCK TEST: [%s]"),
		CentralBooking.m_nLockType == SELECT_CONCUR ? _T("Concurrent Lock") 
			: CentralBooking.m_nLockType == SELECT_MUTEX ? _T("Mutex")
				: CentralBooking.m_nLockType == SELECT_CRITSECT ? _T("Critical Section")
					: _T("unknown") );
	_ftprintf(stderr,_T("\nREAD  Threads: count:[%6lu], cycle[%6lu], timeout[%6lu]"),
		CentralBooking.m_nRead,
		CentralBooking.m_msecsR,
		CentralBooking.m_timeoutR);
	_ftprintf(stderr,_T("\nWRITE Threads: count:[%6lu], cycle[%6lu], timeout[%6lu]"),
		CentralBooking.m_nWrite,
		CentralBooking.m_msecsW,
		CentralBooking.m_timeoutW);
	_ftprintf(stderr,_T("\nSTATS: every [%lu] seconds for [%u] times, reset=%c"),
		interval,
		turns,
		resetStats ? _T('Y') : _T('N'));
	_ftprintf(stderr,_T("\nEXECUTION TIME: [%lu] seconds "),interval * turns);

	_ftprintf(stderr,_T("\n\n...STARTING...\n"));

	CentralBooking.START();

	while ( turns-- > 0 )
	{
		Sleep(interval*1000);
		CentralBooking.dumpStats(resetStats);
	} 

	_ftprintf(stderr,_T("\n\n...STOPPING...\n"));

	CentralBooking.STOP();

	return rc;
}
