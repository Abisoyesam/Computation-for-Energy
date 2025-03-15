function [I, V, P] = mppt_inv(Suns, Tc)
    % mppt_inv uses the inverse PV model to track the maximum power point,
    % where V = PVNR_Inv(I, Suns, Tc) and P = I*V.
    
    % Initial guess for current (A)
    % Using a value near the maximum power current (Imp ~8.19 A from PVNR)
    I = 8;  
    V = PVNR_Inv(I, Suns, Tc);
    P = I * V;
    
    % Define a small step for current perturbation (A)
    D = 0.1;
    I_new = I + D;
    
    % Initialization for the iteration
    dP = 1;
    iter = 0;
    
    while (abs(dP) > 1e-2 && iter < 200)
        iter = iter + 1;
        
        % Compute the new voltage and power using the inverse model
        V_new = PVNR_Inv(I_new, Suns, Tc);
        P_new = I_new * V_new;
        
        % Calculate changes in power and current
        dP = P_new - P;
        dI = I_new - I;
        
        % Update the current, voltage and power for next iteration
        I = I_new;
        V = V_new;
        P = P_new;
        
        % Display iteration data
        sprintf('iter=%d I=%g V=%g, P=%g\n', iter, I, V, P);
        
        % Perturb the current: if the power increased, continue in the same direction;
        % if it decreased, reverse the direction of the current perturbation.
        if dP > 0
            if dI > 0
                I_new = I + D;
            else
                I_new = I - D;
            end
        else
            if dI < 0
                I_new = I + D;
            else
                I_new = I - D;
            end
        end
    end
    
    % Final current value is taken as I_new and recalc voltage and power
    I = I_new;
    V = PVNR_Inv(I, Suns, Tc);
    P = I * V;
end

%{
 function [V, I, P] = mppt_inv(Suns,Tc)
    V=20;
    I = PVNR_Inv(V,Suns,Tc);
    P=I*V;
    D = 0.1;
    Vnew = V+D;
    dP = 1;
    iter=0;
    
    while (abs (dP)> 1e-2 && (iter<200))
        iter=iter+1;
        I = PVNR_Inv(Vnew,Suns,Tc);
        Pnew=I*Vnew;
        dP = Pnew-P;
        dV = Vnew - V;
        V = Vnew;
        P = Pnew;
        sprintf('iter=%d V=%g I=%g, P=%g\n', iter, V, I, P);
        if (dP>0)
            if (dV>0) Vnew = V + D;
            else Vnew = V - D;
            end
        else
            if (dV<0) Vnew = V + D;
            else Vnew = V - D;
            end
        end
        
    end
    % I;
    V=Vnew;
    P=I*V;
    end 
%}
