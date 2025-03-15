% DC Current Control using PI Controller

% System Parameters
R = 0.8;        % Resistance (Ohms)
L = 0.011;      % Inductance (H)
k = 1.91;       % Motor constant (Nm/A)
J = 0.02;       % Inertia (kg*m^2)
a = 1e-2;       % Damping coefficient
Te = L / R;     % Electrical time constant
Tem = J * R / k^2; % Mechanical time constant
G = 20;         % Gain
Ts = 0.2e-3;    % Sampling time (s)

% PI Controller Parameters
Kp = 1;         % Proportional gain
Ki = 1;         % Integral gain

% Simulation Parameters
t_end = 1;      % End time for simulation (s)
t = 0:Ts:t_end; % Time vector

% Initialize variables
i_ref = 1;      % Reference current (A)
i = 0;          % Initial current (A)
e_int = 0;      % Integral of error
u = 0;          % Control input
k = 1;          % Time step index

% Preallocate arrays for storing results
i_array = zeros(size(t));
u_array = zeros(size(t));

% Simulation loop
while k <= length(t)
    % Error calculation
    e = i_ref - i;
    
    % PI Controller
    e_int = e_int + e * Ts;
    u = Kp * e + Ki * e_int;
    
    % System dynamics (discrete-time approximation)
    di_dt = (u - R * i) / L;
    i = i + di_dt * Ts;
    
    % Store results
    i_array(k) = i;
    u_array(k) = u;
    
    % Increment time step index
    k = k + 1;
end

% Plot results
figure;
subplot(2,1,1);
plot(t, i_array);
title('Current Response');
xlabel('Time (s)');
ylabel('Current (A)');
grid on;

subplot(2,1,2);
plot(t, u_array);
title('Control Input');
xlabel('Time (s)');
ylabel('Control Input (V)');
grid on;