// Definition de la classe derivee Gauss-Seidel

#include "lf.h"

#ifndef __GS_H
#define __GS_H

class TGS : public TLF
{
protected :
int IDvMax;
double DvMax;
complex<double>	* Vc, * Vc1,
	* * YL, * KL;


virtual void ReadHead( ifstream& s);
virtual void ReserveMemory();

complex<double> CalcS_injected( int i);
void CalcSg();

void FormKL_YL();
void RecopVc();
void CalcVc1();
virtual void MakeBackData();


// routines de sortie
virtual void OutVersion();
virtual void OutV_delta();
void OutDvMax();
void OutKL_YL();
virtual void OutLineTransfer();
virtual void OutResult();

public :
TGS( char * InN, char * OutN);
virtual ~TGS();
virtual void Run();

};
#endif

