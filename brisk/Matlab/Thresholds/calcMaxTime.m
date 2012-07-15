%Find the maximum time for calculating the score
function [maxTime1,maxTime2, maxTime3, maxTime4, maxTime5, maxTime6 ]  = calcMaxTime(dataset1, dataset2, dataset3, dataset4, dataset5, dataset6)

maxTime1 = max(dataset1(:,18));
maxTime2 = max(dataset2(:,18));
maxTime3 = max(dataset3(:,18));
maxTime4 = max(dataset4(:,18));
maxTime5 = max(dataset5(:,18));
maxTime6 = max(dataset6(:,18));