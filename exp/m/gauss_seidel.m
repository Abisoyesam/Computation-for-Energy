% Gauss-Seidel example
clc,clear all,close all
n = 4;
A = [3 2 1 -1; 5 7 -1 0; -2 3 3 1; 0 2 1 5];
B = [1 2 3 4];
X = [1 1 1 1];
Xnew = [0 0 0 0];
iterlim = 100;
elim = 1e-6;
e = 1e20;
iter = 0

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