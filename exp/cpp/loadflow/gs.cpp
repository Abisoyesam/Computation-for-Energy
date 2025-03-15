/*
	TGS : Objet desdcendant de TLF
	Load Flow avec la methode de Gauss-Seidel
*/

#pragma hdrstop

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <complex>
#include <cstring>
#include <stdlib.h>
#include <conio.h>
#include <exception>

#include "LFmain.h"
#include "gs.h"


TGS::TGS( char * InN, char * OutN)
	:TLF( InN, OutN)
{
/*	AnsiString ph("Gauss-Seidel method\n");
	ph+="for Power Designer 1.22\n";
	ph+="Programmed by Lotfi BAGHLI\n";
	Application->MessageBox( ph.c_str(), "LOAD FLOW PROGRAM",
				MB_OK);
*/
strcpy( Phrase, "Gauss-Seidel method");
}

TGS::~TGS()
{
// Libere l'espace memoire
	delete[] KL;
	delete[] Vc;
	delete[] Vc1;
	for ( int i=0; i<N1; i++)	delete[] YL[i];
	delete[] YL;
}

void TGS::ReadHead( ifstream& s)
{
int i;
double x;
// lecture du nombre de bus N et Npv
s >> N1 >> Npv;
// lecture du nombre d'iterations max et de la tolerance
s >> NIterMax >> Tol; // Ceux de Gauss-Seidel
s >> i >> x; // Ceux de Newton-Raphson n'ecrasent pas ceux de G-S
N=N1-1;
}

void TGS::ReserveMemory()
{
int i, j;
	TLF::ReserveMemory();	// appelle la reservation du pere
									// et cree KL, Vc, Vc1, YL
try{
	KL=new complex<double> [N1];
	Vc=new complex<double> [N1];
	Vc1=new complex<double> [N1];
	YL=new complex<double> * [N1];
		for ( i= 0; i<N1; i++)	YL[i]=new complex<double>[N1];
	}
catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
	NoMemory();
	exit(-1);
	}
// Initialiser
for ( i=0; i<=N; i++)
  {
  KL[i]=complex<double>(0,0);
  Vc[i]=complex<double>(0,0);
  Vc1[i]=complex<double>(0,0);
	for ( j=0; j<=N; j++)	YL[i][j]=complex<double>(0,0);
  }
}

void TGS::FormKL_YL()
{
int i, j;
for (i=0; i<=N; i++)
  for (j=0; j<=N; j++)
	  if (i==j)  KL[i]=conj(Sg[i]+Sd[i])/Y[i][i];
	 else   YL[i][j]=Y[i][j]/Y[i][i];
}

// calcul la puissance injectee au niveau du noeud i
complex<double> TGS::CalcS_injected( int i)
{
int j;
complex<double> sum=complex<double>(0,0);
for ( j=0; j<=N; j++)
		sum+=Y[i][j]*Vc[j];
S[i]=Vc[i]*conj( sum);
return S[i];
}

// Calcul des Sg aux Controlled buses
void TGS::CalcSg()
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

void TGS::RecopVc()
{
for ( int i=1; i<=N; i++)   Vc[i]=Vc1[i];
}

void TGS::CalcVc1()
{
 int i, j;
 double delta, Qcalc, Dv;
 complex<double> Vnew, tempo;

DvMax=0;
for ( i=1; i<=N; i++)   // evite slack bus
  {
  if (i<=Npv || Qgmin[i]!=0 || Qgmax[i]!=0 )
		// special PV & controlled buses
	 {
	 if (Vc[i]!=complex<double>(0,0))	delta=arg(Vc[i]);
				else	delta=0;
	 Vnew=polar(Vb[i], delta);
	 tempo=Y[i][i]*Vnew;
	 for ( j=0; j<=N; j++)
		if ( i!=j ) tempo+=Y[i][j]*Vc[j];
	 Qcalc=imag( Vnew*conj(tempo) -Sd[i]);   // Sd !!!
	 if ( Qgmin[i]==0 && Qgmax[i]==0) Vc[i]=Vnew;
		  else
		  if ( Qcalc<=Qgmax[i] )
			if ( Qcalc>=Qgmin[i] )  Vc[i]=Vnew;
				else Qcalc=Qgmin[i];
			else Qcalc=Qgmax[i];

	 Sg[i]=complex<double>(real(Sg[i]), Qcalc);
	 KL[i]=conj(Sg[i]+Sd[i])/Y[i][i];
	 }
  // for all buses
  Vc1[i]=KL[i]/conj(Vc[i]);
  for ( j=0; j<=i-1; j++)   Vc1[i]-=YL[i][j]*Vc1[j];
  for ( j=i+1; j<=N; j++)   Vc1[i]-=YL[i][j]*Vc[j];
  // recherche de l'ecart max
  Dv=abs( Vc1[i]-Vc[i] );

  if (Dv>DvMax )
		{
		Vc1[i]=Vc[i]+GS_AccelFact*( Vc1[i] - Vc[i] );
		DvMax=Dv;
		IDvMax=i;
		}
  }
}

