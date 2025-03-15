% Numerical Integration Using Fixed-Point Iteration and Newton-Raphson Method
% f(x) = x - cos(x)

clear; clc; close all;

%% Variables
iter_fp = 0; iter_nr = 0;             % iteration counter for fixed-point and Newton-Raphson
iterlim_fp = 100; iterlim_nr = 100;   % maximum number of iterations for fixed-point and Newton-Raphson
elim_fp = 1e-6; elim_nr = 1e-6;       % error tolerance for fixed-point and Newton-Raphson
x0_fp = 0; x0_nr = 0;                 % initial guess for fixed-point and Newton-Raphson
x_fp = x0_fp; x_nr = x0_nr;           % current approximation
e_fp = 1e20; e_nr = 1e20;             % initialize error to a large number

%% Fixed-point iteration loop
fprintf('Resolution by Fixed-point theorem\n');
pt_xt = zeros(1, iter_fp);            % x initialized as empty array
pt_fxt = zeros(1, iter_fp);           % f(x) initialized as empty array

while (e_fp > elim_fp) && (iter_fp < iterlim_fp)
    % fixed point method: x = g(x) = cos(x)
    xnew = cos(x_fp);                 % g_function(x) = cos(x)
    e_fp = abs(xnew - x_fp);             % compute absolute error
    x_fp = xnew;                      % update the approximation
    iter_fp = iter_fp + 1;            % increment the iteration counter

    pt_xt(iter_fp)= x_fp;             % store the x value
    pt_fxt(iter_fp) = x_fp - cos(x_fp);

    fprintf('iter %d: x = %.8f, e = %.8e\n', iter_fp, x_fp, e_fp);
end
fprintf('The fixed point is approximately x = %.8f\n', x_fp);

%% Newton Raphson iteration loop

fprintf('\nResolution by Netwon Raphson Method\n');
nr_xt = zeros(1, iter_nr);  nr_fxt = zeros(1, iter_nr);    % x and f(x) initialized as empty array

while ((e_nr > elim_nr) && (iter_nr < iterlim_nr))
    f = x_nr - cos(x_nr);            % f(x) = x - cos(x)
    df = 1 + sin(x_nr);              % f'(x) = 1 + sin(x) differentiating f(x)
    if df ~= 0
        xnew = x_nr - (f/df);        % Newton-Raphson formula
    else
        fprintf('df = 0, end of iterations\n');
        break
    end

    e_nr = abs(xnew - x_nr);
    x_nr = xnew;
    iter_nr = iter_nr + 1;

    nr_xt(iter_nr) = x_nr;
    nr_fxt(iter_nr) = f;

    fprintf('iter %d: x = %.6f, e = %.5e\n', iter_nr, x_nr, e_nr);
end

%% Plot the graphs
plot(pt_xt, pt_fxt, '-r')
hold;
plot(nr_xt, nr_fxt, 'o-b')
ylabel('f(x) = x - cos(x)')
xlabel('x')
legend('Fixed-Point', 'Newton-Raphson','Location','southwest')
title('Numerical Integration Using Fixed-Point and Newton-Raphson Method')