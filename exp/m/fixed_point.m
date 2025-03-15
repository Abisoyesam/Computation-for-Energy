% Fixed-Point Iteration for solving x = cos(x)
clc; close all; clear all;

fprintf('Resolution by Fixed-point theorem\n');

% variables
iter = 0;          % iteration counter
iterlim = 100;     % maximum number of iterations
elim = 1e-6;       % error tolerance
x0 = 0;            % initial guess (you can change this value)
x = x0;            % current approximation
e = 1e20;          % initialize error to a large number

pt_xt = []; 


% Fixed-point iteration loop
while (e > elim) && (iter < iterlim)
    xnew = cos(x);          % g_function(x) = cos(x)
    e = abs(xnew - x);      % compute absolute error
    x = xnew;               % update the approximation
    iter = iter + 1;        % increment the iteration counter

    pt_xt(iter)= x;
    fprintf('iter %d: x = %.8f, e = %.8e\n', iter, x, e);
end
pt_fxt = pt_xt - cos(pt_xt);
fprintf('The fixed point is approximately x = %.8f\n', x);

figure(1)
plot(pt_xt, pt_fxt)

