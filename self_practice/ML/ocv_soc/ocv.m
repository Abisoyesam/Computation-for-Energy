function ocv = ocv(soc)
    ocv = zeros(size(soc));

    for i = 1:length(soc)
        s = soc(i);
        if (s > 1 || s < 0)
            disp('SoC value out of range')
        elseif s > 0.9
            ocv(i) = 20 * s - 6;
        elseif s > 0.1
            ocv(i) = 12; % constant OCV
        else 
            ocv(i) = 20 * s + 10;
        end
    end
end