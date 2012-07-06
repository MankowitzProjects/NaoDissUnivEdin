function [finalDistance, finalThreshold, rowMax, colMax] = getThreshold(mScore, threshold, distance)

%Determine if there are Nan's already in the matrix
nanMat = isnan(mScore);
[row,col] = find(nanMat==1);
mScore(row,col) =0;
infMat = isinf(mScore);
[rowinf,colinf] = find(infMat==1);
mScore(rowinf,colinf)=0;

% Get the non-zero mins
mScore(~mScore) = nan;
maxScore = max(max(mScore));

%Find the minimum mScore value
[rowMax,colMax] = find(mScore ==maxScore);

%Output the min:
finalScore = mScore(rowMax, colMax);
%The radius and threshold to use are:
%finalDistance = (distance-5) + 5*rowMax; %BRISK BRISK
finalDistance = (distance-0.01) + 0.01*rowMax; %BRISK SURF
finalThreshold = (threshold - 5) + 5*colMax;