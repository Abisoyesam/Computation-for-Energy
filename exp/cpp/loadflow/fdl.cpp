/*
	TFDL : Objet desdcendant de TNR
	Load Flow avec la methode du Fast Decoupled Load Flow
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
#include "fdl.h"


TFDL::TFDL( char * InN, char * OutN)
	:TNR( InN, OutN)
{
/*	AnsiString ph("FDL method\n");
	ph+="for Power Designer 1.22\n";
	ph+="Programmed by Lotfi BAGHLI - feb 1996\n";
	Application->MessageBox( ph.c_str(), "LOAD FLOW PROGRAM",
				MB_OK);
*/
strcpy( Phrase, "Fast Decoupled Load Flow method");
}

TFDL::~TFDL()
{
// Libere l'espace memoire ( pseudo Jacobien )
	delete[] fp;
	delete[] fq;
	delete[] ddelta;
	delete[] dV;
	for ( int i=0; i<=N; i++)	{
										delete[] Bp[i];
										delete[] Bpp[i];
										delete[] BppS[i];
										}
	delete[] Bp;
	delete[] Bpp;
	delete[] BppS;
}

void TFDL::ReserveMemory()
{
int i, j;
	TLF::ReserveMemory();	// appelle la reservation du grand pere
									// et cree Bp, Bpp, BppS
									// fp, fq, ddelta, dV
try{
	Bp=new double * [N1];
	Bpp=new double * [N1];
	BppS=new double * [N1];
	for (i= 0; i<=N; i++)	{
									Bp[i]=new double[N1];
									Bpp[i]=new double[N1];
									BppS[i]=new double[N1];
									}
	fp=new double[N1];
	fq=new double[N1];
	ddelta=new double[N1];
	dV=new double[N1];
	}
catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
		NoMemory();
		exit(-1);
		}

// Initialiser
for ( i=0; i<=N; i++)
  for ( j=0; j<=N; j++)
				{
				Bp[i][j]=0;
				BppS[i][j]=0;
				}
}

