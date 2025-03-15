clear

t = 0, dt = 0.01, tf = 1; % 1 second

i=1

while (t <= tf) 
 tt(i) = t;
 xt(i) = sin(10*t);
 zt(i) = 10*t; % helicoidal motion
 t = t + dt;
 i = i + 1;
end

figure (1)
plot(tt, xt, tt, zt)