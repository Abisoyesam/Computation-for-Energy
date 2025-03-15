function V = terminal_voltage(I, SoC)
    r = 0.05; % Internal resistance in ohms
    V = ocv(SoC) - r * I;
end