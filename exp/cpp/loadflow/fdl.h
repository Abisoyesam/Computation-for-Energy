// Definition de la classe derivee FDL ( de Newton-Raphson )

#include "nr.h"

#ifndef __FDL_H
#define __FDL_H

class  TFDL : public TNR
{
protected :
double * * Bp, * * Bpp, * * BppS,
		 * fp, * fq, * ddelta, * dV;

virtual void ReserveMemory();
virtual void ReadBody( ifstream& s);

void Inverse( double * * A);
void ReformBpp();
void Calc_fp_Worst();
void Calc_fq_Worst();
void Solve_P_UpDateDelta();
void Solve_Q_UpDateV();

// routines de sortie
virtual void OutVersion();
void Outddelta();
void OutdV();

public :
TFDL( char * InN, char * OutN );
virtual ~TFDL();
virtual void Run();

};
#endif

