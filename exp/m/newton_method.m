% Newton Raphson Method for solving x-cos(x)
clc; close all; clear all;

fprintf('Resolution by Netwon Raphson Method\n');

% variables
iter = 0;
iterlim = 100;
elim = 1e-6;
x0 = 0;
x = x0;
e = 1e20;

nr_xt = [];  nr_fxt = [];

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
   
    fprintf('iter %d: x = %.6f, e = %.5e\n', iter-1, x, e);
end



fprintf('The fixed point is approximately x = %.6f\n', x);

figure(1)
plot(nr_xt, nr_fxt)
