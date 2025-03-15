clear; clc; close all;
format longg

f = @(x) x.^3 - 2*x.^2 - 5;
df = @(x) 3*x.^2 - 4*x;

x0 = 2;
tol = 10e-4;
N = 50;

x = newtons(f, df, x0, tol, N);
fprintf("The estimated solution is %.6f\n", x)