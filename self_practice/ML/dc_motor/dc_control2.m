% Current control of a DC motor Using Numerical Integration (Euler) and Proportional-Integral (PI) Controller
clc; clear; close all;

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
Ki_d = Ki * Ts; % Discrete-time integral gain

%% Initialization
tf = 0.005; % Total simulation time in seconds (try 2s)
dt = 0.01e-3; % Time step in seconds
Ntf = floor(tf/dt); % Number of simulation steps
tk = 1; % Indexing array

% System variables
u = 0; % Control input
I = 0; w = 0; Iref = 0; % Current, speed, reference current (try 8A)
vlim = 10; % maximum 200V = 10*G
Iref0 = 1; % Step value of reference current
Cr0 = 0;
Flagtm = 0; % Flag for time
xe = 0; % Integrator state in the PI controller.
t = 0; % Initial time 

% Arrays for storing simulation data
tt = zeros(1,Ntf); % Time vector in seconds
It = zeros(1,Ntf); % Current vector
wt = zeros(1,Ntf); % Speed vector
ut = zeros(1,Ntf); 
vt = zeros(1,Ntf); 
xet = zeros(1,Ntf);
Cet = zeros(1,Ntf);

%% Numerical integration (Euler) + Discrete-time PI Controller

while (t < tf)
    if t >= 0.001
        Iref = Iref0; % Step value of reference current
    end
    tm100 = t - (Ts*floor(t/Ts)); % Time modulo Ts
    % Flagtm logic: ensures the PI controller executes once every 200 µs.
    if (tm100 > dt) 
        Flagtm = 0; 
    end
    if ((tm100 <= dt) && (Flagtm == 0))
        Flagtm = 1;
        % PI Controller
        e = Iref - I; % Error is the difference between desired and actual current.
        xe = xe + Ki_d * e; % Discrete integral update
        v = Kp * e + xe; % PI output before multiplying by gain G

        % TODO: Saturate the control input Exam Question
        % Saturate the control input
        % if (u > vlim)
        %     u = vlim;
        % elseif (u < -vlim)
        %     u = -vlim;
        % end
    end
    u = G*v; % Final control input (voltage) applied to the motor

    % System dynamics
    di_dt = (u - R*I) / L; % Current rate of change
    Ce = k*I; % Electromagnetic torque
    dw = (Ce - a*w - Cr0)/J; % Speed rate of change

    % Update states (Euler Integration)
    I = I + di_dt * dt; % Current
    w = w + dw * dt; % Speed

    % Store the simulation data
    tt(tk) = t; % Time vector
    It(tk) = I; % Current vector
    wt(tk) = w; % Speed vector
    ut(tk) = u; % Control input vector
    vt(tk) = v; % Control input vector
    xet(tk) = xe; % Integrator state vector
    Cet(tk) = Ce; % Electromagnetic torque vector

    % Update time and index for the next iteration
    t = t + dt; % Update time
    tk = tk + 1; % Update index
end

%% plot results 
figure;
subplot(2,1,2),plot(tt, wt,'k');
legend('w(rd/s)');
subplot(2,1,1),plot(tt,It,'b');
legend('I(A)'); 
figure;
subplot(2,1,2),plot(tt, vt,'k');
legend('v(V)');
subplot(2,1,1),plot(tt,xet,'b');
legend('xe(V)')