% course du soleil durant l'année à Tlemcen
%[h, az] = calchaz(34.88,-1.31, 1, 0, 14, 8,6,2016 )
close all; clear all;
deg2rd = 0.017453292519943295769236907684886;
rd2deg = 57.295779513082320876798154814105;
inclinaisonPV = 34;
heure = [5:10./60:22];
for mm = 1:12
    for hh = 1:length(heure)
        [h, az] = calchaz(34.88,-1.31, 1, 0, heure(hh), 1,mm,2016 );
        Hauteur(mm,hh)=h;
        Azimut(mm,hh)=az;
        inclinaison(mm,hh)= h + rd2deg*incline(az*deg2rd, inclinaisonPV*deg2rd);
    end
end
cc=hsv(12);
% Hauteur
figure (1);
hold on;
for mm = 1:12
    plot(heure, Hauteur(mm,:),'color',cc(mm,:));
  
end
% Azimut
figure (2);
hold on;
for mm = 1:12
    plot(heure, Azimut(mm,:),'color',cc(mm,:));
  
end

% inclinaison absolue
figure (3);
hold on;
for mm = 1:12
    plot(heure, inclinaison(mm,:),'color',cc(mm,:));
  
end

figure (4);
plot(heure, Hauteur(6,:),'b', heure, inclinaison(6,:),'r' );
legend('hauteur horizon', 'inclinaison soleil - PV juin');