// lecture du body du fichier d'entree : donnees du reseau.
// Special pour le FDL
void TFDL::ReadBody( ifstream& s)
{
  int Code, i1, i2;
  char Bus[13], Bus1[13], Bus2[13];
  char Comment[81];
  double Pg0, Qg0, PL0, QL0, Vb0, Rs0, Xs0, Gs0, Bs0, Bc0,
	 Amag0, Adelta0, SLN0,
	 Qgmin0, Qgmax0;
  complex<double> a0, y0;


  OutFile.setf(ios::left, ios::adjustfield);
  OutFile << setprecision( 5);  // 5 chifres apres la virgule
  while ( !s.eof())
  {
  s >>  Code;
  if (s.eof()) break;
  OutFile <<setw( 2)<< Code;

  switch ( Code)
	 {
// Bus
	 case 1 :
	  s >> Bus >> Pg0 >> Qg0 >> PL0 >> QL0 >> Vb0;
	  OutFile  <<" "
			<<setw(12)<< Bus <<"  "
			<<setw( 7)<< Pg0 <<"  " <<setw( 7)<< Qg0 <<"  "
			<<setw( 7)<< PL0 <<"  " <<setw( 7)<< QL0 <<"  "
			<<setw( 7)<< Vb0 <<"\n";
	  LookUp( Bus, i1);
	  Sg[i1]=complex<double>( Pg0, Qg0);
	  Sd[i1]=complex<double>(-PL0,-QL0);
	  Vb[i1]=Vb0;
	  break;

// Controlled Bus
	 case 2 :
	  s >>  Bus >> Qgmin0 >> Qgmax0;
	  OutFile  <<" "<<setw(12)<< Bus <<"  "
			<<setw( 7)<< Qgmin0 <<"  " <<setw( 7)<< Qgmax0 <<"\n";
	  LookUp( Bus, i1);
	  Qgmin[i1]=Qgmin0;
	  Qgmax[i1]=Qgmax0;
	  break;

// Line
	 case 3 :
	  s >>  Bus1 >> Bus2 >> Rs0 >> Xs0 >> Gs0 >> Bs0 >> SLN0;
	  OutFile  <<" "<<setw(12)<< Bus1 <<"  " <<setw(12)<< Bus2 <<"  "
			<<setw( 7)<< Rs0 <<"  " <<setw( 7)<< Xs0 <<"  "
			<<setw( 7)<< Gs0 <<"  "<<setw( 7)<< Bs0 <<"  "
			<<setw( 7)<< SLN0 <<"\n";
	  LookUp( Bus1, i1);
	  LookUp( Bus2, i2);
	  SLN[i1][i2]=SLN0;
	  SLN[i2][i1]=SLN0;
	  if ( Rs0 !=0 || Xs0 !=0 )
				  y0=1.0 / complex<double>( Rs0, Xs0);
			else y0=complex<double>(0,0);
	  Y[i1][i1]+=y0+complex<double>( Gs0, Bs0);
	  Y[i2][i2]+=y0+complex<double>( Gs0, Bs0);
	  Y[i1][i2]-=y0;
	  Y[i2][i1]-=y0;
	  if ( Xs0 !=0 )  {
			   Bp[i1][i1]+=1/Xs0;
			   Bp[i2][i2]+=1/Xs0;
				Bp[i1][i2]-=1/Xs0;
			   Bp[i2][i1]-=1/Xs0;
			  }
	  BppS[i1][i1]-=imag(y0)+Bs0;
	  BppS[i2][i2]-=imag(y0)+Bs0;
	  BppS[i1][i2]+=imag(y0);
	  BppS[i2][i1]+=imag(y0);
	  break;

// Transfo
	 case 4 :
	  s >>  Bus1 >> Bus2 >> Rs0 >> Xs0 >> Amag0 >> Adelta0 >> SLN0;
	  OutFile  <<" "<<setw(12)<< Bus1 <<"  " <<setw(12)<< Bus2 <<"  "
			<<setw( 7)<< Rs0 <<"  "<<setw( 7)<< Xs0 <<"  "
			<<setw( 7)<< Amag0 <<"  " <<setw( 6)<< Adelta0 <<"  "
			<<setw( 7)<< SLN0 <<"\n";
	  LookUp( Bus1, i1);
	  LookUp( Bus2, i2);
	  SLN[i1][i2]=SLN0;
	  SLN[i2][i1]=SLN0;
	  a0=polar( Amag0, Adelta0*M_PI/180);
	  if ( Rs0 !=0 || Xs0 !=0 )
				  y0=1.0 / complex<double>( Rs0, Xs0);
			else y0=complex<double>(0,0);

	  Y[i1][i1]+=y0/a0/conj(a0);
	  Y[i2][i2]+=y0;
	  Y[i1][i2]-=y0/conj(a0);
	  Y[i2][i1]-=y0/a0;

	  if ( Xs0 !=0 )  {
			  // nominal tap in Bp
			   Bp[i1][i1]+=1/Xs0;
			   Bp[i2][i2]+=1/Xs0;
			   Bp[i1][i2]-=1/Xs0;
			   Bp[i2][i1]-=1/Xs0;
			  }
	  // no phase shifting in Bpp
	  BppS[i1][i1]-=imag(y0/a0/conj(a0));
	  BppS[i2][i2]-=imag(y0);
	  BppS[i1][i2]+=imag(y0/abs(a0));
	  BppS[i2][i1]+=imag(y0/abs(a0));
	  break;

// Capacite
	 case 5 :
	  s >> Bus >> Bc0;
	  OutFile  <<" "
			<<setw(12)<< Bus <<"  "
			<<setw( 7)<< Bc0 <<"\n";
	  LookUp( Bus, i1);
	  Y[i1][i1]+=complex<double>( 0., Bc0);
	  BppS[i1][i1]-=Bc0; // reactance Shunt
	  break;

// Commentaire
	 case 8 :
	  s.getline( Comment, 80, '\n');
	  OutFile.seekp( OutFile.tellp()-1 );
	  OutFile << Comment << "\n";
	  break;
// GS Accelerator Factor
	 case 9 :
	  s >> GS_AccelFact;
	  OutFile  <<" " << GS_AccelFact <<"\n";
	  break;

// LoadFlow OutPut Options
	 case 10 :
	  s >> LFOpt;
	  OutFile  <<" " << LFOpt <<"\n";
	  break;

	 default :
			cout << "Identifier Code Not Excepted" << " File Format Error";
			ErrorFlag=false;
			exit(1);
	 }   // switch

  }  // while
}


