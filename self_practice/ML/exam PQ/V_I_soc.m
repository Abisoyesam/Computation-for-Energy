
function V = V_I_soc(I, Soc)
    r = 0.05; % internal resistance in ohms
    E = E_soc(Soc); % Ocv corresponding to the SoC
    V = E - I*r;
end