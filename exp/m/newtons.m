function x = newtons(f, df, x0, tol, N)
i = 1;
error = 100;

while (i < N && error > tol)
    x = x0 - (f(x0)/df(x0));
    error = abs(x - x0);
    dat(i,:) = [i x0 x f(x)];

    i = i+1;
    x0 = x
end

dat