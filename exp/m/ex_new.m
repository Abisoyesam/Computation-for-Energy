clear; clc; close all;
format longg

f = @(x) x - cos(x);
df = @(x) 1 + sin(x);

x0 = 0;
tol = 1e-6;
N = 100;

x = newtons(f, df, x0, tol, N);
fprintf("The estimated solution is %.6f\n", x)