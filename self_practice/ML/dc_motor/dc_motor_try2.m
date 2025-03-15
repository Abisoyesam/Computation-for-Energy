% DC motor simulation using Euler numerical integration (Electrical and Mechanical part)

clear; clc; close all;

% Simulation Parameters
tf =  0.4; % Final time
dt = 0.0001; % Time step
Ntf = floor(tf / dt); % Number of steps
tk = 1; % Indexing array

% Parameters
% Formula:
% di_dt = (u - R*I - k*w) / L; Electrical
% dw_dt = (Ce - Cr - a*w) / J; Mechanical
R = 0.8; % Resistance of the motor
L = 0.011; % Inductance of the motor
k = 1.91; % Emf constant
u0 = 0; % Initial voltage
I = 0; % Initial current
t = 0; % Initial time
u = 200; % Input voltage
w = 0; % Initial angular velocity
Cr0 = 0; % Initial torque
a = 1e-2; % Damping coefficient
J = 0.2; % Moment of inertia

% Initialization
tt = zeros(1, Ntf); % Time vector
It = zeros(1, Ntf); % Current vector
ut = zeros(1, Ntf); % Voltage vector
Cet = zeros(1, Ntf); % Electromagnetic torque vector
wt = zeros(1, Ntf); % Angular velocity vector

% Euler integration loop
while (t < tf)
    % Compute derivatives
    dI_dt = (u - R*I - k*w) / L; % Electrical
    Cr = a*w; % Friction torque
    Ce = k*I; % Electromagnetic torque
    dw_dt = (Ce - Cr - a*w) / J; % Mechanical

    % Update state variables
    I = I + dI_dt * dt;
    w = w + dw_dt * dt;

    % Store results
    tt(tk) = t;
    It(tk) = I;
    ut(tk) = u;
    Cet(tk) = Ce;
    wt(tk) = w;

    % Increment loop counter
    t = t + dt;
    tk = tk + 1;
end

% Plot results
figure;
subplot(2,1,1)
plot(tt, It);
title('Current vs Time');
xlabel('Time (s)');
ylabel('Current (A)');
subplot(2,1,2)
plot(tt, wt);
title('Angular Velocity vs Time');
xlabel('Time (s)');
ylabel('Angular Velocity (rad/s)');
