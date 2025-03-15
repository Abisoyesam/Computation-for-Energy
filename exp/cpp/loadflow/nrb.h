// Definition de la classe derivee FDL ( de Newton-Raphson )

#include "nr.h"

#ifndef __NRB_H
#define __NRB_H

class  TNRB : public TNR
{
protected :
virtual void ControlNR();		// virtualy called by TNR::Run();

// routines de sortie
virtual void OutVersion();


public :
TNRB( char * InN, char * OutN );
};
#endif
