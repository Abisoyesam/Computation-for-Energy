function ocv = ocv_sigmoid(soc)
    alpha = 0.01;
    beta = 0.01;

    sigmoid = @(x) 1./(1 + exp(-x));

    ocv = 10 + 2 * sigmoid((soc - 0.1) / beta) + 2 * sigmoid((soc - 0.9) / alpha);
end