//---------------------------------------------------------------------------

#ifndef TSmainH
#define TSmainH
//---------------------------------------------------------------------------

const int TSAllocError	= 0; // <> de celle de LF

const int id_Euler 	= 1,
		id_Trapeze 	= 2,
		id_RK4 		= 3;

//---------------------------------------------------------------------------
	int __fastcall CallTransStab( int TSint, char * InN, char * OutN, char * GraphN);
//---------------------------------------------------------------------------
#endif
