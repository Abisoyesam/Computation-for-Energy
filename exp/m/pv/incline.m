function inc = incline(azs, inclineMax)
% calcule l'inclinaison pour un angle d'azimut donné
inc = asin( sin(inclineMax) *cos(azs));	% h en rd
end