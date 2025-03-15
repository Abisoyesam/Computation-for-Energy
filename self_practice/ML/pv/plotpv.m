function [Vs, Is, Ps] = plotpv(Suns,Tc)
[Vs Is Ps] = mppt(Suns,Tc);
Vt=zeros(1,50);
It=zeros(1,50);
Pt=zeros(1,50);
for k = 0:1:50
    V = k*40/50;
    I = PVNR(V,Suns,Tc);
    P=V*I;
    Vt(k+1)= V;
    It(k+1)= I;
    Pt(k+1)= P;
end
figure(1)
plot(Vt,It);
axis([0 40 0 10]);
xlabel('V(V)');
ylabel('I(A)');
hold on;
plot(Vs, Is,'ob');
grid on;
figure(2)
plot(Vt,Pt);
axis([0 40 0 300]);
xlabel('V(V)');
ylabel('P(W)');
grid on;
hold on;
plot(Vs, Ps,'ob');
end