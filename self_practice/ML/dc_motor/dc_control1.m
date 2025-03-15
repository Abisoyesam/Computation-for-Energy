%% Current control of a DC motor
% This example demonstrates the use of a Proportional-Integral (PI) controller
clc; clear; close all;

%pkg load control

% System Parameters
R = 0.8;        % Resistance (Ohms)
L = 0.011;      % Inductance (H)
k = 1.91;       % Motor constant (Nm/A)
J = 0.02;       % Inertia (kg*m^2)
a = 1e-2;       % Damping coefficient
Te = L / R;     % Electrical time constant
Tem = J * R / k^2; % Mechanical time constant
G = 20;         % DC Gain
Ts = 0.2e-3;    % Sampling time (s) for discrete-time model

%% PI Controller Setup

tr = 0.001; % step response rise time
Ti = Te; % The integral time is chosen equal to the electrical time constant (a common approach called “pole-zero cancellation” or “pole compensation”).
Kp = 3*R*Ti/G/tr; % kp = 3*R*Ti/G/tr
Ki = Kp/Ti; % ki = kp/Ti

%% Discrete-time model of the plant and PI controller
% c2d(sys, Ts, 'method') converts the continuous-time system sys to a discrete-time system using the method 'method'.
% 'method' can be 'zoh' (zero-order hold), 'foh' (first-order hold), or 'tustin' (bilinear transformation).
s = tf('s');
MCC = G/R/(L*s + R); % TF of the plant
MCC_z = c2d(MCC, Ts, 'zoh'); % Discrete-time model of the plant

PI = Kp + Ki/s; % PI controller
PI_z = c2d(PI, Ts, 'zoh'); % Discrete-time model of the PI controller

% Open-loop transfer function
FTBO = series(MCC, PI); % Open-loop transfer function
FTBO_z = series(MCC_z, PI_z); % Discrete-time model of the open-loop transfer function

% Closed-loop transfer function
FTBF = feedback(FTBO, 1); % Continuous-time model
FTBF_z = feedback(FTBO_z, 1); % Discrete-time model

%% Step responses (Continuous-time vs Discrete-time)

% Step response - continous state
figure;
t = 0:1e-5:0.005; % time vector
step(FTBF, t); % step response
[y x] = step(FTBF, t); % step response
sprintf('Overshoot: %.2f%%',  (max(y) - 1) * 100) % overshoot
grid on;

% Step response - discrete state
figure;
t = [0:Ts:0.005]; % time vector
step(FTBF_z, t); % step response
[y x] = step(FTBF_z, t); % step response
sprintf('Overshoot: %.2f%%',  (max(y) - 1) * 100) % overshoot
grid on;