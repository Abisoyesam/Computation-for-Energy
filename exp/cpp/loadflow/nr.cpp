/*
	TNR : Objet desdcendant de TLF
	Load Flow avec la methode de Newton-Raphson
*/

#pragma hdrstop

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <complex>
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include <conio.h>
#include <exception>

#include "LFmain.h"
#include "nr.h"

TNR::TNR( char * InN, char * OutN)
	:TLF( InN, OutN)
{
/*	AnsiString ph("Newton-Raphson method\n");
	ph+="for Power Designer 1.22\n";
	ph+="Programmed by Lotfi BAGHLI\n";
	Application->MessageBox( ph.c_str(), "LOAD FLOW PROGRAM",
				MB_OK);
*/
strcpy( Phrase, "Newton-Raphson method");
}

TNR::~TNR()
{
// Libere l'espace memoire ( Jacobien )
// !!!!! J, x, fpq !!!
	delete[] fpq;
	delete[] x;
	for ( int i=0; i<Nj; i++)	delete[] Jc[i];
	delete[] Jc;
}

void TNR::ReadHead( ifstream& s)
{
// lecture du nombre de bus N et Npv
s >> N1 >> Npv;
// lecture du nombre d'iterations max et de la tolerance
s >> NIterMax >> Tol; // Ceux de Gauss-Seidel
s >> NIterMax >> Tol; // Ceux de Newton-Raphson ecrasent ceux de G-S
N=N1-1;
Nj=2*N+1;
}

void TNR::ReserveMemory()
{
	TLF::ReserveMemory();	// appelle la reservation du pere
									// et cree Jc, fpq, x
try{
	Jc=new double * [Nj];
		for (int i= 0; i<Nj; i++)	Jc[i]=new double[Nj];
	fpq=new double[Nj];
	x=new double[Nj];
	}
catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
	NoMemory();
	exit(-1);
	}
}

void TNR::JacobNR()
{
int i, j;
for ( i=1; i<=N; i++)   // evite le slack bus
  {
// calcul de J1
  Jc[i][i]=0;
  for ( j=0; j<=N; j++)
		if ( i!=j ) {
		  Jc[i][i]+=abs(Y[i][j])*Vmag[j]*Vmag[i]*
				 sin( -argn(Y[i][j])-delta[j]+delta[i] );
		  if ( j!=0 ) Jc[i][j]=-abs(Y[i][j])*Vmag[j]*Vmag[i]*
						 sin( -argn(Y[i][j])-delta[j]+delta[i] );
		  }

// calcul de J2
  Jc[i][i+N]=-2*abs(Y[i][i])*Vmag[i]*cos( -argn(Y[i][i]));
  for ( j=0; j<=N; j++)
		if ( i!=j ) {
		  Jc[i][i+N]-=abs(Y[i][j])*Vmag[j]*
					cos( -argn(Y[i][j])-delta[j]+delta[i] );
		  if ( j!=0 ) Jc[i][j+N]=-abs(Y[i][j])*Vmag[i]*
						 cos( -argn(Y[i][j])-delta[j]+delta[i] );
		  }

// calcul de J3
  Jc[i+N][i]=0;
  for ( j=0; j<=N; j++)
		if ( i!=j ) {
		  Jc[i+N][i]-=abs(Y[i][j])*Vmag[j]*Vmag[i]*
					cos( -argn(Y[i][j])-delta[j]+delta[i] );
		  if ( j!=0 ) Jc[i+N][j]=abs(Y[i][j])*Vmag[j]*Vmag[i]*
						 cos( -argn(Y[i][j])-delta[j]+delta[i] );
		  }

// calcul de J4
  Jc[i+N][i+N]=-2*abs(Y[i][i])*Vmag[i]*sin( -argn(Y[i][i]));
  for ( j=0; j<=N; j++)
      if ( i!=j ) {
		  Jc[i+N][i+N]-=abs(Y[i][j])*Vmag[j]*
				sin( -argn(Y[i][j])-delta[j]+delta[i] );
		  if ( j!=0 ) Jc[i+N][j+N]=-abs(Y[i][j])*Vmag[i]*
						 sin( -argn(Y[i][j])-delta[j]+delta[i] );
		  }

  } // de for i
// eleminer axe 0,0 sauf Jc[0][0]=1 pour le Gauss
for (i=0; i<=2*N; i++)  {
				Jc[i][0]=0;
				Jc[0][i]=0;
				}
Jc[0][0]=1;

// elemination pour les noeuds pv or ELSE->( controlled buses with Pg=0 )
//                                i.e. componsateurs synchrones
for (i=1; i<=N; i++)
  if ( !QOutFlag[i] && ( i<=Npv || Qgmin[i]!=0 || Qgmax[i]!=0 ) )
	 // dans les limites et un controlled bus
  {
  for ( j=0; j<=2*N; j++)
	 {
	 Jc[i+N][j]=0;  // des lignes de dQ
	 Jc[j][i+N]=0;  // et des colonnes de dVmag
	 }
  Jc[i+N][i+N]=1;  // pour Gauss
  }
}

