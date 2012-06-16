function [matchingStats] = calculateMatchingStats(data, row1,row2,row3,row4)

%Matching stats matrix
matchingStats = [];
%Calculate the matching stats for each directory
numMatched = sum(data(row1,16));
totalNumPoints = size(row1,1);
matchingStats(1,1) = sum(data(row1,16))/size(row1,1);
matchingStats(2,1) = sum(data(row2,16))/size(row2,1);
matchingStats(3,1) = sum(data(row3,16))/size(row3,1);
matchingStats(4,1) = sum(data(row4,16))/size(row4,1);
