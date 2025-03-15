function somme=bn( Nh, E, h, Xhere)
%calculation of harmonic n ° h
somme=-0.5;

for i=0 : Nh-1
    if ( rem(i,2)==1 ) somme=somme-cos( h*Xhere(i+1));
    else somme=somme+cos( h*Xhere(i+1));
    end;
end;

somme=somme*4*E/pi/h;
%disp( sprintf('h=%d Bn=%f', h,somme));