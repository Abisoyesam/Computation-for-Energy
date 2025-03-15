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

%% Fixed-point iteration loop
fprintf('Resolution by Fixed-point theorem\n');
pt_xt = [];                    % x initialized as empty array
while (e > elim) && (iter < iterlim)
    xnew = cos(x);          % g_function(x) = cos(x)
    e = abs(xnew - x);      % compute absolute error
    x = xnew;               % update the approximation
    iter = iter + 1;           % increment the iteration counter

    pt_xt(iter)= x;
    fprintf('iter %d: x = %.8f, e = %.8e\n', iter, x, e);
end
pt_fxt = pt_xt - cos(pt_xt);   % f(x) = x - cos(x)
fprintf('The fixed point is approximately x = %.8f\n', x);
% fprintf('\n');

%% Newton Raphson iteration loop 

fprintf('\nResolution by Netwon Raphson Method\n');
iter = 0;          % iteration counter
iterlim = 100;     % maximum number of iterations
elim = 1e-6;       % error tolerance
x0 = 0;            % initial guess (you can change this value)
x = x0;            % current approximation
e = 1e20;          % initialize error to a large number

nr_xt = [];  nr_fxt = [];    % x and f(x) initialized as empty array

while ((e > elim) && (iter < iterlim))
    f = x - cos(x);
    df = 1 + sin(x);
    if df ~= 0
        xnew = x - (f/df);
    else
        fprintf('df = 0, end of iterations\n');
        break
    end
  
    e = abs(xnew - x);
    x = xnew;
    iter = iter + 1;

    nr_xt(iter) = x;
    nr_fxt(iter) = f;
   
    fprintf('iter %d: x = %.6f, e = %.5e\n', iter, x, e);
end

fprintf('\nThe fixed point is approximately x = %.6f\n', x);

%% Plot the graphs
plot(pt_xt, pt_fxt, '-r')
hold;
plot(nr_xt, nr_fxt, 'o-b')
