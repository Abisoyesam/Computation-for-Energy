function E = E_soc(soc)
    if soc > 1 || soc < 0
        disp("Soc is out of range")
    elseif soc > 0.9
        E = 20*soc - 6;
    elseif soc > 0.1
        E = 12;
    else
        E = 20*soc + 10;
    end
end