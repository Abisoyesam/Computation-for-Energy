close all, clear; clc;
crv_table = readtable("flat.csv");
crv = table2array(crv_table(:,2:24));
%plot (crv(2:24,1), crv(2:,2))
h=[0:23];
hc=[0:0.1:23];
figure
%stackedplot(crv);
crvn = crv./mean(crv);
figure
y=crvn(:,2);
plot(h, y);
p = polyfit(h,y,7);
y1 = polyval(p,hc);
figure
plot(h,y,'o')
hold on
plot(hc,y1)
hold off