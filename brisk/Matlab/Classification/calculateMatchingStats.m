function [matchingStats] = calculateMatchingStats(data, row1,row2,row3,row4, row5, row6, row7, row8)

%Matching stats matrix
matchingStats = [];
%Calculate the matching stats for each directory
numMatched = sum(data(row1,19));
totalNumPoints = size(row1,1);

if nargin==9
   %Sum all the matches that were classified as 1
    %divided by the total number of matches
    matchingStats(1,1) = sum(data(row1,19))/size(row1,1);
    matchingStats(2,1) = sum(data(row2,19))/size(row2,1);
    matchingStats(3,1) = sum(data(row3,19))/size(row3,1);
    matchingStats(4,1) = sum(data(row4,19))/size(row4,1);
    matchingStats(5,1) = sum(data(row5,19))/size(row5,1);
    matchingStats(6,1) = sum(data(row6,19))/size(row6,1); 
    matchingStats(7,1) = sum(data(row7,19))/size(row7,1);
    matchingStats(8,1) = sum(data(row8,19))/size(row8,1); 

elseif nargin ==5 %For datasetType==1
    %Sum all the matches that were classified as 1
    %divided by the total number of matches
    matchingStats(1,1) = sum(data(row1,19))/size(row1,1);
    matchingStats(2,1) = sum(data(row2,19))/size(row2,1);
    matchingStats(3,1) = sum(data(row3,19))/size(row3,1);
    matchingStats(4,1) = sum(data(row4,19))/size(row4,1);
elseif nargin==3 %For datasetType ==2 (Overlapping matches)
    matchingStats(1,1) = sum(data(row1,19))/size(row1,1);
    matchingStats(2,1) = sum(data(row2,19))/size(row2,1);
    
elseif nargin ==2 %For datasetType ==2 (Non-Overlapping matches)
    matchingStats(1,1) = sum(data(row1,19))/size(row1,1);
end