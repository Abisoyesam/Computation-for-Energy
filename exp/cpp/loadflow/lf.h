// Definition de la classe Mere LoadFlow abstraite

#include <fstream>
#include <complex>
#include <time.h>

#ifndef __LF_H
#define __LF_H

using namespace std;
double argn( complex<double> z);
//-----------------------------------------------------------------------------
class TLF
{
protected :
// N=1+Npv+Npq
int Npv, N, N1, NIterMax,
	 * QOutFlag, ErrorFlag,
	 Kount, IWorst;
unsigned int LFOpt, LFType;
char Ans;
char InName[81], OutName[81];
char * * List;
double Worst, Tol, GS_AccelFact,
		 * Vmag, * delta,
		 * Vmax, * Vmin,
		 * Qgmin, * Qgmax, * Vb,
		 * * SLN;
complex<double> * * Y, * S, * Sg, * Sd;
ofstream OutFile;
clock_t start, end;

	void NoMemory();
	void LookUp( char * Nom, int & i );
	virtual void ReadHead( ifstream& s) =0;
	virtual void ReserveMemory();
	virtual void ReadBody( ifstream& s);
	virtual void MakeBackData();

	// routines de sortie
	virtual void OutVersion() =0;
	virtual void OutPwo_SLN();
	virtual void OutY();
	virtual void OutV_delta();
	virtual void OutPow_Gen_Absorbed();
	virtual void OutLineTransfer();
	virtual void OutResult();

	public :
	TLF( char * InN, char * OutN );
	virtual ~TLF();
	char Phrase[81];
	int IsItOK();
	virtual void Lecture();
	virtual void Run() =0;
};
#endif

