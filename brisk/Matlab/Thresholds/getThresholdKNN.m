function [finalThreshold] = getThresholdKNN(mScore, threshold)

%Determine if there are Nan's already in the matrix
nanMat = isnan(mScore);
[row,col] = find(nanMat==1);
mScore(row,col) =0;
infMat = isinf(mScore);
[rowinf,colinf] = find(infMat==1);
mScore(rowinf,colinf)=0;

% Get the non-zero mins
mScore(~mScore) = nan;
[maxScore, maxIndex] = max(mScore);

%Output the min:
finalScore = maxScore;
%The radius and threshold to use are:
finalThreshold = (threshold - 5) + 5*maxIndex;