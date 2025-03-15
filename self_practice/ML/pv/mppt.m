function [V, I, P] = mppt(Suns,Tc)
V=20;
I = PVNR(V,Suns,Tc);
P=I*V;
D = 0.1;
Vnew = V+D;
dP = 1;
iter=0;

while (abs (dP)> 1e-2 && (iter<200))
    iter=iter+1;
    I = PVNR(Vnew,Suns,Tc);
    Pnew=I*Vnew;
    dP = Pnew-P;
    dV = Vnew - V;
    V = Vnew;
    P = Pnew;
    sprintf('iter=%d V=%g I=%g, P=%g\n', iter, V, I, P);
    if (dP>0)
        if (dV>0) Vnew = V + D;
        else Vnew = V - D;
        end
    else
        if (dV<0) Vnew = V + D;
        else Vnew = V - D;
        end
    end
    
end
% I;
V=Vnew;
P=I*V;
end