%Test
% figure
%  x = rand(1,41);
%  y = 1:41;
%  plot(x,y,'r.');
%  line([x(21) x(21)],[0 41]);
% %  set(gca,'YLim',[0 41])
% x = 0:1:50;
% maxX = 50;
% 
% a = abs(log10(0.9*x/maxX + 0.1))
% 
% plot(a)

A = [ 0 0 1 0;
      1 2 10 15;
      8 11 19 10];
  
 ans1 = max(max(A))
 
 [rowMax,colMax] = find(A ==ans)
 
 A(rowMax, colMax)
