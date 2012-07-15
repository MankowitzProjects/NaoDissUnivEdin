function [matchingStats] = calculateMatchingStats(data, row1,row2,row3,row4)

%Matching stats matrix
matchingStats = [];
%Calculate the matching stats for each directory
numMatched = sum(data(row1,19));
totalNumPoints = size(row1,1);

if nargin ==5
    %Sum all the matches that were classified as 1
    %divided by the total number of matches
    matchingStats(1,1) = sum(data(row1,19))/size(row1,1);
    matchingStats(2,1) = sum(data(row2,19))/size(row2,1);
    matchingStats(3,1) = sum(data(row3,19))/size(row3,1);
    matchingStats(4,1) = sum(data(row4,19))/size(row4,1);
elseif nargin==3
    matchingStats(1,1) = sum(data(row1,19))/size(row1,1);
    matchingStats(2,1) = sum(data(row2,19))/size(row2,1);
    
elseif nargin ==2
    matchingStats(1,1) = sum(data(row1,19))/size(row1,1);
end