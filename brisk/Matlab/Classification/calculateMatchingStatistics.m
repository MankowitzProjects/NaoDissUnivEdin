
%Calculate the matching statistics
function [overlappingMatchesMatrix, nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, dataset2, dataset3, dataset5, dataset6)

%Find the overlapping image pairs
%Separate the datasets
if nargin ==1
    [row1] = strmatch([1,1], data(:,1:2));
    [row2] = strmatch([2,2], data(:,1:2));
    [row3] = strmatch([3,3], data(:,1:2));
    [row4] = strmatch([4,4], data(:,1:2));
    
    %Create the overlapping dataset
    overlappingDataset = [data(row1,:); data(row2,:); data(row3,:); data(row4,:)];
    
    [row13nm] = strmatch([1,3], data(:,1:2));
    [row14nm] = strmatch([1,4], data(:,1:2));
    [row23nm] = strmatch([2,3], data(:,1:2));
    [row24nm] = strmatch([2,4], data(:,1:2));
    
    %Create the non-overlapping dataset
    nonOverlappingDataset = [data(row13nm,:); data(row14nm,:); data(row23nm,:); data(row24nm,:)];
    
elseif nargin==2
    
    [row5] = strmatch([5,5], data(:,1:2));
    [row6] = strmatch([6,6], data(:,1:2));
    
    
    %Create the overlapping dataset
    overlappingDataset = [data(row5,:); data(row6,:)];
    
    [row56nm] = strmatch([5,6], data(:,1:2));
    
    %Create the non-overlapping dataset
    nonOverlappingDataset = [data(row56nm,:);];
    
    
elseif nargin==3
    [row1] = strmatch([1,1], data(:,1:2));
    [row2] = strmatch([2,2], data(:,1:2));
    [row3] = strmatch([3,3], data(:,1:2));
    [row4] = strmatch([4,4], data(:,1:2));
    
    %Create the overlapping dataset
    overlappingDataset = [data(row1,:); data(row2,:); data(row3,:); data(row4,:)];
    
    [row13nm] = strmatch([1,3], data(:,1:2));
    [row14nm] = strmatch([1,4], data(:,1:2));
    [row23nm] = strmatch([2,3], data(:,1:2));
    [row24nm] = strmatch([2,4], data(:,1:2));
    [row35nm] = strmatch([3,5], data(:,1:2));
    [row36nm] = strmatch([3,6], data(:,1:2));
    [row45nm] = strmatch([4,5], data(:,1:2));
    [row46nm] = strmatch([4,6], data(:,1:2));
    
    %Create the non-overlapping dataset
    nonOverlappingDataset = [data(row13nm,:); data(row14nm,:); data(row23nm,:); data(row24nm,:); data(row35nm,:); data(row36nm,:); data(row45nm,:); data(row46nm,:)];
    
    
elseif nargin==4
    [row1] = strmatch([1,1], data(:,1:2));
    [row2] = strmatch([2,2], data(:,1:2));
    
    %Create the overlapping dataset
    overlappingDataset = [data(row1,:); data(row2,:)];
    
    [row13nm] = strmatch([1,3], data(:,1:2));
    [row24nm] = strmatch([2,4], data(:,1:2));
    
    %Create the non-overlapping dataset
    nonOverlappingDataset = [data(row13nm,:); data(row24nm,:)];
    
    
end


%Generate the matching statistics for overlapping matches
total_matches_overlap = mean(overlappingDataset(:,10));

valid_matches_overlap = mean(overlappingDataset(:,11));

invalid_matches_overlap = mean(overlappingDataset(:,12));

best_matches_overlap = mean(overlappingDataset(:,13));

%Generate the matching statistics for non-overlapping matches

total_matches = mean(nonOverlappingDataset(:,10));

valid_matches = mean(nonOverlappingDataset(:,11));

invalid_matches = mean(nonOverlappingDataset(:,12));

best_matches = mean(nonOverlappingDataset(:,13));

%Generate the stats matrix
overlappingMatchesMatrix = [total_matches_overlap, valid_matches_overlap...
    invalid_matches_overlap];
nonOverlappingMatchesMatrix =[total_matches, valid_matches...
    invalid_matches];


%Now determine the mean number of keypoints
if nargin==1
    meanKeypointsOverlap = mean(data(min(row1):max(row4),6));
    meanKeypointsNonOverlap = mean(data((row4+1):end,6));
elseif nargin ==3
    if dataset3==4
        meanKeypointsOverlap = mean(data(min(row1):max(row4),7));
        meanKeypointsNonOverlap = mean(data((row4+1):end,7));
    else
        meanKeypointsOverlap = mean(data(min(row1):max(row4),6));
        meanKeypointsNonOverlap = mean(data((row4+1):end,6));
    end
elseif nargin==2
    meanKeypointsOverlap = mean(data(min(row5):max(row6),6));
    meanKeypointsNonOverlap = mean(data((row6+1):end,6));
elseif nargin==4
    meanKeypointsOverlap = mean(data(min(row1):max(row2),6));
    meanKeypointsNonOverlap = mean(data((row2+1):end,6));
end
%First for overlapping images
% [total_keypoints_11, numImages11] = calculateMeanKeypoints(data(row1,:));
% [total_keypoints_22, numImages22] = calculateMeanKeypoints(data(row2,:));
% [total_keypoints_33, numImages33] = calculateMeanKeypoints(data(row3,:));
% [total_keypoints_44, numImages44] = calculateMeanKeypoints(data(row4,:));
%
% sumOverlappingKeypoints = total_keypoints_11 + total_keypoints_22 +...
%                             total_keypoints_33 + total_keypoints_44;
%
% numOverlappingImages = numImages11 + numImages22 + numImages33...
%                         + numImages44;

% meanKeypointsOverlap = sumOverlappingKeypoints/numOverlappingImages;

%Now for non-overlapping images
% [total_keypoints_13, numImages13] = calculateMeanKeypoints(data(row1,:));
% [total_keypoints_14, numImages14] = calculateMeanKeypoints(data(row2,:));
% [total_keypoints_23, numImages23] = calculateMeanKeypoints(data(row3,:));
% [total_keypoints_24, numImages24] = calculateMeanKeypoints(data(row4,:));
%
% sumNonOverlappingKeypoints = total_keypoints_13 + total_keypoints_14 +...
%                             total_keypoints_23 + total_keypoints_24;
%
% numNonOverlappingImages = numImages13 + numImages14 + numImages23...
%                         + numImages24;
%
% meanKeypointsNonOverlap = sumNonOverlappingKeypoints/numNonOverlappingImages;


%Add the mean keypoints to the statistics
overlappingMatchesMatrix = [overlappingMatchesMatrix meanKeypointsOverlap];
nonOverlappingMatchesMatrix =[nonOverlappingMatchesMatrix meanKeypointsNonOverlap];
