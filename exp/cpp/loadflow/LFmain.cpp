//---------------------------------------------------------------------------
#pragma hdrstop
#pragma argsused

#include <iostream>
#include <string.h>
#include "LFmain.h"
#include "lf.h"
#include "fdl.h"
#include "gs.h"
#include "nr.h"
#include "nrb.h"

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <stdio.h>

//---------------------------------------------------------------------------
//  Programme de calcul d'ecoulement de puissance version ligbe de commande
// Lotfi BAGHLI 06/2020
int __fastcall CallLoadFlow( int LFint, char * InN, char * OutN)
{
TLF * LFProg;
  switch( LFint)
		{
		case id_GS :	LFProg=new TGS( InN, OutN); break;
		case id_NR :	LFProg=new TNR( InN, OutN); break;
		case id_NRB :	LFProg=new TNRB( InN, OutN); break;
		case id_FDL :	LFProg=new TFDL( InN, OutN); break;
		default :	return AllocError;
		}
	if (LFProg==NULL) {
					cout << "Out of memory error" << " : Error in calling the Load Flow Program";
					exit(1);
					}
	cout << "Methode : " <<LFProg->Phrase;
	// LFType=LFint
	LFProg->Lecture();
	//	Application->MessageBox( "Lecture OK", "LOAD FLOW PROGRAM", MB_OK);
	LFProg->Run();
	//	Application->MessageBox( "Run OK", "LOAD FLOW PROGRAM", MB_OK);

	int ReturnFlag=LFProg->IsItOK();
	delete LFProg;

//	//debug
//	char SS[150];
//	strcpy( SS, "notepad.exe ");
//	strcat( SS, OutN);
//	WinExec( SS, SW_SHOW);

	return ReturnFlag;
}
//---------------------------------------------------------------------------
 int _tmain(int argc, _TCHAR* argv[])
{
	if (argc==4)
		{
//		string SS( argv[1]);
		int LFint= atoi(argv[1]);
//		string InN( argv[2]);
//		string OutN( argv[3]);
		CallLoadFlow( LFint, argv[2], argv[3]);
		}

	return 0;
}
//---------------------------------------------------------------------------


