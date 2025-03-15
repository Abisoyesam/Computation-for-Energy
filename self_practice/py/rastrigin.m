function val = rastrigin(x)
    % RASTRIGIN evaluates the Rastrigin function for an m-dimensional vector x.
    % The function is defined as:
    %   val = 3*m + sum( x.^2 - 3*cos(2*pi*x) )
    % where m is the length of x.
    
    m = length(x);
    val = 3 * m + sum(x.^2 - 3 * cos(2*pi*x));
end
