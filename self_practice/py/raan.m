clear all; close all; clc;

a = -5:0.01:5;
x_axis = (a+1).^2 .* sin(a);

plot(x_axis)
grid
ylim([-5 5]) 