// Permet le calcul de l'inverse de A
void TFDL::Inverse( double * * A)
{
int i, j, k;
double M, * * B, * * C;

	try {                                // TEST FOR EXCEPTIONS.
	B=new double * [N1];
	C=new double * [N1];
		for ( i=0; i<=N; i++)	{
										B[i]=new double[N1];
										C[i]=new double[N1];
										}
		} // of try
	catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
		NoMemory();
		exit(-1);
		}

for (i=0; i<=N; i++)
  for (j=0; j<=N; j++)
	 if ( i==j )  B[i][i]=1;
	else	 B[i][j]=0;

// Triangularisation des matrices A et B
  for (k=0; k<=N-1; k++)
	  {
	  if (A[k][k]==0)  {
			 // Permutation ligne-ligne
			 i=k+1;
			 while ( (A[i][k]==0) && (i<=N) )  i++;
			      if (i>N) {
						 cout << "Impossible inversion : all pivots are null"
								<< 	"Computing Error";
						 ErrorFlag=false;
						 exit(1);
						 }
			 for ( j=k; j<=N; j++)  {
						M=A[k][j];
						A[k][j]=A[i][j];
						A[i][j]=M;
						}
			 for ( j=1; j<=N; j++)  {
						M=B[k][j];
						B[k][j]=B[i][j];
						B[i][j]=M;
						}
			 // fin de permutation ligne ligne
			 }

     for (i=k+1; i<=N; i++)
	   {
		M=A[i][k]/A[k][k];
		for (j=k; j<=N; j++)  A[i][j]-=M*A[k][j];
		for (j=1; j<=N; j++)  B[i][j]-=M*B[k][j];
	   }
     }
// Fin de la triangularisation, calcul de l'inverse
for (k=1; k<=N; k++)
  {
  C[N][k]=B[N][k]/A[N][N];
  for (i=N-1; i>=1; i--)
	  {
     C[i][k]=B[i][k];
	  for (j=i+1; j<=N; j++)  C[i][k]-=A[i][j]*C[j][k];
	  C[i][k]/=A[i][i];
	  }
  }
// Recop C->A
for (i=1; i<=N; i++)
  for (j=1; j<=N; j++)  A[i][j]=C[i][j];

// Release B et C
for ( i=0; i<=N; i++)	{
								delete[] B[i];
								delete[] C[i];
								}
delete[] B;
delete[] C;
}

// S'ocuppe de controller les PV->PQ et d'inverser Bpp
void TFDL::ReformBpp()
{
int i, j;
for (i=0; i<=N; i++)     // retire la version store de Bpp
  for (j=0; j<=N; j++)   Bpp[i][j]=BppS[i][j];

for (i=0; i<=N; i++)  {
				Bpp[0][i]=0;
				Bpp[i][0]=0;
				}
Bpp[0][0]=1;
// appelle le calcul des puissances
ControlNR();
// elemination pour les noeuds pv or ELSE->( controlled buses with Pg=0 )
//                                i.e. componsateurs synchrones
for (i=1; i<=N; i++)
  if ( !QOutFlag[i] && ( i<=Npv || Qgmin[i]!=0 || Qgmax[i]!=0 ) )
	 // dans les limites et un controlled bus
  {
  for ( j=1; j<=N; j++)
	 {
	 Bpp[i][j]=0;  // des lignes de dQ
	 Bpp[j][i]=0;  // et des colonnes de dVmag
	 }
  Bpp[i][i]=1;  // pour L'inversion
  }
Inverse( Bpp);
}

// Calcul du vecteur mismatch fp et du Worst
void TFDL::Calc_fp_Worst()
{
double dP;
for ( int i=1; i<=N; i++)  	    CalcS_injected( i);
	// |- car pas tous les S[i] calculee dans ControlNR
fp[0]=0;
Worst=0;
IWorst=0;
for ( int i=1; i<=N; i++)  // evite le slack
  {
  dP=real( Sg[i]+Sd[i]-S[i]);
  fp[i]=dP/Vmag[i];
  if ( Worst<fabs( dP))  {
				 Worst=fabs( dP);
				 IWorst=i;
				 }
  }
}

