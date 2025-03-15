// Definition de la classe derivee Newton-Raphson

#include "lf.h"

#ifndef __NR_H
#define __NR_H

class TNR : public TLF
{
protected :
int Nj;
double * * Jc, * fpq, * x;

virtual void ReadHead( ifstream& s);
virtual void ReserveMemory();

complex<double> CalcS_injected( int i);
void CalcSg();

void JacobNR();
virtual void ControlNR();
void CalcMismatch();
void CalcWorstNR();
void Split();
void Pivot( int k);
void Gauss();
// routines de sortie
virtual void OutVersion();
virtual void OutJ();
virtual void OutMismatch();
virtual void OutX();
virtual void OutWorstNR();

public :
TNR( char * InN, char * OutN );
virtual ~TNR();
virtual void Run();

};
#endif

