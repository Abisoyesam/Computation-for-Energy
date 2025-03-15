function harmo=harm(i)
%calculation of the number of harmonic n ° i
if (i==0) harmo=1;
 elseif (rem(i,2)==1) harmo=i*3+2;
 else harmo=(i/2)*6+1;
 end
%disp( sprintf('i=%d harm=%d', i,harmo));