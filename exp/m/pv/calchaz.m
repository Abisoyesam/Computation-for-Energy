function [hauteur,azcorrige] = calchaz(latitudeinput, longitudeinput, Timezoneinput, DSTinput, heurelocaleinput, jour, mois, annee)
% calchaz(34.88,-1.31, 1, 0, 14, 8,6,2016 )
% 2 Algérie          Tlemcen          34.88       -1.31       1        0   1 
Mn2PI	=2*pi;
pin2	=pi/2;
deg2rd = 0.017453292519943295769236907684886;
rd2deg = 57.295779513082320876798154814105;
SolarSiderealCorrection = 1.002738;

latitude = deg2rd*latitudeinput;
longitude = deg2rd*longitudeinput;
%	CountryName = CountryNameinput;
%	TownName = VilleSelect;

TimeZone = Timezoneinput;
DST = DSTinput;
heurelocale = heurelocaleinput;


Soleil.az = 0;		% coord. horizontal
Soleil.h = 0;
Soleil.delta = 0;		% coord equatorial
Soleil.tau = 0;
Soleil.RA = 0;		% ascension droite
Soleil.L = 0;			%longitude rï¿½ele

% jour/mois/annï¿½e/heure.xxx
date.j = jour;
date.m = mois;
date.a = annee;
date.h = 0; %sera écrasé

% % milisec, secondes/, minutes, heures
% heure.ms = 0;
% heure.s = 0;
% heure.m = 0;
% heure.h = 0;

%struct TownData { char CountryName[Namelenght], TownName[Namelenght];
%									double longitude, latitude;
%									int TimeZoneTown, Convention, DST;
%							};

% datenanNow;
% ActualDate;	%format JS
% Njours=new Array(12);
%
% JD, T,  % Jour Julien, Julien Century : T,
%	theta0, theta,  % theta0 : temps sideral ï¿½ Greenwich
%	longitude, latitude;
% TimeZone, TimeZoneTown, NTowns, TownSelected, Verbose, Convention;
% DST, DSTApplies;

Mounths=char('Janvier', 'Février', 'Mars', 'Avril', 'Mai', 'Juin', 'Juillet', 'Août', 'Septembre', 'Octobre', 'Novembre', 'Décembre');
MounthsAbrev=char('jan', 'févr', 'mars', 'avr', 'mai',	'juin',	'juil','août', 'sept', 'oct',	'nov',	'déc');
Days=char('Lundi', 'Mardi', 'Mercredi', 'Jeudi', 'Vendredi', 'Samedi', 'Dimanche');


%%--------------------------------------------------------------------------
% Julian day: 86400 s, Julian year: 365.25 d, Julian Century: 36525 d
% Julian Day (valid from 1900/3/1 to 2100/2/28)
%function JulianDay (ndate, nmonth, nyear, nUT)
%{
%	if (nmonth<=2) {nmonth=nmonth+12; nyear=nyear-1;}
%	return floor(365.25*nyear) + (int)(30.6001*(nmonth+1)) - 15 + 1720996.5 + ndate + nUT/24.0;
%}
%%-------------------------------------------------------------------------
function  [JC] = JulianCentury (ndate, nmonth, nyear, nUT)
% Julian century depuis 01/01/2000 ï¿½ 12h UT
    if (nmonth<=2) 
    nmonth = nmonth+12;
    nyear = nyear-1;
    end
  JC = (floor(365.25*nyear) + floor(30.6001*(nmonth+1)) - 15 + 1720996.5 + ndate + nUT/24.0 - 2451545.0)/36525.0;
end 
%%-------------------------------------------------------------------------
%Solar Coordinates (according to: Jean Meeus: Astronomical Algorithms), accuracy of 0.01 degree 
function result = SoleilLongitude( nT)   % L
	nM = 357.52910 + 35999.05030*nT - 0.0001559*nT*nT - 0.00000048*nT*nT*nT; % mean anomaly, degree
	nL0 = 280.46645 + 36000.76983*nT + 0.0003032*nT*nT; % mean longitude, degree 
	nDL = (1.914600 - 0.004817*nT - 0.000014*nT*nT)*sin(deg2rd*nM) + (0.019993 - 0.000101*nT)*sin(deg2rd*2*nM) + 0.000290*sin(deg2rd*3*nM); 
	nL=nL0 + nDL; % true longitude, deg sans modulo
	nLm=nL-360*floor(nL/360);
	if (nLm==0)
        nLm = nLm+360;
    end
	result= nLm*deg2rd; % true , rd 
