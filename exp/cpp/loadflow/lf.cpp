/*
	TLF : Objet virtuel pure ( de base ) contenant les donnees du reseau et
	les methodes de calcul et d'affichage du Load Flow
*/

#pragma hdrstop

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <complex>
#include <cstring>
#include <cstdlib>
#include <conio.h>
#include <exception>

#include "LFmain.h"
#include "lf.h"

//-----------------------------------------------------------------------------
double argn( complex<double> z)
{
	if (abs(z)!=0) return arg(z);
		else	return 0;
}
//-----------------------------------------------------------------------------
TLF::TLF( char * InN, char * OutN )
{
  ErrorFlag=true;
	strcpy( InName,InN);
	strcpy( OutName,OutN);
	// associe le stream au fichier
OutFile.open( OutName, ios::out);
if ( !OutFile)	{
		cout << "Impossible to open "<< OutName <<" file : "<<"File Error";
		ErrorFlag=false;
		}
// Cannot call Virtual Methods from the constructor
}
//-----------------------------------------------------------------------------
TLF::~TLF()
{
// Libere l'espace memoire
int i;
	delete[] QOutFlag;
	for ( i=0; i<=N; i++)	delete[] List[i];
	delete[] List;
	delete[] Vmag;
	delete[] delta;
	delete[] Qgmin;
	delete[] Qgmax;
	delete[] Vb;
	for ( i=0; i<=N; i++)	delete[] SLN[i];
	delete[] SLN;
	for ( i=0; i<=N; i++)	delete[] Y[i];
	delete[] Y;
	delete[] S;
	delete[] Sg;
	delete[] Sd;
}
//-----------------------------------------------------------------------------
// Plus de memoire !
void TLF::NoMemory()
{
	cout <<  "Out of memory error : Error in Load Flow Program";
	ErrorFlag=false;
}
//-----------------------------------------------------------------------------
// Routine  qui cherche l'indice a partir du nom Bus
void TLF::LookUp( char * Nom, int & i )
{
for ( int j=0; j<=N; j++)
  {
  i=j;
  if ( strcmp( List[j],Nom)==0 ) return;
  if ( (List[j])[0]=='\0' )  { strcpy( List[i], Nom);
				  return;
				}
  }
  cout << "Names mismatch ( LookUp routine)" <<
				  " : Error in Load Flow Program";
  ErrorFlag=false;
  exit(1);
};
//-----------------------------------------------------------------------------
// Reserve l'espace memoire
void TLF::ReserveMemory()
{
int i,j;
	try {                                // TEST FOR EXCEPTIONS.
QOutFlag=new int[N1];
List=new (char * [N1]);
  for ( i=0; i<=N; i++)	{
								List[i]=new char[13];
								(List[i])[0]='\0';
								}
Vmag=new double [N1];
delta=new double[N1];
Qgmin=new double[N1];
Qgmax=new double[N1];
Vb=new double[N1];
SLN=new double * [N1];
	for ( i= 0; i<=N; i++)	SLN[i]=new double[N1];

Y=new complex<double> * [N1];
	for ( i=0; i<=N; i++)	Y[i]=new complex<double>[N1];
S=new complex<double> [N1];
Sg=new complex<double> [N1];
Sd=new complex<double> [N1];
		} // of try
	catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
		NoMemory();
		exit(-1);
		}

// initialisation
for ( i=0; i<=N; i++)
  {
  S[i]=0.0;
  Sg[i]=0.0;
  Sd[i]=0.0;
  Vb[i]=0;
  Qgmin[i]=0;
  Qgmax[i]=0;
 for ( j=0; j<=N; j++)
	{
	Y[i][j]=0.0;
	SLN[i][j]=0;
	}
  }
}
//-----------------------------------------------------------------------------
// lecture du body du fichier d'entree : donnees du reseau.
void TLF::ReadBody( ifstream& s)
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
	  break;

