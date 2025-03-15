% DC Electrical Motor Simulation
% Using Numerical Integration (Euler)
clear; close all; clc;

% Simulation parameters
tf = 0.5; % Final time
dt = 0.0001; % Time step df < l/R inductor / resistor
Ntf = floor(tf / dt); % Number of steps
tk = 1; % indexing array

% Parameters
% Formula:
% di_dt = (u - R*I - k*w) / L;
R = 0.8; % Resistance of the motor
L = 0.011; % Inductance of the motor
k = 1.91; % Emf constant
u0 = 0; % Initial voltage
I = 0; % Initial current
t = 0; % Initial time
u = 8; % Input voltage
w = 0; % Initial angular velocity

% Initialization
tt = zeros(1, Ntf); % Time vector
It = zeros(1, Ntf); % Current vector
ut = zeros(1, Ntf); % Voltage vector

% Euler integration loop
while t < tf
    % Compute derivatives
    dI_dt = (u - R*I - k*w) / L;
    % Update state variables
    I = I + dI_dt * dt;

    % Store results
    tt(tk) = t;
    It(tk) = I;
    ut(tk) = u;

    % Increment loop counter
    t = t + dt;
    tk = tk + 1;
end

% Plot results
figure;
plot(tt, It);
title('Current vs Time');
xlabel('Time (s)'); 
ylabel('Current (A)');