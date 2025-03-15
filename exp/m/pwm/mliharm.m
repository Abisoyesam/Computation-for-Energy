%initialization
clc,clear all,close all
E=622; Es2=E/2;
Vfmax=130
Nh=3; % Nh harm to impose
X(Nh,1)=0;
% initialize Xn1
for i=0 :Nh-1
    Xn1(i+1,1)=(i+1)*pi/3.0/(Nh+1);
end
iter=0;
FlagOut=0;
err=1e-10;
errMax=100;
while ( (errMax>err) & (FlagOut<50))
    Xn=Xn1;
    for i=0 :Nh-1
        fun(i+1)=fonctn( Nh, E,Vfmax, i, Xn);
    end
    errMax=max(abs(fun));
    Jac=jacob( Nh, E, Vfmax, Xn);
    Xn1=Xn-inv(Jac)*fun';
    % verifOut
    for i=0 :Nh-1
        if (Xn1(i+1)>pi/2) Xn1(i+1)=pi/2;
            FlagOut=FlagOut+1;
        end
        if (Xn1(i+1)<0) Xn1(i+1)=0;
            FlagOut=FlagOut+1;
        end
    end
    iter=iter+1;
    disp( '------------------------');
    disp( sprintf('iter=%d funct=', iter));
    disp( sprintf('%f ', fun));
    disp( 'Xn1=');
    disp( sprintf('%f ', Xn1));
    disp( sprintf('Erreur Max=%f', errMax));
end;
if (FlagOut>40) disp("Errorrr");
end;
disp( '*************');
disp( '** resultat :');
disp( sprintf('iter=%d', iter));
disp( 'Les alphas =');
disp( Xn1);
disp( 'funct=');
disp( sprintf('%f ', fun));
disp( sprintf('Erreur Max=%f', errMax))