// Capacite
	 case 5 :
	  s >> Bus >> Bc0;
	  OutFile  <<" "
			<<setw(12)<< Bus <<"  "
			<<setw( 7)<< Bc0 <<"\n";
	  LookUp( Bus, i1);
	  Y[i1][i1]+=complex<double>( 0., Bc0);
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
			cout << "Identifier Code Not Excepted : File Format Error";
			ErrorFlag=false;
			exit(1);
	 }   // switch

  }  // while
}
//-----------------------------------------------------------------------------
void TLF::Lecture()
{
// Coz cannot call Virtual Methods from the constructor
	OutVersion();

	ifstream InFile ( InName, ios::in);
	if ( !InFile)	{
		cout << "Impossible to open "<< InName <<" file" << ": File Error";
		ErrorFlag=false;
		exit(1);
		}

	LFOpt=id_YMatrixOut;
	ReadHead( InFile);
	ReserveMemory();		// pourra t elle etre appelee a partir de ReadBody
								// en virtual casting ?
	ReadBody( InFile);
	InFile.close();
}
//-----------------------------------------------------------------------------
// Routine d'impression de la puissance injectee Sg+Sd
// et de la puissance maximale que peut transporter une ligne SLN
void TLF::OutPwo_SLN()
{
int i, j;
	OutFile << "\n\n----------  Power injected (pu)  ------------\n";
	OutFile <<
	 "Bus Name          real        imag        magnitude   arg(deg)\n";
	for ( i=0; i<=N; i++)
		OutFile <<setw( 2)<<i<<"  "<<setw(12)<< List[i] <<"  "
			 <<setw( 10)<< real(Sg[i]+Sd[i]) <<"  "
			<<setw( 10)<< imag(Sg[i]+Sd[i]) <<"  "
			 <<setw( 10)<< abs(Sg[i]+Sd[i]) <<"  "
			 <<setw( 10)<< argn(Sg[i]+Sd[i])*180/M_PI << "\n";

	OutFile << "\n----------  Line maximum rating  ----------\n";
	for ( i=0; i<=N; i++)
		for ( j=0; j<=N; j++)
		 if (i<j && SLN[i][j]!=0)
				 OutFile <<setw(12)<< List[i]<<" "<<setw(12)<< List[j] <<"  "
					 <<"SLN["<<setw( 2)<<i<<","<<setw( 2)<<j<<"]="
				 <<setw( 10)<< SLN[i][j] << "\n";
}
//-----------------------------------------------------------------------------
void TLF::OutY()
{
int i, j;
	OutFile << "\n---- YBUS matrix  ( no null elements )------\n";
	OutFile << "Busi         Busj                  real        imag        magnitude   arg(deg)\n";
	for ( i=0; i<=N; i++)
		for ( j=0; j<=N; j++)
		if ( Y[i][j]!=complex<double>(0,0) && i<=j )
		 OutFile <<setw(12)<< List[i]<<" "<<setw(12)<< List[j] <<" "
			<<"Y["<<setw( 2)<<i<<","<<setw( 2)<<j<<"]="
			 <<setw( 10)<< real(Y[i][j]) <<"  "
			 <<setw( 10)<< imag(Y[i][j]) <<"  "
			 <<setw( 10)<< abs(Y[i][j]) <<"  "
			 <<setw( 10)<< argn(Y[i][j])*180/M_PI << "\n";
}
//-----------------------------------------------------------------------------
void TLF::OutV_delta()   // applelee apres recalcul de Vc dans TransferPQ
{
	OutFile << "\n----------  Bus voltage  ----------\n";
	OutFile <<
	"Bus                 real        imag        magnitude   arg(deg)\n";
	for ( int i=0; i<=N; i++)
		OutFile <<setw(12)<< List[i] <<"  " <<"V["<<setw( 2)<<i<<"]="
			<<setw( 10)<< real( polar(Vmag[i], delta[i])) <<"  "
			<<setw( 10)<< imag( polar(Vmag[i], delta[i])) <<"  "
			<<setw( 10)<< Vmag[i] <<"  "
			<<setw( 10)<< delta[i]*180/M_PI << "\n";
}
//-----------------------------------------------------------------------------
void TLF::OutPow_Gen_Absorbed()   // affiche la puissance generee et consommee
{
complex<double> TotalSg=0, TotalSd=0;
	OutFile << "\n-------  Injected power at all buses  ----------\n";
	OutFile << "  Generated power :\n";
	OutFile <<
	"Bus                  real        imag        magnitude   arg(deg)\n";
	for ( int i=0; i<=N; i++)
		{
		OutFile <<setw(12)<< List[i] <<"  " <<"Sg["<<setw( 2)<<i<<"]="
			<<setw( 10)<< real(Sg[i]) <<"  "
			<<setw( 10)<< imag(Sg[i]) <<"  "
			<<setw( 10)<< abs(Sg[i])  <<"  "
			<<setw( 10)<< argn(Sg[i])*180/M_PI << "\n";
		TotalSg+=Sg[i];
		}
	OutFile << "\n  Power demand :\n";
	OutFile <<
	"Bus                  real        imag        magnitude   arg(deg)\n";
	for ( int i=0; i<=N; i++)
		{
		OutFile <<setw(12)<< List[i] <<"  " <<"Sd["<<setw( 2)<<i<<"]="
			<<setw( 10)<< real(Sd[i]) <<"  "
			<<setw( 10)<< imag(Sd[i]) <<"  "
			<<setw( 10)<< abs(Sd[i])  <<"  "
			<<setw( 10)<< argn(Sd[i])*180/M_PI << "\n";
		TotalSd+=Sd[i];
		}
	OutFile <<
	 "-------------------------------------------------------------------\n";
	OutFile <<
	"                     real        imag        magnitude   arg(deg)\n";
	OutFile << "Total generation     "
			<<setw( 10)<< real(TotalSg) <<"  "
			<<setw( 10)<< imag(TotalSg) <<"  "
			<<setw( 10)<< abs(TotalSg)  <<"  "
			<<setw( 10)<< argn(TotalSg)*180/M_PI << "\n";
	OutFile << "Total demand         "
			<<setw( 10)<< real(TotalSd) <<"  "
			<<setw( 10)<< imag(TotalSd) <<"  "
			<<setw( 10)<< abs(TotalSd)  <<"  "
			<<setw( 10)<< argn(TotalSd)*180/M_PI << "\n";
	OutFile << "AC losses            "
			<<setw( 10)<< real(TotalSg+TotalSd) <<"  "
			<<setw( 10)<< imag(TotalSg+TotalSd) <<"  "
			<<setw( 10)<< abs(TotalSg+TotalSd)  <<"  "
			<<setw( 10)<< argn(TotalSg+TotalSd)*180/M_PI << "\n";
}
//-----------------------------------------------------------------------------
void TLF::OutLineTransfer()
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
      Sij=conj( Y[i][j])*Vmag[i]*
			   (polar( Vmag[j], delta[i]-delta[j])-Vmag[i]);
      a=abs( Sij);
      b=fabs( SLN[i][j]);   // SLN en pu
      if ( i!=j && a!=0 )
		    OutFile
			 <<setw(12)<< List[i]<<" "<<setw(12)<< List[j] <<" "
		    <<"S["<<setw( 2)<<i<<","<<setw( 2)<<j<<"]="
		    <<setw( 10)<< real(Sij) <<"  "
		    <<setw( 10)<< imag(Sij) <<"  "
		    <<setw( 10)<< a <<"  "
				<<setw( 10)<< argn(Sij)*180/M_PI  <<"\n";
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
//-----------------------------------------------------------------------------
void TLF::OutResult()
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
			<<setw( 10)<< real( polar(Vmag[i], delta[i])) <<"  "
		   <<setw( 10)<< imag( polar(Vmag[i], delta[i])) <<"  "
		   <<setw( 10)<< Vmag[i] <<"  "
		   <<setw( 10)<< delta[i]*180/M_PI << "\n";
	   OutFile << "  Generated power : "
		   <<setw( 10)<< real(Sg[i]) <<"  "
		   <<setw( 10)<< imag(Sg[i]) <<"  "
		   <<setw( 10)<< abs(Sg[i])  <<"  "
		   <<setw( 10)<< argn(Sg[i])*180/M_PI << "\n";
	   OutFile << "  Power demand    : "
			<<setw( 10)<< real(Sd[i]) <<"  "
		   <<setw( 10)<< imag(Sd[i]) <<"  "
		   <<setw( 10)<< abs(Sd[i])  <<"  "
		   <<setw( 10)<< argn(Sd[i])*180/M_PI << "\n";
       OutFile <<
       "  Power transfer  : Bus name     Bus n'     real        imag\n";
	 // for each i bus
    for ( j=0; j<=N; j++)
      {
      Sij=conj( Y[i][j])*Vmag[i]*
			   (polar( Vmag[j], delta[i]-delta[j])-Vmag[i]);
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
//-----------------------------------------------------------------------------
// Genere le OutName.$$$ pour POWER DESIGNER 1.22
void TLF::MakeBackData()
{
char Name[81];
strncpy( Name, OutName, strlen( OutName)-3);
Name[strlen( OutName)-3]='\0';
strcat( Name, "$$$");
ofstream OutBack ( Name, ios::out);
if ( !OutBack)	{   cout << "Impossible to open " << Name << " file"<< " : File Error";
					exit(1);
					}
OutBack << N <<"  "<< Npv << "\n";
for ( int i=0; i<=N; i++)
		OutBack <<setw( 10)<< Vmag[i] <<"  "
			<<setw( 10)<< delta[i]*180/M_PI << "\n";
// SlackBus Power Generation
OutBack <<setw( 10)<< real(Sg[0]) <<"  "
	<<setw( 10)<< imag(Sg[0]) << "\n";
// PV Buses Power Generation
for ( int i=1; i<=Npv; i++)
	   OutBack <<setw( 10)<< imag(Sg[i]) <<"\n";
OutBack.close();
}
//-----------------------------------------------------------------------------
int TLF::IsItOK()
{
return ErrorFlag;
}
//-----------------------------------------------------------------------------

