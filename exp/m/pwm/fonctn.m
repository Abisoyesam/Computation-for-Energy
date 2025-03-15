function f=fonctn( Nh, E, Vfmax, i, Xhere)
%calculation of harmonic n ° h - value to be imposed
if (i==0) f=bn(Nh, E,1, Xhere)-Vfmax;
 else f=bn(Nh, E, harm(i), Xhere);
 end;
%disp( sprintf('i=%d fonctn=%f', i,f))