// Calcul du vecteur mismatch fp et du Worst
void TFDL::Calc_fq_Worst()
{
double dQ;
ReformBpp();    // refait l'inversion
		// et determine qui devient PQ et qui reste PV
for ( int i=1; i<=N; i++)  	    CalcS_injected( i);
	// |- car pas tous les S[i] calculee dans ControlNR
fq[0]=0;
Worst=0;
IWorst=0;
for ( int i=1; i<=N; i++)  // evite le slack
  {
  dQ=imag( Sg[i]+Sd[i]-S[i]);
  fq[i]=dQ/Vmag[i];
  if ( !QOutFlag[i] && ( i<=Npv || Qgmin[i]!=0 || Qgmax[i]!=0 ) )
		fq[i]=0;  // pas d'influence sur dV ie Bus PV
  if ( (i>Npv) || ( QOutFlag[i] && ( Qgmin[i]!=0 || Qgmax[i]!=0 ) ) )
		 if ( Worst<fabs( dQ) ) {
					   Worst=fabs( dQ);
						IWorst=i;
						}
  }
}

void TFDL::Solve_P_UpDateDelta()
{
for ( int i=1; i<=N; i++) // evite slack bus
    {
    ddelta[i]=0;
    for ( int j=1; j<=N; j++) // evite slack bus
			 ddelta[i]+=Bp[i][j]*fp[j];
    delta[i]+=ddelta[i];  // + car on resoud fp=Bp*ddelta
    }
}

void TFDL::Solve_Q_UpDateV()
{
for ( int i=1; i<=N; i++) // evite slack bus
	 {
	 dV[i]=0;
	 for ( int j=1; j<=N; j++) // evite slack bus
			 dV[i]+=Bpp[i][j]*fq[j];
	 Vmag[i]+=dV[i];       // + car on resoud fp=Bp*ddelta
	 }
}


void TFDL::OutVersion()
{
OutFile << "8 --------------  Load flow studies  -------------\n";
OutFile << "8 ------------  Fast Decoupled method  -----------\n";
}

void TFDL::Outddelta()
{
int i;
OutFile << "\n----------  ddelta vector  ---------\n";
  for ( i=1; i<=N; i++)
		if ( fp[i]!=0) OutFile <<setw(12)<< List[i]<<" "
			  <<"ddelta["<< setw(2) <<i<<"]=" << ddelta[i] << "\n";
}

void TFDL::OutdV()
{
int i;
OutFile << "\n----------  dV vector  ---------\n";
  for ( i=1; i<=N; i++)
		if ( fq[i]!=0) OutFile <<setw(12)<< List[i]<<" "
			  <<"dV["<< setw(2) <<i<<"]=" << dV[i] << "\n";
}

void TFDL::Run()
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
	// elemine le slack bus ( 0 ) de Bp
	for (i=0; i<=N; i++)  {
					Bp[i][0]=0;
					Bp[0][i]=0;
					}
	Bp[0][0]=1;
	// Must inverse Bp and test for PV->PQ before inversing Bpp
	Inverse( Bp);
	ReformBpp();  // special coz it changes
	// Debut de la simul; set the cursor to an hourglass
//	FormLoadFlow->ProgressBar1->Position=0;
//	HCURSOR oldCur = ::SetCursor(::LoadCursor(0, IDC_WAIT));

	Kount=0;
	int KP=1,  KQ=1;
	// le temps initial
	start = clock();

	// la boucle principale
	do{
	// Boucle de resolution de fp=Bp*ddelta
	if (LFOpt & id_V_auxOut)  OutV_delta();
		Calc_fp_Worst(); //  appelle ControlNR()
	if (LFOpt & id_ArrilagaOut)  OutWorstNR();
		if ( Worst<=Tol || Kount>=NIterMax ) {
							 KP=0;
							 if (KQ==0)  break; //sort de la boucle
							 }
			else {
					Solve_P_UpDateDelta();
					if (LFOpt & id_JacobienOut)  Outddelta();
					KQ=1;
					Kount++;
					}
	// Boucle de resolution de fq=Bpp*dV
	if (LFOpt & id_V_auxOut)  OutV_delta();
		Calc_fq_Worst(); //  appelle ReformBpp()
	if (LFOpt & id_ArrilagaOut)  OutWorstNR();
		if ( Worst<=Tol || Kount>=NIterMax ) {
							 KQ=0;
							 if (KP==0)  break; //sort de la boucle
							 }
			else {
					Solve_Q_UpDateV();
					if (LFOpt & id_JacobienOut)  OutdV();
					KP=1;
					Kount++;
					}

//		FormLoadFlow->ProgressBar1->Position=100.0*Kount/NIterMax;
//		FormLoadFlow->Invalidate();
		}while (1);
	// fin des iterations : affiche les resultats

	end = clock();
	// fin de la simul; restaure cursor
//		::SetCursor(oldCur);

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

