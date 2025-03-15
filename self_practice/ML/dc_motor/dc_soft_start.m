% DC Motor Soft Starting using Euler Integration
clear; clc; close all;

%% TODO: Use a ramp voltage instead of a step voltage to soft start the motor
% Change the voltage step to 200V/s
% Change the voltage step to a ramp finishing to 200V at 0.5s, Change tf to 2 seconds
% Apply a load Cr0 = 10Nm at t = 1s

% Simulation Parameters
tf = 2;   % Final simulation time
dt = 0.0001; % Time step
Ntf = floor(tf / dt); % Number of steps
tk = 1; % Indexing array

% System Parameters
R = 0.8; % Resistance of the motor
L = 0.011; % Inductance of the motor
k = 1.91; % Emf constant
a = 1e-2; % Damping coefficient
J = 0.2; % Moment of inertia of the motor

% Initial Conditions
u = 0; % initial applied voltage
u0 = 200; % step voltage
w = 0; % Initial angular velocity
Cr0 = 0; % Initial torque
I = 0; % Initial current
t = 0; % Initial time

% Initialization
tt = zeros(1, Ntf); % Time vector
It = zeros(1, Ntf); % Current vector    
ut = zeros(1, Ntf); % Voltage vector
wt = zeros(1, Ntf); % Angular velocity vector
Cet = zeros(1, Ntf); % Electromagnetic torque vector

% Euler Integration Loop
while (t < tf)
    % if (t >= 0.01)
    %     % u = 200/0.1 = 2000; % Step voltage
    %     u = u0/0.1 * (t - 0.01); % Ramp voltage
    % end
   
    % Compute derivatives
    dI_dt = (u - R*I - k*w) / L; % Electrical
    Cr = a*w; % Friction torque
    Ce = k*I; % Electromagnetic torque
    dw_dt = (Ce - Cr - Cr0) / J; % Mechanical

    % Update state variables
    I = I + dI_dt * dt;
    w = w + dw_dt * dt;

    % Store results
    tt(tk) = t;
    It(tk) = I;
    ut(tk) = u;
    wt(tk) = w;
    Cet(tk) = Ce;

    % Increment loop counter
    t = t + dt;
    tk = tk + 1;
end

% Plot results
figure;
subplot(2,1,1)
plot(tt, It, tt,ut);
title('Voltage vs Time');
xlabel('Time (s)');
ylabel('Voltage (V)');
legend('Current');

subplot(2,1,2)
plot(tt, wt);
title('Angular Velocity vs Time');
xlabel('Time (s)');
ylabel('Angular Velocity (rad/s)');
legend('Angular Velocity');