% Battery SOC Estimation: Discharging, Charging, and Idle Scenario
clc; clear; close all;

%% Simulation parameters
tf = 24; % Total simulation time in hours
dt = 1/10; % Accelerated time step (0.1)
Ntf = floor(tf/dt); % Number of simulation steps
t = 0; % Initial time in hour

%% Battery parameters
Qn = 5; % Battery capacity in Ah
soc = 1; % Initial SoC (100%)
i_dl = 0.0167; % Idle current consumption in A
i = 0; % Initial current consumption in A

%% Initialization
tt = zeros(1,Ntf); % Time vector 
soc_t = zeros(1,Ntf); % SoC vector
i_c = zeros(1,Ntf); % Current charging vector
i_d = zeros(1,Ntf); % Current discharging vector
tk = 1; % Indexing array

%% Numerical integration (Euler) using a while loop
while (t < tf)
    if t <= 9 
        i = i_dl; % Idle scenario (0 - 9am)
        i_d(tk) = i;
    elseif t > 9 && t <= 10.5
        i = 0.1; % Standby scenario (9 - 10:30am)
        i_d(tk) = 1;
    elseif t > 10.5 && t <= 10.75
        i = 0.5; % Tea break (10:30 - 10:45am)
        i_d(tk) = i;
    elseif t > 10.75 && t <= 12
        i = i_dl; % Idle scenario
        i_d(tk) = i;
    elseif t > 12 && t <= 13.5
        i = 1.0; % Lunch break (12 - 1:30pm)
        i_d(tk) = i;
    elseif t > 13.5 && t <= 15
        i = 0.1; % Standby mode (1:30 - 3pm)
        i_d(tk) = i;
    elseif t > 15 && t <= 15.25
        i = 0.55; % Short break (3 - 3:15pm)
        i_d(tk) = i;
    elseif t > 15.25 && t <= 16 
        i = 0.1; % Standby mode (3:15 - 4pm)
        i_d(tk) = i;
    elseif t > 16 && t <= 17 
        i = 0.3; % Bus station and Surfing Internet (4 - 5:00pm)
        i_d(tk) = i;
    elseif t > 17 && t <= 18.5
        i = -1.6973; % Charging Mode (5 - 6:30pm)
        i_c(tk) = i;
    elseif t > 18.5 && t <= 22
        i = 1.32; % Discharging mode (6:30 - 10pm)
        i_d(tk) = i;
    elseif t > 22 && t <= 23.5   
        i = -2.98; % Charging (10 - 11:30pm)
        i_c(tk) = i;
    else
        i = i_dl; % Idle mode (11:30 - 12am)
        i_d(tk) = i;
    end

    % Euler integration
    % deltaQ = i * dt; % Current consumption
    % soc = soc - deltaQ/Qn; % Discharging scenario
    soc = soc - (i * dt)/Qn; % Discharging scenario

    % Fill the array
    soc_t(tk) = soc;
    tt(tk) = t;

    % Increment
    t = t + dt;
    tk = tk + 1;
end

%% Plot Figure
figure;
plot(tt,soc_t*100)
xlabel('Time (hours)');
ylabel('SoC (%)');
title('Battery SoC Estimation');
xlim([0 tf])
ylim([0 102])
legend('Current','Location','southeast');
grid on;

figure;
plot(tt,i_d, tt, i_c);
xlim([9 tf])
xlabel('Time (hours)');
ylabel('Current (Ah)');
title('Charging and Discharging Cycle');
legend('Discharging Cycle','Charging Cycle','Location','southwest');