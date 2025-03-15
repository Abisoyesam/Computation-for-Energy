%% Battery SoC Estimation with Euler Method in MATLAB

% Battery parameters
batteryCapacity = 3000;   % Battery capacity in mAh
initialSOC = 1;           % Initial State-of-Charge (100%)

% Simulation parameters
T_total = 2;              % Total simulation time in hours
dt = 1/60;                % Time step (1 minute in hours)
t = 0:dt:T_total;         % Time vector (in hours)

% Preallocate arrays
SoC = zeros(size(t));     % Battery SoC vector (normalized, 1 = 100%)
SoC(1) = initialSOC;      % Set initial SoC to 100%
I = zeros(size(t));       % Current consumption vector (in mA)

% Define usage scenarios based on time (converted to minutes)
for k = 1:length(t)
    current_time = t(k)*60;  % Convert time from hours to minutes
    if current_time <= 30
        I(k) = 10;         % 0-30 min: Standby mode (10 mA)
    elseif current_time <= 45
        I(k) = 200;        % 30-45 min: Active call (200 mA)
    elseif current_time <= 60
        I(k) = 50;         % 45-60 min: Browsing (50 mA)
    elseif current_time <= 90
        I(k) = 400;        % 60-90 min: Video streaming (400 mA)
    else
        I(k) = 10;         % 90-120 min: Standby mode (10 mA)
    end
end

% Numerical integration using the Euler method
for k = 2:length(t)
    % Calculate the mAh consumed in this time step
    consumed = I(k-1) * dt;  % dt is in hours, so I in mA gives mAh
    % Update the remaining battery capacity
    remainingCapacity = SoC(k-1)*batteryCapacity - consumed;
    % Update the SoC (normalized)
    SoC(k) = remainingCapacity / batteryCapacity;
    % Ensure SoC does not drop below 0
    SoC(k) = max(SoC(k), 0);
end

% Plot the Battery SoC over time
figure;
plot(t*60, SoC*100, 'LineWidth', 2);
xlabel('Time (minutes)');
ylabel('Battery SoC (%)');
title('Battery State-of-Charge Estimation');
grid on;
