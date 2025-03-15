% Gauss-Seidel example
clc,clear all,close all
%% variables
n = 3; % numbers of equations and unknowns
A = [45 2 3; -3 22 2; 5 1 20]; % 3×3 coefficient matrix of the equation
B = [58 47 67]; % (RHS) numbers after equals to [A] = B
X = [0 0 0]; % initial guess
Xnew = [0 0 0]; % placeholder to store newly computed value but initialized with zeros
iterlim = 100; % numbers of iterations (a safeguard in case it doesn.t converge quickly).
elim = 1e-6; % The tolerance for stopping (good solution if computed error is less than this).
e = 1e20; % large initial value for the error, ensuring the while loop will run at least once
iter = 0; % iteration counter starts at 0.
%% Loop
while ((e > elim) && (iter < iterlim))
    e=0;
    for i = 1:n
        sum = B(i);
        for j = 1:i-1
            sum = sum -A(i,j)*Xnew(j);
        end
        for j = i+1:n
            sum = sum -A(i,j)*X(j);
        end
        Xnew(i) = sum/A(i,i);
        e = e + abs(Xnew(i)-X(i));
    end

    X = Xnew;
    iter = iter +1;
    fprintf ("iter %d e = %g\n", iter, e);
    disp(X);
    fprintf ("\n");
end