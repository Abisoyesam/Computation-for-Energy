#include <iomanip>
#include <cstring>

#ifndef __TSDERIVE_H
#include "tsderive.h"
#endif

//-----------------------------------------------------------------------------
// Euler 
//-----------------------------------------------------------------------------
TEuler::TEuler( char * InN, char * OutN, char * GraphN )
		 :TTS( InN, OutN, GraphN)
{
/*	string ph("Euler method\n");
	ph+="for Power Designer 1.3\n";
	ph+="Programmed by Lotfi BAGHLI\n";
	Application->MessageBox( ph.c_str(), "Trans Stab PROGRAM",
				MB_OK);
*/
strcpy( Phrase, "Modified Euler method");
}
//-----------------------------------------------------------------------------
TEuler::~TEuler()
{
	delete[] Xvc;
	delete[] dXvc;
}
//-----------------------------------------------------------------------------
void TEuler::OutVersion()
{
OutFile << "8 ---------  Transient Stability studies  --------\n";
OutFile << "8 ---------     Modified Euler method     --------\n";
}
//-----------------------------------------------------------------------------
void TEuler::ReserveMemory()
{
	TTS::ReserveMemory();	// appelle la reservation du pere
	try {                                // TEST FOR EXCEPTIONS.
		// Varibles d'etats Xv ie arg( Eg) et vitesse w
		Xvc=new double [Npv1*2];
		dXvc=new double [Npv1*2];
		} // of try
	catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
		NoMemory();
		exit(-1);
		}
}
//-----------------------------------------------------------------------------
// avec Xv[i] -> arg( Eg[i] )
// et Xv[Npv+1] -> sa derivee ie W
void TEuler::EulerPredictor()
{
	for ( int i=0; i<=Npv; i++)
		{
		dXvp[i]=Xv[Npv1+i]-Ws;          //   |--Amortissement
		dXvp[Npv1+i]=Ws/2/H[i]*( Pm[i]-Pg[i]-D[i]*dXvp[i]);
		Xvp[i]=Xv[i]+dXvp[i]*dt;
		Xvp[Npv1+i]=Xv[Npv1+i]+dXvp[Npv1+i]*dt;
		}
}
//-----------------------------------------------------------------------------
void TEuler::ModifiedEulerCorrector()
{
complex<double> Ig;
int i;
	for ( i=0; i<=Npv; i++)
		{
	// avec boucles non separees
			 dXvc[i]=Xvp[Npv1+i]-Ws;         //   |--Amortissement
			 dXvc[Npv1+i]=Ws/2/H[i]*( Pm[i]-Pg[i]-D[i]*dXvc[i]);  // the new Pg
			 Xv[i]+=( dXvp[i]+dXvc[i] )*dt/2;
			 Xv[Npv1+i]+=( dXvp[Npv1+i]+dXvc[Npv1+i] )*dt/2;

			 Eg[i]=polar( abs( Eg[i]), Xv[i] );
			 // une seul passage only pour la Modified Euler
		}
	// A l'exterieur de la boucle pour Booster la vitesse
	LFGauss();
CalcPg();
}
//-----------------------------------------------------------------------------
void TEuler::IntegreSysteme()
{
  EulerPredictor();
  // new Eg                   avec les angle Xvp -----|
  for ( int i=0; i<=Npv; i++)  Eg[i]=polar( abs( Eg[i]), Xvp[i] );
  LFGauss();
  CalcPg(); // new Pg for the corrector

  t+=dt; // incremente time
  // les trapezes mais une fois seulement donc Modified Euler
  ModifiedEulerCorrector();
  // la remise a jour Eg, LFGAuss et CalcPG dans ModifiedEulerCorrector()
}
//-----------------------------------------------------------------------------
// Trap�ze
//-----------------------------------------------------------------------------
TTrapeze::TTrapeze( char * InN, char * OutN, char * GraphN )
		 :TEuler( InN, OutN, GraphN)
{
/*	AnsiString ph("Trapeze method\n");
	ph+="for Power Designer 1.3\n";
	ph+="Programmed by Lotfi BAGHLI\n";
	Application->MessageBox( ph.c_str(), "Trans Stab PROGRAM",
				MB_OK);
*/
	strcpy( Phrase, "Trapeze method");
}
//-----------------------------------------------------------------------------
void TTrapeze::OutVersion()
{
	OutFile << "8 ---------  Transient Stability studies  --------\n";
	OutFile << "8 ---------    Trapeze Corrector method   --------\n";
}
//-----------------------------------------------------------------------------
void TTrapeze::TrapezeCorrector()
{
complex<double> Ig;
int i, a;
for ( i=0; i<=Npv; i++)
  {
// avec boucles non separees
  do   {
		 a=0;  // Flag de sortie
		 dXvc[i]=Xvp[Npv1+i]-Ws;         //   |--Amortissement
		 dXvc[Npv1+i]=Ws/2/H[i]*( Pm[i]-Pg[i]-D[i]*dXvc[i]);  // the new Pg
		 Xvc[i]=Xv[i]+( dXvp[i]+dXvc[i] )*dt/2;
		 Xvc[Npv1+i]=Xv[Npv1+i]+( dXvp[Npv1+i]+dXvc[Npv1+i] )*dt/2;

		 if ( fabs( Xvp[i]-Xvc[i])<=Tol )  a++;
		 if ( fabs( Xvp[Npv1+i]-Xvc[Npv1+i])<=Tol && a)  break;

		 Xvp[i]=Xvc[i];
		 Xvp[Npv1+i]=Xvc[Npv1+i];
		 Eg[i]=polar( abs( Eg[i]), Xvp[i] );
		 LFGauss();
		 // new Pg for the corrector
		 Ig=( Eg[i]-polar( Vmag[i], delta[i]) )*yg[i];
		 if ( LGFlag==1 && i==ILG ) Ig=0;  // Gen debranche
		 Pg[i]=real( Eg[i]*conj( Ig) );
		 // recalcule le correcteur

		 } while(1);
  // apres convergence de l'arg et de la vitesse
  Xv[i]=Xvc[i];
  Xv[Npv1+i]=Xvc[Npv1+i];
  if (TSOpt & id_TrapezeOut)
	OutFile <<"      Max Xvc-Xvp [" << setw(2)<< i << "] =";
  if (TSOpt & id_TrapezeOut)
	if (fabs( Xvp[i]-Xvc[i]) > fabs( Xvp[Npv1+i]-Xvc[Npv1+i]) )
			OutFile << fabs( Xvp[i]-Xvc[i]) <<"\n";
	 else OutFile <<fabs( Xvp[Npv1+i]-Xvc[Npv1+i]) <<"\n";
  }
}
//-----------------------------------------------------------------------------
void TTrapeze::IntegreSysteme()
{
	int i;
  // Euler predictor
  EulerPredictor();
  // new Eg                   avec les angle Xvp -----|
  for ( i=0; i<=Npv; i++)  Eg[i]=polar( abs( Eg[i]), Xvp[i] );
  LFGauss();
  CalcPg(); // new Pg for the corrector

  t+=dt; // incremente time

  // les trapezes avec plusieurs iteration ci necessaire !
  TrapezeCorrector();
  // remet a jour Eg apres trapeze
  for ( i=0; i<=Npv; i++)  Eg[i]=polar( abs( Eg[i]), Xv[i] );
  LFGauss();
  CalcPg();
}
//-----------------------------------------------------------------------------
// Runge-Kutta 4
//-------------------------------------------------------------------
TRK4::TRK4( char * InN, char * OutN, char * GraphN )
		 :TTS( InN, OutN, GraphN)
{
/*	AnsiString ph("Runge Kutta 4 method\n");
	ph+="for Power Designer 1.3\n";
	ph+="Programmed by Lotfi BAGHLI\n";
	Application->MessageBox( ph.c_str(), "Trans Stab PROGRAM",
				MB_OK);
*/
	strcpy( Phrase, "Runge Kutta 4 method");
}
//-----------------------------------------------------------------------------
TRK4::~TRK4()
{
	delete[] K1;
	delete[] K2;
	delete[] K3;
	delete[] K4;
}
//-----------------------------------------------------------------------------
void TRK4::OutVersion()
{
	OutFile << "8 ---------  Transient Stability studies  --------\n";
	OutFile << "8 ---------      Runge Kutta method       --------\n";
}
//-----------------------------------------------------------------------------
void TRK4::ReserveMemory()
{
	TTS::ReserveMemory();	// appelle la reservation du pere
	try {                                // TEST FOR EXCEPTIONS.
		K1=new double [Npv1*2];
		K2=new double [Npv1*2];
		K3=new double [Npv1*2];
		K4=new double [Npv1*2];
		} // of try
	catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
		NoMemory();
		exit(-1);
		}
}
//-----------------------------------------------------------------------------
void TRK4::IntegreSysteme()
{
complex<double> Ig;
int i;
// Boucle K1
	 for ( i=0; i<=Npv; i++)
		 {
		 dXvp[i]=Xv[Npv1+i]-Ws;         //   |--Amortissement
		 dXvp[Npv1+i]=Ws/2/H[i]*( Pm[i]-Pg[i]-D[i]*dXvp[i]);  // the new Pg
		 K1[i]=dXvp[i]*dt;
		 K1[Npv1+i]=dXvp[Npv1+i]*dt;

		 Xvp[i]=Xv[i]+K1[i]/2;
		 Xvp[Npv1+i]=Xv[Npv1+i]+K1[Npv1+i]/2;

		 Eg[i]=polar( abs( Eg[i]), Xvp[i] );
		 }
	 LFGauss();
	 CalcPg();
//    t+=dt/2; // une 1iere demi-incrementation

// Boucle K2
	 for ( i=0; i<=Npv; i++)
		 {
		 dXvp[i]=Xvp[Npv1+i]-Ws;         //   |--Amortissement
		 dXvp[Npv1+i]=Ws/2/H[i]*( Pm[i]-Pg[i]-D[i]*dXvp[i]);  // the new Pg
		 K2[i]=dXvp[i]*dt;
		 K2[Npv1+i]=dXvp[Npv1+i]*dt;

		 Xvp[i]=Xv[i]+K2[i]/2;
		 Xvp[Npv1+i]=Xv[Npv1+i]+K2[Npv1+i]/2;

		 Eg[i]=polar( abs( Eg[i]), Xvp[i] );
		 }
	 LFGauss();
	 CalcPg();

// Boucle K3
	 for ( i=0; i<=Npv; i++)
		 {
		 dXvp[i]=Xvp[Npv1+i]-Ws;         //   |--Amortissement
		 dXvp[Npv1+i]=Ws/2/H[i]*( Pm[i]-Pg[i]-D[i]*dXvp[i]);  // the new Pg
		 K3[i]=dXvp[i]*dt;
		 K3[Npv1+i]=dXvp[Npv1+i]*dt;

		 Xvp[i]=Xv[i]+K3[i];
		 Xvp[Npv1+i]=Xv[Npv1+i]+K3[Npv1+i];

		 Eg[i]=polar( abs( Eg[i]), Xvp[i] );
		 }
	 LFGauss();
	 CalcPg();
//    t+=dt/2; // une 2eme demi-incrementation

// Boucle K4 et fin
	 for ( i=0; i<=Npv; i++)
		 {
		 dXvp[i]=Xvp[Npv1+i]-Ws;         //   |--Amortissement
		 dXvp[Npv1+i]=Ws/2/H[i]*( Pm[i]-Pg[i]-D[i]*dXvp[i]);  // the new Pg
		 K4[i]=dXvp[i]*dt;
		 K4[Npv1+i]=dXvp[Npv1+i]*dt;
		 // Speciale : UpDate Xv
		 Xv[i]+=( K1[i]+2*( K2[i]+K3[i] )+K4[i] )/6;
		 Xv[Npv1+i]+=( K1[Npv1+i]+2*( K2[Npv1+i]+K3[Npv1+i] )+K4[Npv1+i] )/6;

		 Eg[i]=polar( abs( Eg[i]), Xv[i] );
		 }
	 LFGauss();
	 CalcPg();
// On incremente une fois pour toute le t externe
t+=dt;
}

