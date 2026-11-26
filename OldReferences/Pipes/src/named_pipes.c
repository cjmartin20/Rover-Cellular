//in file named_pipes.c
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <string.h>
#include "randapi.h"

#define MaxLen 100
#define PipeStdIn 0 //stdin for Unix is 0
#define PipeStdOut 1 //stdout for Unix is 1

int main ( ) { //bridge 

	int ret, ret2, comPipes[2]; //comPipes[0] for input, comPipes[1] for output
	char buff[ MaxLen + 1 ];

	time_t t = time(NULL);
	struct tm startTime = *localtime( &t );

	//sketch Starship Enterprise with time logo
	printf( "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
	printf( "+    *                       -------------------------------------                           _          +\n" );
	printf( "+     *                     |  USS Enterprise Systems Monitoring  |                         / \\         +\n" );
	printf( "+                            -------------------------------------                  *       \\_/         +\n" );
	printf( "+                    *              TIME ON DECK [%02d:%02d:%02d]                                             +\n", startTime.tm_hour, startTime.tm_min, startTime.tm_sec);
	printf( "+              __________________________________                                                       +\n");
	printf( "+             /_        NCC-1701-D              /\\      _________,--^--,________                        +\n" );
	printf( "+             \\_________________________________\\/     /                  [][][]\\                       +\n");
	printf( "+                                       | |            \\_              _________/                       +\n" );
	printf( "+                                       | |             _-        ___---                                +\n" );
	printf( "+         *                  *          | |           _-    ___---                     *                +\n" );
	printf( "+                                   ,-----------------     (___                      *                  +\n" );
	printf( "+               *                  |                           }                                        +\n" );
	printf( "+                                   `----__                  _-'         *                        *     +\n" );
	printf( "+                                          ------------------                                           +\n" );
	printf( "+                                                                                                       +\n" );
	printf( "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" );


	if( pipe( comPipes ) == 0 ) { //create pipes
	// seed random number generator
	seedRandGenerator( );

		pid_t navID = fork( );
		if ( navID == 0) { // create navigation, child 1
			
			pid_t lsID = fork( );
			if( lsID == 0 ) { // create life support – child 2
				struct tm currTime = *localtime( &t );
				float lsTime;

				char lsBuff[ MaxLen + 1 ];
				char *gasFormat = {"LS  : breathing gas levels adjustment	 	- Elapsed Time : %06.03f sec"};
				char *litFormat = {"LS  : temperature and lighting adjustment	- Elapsed Time : %06.3f sec"};
				int n = 1; 

				while( n > 0 ) {
					//adjust gas levels
					lsTime = getRandExponential() * 5.0;
					sleep( lsTime );
					sprintf(&lsBuff[0], gasFormat, lsTime);
					ret = write( comPipes[ PipeStdOut ], lsBuff, MaxLen );
					//adjust lighting and temperature
					lsTime = getRandExponential() * 4.0;
					sleep( lsTime );
					sprintf(&lsBuff[0], litFormat, lsTime);
					ret = write( comPipes[ PipeStdOut ], lsBuff, MaxLen );
					ret = wait( NULL );
					t = time(NULL);
					currTime = *localtime( &t );
					n--;
				} // end while - life support loop
			} //end life support process
			else {
				// child1 navigation
				struct tm currTime = *localtime( &t );
				float navTime;

				char navBuff[ MaxLen + 1 ];
				char *navFormat = {"NAV : navigation bearing adjustment		- Elapsed Time : %06.3f sec"};
				int n = 1; 

				while( n > 0 ) {
					navTime = getRandFloat() * 6.0;
					sleep( navTime );
					sprintf(&navBuff[0], navFormat, navTime);
					ret = write( comPipes[ PipeStdOut ], navBuff, MaxLen );
					t = time(NULL);
					currTime = *localtime( &t );
					n--;
				}  // end while – navigation loop
			} //end navigation process
			// code common to life support and navigation
			ret = wait( NULL );
		}
		else  { // bridge
			struct tm currTime = *localtime( &t );
			int knt = 3;
			while ( knt > 0 ) { //for later use startTime.tm_min + 1 > currTime.tm_min
				ret = read( comPipes[ PipeStdIn ], buff, MaxLen );
				printf( "%s", buff );
				t = time(NULL);
				currTime = *localtime( &t );
				printf( " RECIEVED AT BRIDGE [%02d:%02d:%02d]\n", currTime.tm_hour, currTime.tm_min, currTime.tm_sec);
				knt--;
			}  //end while
		}  // end bridge specific
		// code common to bridge and life support
		close( comPipes[ PipeStdIn ] );
		close( comPipes[ PipeStdOut ] );
	}
	return 0;
} // end program

