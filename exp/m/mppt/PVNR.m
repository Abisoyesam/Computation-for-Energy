function I = PVNR(V,G, T)
T=T+273.15;
%G=1000
V=V/60;

Isc=8.69;
Voc=0.61;
Imp=8.19;
Vmp=30.55;
Pmax_e=250;
Kv=-0.0038;
Ki=0.004;
Ns=60;

k=1.3806503e-23;
q=1.60217646e-19;
a=1.3;
Rs=0.04/60;

Rp=(1.2711e+03)/60;

Gn=1000;
Tn=298.15;

dT=T-Tn;

Vt=k*T/q;
Vtn=k*Tn/q;


Io=(Isc+Ki*dT)/(exp(q*((Voc)+Kv*dT)/(a*k*Tn))-1);

Iph=(Isc+Ki*dT)*G/Gn;

I=4;
    g=Iph-Io*(exp(q*(V+I*Rs)/(k*T*a))-1)-((V+I*Rs)/Rp)-I;
    
    while(abs(g)>0.001)
      g=Iph-Io*(exp(q*(V+I*Rs)/(k*T*a))-1)-((V+I*Rs)/Rp)-I;
      glin=(-Io*q*Rs/(k*T*a))*exp(q*(V+I*Rs)/(k*T*a))-(Rs/Rp)-1;
  if (glin ~=0) 
    I=I-g/glin;
    else
    sprintf('error glin zero')
    break
    end      
  end
  if I<0
    I=0;
    end
end
