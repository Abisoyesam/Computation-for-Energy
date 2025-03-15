clc; clear; close all;

tf = 2; % total time in hours
dt = 1/60; % time step in minute
tk = 1; % arrary index
t = 0; % initial time
Ntf = floor(tf/dt); % numbers of step

% Battery Parameter
Qn = 4.4; %4400mAh
I = 1; % 1A
s = 1; % state of charge

% array filling
ss = zeros(1,Ntf);
tt = zeros(1, Ntf);


while t < tf
    % rectangular method
    s = s - (I*dt)/Qn;

    % fill the array
    ss(tk) = s;
    tt(tk) = t;

    % increment
    t = t + dt;
    tk = tk + 1;
end

% plot
plot(tt,ss)
ylabel("SoC")
xlabel("Time (s)")