function val = rastrigin(x)
    % the parameter x is the current position and an array
    % dimentionality (m)
    m = length(x);

    % rastrigin function Eval = 3m + summation(x^2 - 3cos(2*pi*x)
    sum = 0;
    % find the summation by looping through
    for i = 1 : m
        sum = sum + (x(i).^2 - 3*cos(2*pi*x(i)));
    end
    
    % fitness value of the rastrigin function
    val = 3*m + sum;
end