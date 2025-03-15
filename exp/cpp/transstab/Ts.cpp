/*
	TTS : Objet virtuel pure ( de base ) contenant les donnees du reseau et
	les methodes de calcul et de sortie
	de la simulation de stabilite transitoire
*/
#pragma hdrstop

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <complex>
#include <cstring>
#include <cstring>
#include <cstdlib>
#include <conio.h>
#include <exception>

#include "TSmain.h"
#include "ts.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double argn( complex<double> z)
{
    if (abs(z)!=0) return arg(z);
    else	return 0;
}
//-----------------------------------------------------------------------------
TTS::TTS( char * InN, char * OutN, char * GraphN )
{
    ErrorFlag=true;
    strcpy( InName,InN);
    strcpy( OutName,OutN);
    strcpy( OutGraphName,GraphN);
    // associe le stream au fichier
    OutFile.open( OutName, ios::out);
    if ( !OutFile)	{
        cout << "Impossible to open "<< OutName <<" file : "<<"File Error";
        ErrorFlag=false;
    }
    GraphFile.open ( OutGraphName, ios::out);
    if ( !GraphFile) {
        cout << "Impossible to open "<< OutName <<" OutGraphName : "<<"File Error";
        ErrorFlag=false;
    }
// Cannot call Virtual Methods from the constructor
}
//-----------------------------------------------------------------------------
TTS::~TTS()
{
    int i;
// Libere l'espace memoire
    for ( i=0; i<=N; i++)	delete[] List[i];
    delete[] List;
    delete[] Vmag;
    delete[] delta;
    for ( i=0; i<=N; i++)	delete[] Y[i];
    delete[] Y;
    delete[] S;
    delete[] Sg;
    delete[] Sd;
    // Special Trans Stab
    delete[] H;
    delete[] D;
    delete[] yg;
    // Liste int des machines a observer
    delete[] MachObs;
    // only PV & Slack Buses
    delete[] Eg;
    delete[] Pm;
    delete[] Pg;
    delete[] Xv;
    delete[] Xvp;
    delete[] dXvp;
    // All buses
    delete[] yL;
    delete[] Vc;
}
//-----------------------------------------------------------------------------
// Plus de memoire !
void TTS::NoMemory()
{
    cout <<  "Out of memory error : Error in Load Flow Program";
    ErrorFlag=false;
}
//-----------------------------------------------------------------------------
// Routine  qui cherche l'indice a partir du nom Bus
void TTS::LookUp( char * Nom, int & i )
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
}
//-----------------------------------------------------------------------------
void TTS::ReadHead( ifstream& s)
{
    s >> N1 >> Npv;
    N=N1-1;
    Npv1=Npv+1;
    Nobs=0;
}
//-----------------------------------------------------------------------------
// Reserve l'espace memoire
void TTS::ReserveMemory()
{
    int i,j;
    try {                                // TEST FOR EXCEPTIONS.
        List=new (char * [N1]);
        for ( i=0; i<=N; i++)	{
            List[i]=new char[13];
            (List[i])[0]='\0';
        }
        Vmag=new double [N1];
        delta=new double[N1];
        Y=new complex<double> * [N1];
        for ( i=0; i<=N; i++)	Y[i]=new complex<double>[N1];
        S=new complex<double> [N1];
        Sg=new complex<double> [N1];
        Sd=new complex<double> [N1];
        // Special Trans Stab
        H=new double [Npv1];
        D=new double [Npv1];
        yg=new complex<double> [Npv1];
        // Liste int des machines a observer
        MachObs=new int [NobsMax+1];
        // only PV & Slack Buses
        Eg=new complex<double> [Npv1];
        Pm=new double [Npv1];
        Pg=new double [Npv1];
        Xv=new double [Npv1*2];
        Xvp=new double [Npv1*2];
        dXvp=new double [Npv1*2];
        // All buses
        yL=new complex<double> [N1];
        Vc=new complex<double> [N1];
    } // of try
    catch (std::bad_alloc) {  // ENTER THIS BLOCK ONLY IF std::bad_alloc IS THROWN.
        NoMemory();
        exit(-1);
    }
//-----------------------------------------------------------------------------
// initialisation
    for ( i=0; i<=N; i++)
    {
        S[i]=complex<double>(0,0);
        Sg[i]=complex<double>(0,0);
        Sd[i]=complex<double>(0,0);
        for ( j=0; j<=N; j++)		Y[i][j]=complex<double>(0,0);
        yL[i]=complex<double>(0,0);
    }
// Initialiser attention aux dimensions
    for ( i=0; i<=Npv; i++)
    {
        Eg[i]=complex<double>(0,0);
        Pm[i]=0;
        Pg[i]=0;
    }
    If=0; Ip=0; Iq=0; ISL=0; IDL=0; ILG=0;
    FBt0=0; FBt1=0;
    LOt0=0; LOt1=0;
    SLt0=0; SLt1=0;
    DLt0=0; DLt1=0;
    LGt0=0; LGt1=0;
}
//-----------------------------------------------------------------------------
// lecture du body du fichier d'entree : donnees du reseau.
void TTS::ReadBody( ifstream& s)
{
    int Code, i1, i2;
    char Bus[13], Bus1[13], Bus2[13];
    char Comment[81];
    double Pg0, Qg0, PL0, QL0, Vb0, Rs0, Xs0, Gs0, Bs0, Bc0,
            Amag0, Adelta0, SLN0,
            Vmag0, delta0,
            H0, D0, Rg0, Xdp0;
    complex<double> a0, y0;

    OutFile.setf(ios::left, ios::adjustfield);
//  OutFile << setprecision( 5);  // 5 chifres apres la virgule
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

// Special Trans Stab
// Donnees initiales du load flow
            case 11 :
                s >> Bus >> Vmag0 >> delta0;
                OutFile  <<" "
                         <<setw(12)<< Bus <<"  "
                         <<setw( 7)<< Vmag0 <<"  "<<setw( 9)<< delta0 <<"\n";
                LookUp( Bus, i1);
                Vmag[i1]=Vmag0;
                delta[i1]=delta0*M_PI/180; // converti deg -> rad
                break;

// Caracteristiques des machines
            case 16 :
                s >> Bus >> H0 >> D0 >>  Rg0 >> Xdp0;
                OutFile  <<" "
                         <<setw(12)<< Bus <<"  "
                         <<setw( 8)<< H0 <<"  "<<setw( 8)<< D0 <<"  "
                         <<setw( 7)<< Rg0 <<"  "<<setw( 7)<< Xdp0 <<"\n";
                LookUp( Bus, i1);
                H[i1]=H0;
                D[i1]=D0;
                if ( Rg0 !=0 || Xdp0 !=0 )  yg[i1]=1.0 / complex<double>( Rg0, Xdp0);
                else yg[i1]=complex<double>( 1E20, 1E20);
                break;

// Le nom des bus a osculter Nobs<=Nobsmax
            case 17 :
                s >> Bus ;
                OutFile  <<" "
                         <<setw(12)<< Bus <<"\n";
                LookUp( Bus, i1);
                if (i1>Npv) 	     {
                    cout << "Bus monitored "<< Bus <<" is not a PV Bus\n";
                    ErrorFlag=false;
                    exit(1);
                }
                if (Nobs==NobsMax) {
                    cout << "Too much machines to monitor : File Format Error\n";
                    ErrorFlag=false;
                    exit(1);
                }
                MachObs[ Nobs]=i1; // de 0 a NobsMax
                Nobs++;
                break;

// Bus du defaut
            case 18 :
                s >> Bus >> FBt0 >> FBt1 >> Gf >> Bf;
                OutFile  <<" "
                         <<setw(12)<< Bus <<"  "
                         <<setw( 6)<< FBt0 <<"  "<<setw( 6)<< FBt1 <<"  "
                         <<setw( 8)<< Gf   <<"  "<<setw( 8)<< Bf   <<"\n";
                LookUp( Bus, i1);
                If=i1;   // numero du bus
                break;

// Line ouverte
            case 19 :
                s >>  Bus1 >> Bus2 >> LOt0 >> LOt1;
                OutFile  <<" "
                         <<setw(12)<< Bus1 <<"  " <<setw(12)<< Bus2 <<"  "
                         <<setw( 6)<< LOt0 <<"  "<<setw( 6)<< LOt1 <<"\n";
                LookUp( Bus1, i1);
                LookUp( Bus2, i2);
                Ip=i1;   // numero du bus1
                Iq=i2;   // numero du bus2
                break;

// sauve les elements de la ligne de defaut : OpenedLine
            case 25 :
                s >>  Bus1 >> Bus2 >> Rs0 >> Xs0 >> Gs0 >> Bs0;
                OutFile  <<" "<<setw(12)<< Bus1 <<"  " <<setw(12)<< Bus2 <<"  "
                         <<setw( 7)<< Rs0 <<"  " <<setw( 7)<< Xs0 <<"  "
                         <<setw( 7)<< Gs0 <<"  "<<setw( 7)<< Bs0 <<"\n";
                LookUp( Bus1, i1);
                LookUp( Bus2, i2);
                ysh_OpenIpIq=complex<double>( Gs0, Bs0);
                ysh_OpenIqIp=ysh_OpenIpIq;
                if ( Rs0 !=0 || Xs0 !=0 )
                    ys_OpenIpIq=1.0 / complex<double>( Rs0, Xs0);
                else  ys_OpenIpIq=complex<double>(0,0);
                ys_OpenIqIp=ys_OpenIpIq;
                if ( Ip!=i1 || Ip!=i1 )
                {
                    cout << "The Opened Line has data error : File Format Error";
                    ErrorFlag=false;
                    exit(1);
                }
                break;

// Bus du Switched Load
            case 20 :
                s >> Bus >> SLt0 >> SLt1 >> SLPL >> SLQL;
                OutFile  <<" "
                         <<setw(12)<< Bus <<"  "
                         <<setw( 6)<< SLt0 <<"  "<<setw( 6)<< SLt1<<"  "
                         <<setw( 7)<< SLPL <<"  "<<setw( 7)<< SLPL <<"  "
                         <<"\n";
                LookUp( Bus, i1);
                ISL=i1;   // numero du bus
                break;

// Bus du Dropped Load
            case 21 :
                s >> Bus >> DLt0 >> DLt1 >> DLPL >> DLQL;
                OutFile  <<" "
                         <<setw(12)<< Bus <<"  "
                         <<setw( 6)<< DLt0 <<"  "<<setw( 6)<< DLt1<<"  "
                         <<setw( 7)<< DLPL <<"  "<<setw( 7)<< DLPL <<"  "
                         <<"\n";
                LookUp( Bus, i1);
                IDL=i1;   // numero du bus
                break;

// Bus du Loss Generator
            case 24 :
                s >> Bus >> LGt0 >> LGt1 ;
                OutFile  <<" "
                         <<setw(12)<< Bus <<"  "
                         <<setw( 6)<< LGt0 <<"  "<<setw( 6)<< LGt1 <<"\n";
                LookUp( Bus, i1);
                ILG=i1;   // numero du bus
                break;

// Parametres de simulation
            case 22 :
                s >> dt >> t2 >> NiterMax >> Tol >> eps
                  >> FaultType >> freq;
                OutFile  <<" "
                         <<setw( 6)<< dt <<"  "<<setw( 6)<< t2 <<"  "
                         <<setw( 4)<< NiterMax <<"  "
                         <<setw( 6)<< Tol <<"  "<<setw( 6)<< eps <<"  "
                         <<setw( 4)<< FaultType<<"  "<<setw( 4)<< freq <<"\n";
                // Calcule Ws
                Ws=2*M_PI*freq;
                break;

// TransStab OutPut Options
            case 23 :
                s >> TSOpt;
                OutFile  <<" " << TSOpt <<"\n";
                break;

            default :
                cout << "Identifier Code Not Excepted : File Format Error";
                ErrorFlag=false;
                exit(1);
        }   // switch

    }  // while
