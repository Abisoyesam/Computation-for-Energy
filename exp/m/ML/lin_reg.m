close all, clear; clc;
rng(1);
x = 1:50; 
y = -0.3*x + 2*randn(1,50); 
% p = polyfit(x,y,1); 
% f = polyval(p,x); 
% figure
% plot(x,y,'o') 
% legend('data') 
% figure
% plot(x,y,'o',x,f,'-') 
% legend('data','linear fit')

% how fit
figure
[p,S] = polyfit(x,y,1); 
[y_fit,delta] = polyval(p,x,S);plot(x,y,'bo')
hold on
plot(x,y_fit,'r-')
plot(x,y_fit+2*delta,'m--',x,y_fit-2*delta,'m--')
title('Linear Fit of Data with 95% Prediction Interval')
legend('Data','Linear Fit','95% Prediction Interval')