end 
%%-------------------------------------------------------------------------
% convert ecliptic longitude L (rd) to right ascension RA and declination delta
function [RA, delta] = eclipticL2RAndelta( nT, nSol)
% convert ecliptic longitude L (rd) to right ascension RA and declination delta
% old approxim  neps = deg2rd*23.43999; % obliquity of ecliptic
	neps = deg2rd*(23.4393 -0.013*nT);	% obliquity of ecliptic
	nX = cos(nSol.L); nY = cos(neps)*sin(nSol.L); nZ = sin(neps)*sin(nSol.L); nR = sqrt(1.0-nZ*nZ); 
	delta = atan2(nZ, nR); % in rd 
	RA = 2*atan2(nY, nX+nR); %  in rd 
end 

%%-------------------------------------------------------------------------
function theta0 = TempsSideral( nT)   % theta0
%compute sidereal time at Greenwich (according to: Jean Meeus: Astronomical Algorithms ) 
%	T siecle julien depuis 2000
	ntheta0=280.46061837 + 360.98564736629*nT*36525 + 0.000387933*nT*nT - nT*nT*nT/38710000.0;
	ntheta0m=ntheta0-360*floor(ntheta0/360);
	if (ntheta0m<0)
        ntheta0m = ntheta0m+360;
    end
	theta0 = ntheta0m*deg2rd;    % TempsSideralen rd
end 
%%-------------------------------------------------------------------------
function [h, az] = equatorial2horizontal( nbeta, nSol)
%convert tau, delta to horizon coordinates of the observer (altitude h, azimuth az) , needs latitude : beta
	h=asin( sin(nbeta)*sin(nSol.delta) + cos(nbeta)*cos(nSol.delta)*cos(nSol.tau));	% h en rd
	az =atan2( -sin(nSol.tau), cos(nbeta)*tan(nSol.delta) - sin(nbeta)*cos(nSol.tau))-pi;	% az en rd du SUD
	if (az<0)
        az = az+2*pi;
    end
end 
%%-------------------------------------------------------------------------
function Calchaz_in()
	date.h=heurelocale-TimeZone;	% heure GMT
	if (date.h<0) 
        date.h = date.h + 24;
        date.j = date.j-1;
    end
	T=JulianCentury (date.j, date.m, date.a, date.h);
	Soleil.L=SoleilLongitude(T);
	[Soleil.RA, Soleil.delta] = eclipticL2RAndelta( T, Soleil);
	theta0=TempsSideral( T);
	theta=theta0+longitude;
	Soleil.tau=theta-Soleil.RA;
	[Soleil.h, Soleil.az] =equatorial2horizontal( latitude, Soleil);
%	equatorial2horizontal( latitude, Soleil.tau, Soleil.delta, Soleil.h, Soleil.az);
end
%%-------------------------------------------------------------------------
%function CalculeNjdumois( m, annee)
%{
%Nonbissextile = floor(annee-4*floor(annee/4)+2)/3;  % 1 pour une annï¿½e commune et  0 pour une bissextile)
%	if (m==1 || m==3 || m==5 || m==7
%	 || m==8 || m==10 || m==12) 	return 31;
%			else	{
%						if (m==2) {if (Nonbissextile) return 28;
%															else				return 29;
%											}
%								else  return 30;
%				}
%}

	Calchaz_in();
	azcorrige=Soleil.az;
	if (azcorrige<-pi)
        azcorrige = azcorrige + Mn2PI;
    end
	if (azcorrige> pi)
        azcorrige = azcorrige - Mn2PI;
    end
	hauteur = Soleil.h;
%% en degrés    
    hauteur = hauteur*rd2deg;
    azcorrige = azcorrige*rd2deg;
    %Days(i,:)
end