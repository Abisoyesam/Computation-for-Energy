function V = PVNR_Inv(I, G, T)
    % Convert temperature to Kelvin
    T = T + 273.15;
    
    % PV module parameters
    % V = V/60;
    Isc = 8.69;      Voc = 0.61;
    Imp = 8.19;      Vmp = 30.55;  
    Pmax_e = 250;    Kv = -0.0038;  
    Ki = 0.004;      Ns = 60;
    
    % Physical constants
    k = 1.3806503e-23;    
    q = 1.60217646e-19;
    
    % Diode and resistance parameters
    a = 1.3;             
    Rs = 0.04 / Ns;      
    Rp = (1.2711e+03) / Ns;
    
    % Reference conditions and temperature difference
    Gn = 1000;         
    Tn = 298.15;       
    dT = T - Tn;
    
    % Thermal voltages (not used directly below, but defined as in PVNR)
    Vt = k*T/q;        
    Vtn = k*Tn/q;
    
    % Calculate the diode reverse saturation current (Io) and the
    % light-generated current (Iph) for the given irradiance G and temperature T.
    Io = (Isc + Ki * dT) / (exp(q * ((Voc) + Kv * dT) / (a * k * Tn)) - 1);
    Iph = (Isc + Ki * dT) * G / Gn;
    
    % Our unknown is V_cell = (module voltage)/Ns. We start with an initial guess.
    V_cell = Vmp / Ns;  % initial guess per cell
    % V_cell = 4;
    
    % Define the function f(V_cell) that should equal zero.
    g = Iph - Io * (exp(q*(V_cell + I * Rs) / (a * k * T)) - 1) - ((V_cell + I * Rs) / Rp) - I;
    
    
    % Newton-Raphson iteration for V_cell
    while (abs(g) > 0.001)
        g = Iph - Io * (exp(q*(V_cell + I * Rs) / (a * k * T)) - 1) - ((V_cell + I * Rs) / Rp) - I;

        exp_term = exp(q*(V_cell + I * Rs) / (a * k * T));
        
        % Derivative of f with respect to V_cell
        glin = - Io * (q / (a * k * T)) * exp_term - (1 / Rp);
        
        if glin == 0
            error('Derivative zero, cannot proceed');
        end
        
        % Update V_cell
        V_cell = V_cell - g / glin;
        
        
        % Recompute f with the new V_cell
        g = Iph - Io * (exp(q*(V_cell + I * Rs) / (a * k * T)) - 1) - ((V_cell + I * Rs) / Rp) - I;
    end
    
    % Convert per-cell voltage back to module voltage
    V = V_cell * Ns;
    
    % Ensure voltage is non-negative
    if V < 0
        V = 0;
    end
end