// calcul la puissance injectee au niveau du noeud i
complex<double> TNR::CalcS_injected( int i)
{
int j;
complex<double> sum=complex<double>(0,0);
for ( j=0; j<=N; j++)
	 sum+=conj( Y[i][j])*polar( Vmag[j], delta[i]-delta[j] );
S[i]=Vmag[i]*sum;
return S[i];
}

// Calcul des Sg aux Controlled buses
void TNR::CalcSg()
{
int i;
for ( i=0; i<=N; i++)
  if ( i<=Npv || Qgmin[i]!=0 || Qgmax[i]!=0 )
  // calcule Sg pour les Bus PV et componsateurs synchrones
  {
		//  S[i] --|
  if (i==0) Sg[0]=CalcS_injected( 0) -Sd[0];   // Calc S injected + S load
		 else Sg[i]=complex<double>( real(Sg[i]), imag( CalcS_injected( i)-Sd[i]) );
  }
}

// Determine les noeuds PV->PQ
void TNR::ControlNR()
{
int i;
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
		}
}

// Routine de calcul du mismatch
void TNR::CalcMismatch()
{ int i;
ControlNR();  // determine qui devient PQ et qui reste PV

for ( i=1; i<=N; i++)  	    CalcS_injected( i);
	// |- car pas tous les S[i] calculee dans ControlNR
fpq[0]=0;
for ( i=1; i<=N; i++)  // evite le slack
  {
  fpq[i]=real( Sg[i]+S d[i]-S[i]);
  fpq[i+N]=imag( Sg[i]+Sd[i]-S[i]);

  if ( !QOutFlag[i] && ( i<=Npv || Qgmin[i]!=0 || Qgmax[i]!=0 ) )
		fpq[i+N]=0;  // pas d'influence sur dV ie Bus PV
  }
}

// Calcul du plus grand  mismatch
void TNR::CalcWorstNR()
{
Worst=0;
IWorst=0;
for ( int i=1; i<=N; i++) // evite slack bus
  {
// attention fabs pour un flottant
// ajoutee a NRP5 teste aussi les PV devenus PQ
  if ( (i>Npv) || ( QOutFlag[i] && ( Qgmin[i]!=0 || Qgmax[i]!=0 ) ) )
		 if ( Worst<fabs( fpq[i+N]) ) {
					  Worst=fabs( fpq[i+N]);
					  IWorst=i;
					  }
  if ( Worst<fabs( fpq[i]))  {
				 Worst=fabs( fpq[i]);
				 IWorst=i;
				 }
  }
}

void TNR::Split()
{
for ( int i=1; i<=N; i++) // evite slack bus
  {
  delta[i]-=x[i];       // - car on resoud Jc*x=-fpq
			// a-=b <=> a=a-b
  Vmag[i]-=x[i+N];        // a PV bus can become PQ
  }
}

// Resolution du systeme AX=B par la triangularisation de Gauss
// sous routine Pivotation ligne-ligne
void TNR::Pivot( int k)
{
  int j, L=k+1;
  double M;
										//-----**** corrected 2*N a la place de N
  while ( Jc[L][k]==0 && L<2*N ) L++;
  if (L>=2*N && Jc[L][k]==0 ) {
		cout << "Impossible triangularisation : all pivots are null"
		  << " : Error in Load Flow Program";
		exit(1);
		}
  M=fpq[k];
  fpq[k]=fpq[L];
  fpq[L]=M;
  for ( j=k; j<=2*N; j++)  {
			 M=Jc[k][j];
			 Jc[k][j]=Jc[L][j];
			 Jc[L][j]=M;
			 }
}

void TNR::Gauss()
{
int k, i, j;
double M;
// Triangularisation de la matrice
  for (k=0; k<=2*N-1; k++)
     {
     if (Jc[k][k]==0)  Pivot( k);
     for (i=k+1; i<=2*N; i++)
	   {
		M=Jc[i][k]/Jc[k][k];
	   fpq[i]=fpq[i]-M*fpq[k];
	   for (j=k; j<=2*N; j++)  Jc[i][j]-=M*Jc[k][j];
	   }
     }
  //  Resoulution
  x[2*N]=fpq[2*N]/Jc[2*N][2*N];
  for (i=2*N-1; i>=0; i--)
     {
     x[i]=fpq[i];
     for (j=i+1; j<=2*N; j++)  x[i]-=Jc[i][j]*x[j];
     x[i]/=Jc[i][i];
     }
}



