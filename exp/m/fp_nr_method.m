%% Assessment: Fixed-Point Theorem and Newton-Raphson
% equation x - cos(x)
clear; clc; close all;
%% Variables

iter = 0;          % iteration counter
iterlim = 100;     % maximum number of iterations
elim = 1e-6;       % error tolerance
x0 = 0;            % initial guess (you can change this value)
x = x0;            % current approximation
e = 1e20;          % initialize error to a large number

pt_xt = [];        % fixed point x values
nr_xt = [];        % newton raphson x values

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

% Newton Raphson iteration loop
x0 = 0;            % initial guess (you can change this value)
x = x0;            % current approximation
iter_2 = 1;
while ((e > elim) && (iter_2 < iterlim))
    f = x - cos(x);
    df = 1 + sin(x);

    xnew = x - (f/df);

    if (df ~= 0)
        xnew = x - (f/df);
    else
        fprintf('df = 0, end of iterations\n');
        break
    end

    e = abs(xnew - x);
    x = xnew;
    iter_2 = iter_2 + 1;

    nr_xt(iter_2) = x;

   
    fprintf('iter %d: x = %.6f, e = %.5e\n', iter, x, e);
end

nr_fxt = nr_xt - cos(nr_xt);

% Plot the graphs
figure(1)
plot(pt_xt, pt_fxt)
hold;
plot(nr_xt, nr_fxt)
