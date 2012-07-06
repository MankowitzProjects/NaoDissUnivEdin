%Find the maximum time for calculating the score
function [maxTime1,maxTime2, maxTime3, maxTime4 ]  = calcMaxTime(dataset1, dataset2, dataset3, dataset4)

maxTime1 = max(dataset1(:,18));
maxTime2 = max(dataset2(:,18));
maxTime3 = max(dataset3(:,18));
maxTime4 = max(dataset4(:,18));