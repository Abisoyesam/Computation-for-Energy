function Jac=jacob( Nh, E, Vfmax, Xn)
% calculation of Jacobian
deltaX=1e-8;
for j=0 : Nh-1
    Xp=Xn; % copy
    Xp(j+1)=Xp(j+1)+deltaX;
    for i=0 : Nh-1
        Jac(i+1,j+1)=( fonctn( Nh, E, Vfmax, i, Xp) - fonctn( Nh, E,Vfmax,i, Xn) )/deltaX;
    end;
end;
%disp( Jac);