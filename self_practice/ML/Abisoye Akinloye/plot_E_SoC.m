clc; clear; close all;

% Parameters
SoC_Vec = linspace(1, 0, 1000); % Create a vector of SoC values from 1 (full) down to 0 (empty)
% Qn = 4400;         % Nominal capacity in Ah
% ts = 1/3600;     % Sampling period in hours (1 sec = 1/3600 h)
% SoC = 1.0;       % Initial state-of-charge (1 = fully charged)

% Compute OCV using the linear and sigmoid smoothing
ocv_linear = ocv(SoC_Vec); % Call the ocv function
ocv_sigmoid = ocv_sigmoid(SoC_Vec); % Call the ocv_sigmoid function

subplot(2,1,1);
plot(SoC_Vec, ocv_linear, 'b');
xlabel('State of Charge (SoC)');
ylabel('OCV (V)');
title('Linear OCV-SoC');
grid on;
set(gca, 'XDir', 'reverse');  % Reverse the x-axis so 1 is on the left

subplot(2,1,2);
plot(SoC_Vec, ocv_sigmoid, 'r');
xlabel('State of Charge (SoC)');
ylabel('OCV (V)');
title('Sigmoid Smoothed OCV-SoC');
grid on;
set(gca, 'XDir', 'reverse');
% gca -> get current axis. By setting the 'XDir' property to 'reverse', the x-axis is flipped so that the values decrease from left to right

figure;
plot(SoC_Vec, ocv_linear, 'b');
hold on;
plot(SoC_Vec, ocv_sigmoid, '--r');
xlabel('State of Charge (SoC)');
ylabel('OCV (V)');
grid on;
set(gca, 'XDir', 'reverse');
% gca -> get current axis. By setting the 'XDir' property to 'reverse', the x-axis is flipped so that the values decrease from left to right