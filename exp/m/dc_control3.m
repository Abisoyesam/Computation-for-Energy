%% Current control of a DC Motor
clc,clear all,close all
%% Parameters
R= 0.8; L = 0.011; k = 1.91; J =0.2; a= 50E-2; % frottement visqueux
Te=L/R;
Tem=J*R/k/k;
G=20;
Ts=0.2e-3; % 200 us

%% Current (I) PI Controller
tr=0.001; % temps de tep à 1 ms
Ti=Te;
Kp=3*R*Ti/G/tr;
Ki=Kp/Ti;
Ki_d=Ki*Ts;
v = 0;

%% initialization
tf=2; % final simulation time
dt=0.01e-3; % time step
tk=1; % array indice
Ntf=floor(tf/dt);

%% system variables
u=0; I=0; w=0; Iref=0;
xe=0; Flagtm=0;
Vlim=10; % maximum 200 V=10*G
Iref0 = 8; % step value
Cr0 = 0;

% initialization of arrays and
tt=zeros(1, Ntf); t=0;
It=zeros(1, Ntf); I=0.0; % Current
ut=zeros(1, Ntf);
wt=zeros(1, Ntf);
vt=zeros(1, Ntf);
xet=zeros(1, Ntf);
Cet=zeros(1, Ntf);

%%DC Motor simulation with current control
%Integral part (xe) and voltage at the output of the PI controller
while (t<tf) %% while loop
    if t>=0.001 % at 1 ms, ther is z stre
        Iref=Iref0;
    end

    %% MODULULO to have an execution at a different sampling period
    tm100=t-(Ts*floor(t/Ts));
    if(tm100>dt) Flagtm=0; end;
    if((tm100<=dt) && (Flagtm==0))
        Flagtm=1;

        % Controller
        e=Iref-I;
        if ((v < Vlim) && (v > -Vlim))
            xe = xe + Ki_d*e;
        end
        xe=xe+Ki_d*e;
        v=Kp*e+xe;

        if (v>Vlim) 
            v = Vlim; 
        end;

        if (v<-Vlim) 
            v = -Vlim; 
        end;
    end;

    u=G*v;
    %dI = (u-R*I)/L;
    dI = (u-R*I-k*w)/L;
    Ce = k*I;
    dw = (Ce-a*w-Cr0)/J;
    
    % integration
    I = I + dI*dt;
    w = w + dw*dt;
    tt(tk)=t; %% array filling
    It(tk)=I;
    ut(tk)=u;
    wt(tk)=w;
    Cet(tk)=Ce;
    vt(tk)=v;
    xet(tk)=xe;
    t=t+dt;
    tk=tk+1; % array indice
end % while end
%% plot results
figure(1)
subplot(2,1,2),plot(tt, wt,'k');
legend('w(rd/s)');
subplot(2,1,1),plot(tt,It,'b');
legend('I(A)');
figure(2)
subplot(2,1,2),plot(tt, vt,'k');
legend('v(V)');
subplot(2,1,1),plot(tt,xet,'b');
legend('xe(V)');
