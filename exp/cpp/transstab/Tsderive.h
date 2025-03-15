// Definition des classes derivees de TTS

#ifndef __TS_H
#include "ts.h"
#endif

#ifndef __TSDERIVE_H
#define __TSDERIVE_H

class TEuler : public TTS
{
protected :
virtual void ReserveMemory();
virtual void OutVersion();
virtual void IntegreSysteme();
void EulerPredictor();
void ModifiedEulerCorrector();

public :
TEuler( char * InN, char * OutN, char * GraphN );
virtual ~TEuler();
};

class TTrapeze : public TEuler
{
protected :
virtual void OutVersion();
virtual void IntegreSysteme();
void TrapezeCorrector();

public :
TTrapeze( char * InN, char * OutN, char * GraphN );
};


class TRK4 : public TTS
{
protected :
double * K1, * K2, * K3, * K4;

virtual void ReserveMemory();
virtual void OutVersion();
virtual void IntegreSysteme();

public :
TRK4( char * InN, char * OutN, char * GraphN );
virtual ~TRK4();
};

#endif

