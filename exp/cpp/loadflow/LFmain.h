//---------------------------------------------------------------------------

#ifndef LFmainH
#define LFmainH
//---------------------------------------------------------------------------


// code des methodes
const int id_GS  = 0,
			id_NR  = 1,
			id_NRB = 2,
			id_FDL = 3;
const int AllocError	= 0;
// Constantes LoadFlow Output Options
const int id_YMatrixOut  =   1,
		id_KL_YLOut    =   2,
		id_JacobienOut =   4,
		id_SLineMaxOut =   8,
		id_Sg_SdOut    =  16,
		id_LineTransOut=  32,
		id_VOut        =  64,
		id_V_auxOut    =  128,
		id_ArrilagaOut =  256;
//---------------------------------------------------------------------------
	int __fastcall CallLoadFlow( int LFint, char * InN, char * OutN);
//---------------------------------------------------------------------------
#endif
