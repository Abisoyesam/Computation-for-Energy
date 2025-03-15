function [Is, Vs, Ps] = plotpv_inv(Suns, Tc)
    % plotpv_inv generates the inverse PV curves using PVNR_Inv.
    % It first finds the maximum power point using mppt_inv,
    % then sweeps over a range of current values to build I-V and I-P curves.
    
    % Obtain the maximum power point (MPP) from mppt_inv:
    [Is, Vs, Ps] = mppt_inv(Suns, Tc);
    
    % Define a sweep over current values.
    % We use 0 to slightly above the short-circuit current (~8.69 A), e.g. 0 to 9 A.
    N = 50;
    Imax = 9;  
    It = linspace(0, Imax, N+1);
    Vt = zeros(1, N+1);
    Pt = zeros(1, N+1);
    
    % For each current, calculate the corresponding voltage and power.
    for k = 1:length(It)
        I_val = It(k);
        V_val = PVNR_Inv(I_val, Suns, Tc);
        Vt(k) = V_val;
        Pt(k) = I_val * V_val;
    end
    
    % Plot the V = f(I) curve.
    figure(1)
    plot(It, Vt, 'b-', 'LineWidth', 2);
    hold on;
    plot(Is, Vs, 'ob', 'MarkerSize',8, 'MarkerFaceColor','b');
    axis([0 10 0 40 ]);
    xlabel('Current, I (A)');
    ylabel('Voltage, V (V)');
    title('PV Module: Voltage vs. Current');
    grid on;
    hold off;
    
    % Plot the P = f(I) curve.
    figure(2)
    plot(It, Pt, 'r-', 'LineWidth', 2);
    hold on;
    plot(Is, Ps, 'ob', 'MarkerSize',8, 'MarkerFaceColor','b');
    % axis([0 300 0 40]);
    xlabel('Current, I (A)');
    ylabel('Power, P (W)');
    title('PV Module: Power vs. Current');
    grid on;
    hold off;
end

%{
 function [Vs, Is, Ps] = plotpv_inv(Suns,Tc)
    [Vs Is Ps] = mppt_inv(Suns,Tc);
    Vt=zeros(1,50);
    It=zeros(1,50);
    Pt=zeros(1,50);
    for k = 0:1:50
        V = k*40/50;
        I = PVNR_Inv(V,Suns,Tc);
        P=V*I;
        Vt(k+1)= V;
        It(k+1)= I;
        Pt(k+1)= P;
    end

    figure(1)
    plot(It, Vt);
    axis([0 40 0 10]);
    ylabel('V(V)');
    xlabel('I(A)');
    hold on;
    plot(Is,Vs,'ob');
    grid on;

    figure(2)
    plot(Pt,Vt);
    axis([0 40 0 300]);
    ylabel('V(V)');
    xlabel('P(W)');
    grid on;
    hold on;
    plot(Ps,Vs, 'ob');
end 
%}
