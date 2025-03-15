//---------------------------------------------------------------------------
#pragma hdrstop
#pragma argsused

#include <iostream>
#include "TSmain.h"
#include "ts.h"
#include "Tsderive.h"

#ifdef _WIN32
#include <tchar.h>
#else
typedef char _TCHAR;
  #define _tmain main
#endif
//---------------------------------------------------------------------------
/*  Programme de stabilite transitoire
	Lotfi BAGHLI 06/2020
*/
int __fastcall CallTransStab( int TSint, char * InN, char * OutN, char * GraphN) {
TTS * TSProg;
  switch( TSint)
		{
		case id_Euler 	:	TSProg=new TEuler( InN, OutN, GraphN ); break;
		case id_Trapeze :	TSProg=new TTrapeze( InN, OutN, GraphN ); break;
		case id_RK4 		:	TSProg=new TRK4( InN, OutN, GraphN ); break;
		default :	return TSAllocError;
		}
	if (TSProg==NULL) {
                    cout << "Out of memory error" << " : Error in calling the Transient Stability Program";
					exit(1);
					}
    cout << "Methode : " <<TSProg->Phrase;
	TSProg->Lecture();
	//	Application->MessageBox( "Lecture OK", "Transient Stability PROGRAM", MB_OK);
	TSProg->Run();
	//	Application->MessageBox( "Run OK", "Transient Stability PROGRAM", MB_OK);

	int ReturnFlag=TSProg->IsItOK();
	delete TSProg;

//	//debug
//	char SS[150];
//	strcpy( SS, "notepad.exe ");
//	strcat( SS, OutN);
//	WinExec( SS, SW_SHOW);

//	char SS[150];
//	strcpy( SS, "graph.exe ");
//	strcat( SS, GraphN);
//  WinExec( SS.c_str(), SW_SHOW);

	return ReturnFlag;
}
//---------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    if (argc==5)
    {
//		string SS( argv[1]);
        int TSint= atoi(argv[1]);
//		string InN( argv[2]);
//		string OutN( argv[3]);
//		string GraphN( argv[4]);
        CallTransStab( TSint, argv[2], argv[3], argv[4]);
    }

    return 0;
}
//---------------------------------------------------------------------------

