/*
	TNRB : Objet desdcendant de TNR
	Load Flow avec la methode du Newton-Raphson with back off
*/

#pragma hdrstop

#include <iostream>
#include <math.h>
#include <complex>
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include <conio.h>

#include "LFmain.h"
#include "nrb.h"


TNRB::TNRB( char * InN, char * OutN)
	:TNR( InN, OutN)
{
/*	AnsiString ph("NRB method\n");
	ph+="for Power Designer 1.22\n";
	ph+="Programmed by Lotfi BAGHLI - feb 1996\n";
	Application->MessageBox( ph.c_str(), "LOAD FLOW PROGRAM",
				MB_OK);
*/
strcpy( Phrase, "Newton-Raphson with back off method");
}

// Determine les noeuds PV->PQ
void TNRB::ControlNR()
{
int i;
double Vold, Qcalc;
// special controlled Buses
for ( i=0; i<=N; i++)    QOutFlag[i]=0;  // au debut aucun depassement en Qg
// si depassement at controlled Bus ( PV or else ), on remplace puis flag->1

if (!( Kount>=1 && ( Worst< Tol*20 || Worst< 0.2 )))  return;
// Proche de la convergence, on teste les depassement des Qg lim
// Calcule Sg pour les controlled buses
CalcSg();
for ( i=1; i<=N; i++)
 if ( Qgmin[i]!=0 || Qgmax[i]!=0 )
		{
  if ( imag(Sg[i])<Qgmin[i] ) {
					QOutFlag[i]=1;
					Sg[i]=complex<double>(real(Sg[i]), Qgmin[i]);
					}
  if ( imag(Sg[i])>Qgmax[i] ) {
					QOutFlag[i]=1;
					Sg[i]=complex<double>(real(Sg[i]), Qgmax[i]);
					}
// il faut voir la possibilite de retour PQ vers un PV
		// special PV & controlled buses
// __________++++++++++________
	if ( Vmag[i]!=Vb[i] )
	{
//	cout<<" testing "<<i<<" Vmag="<<Vmag[i]<<" -> Vb="<<Vb[i];
	OutFile<<" testing "<<i<<" Vmag="<<Vmag[i]<<" -> Vb="<<Vb[i];
	Vold=Vmag[i]; // sauve
	Vmag[i]=Vb[i];

	Qcalc=imag( CalcS_injected( i)-Sd[i]);
	if (  Qcalc<Qgmin[i] || Qcalc>Qgmax[i] ) {
//					cout<<" didn't pass "<<Vold<<"\n";
					OutFile<<" didn't pass "<<Vold<<"\n";
					Vmag[i]=Vold;
						 }
					else {
//					cout<<" passed "<<Vmag[i]<<"\n";
					OutFile<<" passed "<<Vmag[i]<<"\n";
					}
	// si depassement remet l'ancien Vmag
	}
// __________++++++++++________
		} // if Contolled bus
}

void TNRB::OutVersion()
{
OutFile << "8 --------------  Load flow studies  -------------\n";
OutFile << "8 ---  Newton Raphson method ( Backoff variant ) ---\n";
}