// controle du nombre de bus observes
    if (Nobs>0) Nobs--; // Nobs=Nombre de bus observes -1
}
//-----------------------------------------------------------------------------
void TTS::Lecture()
{
// Coz cannot call Virtual Methods from the constructor
    OutVersion();

    ifstream InFile ( InName, ios::in);
    if ( !InFile)	{
        cout << "Impossible to open "<< InName <<" file" << ": File Error";
        ErrorFlag=false;
        exit(1);
    }

    TSOpt=id_YMatrixOut;
    ReadHead( InFile);
    ReserveMemory();		// pourra t elle etre appelee a partir de ReadBody
    // en virtual casting ?
    ReadBody( InFile);
    InFile.close();
    VerifPVH();
}
//-----------------------------------------------------------------------------
// Routine de verification si tous les H[i] sont non nuls pour les bus PV
void TTS::VerifPVH()
{
    int i;
    for (i=0; i<=Npv; i++)
        if ( H[i]==0 ) {
            cout << "the PV bus " << List[i] << " has H=0 : File Error";
            ErrorFlag=false;
            exit(1);
        }
}
//-----------------------------------------------------------------------------
// Sauve les elements de
// la Matrice Y avant defaut et apres ModifyY ( pour avoir yL, yg )
void TTS::SaveY()
{
// Sauve les Y qu'on va modifier
    YsaveIf=Y[If][If];  // FaultedBus
    YsaveIpp=Y[Ip][Ip]; // OpenedLine
    YsaveIqq=Y[Iq][Iq];
    YsaveIpq=Y[Ip][Iq];
    YsaveIqp=Y[Iq][Ip];
    YsaveISL=Y[ISL][ISL]; // SwitchedLoad
    YsaveIDL=Y[IDL][IDL]; // DroppedLoad
    YsaveILG=Y[ILG][ILG]; // LossGen
}
//-----------------------------------------------------------------------------
// Calcule les admitances de charge equivalentes
void TTS::CalcYload()
{
    int i;
    for ( i=0; i<=N; i++)
        yL[i]=-conj( Sd[i])/Vmag[i]/Vmag[i];
}
//-----------------------------------------------------------------------------
// Modifie Y pour y inclure les yLoad et yg
void TTS::ModifY()
{
    for ( int i=0; i<=N; i++)
        if ( i<=Npv )  Y[i][i]+=yL[i]+yg[i];
        else	   Y[i][i]+=yL[i];
}
//-----------------------------------------------------------------------------
void TTS::CalcInit()
{
    complex<double> V, Ig;
    for ( int i=0; i<=Npv; i++)
    {
        // D'apres la puissance generee Sg avant defaut
        V=polar( Vmag[i], delta[i]);
        Ig=conj( Sg[i]/V );
        Eg[i]=V+Ig/yg[i];
        Pg[i]=real( Eg[i]*conj( Ig) );

        Pm[i]=Pg[i];
        Xv[i]=argn( Eg[i]); // Argument
        Xv[Npv1+i]=Ws;     // Vitesse initiale de synchronisme
    }
}
//-----------------------------------------------------------------------------
// va disparaitre car defaut dynamique
void TTS::ModifY_Default()
{
// remet les Y a leur valeur sans default
    Y[If][If]=YsaveIf;  // FaultedBus
    Y[Ip][Ip]=YsaveIpp; // OpenedLine
    Y[Iq][Iq]=YsaveIqq;
    Y[Ip][Iq]=YsaveIpq;
    Y[Iq][Ip]=YsaveIqp;
    Y[ISL][ISL]=YsaveISL; // SwitchedLoad
    Y[IDL][IDL]=YsaveIDL; // DroppedLoad
    Y[ILG][ILG]=YsaveILG; // LossGen
// Si en est en phase dudit defaut alors changer...
    if (FBFlag==1) Y[If][If]+=complex<double>( Gf, Bf);
    if (LOFlag==1) {
        Y[Ip][Ip]-=ys_OpenIpIq+ysh_OpenIpIq;
        Y[Iq][Iq]-=ys_OpenIqIp+ysh_OpenIqIp;
        Y[Ip][Iq]+=ys_OpenIpIq;
        Y[Iq][Ip]+=ys_OpenIqIp;
    }
    if (SLFlag==1) Y[ISL][ISL]+=complex<double>( SLPL, -SLQL);
    if (DLFlag==1) Y[IDL][IDL]-=complex<double>( DLPL, -DLQL);
    if (LGFlag==1) Y[ILG][ILG]-=yg[ILG]; // Gen debranche
}
//-----------------------------------------------------------------------------
// Ecoulement de puissance par la methode de Gauss-Seidel
// avec prise en compte de Eg , yg et matrice Y modifiee
void TTS::LFGauss()
{
    int i, j, Iter=0;
    double Dv, DvMax;
    complex<double> sum;

    // recopie Vmag, delta dans Vc ( le sauve juste au debut )
    for ( i=0; i<=N; i++)  Vc[i]=polar( Vmag[i], delta[i] );

    do
    {
        DvMax=0;
        // boucle de calcul des V
        for ( i=0; i<=N; i++)   // n'evite pas le slack bus !!!
        {
            // for all buses
            sum=complex<double>(0,0);
            for ( j=0; j<=N; j++)   if ( i!=j) sum+=Y[i][j]*Vc[j];
            // |---
            if ( i<=Npv && !( i==ILG && LGFlag==1) )
                Vc[i]=(-sum+yg[i]*Eg[i])/Y[i][i];
            else	   Vc[i]= -sum/Y[i][i];

// sort Vc[i]
//cout << " Vc["<<i<<"]="<<Vc[i]<<"\n";

            // recherche de l'ecart max
            sum=Vc[i]-polar( Vmag[i], delta[i]); // used below
            Dv=abs( sum);
            if (Dv>DvMax )
            {
                Vc[i]=polar( Vmag[i], delta[i])
                      +GS_AccelFact*( sum );
                DvMax=Dv;
            }
        } // de for i
        // recopie Vc -> Vmag, delta
        for ( i=0; i<=N; i++)  {
            Vmag[i]=abs( Vc[i]);
            delta[i]=argn( Vc[i]);
        }

        Iter++;
    } while ( DvMax>eps && Iter<=NiterMax);
    if (TSOpt & id_LFErrorOut)
        OutFile << "      Gauss-Seidel : N Iterations "<<Iter
                << " Max Error "<<DvMax<<"\n";
}
//-----------------------------------------------------------------------------
// Calcule Pg a partir de Eg
void TTS::CalcPg()
{
    complex<double> Ig;
    for ( int i=0; i<=Npv; i++)
    {
        // D'apres la puissance generee Sg avant defaut
        Ig=( Eg[i]-polar( Vmag[i], delta[i]) )*yg[i];
        if ( LGFlag==1 && i==ILG ) Ig=0;  // Gen debranche
        Pg[i]=real( Eg[i]*conj( Ig) );
    }
}
//-----------------------------------------------------------------------------
// Routine d'affichage sur OutFile
void TTS::OutY()
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
void TTS::OutV_delta()   // applelee apres recalcul de Vc dans TransferPQ
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
void TTS::OutGraphInit()
{
    int i, k;
// justification a gauche !
    GraphFile.setf(ios::left, ios::adjustfield);
    GraphFile << setprecision( 6) << "time    ";
    for ( i=0; i<=Nobs; i++)
    {
        k=MachObs[i];
        GraphFile << "del_" <<setw(12)<< List[k]
                  << " wr_" <<setw(12)<< List[k]
                  << " V__" <<setw(12)<< List[k]
                  << " Pg_" <<setw(12)<< List[k]<<" ";
        // pb au formattage
    }
    GraphFile <<"\n";
}
//-----------------------------------------------------------------------------
void TTS::OutXvGraph()
{
    int i, k;
    GraphFile <<setw( 6)<< t <<"  ";
    for ( i=0; i<=Nobs; i++)
    {
        k=MachObs[i];
        GraphFile <<setw( 16)<< Xv[k]*180/M_PI <<" "
                  <<setw( 16)<< Xv[Npv1+k]/Ws
                  <<setw( 16)<< Vmag[k]
                  <<setw( 16)<< Pg[k] ;
    }
    GraphFile <<"\n";
}
//-----------------------------------------------------------------------------
void TTS::OutEgPgVmag()
{
    int i, k;
    complex<double> Ig;

    OutFile << "\ntime : "<<setw( 6)<< t << "\n";
    for ( i=0; i<=Nobs; i++)
    {
        k=MachObs[i];
        Ig=( Eg[k]-polar( Vmag[k], delta[k]) )*yg[k];

        OutFile << "    Bus monitored : "<<setw(12)<< List[k]
                << "         Bus number : "<<setw( 2)<<k<<"\n"
                << "    Ig = "<<  Ig <<"  "
                << "    Ig ( mag, ang)= "
                <<setw( 6)<< abs( Ig) <<"  "
                <<setw( 6)<< argn( Ig)*180/M_PI <<"\n"
                << "    Eg = "<< Eg[k] <<"  "
                << "    Eg ( mag, ang)= "
                <<setw( 6)<< abs( Eg[k]) <<"  "
                <<setw( 6)<< argn( Eg[k])*180/M_PI <<"\n";
        OutFile << "    V ( mag, ang)= "
                <<setw( 10)<< Vmag[k] <<"  "
                <<setw( 10)<< delta[k]*180/M_PI << "\n"
                << "    Pg = "<<setw( 6)<< Pg[k]<<"\n"
                << "    Internal angle ( deg) = "
                <<setw( 6)<< Xv[k]*180/M_PI <<"  "
                << "    Speed (p.u.) = "
                <<setw( 6)<< Xv[Npv1+k]/Ws <<"\n";
        OutFile <<
                "    ---------------------------------------------------------------------\n";
    }
}
//-----------------------------------------------------------------------------
void TTS::OutModif()
{
    OutFile << "\n----------  Y Matrix has been modified  ----------\n";
    OutFile <<   "            at time : "<<setw( 6)<< t << "\n";
}
//-----------------------------------------------------------------------------
void TTS::OutYg()
{
    OutFile << "\n---------  Machine Admittance  --------\n";
    for ( int i=0; i<=Npv; i++)
        OutFile <<setw(12)<< List[i] <<"  " <<"yg["<<setw( 2)<<i<<"]="
                << yg[i]<<"\n";
}
//-----------------------------------------------------------------------------
void TTS::OutYL()
{
    OutFile << "\n----------  Load Admittance  ----------\n";
    for ( int i=0; i<=N; i++)
        OutFile <<setw(12)<< List[i] <<"  " <<"yL["<<setw( 2)<<i<<"]="
                << yL[i]<<"\n";
}
//-----------------------------------------------------------------------------
int TTS::IsItOK()
{
    return ErrorFlag;
}
//-----------------------------------------------------------------------------
void TTS::Run()
{
// le temps initial
    t=0;
//Application->MessageBox( "TRying to RUN", "Trans Stab Prog",MB_OK);
    int WillModifyY;
// Ici l'initialisation de Vmag, delta se fait a partir
// de la lecture de la carte 11
// infos de Power Designer 1.30 !
    if (TSOpt & id_YMatrixOut) OutY();
    CalcYload(); // Calcule les admitances de charge equivalentes
    if (TSOpt & id_YMatrixOut) OutYg();
    if (TSOpt & id_YMatrixOut) OutYL();

    ModifY(); // Modifie Y en incluant les yL et yg
    OutModif(); // affiche message puis Y
    if (TSOpt & id_YMatrixOut) OutY();
    SaveY(); // appelee apres ModifY pour avoir yL, yg dans les elements Ysave
// Calcule les Eg, Pm initiaux
// and Set initial State Variables Xv
    CalcInit();

    OutGraphInit(); // entete du GraphFile

// Clear Fault Flags
    FBFlag=0;
    LOFlag=0;
    SLFlag=0;
    DLFlag=0;
    LGFlag=0;
    OutXvGraph(); // sort donnees pour graph
    // sort Eg, Angle, Vmag, delta, Ig...
    if (TSOpt & id_MachineOut)  OutEgPgVmag();

// Debut de la simul; no cursor no hourglass

    start = clock();
// la boucle principale
    do{    // while ( t<=t2 );
        // test si un defaut apparait
        WillModifyY=0;
        // FaultedBus
        if ( t>=FBt0 && FBFlag==0 && ((FaultType & id_FaultedBus)==id_FaultedBus))
        { FBFlag=1;
            WillModifyY=1; }
        if ( t>=FBt1 && FBFlag==1 ) { FBFlag=2;
            WillModifyY=1; }
        // OpenedLine
        if ( t>=LOt0 && LOFlag==0 && ((FaultType & id_OpenedLine)==id_OpenedLine))
        { LOFlag=1;
            WillModifyY=1; }
        if ( t>=LOt1 && LOFlag==1 ) { LOFlag=2;
            WillModifyY=1; }
        // SwitchedLoad
        if ( t>=SLt0 && SLFlag==0 && ((FaultType & id_SwitchedLoad)==id_SwitchedLoad))
        { SLFlag=1;
            WillModifyY=1; }
        if ( t>=SLt1 && SLFlag==1 ) { SLFlag=2;
            WillModifyY=1; }
        // DroppedLoad
        if ( t>=DLt0 && DLFlag==0 && ((FaultType & id_DroppedLoad)==id_DroppedLoad))
        { DLFlag=1;
            WillModifyY=1; }
        if ( t>=DLt1 && DLFlag==1 ) { DLFlag=2;
            WillModifyY=1; }
        // LossGen
        if ( t>=LGt0 && LGFlag==0 && ((FaultType & id_LossGen)==id_LossGen))
        { LGFlag=1;
            WillModifyY=1; }
        if ( t>=LGt1 && LGFlag==1 ) { LGFlag=2;
            WillModifyY=1; }


        if ( WillModifyY==1) 	{
            ModifY_Default();
            OutModif();
            if (TSOpt & id_YMatrixOut) OutY();
            // Recalcule
            LFGauss();
            CalcPg();
        }

        // Routine d'integration numerique
        IntegreSysteme();

        OutXvGraph(); // sort donnees pour graph
        // et pour le .TSO
        if (TSOpt & id_MachineOut)  OutEgPgVmag();
        if (TSOpt & id_V_auxOut)  OutV_delta();

        // reboucle
    } while ( t<=t2 );
// fin de la simulation : affiche son temps d'execution
    end = clock();
// fin de la simul; no restaure cursor

    OutFile <<"Total time : "<< (end - start)/CLK_TCK << " s\n";
// Ferme le fichier de sortie
    OutFile.close();
    GraphFile.close();
}
//-----------------------------------------------------------------------------