void TNR::OutVersion()
{
OutFile << "8 --------------  Load flow studies  -------------\n";
OutFile << "8 ------------  Newton Raphson method  -----------\n";
}

void TNR::OutJ()
{
int i, j;
  OutFile << "\n---------- Matrice J   ( no null elements ) ----------\n";
  for ( i=1; i<=2*N; i++)
    for ( j=1; j<=2*N; j++)
     if ( fabs(Jc[i][j])>=1E-10 &&
		!( (i>=1+N && i<=N+Npv) || (j>=1+N && j<=N+Npv) ))
	   OutFile <<"Jc["<<i<<","<<j<<"]=" << Jc[i][j] << "\n";
}

void TNR::OutMismatch()
{
int i;
  OutFile << "\n\n----------  Mismatch  ----------\n";
  for ( i=1; i<=2*N; i++)
      if ( fpq[i]!=0 && !( i>=1+N && i<=N+Npv ))
	   OutFile <<"fpq["<< setw(2) <<i<<"]=" << fpq[i] << "\n";
}

void TNR::OutX()
{
int i;
  OutFile << "\n----------  x vector  ( d_delta & d_V )  ----------\n";
  for ( i=1; i<=2*N; i++)
      if ( fpq[i]!=0 && !( i>=1+N && i<=N+Npv ))
	   OutFile <<"x["<< setw(2) <<i<<"]=" << x[i] << "\n";
}

void TNR::OutWorstNR()
{
OutFile <<"\nIteration Kount="<< Kount << "\n";
//cout    <<"\nIteration Kount="<< Kount << "\n";
OutFile <<"Max_Mismatch="<< Worst <<" at Bus n' "<< IWorst<< " : "
	<<setw(12)<< List[IWorst]<<"\n";
//cout	<<"Max_Mismatch="<< Worst <<" at Bus n' "<< IWorst<< " : "
//	<<setw(12)<< List[IWorst]<<"\n";
}

void TNR::Run()
{
int i;
	if (LFOpt & id_SLineMaxOut) OutPwo_SLN();
	if (LFOpt & id_YMatrixOut) OutY();

	// Initialisation de Vmag, delta
	for ( i=0; i<=N; i++)
		if (i<=Npv || Qgmin[i]!=0 || Qgmax[i]!=0 )
				 { Vmag[i]=Vb[i];
			 delta[i]=0;
				 }
		 else  { Vmag[i]=Vb[0]; // slack bus magn
			 delta[i]=0;
				 }
	// Debut de la simul; set the cursor to an hourglass
//	FormLoadFlow->ProgressBar1->Position=0;
//	HCURSOR oldCur = ::SetCursor(::LoadCursor(0, IDC_WAIT));
	Kount=0;
	// le temps initial
	start = clock();
	// la boucle principale
	do{
		if (LFOpt & id_V_auxOut)  OutV_delta();
		CalcMismatch(); //  appelle ControlNR()
	//  OutMismatch();
		CalcWorstNR();
		if (LFOpt & id_ArrilagaOut) OutWorstNR();
		if ( Worst<=Tol || Kount>=NIterMax ) break;	// sort de la boucle
//		FormLoadFlow->ProgressBar1->Position=100.0*Kount/NIterMax;
//		FormLoadFlow->Invalidate();
		Kount++;
		JacobNR();
		if (LFOpt & id_JacobienOut)   OutJ();  // affiche le jacobien
		// resoud
		Gauss();
		Split();  //  transforme x en d_delta et d_V et update
		if (LFOpt & id_JacobienOut)  OutX();
		} while (1);
	// fin des iterations : affiche les resultats

	end = clock();
// fin de la simul; restaure cursor
//	::SetCursor(oldCur);

	if (LFOpt & id_ArrilagaOut) OutResult();
	if (LFOpt & id_VOut)  OutV_delta();
	if (LFOpt & id_Sg_SdOut) OutPow_Gen_Absorbed();
	if (LFOpt & id_LineTransOut)  OutLineTransfer();

	OutFile <<"\nNumber of iterations : "<< Kount<< "\n";
	OutFile <<"Total time : "<< (end - start)/CLK_TCK << " s\n";
	OutFile <<"Mean time per iteration : "<<(end - start)/CLK_TCK/Kount<< " s\n";
	// Ferme le fichier de sortie
	OutFile.close();
	MakeBackData();
}

