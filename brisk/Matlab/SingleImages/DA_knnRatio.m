%Knn Ratio image comparison function
clear all
clc
%Load all the comparison files into matlab
%load 'singleImages_SBRISK_SBRISK_07052012_1812_46.mat'

%SBRISK
%Load false matches
% load 'singleImageData_matches_SBRISK_SBRISK_KNN_11052012_1030_false.mat'
%Load valid matches
% load 'singleImageData_matches_SBRISK_SBRISK_KNN_11052012_1030_identical.mat'

%U-BRISK
% load 'singleImageData_matches_SBRISK_UBRISK_KNN_11052012_1030_false.mat'
% load 'singleImageData_matches_SBRISK_UBRISK_KNN_11052012_1030_identical.mat'
%Compute Matching Statistics

%BRISK4
% load 'singleImageData_matches_BRISK4_BRISK4_KNN_11052012_1030_false.mat'
% load 'singleImageData_matches_BRISK4_BRISK4_KNN_11052012_1030_identical.mat'

%BRISK SURF2D
load 'singleImageData_matches_SBRISK_SURF2D_KNN_11052012_1030_false.mat'
load 'singleImageData_matches_SBRISK_SURF2D_KNN_11052012_1030_identical.mat'

%Choose which diagram to plot
isDistance = -1;

%The matching vector
matchVector = [];

%Get the matching indices for false matches
neighbor0False  = strmatch([0], matchesFalse(:,19));
neighbor1False = strmatch([1], matchesFalse(:,19));

%Get the matching indices for true matches
neighbor0True = strmatch([0], matches(:,19));
neighbor1True = strmatch([1], matches(:,19));

%Get the number of indices
numIndicesFalse = size(neighbor0False,1);
%The true knn number of indices
numIndicesTrue = size(neighbor0True,1);
%Initialise the knn false variable
knnRatioFalse=  zeros(numIndicesFalse, 1);
%The true knn variable
knnRatioTrue=  zeros(numIndicesTrue, 1);

%The ratio of false matches
knnRatioFalse =(matchesFalse(neighbor0False,20)./matchesFalse(neighbor1False,20));


%The ratio of true matches
knnRatioTrue =(matches(neighbor0True,20)./matches(neighbor1True,20));

%Determine if there are Nan's already in the matrix
nanMat = isnan(knnRatioTrue);
[row,col] = find(nanMat==1);
knnRatioTrue(row,col) =0;


%The ratio of false matches
knnRatioFalse_mean = mean(knnRatioFalse)
%The ratio of true matches
knnRatio_mean = mean(knnRatioTrue)

%Work out the standarddeviation
knnRatioFalse_stdDev = std(knnRatioFalse);
knnRatioTrue_stdDev = std(knnRatioTrue);

%Calculate the mean distance between matches
distance_false = (matchesFalse(neighbor1False,20) - matchesFalse(neighbor0False,20));

distance_true = matches(neighbor1True,20) - matches(neighbor0True,20);

%Remove the Nan's from the matrix
nanMat = isnan(distance_false);
[row,col] = find(nanMat==1);
distance_false(row,col) =0;

nanMat = isnan(distance_true);
[row,col] = find(nanMat==1);
distance_true(row,col) =0;

%Calculate the standard deviations
distance_false_std=std(distance_false);
distance_true_std = std(distance_true);

distance_false_mean = mean(distance_false)
distance_true_mean = mean(distance_true)

if isDistance  ==1
plot([1:numIndicesTrue], distance_true, 'b-', [1:numIndicesTrue],distance_false(1:numIndicesTrue), 'r-')
hold on 
axis([0 numIndicesTrue 0 1.1*max(distance_true)])
xlabel('Match Index');
ylabel('Distance between KNN');
title('The Distance between 1st and 2nd KNN neighbors');
hleg1 = legend('KNN Distance - True Matches','KNN Distance - False Matches');
set(hleg1,'Location','NorthEast')
elseif isDistance == 0
plot([1:numIndicesTrue], knnRatioTrue, 'b-', [1:numIndicesTrue],knnRatioFalse(1:numIndicesTrue), 'r-')
hold on 
axis([0 numIndicesTrue -1 1.1])
xlabel('Match Index');
ylabel('KNN Ratio');
title('The KNN Ratio');
hleg1 = legend('KNN Ratio - True Matches','KNN Ratio - False Matches');
set(hleg1,'Location','SouthEast')
end
%Get the knn ratio for valid matches
% [knnValidMatches, knnInvalidMatches] = getKnnRatios(matches);
% 
% %Get the knn ratio for the inavlid matches
% [knnFalseMatches, knnInvalidMatches] = getKnnRatios(matchesFalse);
% 
% valid_ratio = mean(knnValidMatches)
% false_ratio = mean(knnFalseMatches)