void TGS::OutVersion()
{
OutFile << "8 --------------  Load flow studies  -------------\n";
OutFile << "8 -------------  Gauss Seidel method  ------------\n";
}

void TGS::OutV_delta()   // applelee apres recalcul de recop Vc
{
  OutFile << "\n----------  Bus voltage  ----------\n";
  OutFile <<
  "Bus                 real        imag        magnitude   arg(deg)\n";
  for ( int i=0; i<=N; i++)
		OutFile <<setw(12)<< List[i] <<"  " <<"V["<<setw( 2)<<i<<"]="
			<<setw( 10)<< real(Vc[i]) <<"  "
			<<setw( 10)<< imag(Vc[i]) <<"  "
			<<setw( 10)<< abs(Vc[i]) <<"  "
			<<setw( 10)<< ((Vc[i]!=complex<double>(0,0)) ?	arg(Vc[i])*180/M_PI : 0) << "\n";
}

// affiche ecarts max
void TGS::OutDvMax()
{
OutFile <<"\nIteration Kount="<< Kount << "\n";
// cout    <<"\nIteration Kount="<< Kount << "\n";
OutFile <<"DvMax="<< DvMax <<" at Bus n' "<< IDvMax << " : "
	<<setw(12)<< List[IDvMax]<<"\n";
// cout    <<"DvMax="<< DvMax <<" at Bus n' "<< IDvMax << " : "
//	<<setw(12)<< List[IDvMax]<<"\n";
}

void TGS::OutKL_YL()
{
int i, j;
OutFile << "\n-------- KL elements  ----------\n";
OutFile << "Busi                real        imag\n";
for (i=0; i<=N; i++)
	OutFile    <<setw(12)<< List[i]<<" "<<"KL["<<setw( 2)<<i<<"]="
			<<setw( 10)<< real(KL[i]) <<"  "
			<<setw( 10)<< imag(KL[i]) <<"\n";

OutFile << "\n-------- YL elements  ----------\n";
OutFile << "Busi         Busj                   real        imag\n";
for (i=0; i<=N; i++)
  for (j=0; j<=N; j++)
	if (YL[i][j]!=complex<double>(0,0)) OutFile
			<<setw(12)<< List[i]<<" "<<setw(12)<< List[j] <<" "
			<<"YL["<<setw( 2)<<i<<","<<setw( 2)<<j<<"]="
			<<setw( 10)<< real(YL[i][j]) <<"  "
			<<setw( 10)<< imag(YL[i][j]) <<"\n";
}

void TGS::OutLineTransfer()
{
int i,j, Problems=0;
double a, b;
complex<double> Sij;
  OutFile << "\n-----  Line power transfer ( bus i --> j at i  -----\n";
  OutFile <<
"Busi         Busj                  real        imag        mag (pu)    arg(deg)\n";
  for ( i=0; i<=N; i++)
    for ( j=0; j<=N; j++)
      {
		Sij=conj( Y[i][j])*Vc[i]*conj( Vc[j]-Vc[i]);
		a=abs( Sij);
		b=fabs( SLN[i][j]);   // SLN en pu
      if ( i!=j && a!=0 )
		    OutFile
		    <<setw(12)<< List[i]<<" "<<setw(12)<< List[j] <<" "
		    <<"S["<<setw( 2)<<i<<","<<setw( 2)<<j<<"]="
			 <<setw( 10)<< real(Sij) <<"  "
		    <<setw( 10)<< imag(Sij) <<"  "
				<<setw( 10)<< a <<"  "
				<<setw( 10)<< ((Sij!=complex<double>(0,0)) ?	arg(Sij)*180/M_PI : 0)  <<"\n";
			if ( a>b && b!=0 )
		    {
		    Problems=1;
		    OutFile
		    <<"Power["<<setw( 2)<<i<<","<<setw( 2)<<j<<"]="
		    <<setw( 10)<< a<<" > Smax["<<setw( 2)<<i<<","
		    <<setw( 2)<<j<<"]="<<setw( 10)<< b
		    <<" WARNING excess power transfer\n";
		    }
      }
if ( !Problems) OutFile << "  No excess power transfer\n";
}

