% Simulation of DCM at standstill, voltage Step
clc,clear all,close all
%% initialisation
tf=0.4; % final simulation time
dt=0.0001; % time step
tk=1; % array indice
Ntf=floor(tf/dt);
%% systeme variables
R= 0.8; L = 0.011; k = 1.91; J =0.2; a= 1E-2;
u=0; I=0; w=0; t=0;
u0 = 200; Cr0 = 0;
% initialization
tt=zeros(1, Ntf); It=zeros(1, Ntf); ut=zeros(1, Ntf); 
wt=zeros(1, Ntf); Cet=zeros(1, Ntf);
%% boucle du temps de simul
while (t<tf)
 if t>=0.01 % 10 ms power the DCM by a voltage step
 u=u0; 
 end
 dI = (u-R*I-k*w)/L;
 Ce = k*I; dw = (Ce-a*w-Cr0)/J;
 % integration rectangle method
 I = I + dI*dt;
 w = w + dw*dt;
 %% array filling
 tt(tk)=t; It(tk)=I; ut(tk)=u;
 wt(tk)=w; Cet(tk)=Ce;
 t=t+dt; tk=tk+1; % array indice
end % while end
%% plot results
figure(1)
subplot(2,1,2),plot(tt, wt,'k');
legend('w(rd/s)');
subplot(2,1,1),plot(tt,It,'b',tt,ut,'r');
legend('I(A)');