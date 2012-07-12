
%Calculate the matching statistics
function [overlappingMatchesMatrix, nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data)

%Find the overlapping image pairs
%Separate the datasets

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
%First for overlapping images
[total_keypoints_11, numImages11] = calculateMeanKeypoints(data(row1,:));                       
[total_keypoints_22, numImages22] = calculateMeanKeypoints(data(row2,:));                       
[total_keypoints_33, numImages33] = calculateMeanKeypoints(data(row3,:));                       
[total_keypoints_44, numImages44] = calculateMeanKeypoints(data(row4,:));                       

sumOverlappingKeypoints = total_keypoints_11 + total_keypoints_22 +...
                            total_keypoints_33 + total_keypoints_44;
                        
numOverlappingImages = numImages11 + numImages22 + numImages33...
                        + numImages44;

meanKeypointsOverlap = sumOverlappingKeypoints/numOverlappingImages;

%Now for non-overlapping images
[total_keypoints_13, numImages13] = calculateMeanKeypoints(data(row1,:));                       
[total_keypoints_14, numImages14] = calculateMeanKeypoints(data(row2,:));                       
[total_keypoints_23, numImages23] = calculateMeanKeypoints(data(row3,:));                       
[total_keypoints_24, numImages24] = calculateMeanKeypoints(data(row4,:));                       

sumNonOverlappingKeypoints = total_keypoints_13 + total_keypoints_14 +...
                            total_keypoints_23 + total_keypoints_24;
                        
numNonOverlappingImages = numImages13 + numImages14 + numImages23...
                        + numImages24;

meanKeypointsNonOverlap = sumNonOverlappingKeypoints/numNonOverlappingImages;


%Add the mean keypoints to the statistics
overlappingMatchesMatrix = [overlappingMatchesMatrix meanKeypointsOverlap];
nonOverlappingMatchesMatrix =[nonOverlappingMatchesMatrix meanKeypointsNonOverlap];
