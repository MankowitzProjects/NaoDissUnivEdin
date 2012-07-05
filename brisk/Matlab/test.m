%Test
figure
 x = rand(1,41);
 y = 1:41;
 plot(x,y,'r.');
 line([x(21) x(21)],[0 41]);
 set(gca,'YLim',[0 41])