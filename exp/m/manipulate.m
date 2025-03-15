% Given string
tempStr = '73.5 °F';

% Extract the numeric value using sscanf
numValue = sscanf(tempStr, '%f');

% Display the numeric value
disp(numValue);