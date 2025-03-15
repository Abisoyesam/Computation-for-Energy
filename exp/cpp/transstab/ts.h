// Definition de la classe Mere TransStab abstraite

#include <fstream>
#include <complex>
#include <time.h>


#ifndef __TS_H
#define __TS_H
using namespace std;
const int
  NobsMax = 4,

		id_YMatrixOut  =   1,
		id_MachineOut  =   2,
		id_V_auxOut    =   4,
		id_LFErrorOut  =   8,
		id_TrapezeOut  =  16,

  id_FaultedBus   = 1,
  id_OpenedLine   = 2,
  id_SwitchedLoad = 4,
  id_DroppedLoad  = 8,
  id_LossGen      = 16;

double argn( complex<double> z);
//-----------------------------------------------------------------------------
class TTS
{
protected :
	int ErrorFlag,
		 Npv, N, N1, Npv1,
		 If, Ip, Iq,
		 ISL, IDL, ILG,
		 FBFlag, LOFlag, SLFlag, DLFlag, LGFlag,
		 FaultType, NiterMax,
		 * MachObs, Nobs;
	unsigned int TSOpt;
    char InName[81], OutName[81], OutGraphName[81];
    char * * List;
	double Worst, Tol, eps, // Tol->Trapeze et eps->GaussSeidel
			 dt, t2, t, freq, Ws, GS_AccelFact,
			 FBt0, FBt1, Gf, Bf,      // Faulted Bus ( 3 phases)
			 LOt0, LOt1,              // Opened Line
			 SLt0, SLt1, SLPL, SLQL,  // Switched Load
			 DLt0, DLt1, DLPL, DLQL,  // Dropped Load
			 LGt0, LGt1,         	// Loss Gen
			 * Vmag, * delta,
			 * H, * D, * Pm, * Pg,
			 * Xv, * Xvp, * Xvc, * dXvp, * dXvc;
	complex<double> YsaveIf, YsaveIpp, YsaveIqq, YsaveIpq, YsaveIqp,
		YsaveISL, YsaveIDL, YsaveILG,
		ys_OpenIpIq, ys_OpenIqIp, ysh_OpenIpIq, ysh_OpenIqIp,
		* * Y, * S, * Sg, * Sd, * Vc,
		* Eg, * yg,
		* yL;
	ofstream OutFile, GraphFile;
	clock_t start, end;

	void NoMemory();
	virtual void OutVersion() =0;
	void LookUp( char * Nom, int & i );
	void ReadHead( ifstream& s);
	void ReadBody( ifstream& s);

	void VerifPVH();
	void SaveY();  // sauve les elements de la Matrice Y pour les defauts
	void CalcYload(); // Calcule les admitances de charge equivalentes
	void ModifY();
	// Calcule les Eg, Pm initiaux	and Set initial State Variables Xv
	void CalcInit();
	void ModifY_Default(); // Dans Y inclu des defauts en dynamique !
	void LFGauss(); // Ecoulement de puissance avec Gauss-Seidel
	void CalcPg(); // Calcule Pg a partir de Eg

	virtual void IntegreSysteme()=0;

	// routines de sortie
	void OutY();
	void OutV_delta();
	void OutGraphInit();
	void OutXvGraph();
	void OutEgPgVmag();
	void OutModif();
	void OutYg();
	void OutYL();

	virtual void ReserveMemory();

public :
	TTS( char * InN, char * OutN, char * GraphN );
	virtual ~TTS();
	int IsItOK();
    char Phrase[81];
	virtual void Lecture();
	virtual void Run();
};
#endif

