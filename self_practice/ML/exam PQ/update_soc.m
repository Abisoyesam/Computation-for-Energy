%% Battery and Simulation Parameters
batteryCapacity = 4400;    % Battery capacity in mAh
initialSOC      = 100;     % Initial state-of-charge in percentage

t_total = 3600;            % Total simulation time in seconds (1 hour)
dt      = 1;               % Time step in seconds
time    = 0:dt:t_total;    % Time vector

% Example Current Profile:
% Assuming a constant discharge current of 1000 mA.
current = 1000 * ones(size(time));   % Current in mA

%% Initialize SoC Vector
SOC = zeros(size(time));
SOC(1) = initialSOC;

%% Calculate SoC Over Time Using a While Loop
i = 2;  % Start from the second element since the first is the initial SOC
while i <= length(time)
    % Calculate charge consumed during the time step (in mAh)
    consumed = current(i-1) * (dt / 3600);
    
    % Update SoC: subtract the percentage of battery capacity consumed
    SOC(i) = SOC(i-1) - (consumed / batteryCapacity) * 100;
    
    % Increment index
    i = i + 1;
end

%% Plot the Battery SoC vs. Time
figure;
plot(time/60, SOC, 'LineWidth', 2);
xlabel('Time (minutes)');
ylabel('State of Charge (%)');
title('Battery State of Charge vs. Time');
grid on;