void TGS::OutResult()
{
int i, j;
complex<double> Sij;
double a, b;
 OutFile
<< "\n-----------------------  Output Bus Data  -------------------------\n"
   <<"-------------------------------------------------------------------\n";
  for ( i=0; i<=N; i++)
	   {
	   OutFile << "Bus name : "<<setw(12)<< List[i]
		   << "         Bus number : "<<setw( 2)<<i<<"\n";
	   OutFile <<
	"                    real        imag        magnitude   arg(deg)\n";
	   OutFile << "  Voltage         : "
		   <<setw( 10)<< real(Vc[i]) <<"  "
		   <<setw( 10)<< imag(Vc[i]) <<"  "
		   <<setw( 10)<< abs(Vc[i]) <<"  "
			 <<setw( 10)<< ((Vc[i]!=complex<double>(0,0)) ?	arg(Vc[i])*180/M_PI : 0) << "\n";
		 OutFile << "  Generated power : "
		   <<setw( 10)<< real(Sg[i]) <<"  "
		   <<setw( 10)<< imag(Sg[i]) <<"  "
		   <<setw( 10)<< abs(Sg[i])  <<"  "
			<<setw( 10)<< ((Sg[i]!=complex<double>(0,0)) ?	arg(Sg[i])*180/M_PI : 0) << "\n";
		 OutFile << "  Power demand    : "
		   <<setw( 10)<< real(Sd[i]) <<"  "
		   <<setw( 10)<< imag(Sd[i]) <<"  "
		   <<setw( 10)<< abs(Sd[i])  <<"  "
			 <<setw( 10)<< ((Sd[i]!=complex<double>(0,0)) ?	arg(Sd[i])*180/M_PI : 0) << "\n";
			 OutFile <<
       "  Power transfer  : Bus name     Bus n'     real        imag\n";
    // for each i bus
    for ( j=0; j<=N; j++)
      {
		Sij=conj( Y[i][j])*Vc[i]*conj( Vc[j]-Vc[i]);
		a=abs( Sij);
		b=fabs( SLN[i][j]);   // SLN en pu
      if ( i!=j && a!=0 )
		    OutFile
		    <<"        to        : "<<setw(12)<< List[j] <<" "
		    <<setw( 2)<<j<<"         "
		    <<setw( 10)<< real(Sij) <<"  "
		    <<setw( 10)<< imag(Sij) <<"\n";
		if ( a>b && b!=0 )
		    {
		    OutFile
		    <<"                    Power="
		    <<setw( 10)<< a<<" > Smax="<<setw( 10)<< b
		  <<"\n                    WARNING excess power transfer\n";
		    }
      }
      OutFile <<
      "              Total Mismatch                real        imag\n";
       // 4 chiffres apres la virgule
      OutFile << "                                            "
      << setprecision( 4)
      <<setw( 10)<< real( Sg[i]+Sd[i]-S[i]) <<"  "
      <<setw( 10)<< imag( Sg[i]+Sd[i]-S[i]) <<"\n"
      << setprecision( 5);
		OutFile <<
	 "-------------------------------------------------------------------\n";
		} // fin de for ( i..)
}


// Genere le OutName.$$$ pour POWER DESIGNER 1.14
void TGS::MakeBackData()
{
char Name[81];
strncpy( Name, OutName, strlen( OutName)-3);
Name[strlen( OutName)-3]='\0';
strcat( Name, "$$$");
ofstream OutBack ( Name, ios::out);
if ( !OutBack)	{
					cout << "Impossible to open "<< Name<<" file : "<< "File Error";
					exit(1);
					}
OutBack << N <<"  "<< Npv << "\n";
for ( int i=0; i<=N; i++)
		OutBack <<setw( 10)<< abs(Vc[i]) <<"  "
			<<setw( 10)<< ((Vc[i]!=complex<double>(0,0)) ?	arg(Vc[i])*180/M_PI : 0) << "\n";
// SlackBus Power Generation
OutBack <<setw( 10)<< real(Sg[0]) <<"  "
	<<setw( 10)<< imag(Sg[0]) << "\n";
// PV Buses Power Generation
for ( int i=1; i<=Npv; i++)
		OutBack <<setw( 10)<< imag(Sg[i]) <<"\n";
OutBack.close();
}

void TGS::Run()
{
int i;
if (LFOpt & id_SLineMaxOut) OutPwo_SLN();
if (LFOpt & id_YMatrixOut) OutY();
FormKL_YL();
if (LFOpt & id_KL_YLOut) OutKL_YL();

// Initialisation de Vmag, delta
for ( i=0; i<=N; i++)
	if (i<=Npv || Qgmin[i]!=0 || Qgmax[i]!=0 )
		Vc[i]=complex<double>(Vb[i], 0);
		 else     Vc[i]=complex<double>(Vb[0], 0); // slack bus magn
Vc1[0]=Vc[0];     // Slack will never change !

//Application->MessageBox("TRying to RUN", "GS Load Flow PROGRAM",MB_OK);

// Debut de la simul; set the cursor to an hourglass
//	FormLoadFlow->ProgressBar1->Position=0;
//	HCURSOR oldCur = ::SetCursor(::LoadCursor(0, IDC_WAIT));


Kount=0;
// le temps initial
start = clock();

// la boucle principale
do{
  CalcVc1();
//	FormLoadFlow->ProgressBar1->Position=100.0*Kount/NIterMax;
//	FormLoadFlow->Invalidate();

  Kount++;
  RecopVc();
	if (LFOpt & id_V_auxOut)  OutV_delta();
  if (LFOpt & id_ArrilagaOut)  OutDvMax();

  if ( DvMax<=Tol || Kount>=NIterMax)  break; // sort de la boucle

  }while (1);
// fin des iterations : affiche les resultats

end = clock();
// fin de la simul; restaure cursor
//	::SetCursor(oldCur);
// calcule les puissances injectees et generees avec les methodes GS
CalcSg();
for ( i=1; i<=N; i++)  CalcS_injected( i);

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

