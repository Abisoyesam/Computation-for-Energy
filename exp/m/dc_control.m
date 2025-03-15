%% Current control of DC Motor
clc,clear all,close all
%pkg load control
%% Parameters
R= 0.8; L = 0.011; k = 1.91; J =0.02; a= 1E-2;
Te=L/R;
Tem=J*R/k/k;
G=20;
Ts=0.2e-3;

%% PI control of I

tr=0.001; % step time at 1 ms
Ti=Te; % perfect pole compensation
%Ti=0.8*Te; % bad pole compensation
Kp=3*R*Ti/G/tr;
Ki=Kp/Ti;
s=tf('s');

% TF DCM
MCC=G/R/(1+s*Te)
MCC_z=c2d(MCC, Ts, 'zoh') % discrete format
% TF PI
PI=Kp+Ki/s
PI_z=c2d(PI, Ts, 'zoh')
FTBO=series(MCC, PI)
FTBO_z=series(MCC_z, PI_z)

% Closed Loop transfer function
FTBF=feedback(FTBO,1);
FTBF_z=feedback(FTBO_z,1);

% step response - continious state
figure(1);
t=[0: 1e-5: 0.005];
step(FTBF,t);
[y x]=step(FTBF,t);
sprintf('Over shoot=%5.2f %%', 100*(max(y)-1))

% step response - discrete state
figure(2);
t=[0: Ts: 0.005];
step(FTBF_z,t);
[y x]=step(FTBF_z,t);
sprintf('Over shoot=%5.2f %%', 100*(max(y)-1))