% Gauss seidel numerical method
clc; clear; close all;

n = 4; % no of iterations
A = [
    3 2 1 -1;
    5 7 -1 0;
    -2 3 3 1;
    0 2 1 5
]; % Coefficient matrix
B = [1 2 3 4]; % RHS matrix
x = [1 1 1 1]; % Coefficient matrix [x1 x2 x3 x4]
xnew = [0 0 0 0]; % Initial guess
iter = 0; % Iteration counter
iterlim = 100; % Maximum number of iterations
elim = 1e-6; % Error limit
e = 1e20; % Initial error

% Gauss seidel iteration
while ((e > elim)&&(iter < iterlim))
    e = 0; % Reset error: e != ||xnew - x|| in GS
    for i = 1:n
        sum = B(i); % Sum of RHS matrix
        for j = 1:i-1
            sum = sum - A(i,j)*xnew(j); % Forward substitution
        end
        for j = i+1:n
            sum = sum - A(i,j)*x(j); % Backward substitution
        end
        xnew(i) = sum/A(i,i); % Update xnew
        e = e + abs(xnew(i) - x(i)); % Update error
    end
    x = xnew; % Update x
    iter = iter + 1; % Update iteration counter
    fprintf('iter=%d e=%g\n', iter, e);
    disp('----------------------------------------');
    disp(x);
    fprintf('\n');
end

% Solving by MATLAB Inverse Matrix
x = A\B';
% x = inv(A)*B'; % Alternative method
disp('Solution by MATLAB Inverse Matrix');  
disp(x);