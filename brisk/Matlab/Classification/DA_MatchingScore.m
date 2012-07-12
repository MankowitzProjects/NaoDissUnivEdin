%Overall Image statistics
knn = 0;

if knn
%SBRISK UBRISK
%load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
%load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'

%For SBRISK
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'

%For BRISK4
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'


%For SBRISK SURF2D
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'
else
    
%For BRISK0 UBRISK
% load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'

%For SBRISK
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070521012_2318_78_110_mScoreFixed.mat'
%load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'

%For BRISK4
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070521012_2318_85_12125_mScoreFixed.mat'
% load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'

%SBRISK SURF2D
%load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'



end



global stats;

%Separate the datasets

[row1,col1] = find(data(:,1)==1 & data(:,2)==1);
[row2,col2] = find(data(:,1)==2 & data(:,2)==2);
[row3,col3] = find(data(:,1)==3 & data(:,2)==3);
[row4,col4] = find(data(:,1)==4 & data(:,2)==4);


[row13nm,col13nm] = find(data(:,1)==1 & data(:,2)==3);
[row14nm,col14nm] = find(data(:,1)==1 & data(:,2)==4);
[row23nm,col23nm] = find(data(:,1)==2 & data(:,2)==3);
[row24nm,col24nm] = find(data(:,1)==2 & data(:,2)==4);

%We have filtered the datasets by directory
dataset1 = data(row1,:);
dataset2 = data(row2,:);
dataset3 = data(row3,:);
dataset4 = data(row4,:);

dataset13nm = data(row13nm,:);
dataset23nm = data(row23nm,:);
dataset14nm = data(row14nm,:);
dataset24nm = data(row24nm,:);

%Generate the statistics for each matching dataset
generateClassificationStats(dataset1,1);
generateClassificationStats(dataset2,2);
generateClassificationStats(dataset3,3);
generateClassificationStats(dataset4,4);

%Generate stats for each non-matching dataset
generateClassificationStats(dataset13nm,5);
generateClassificationStats(dataset23nm,6);
generateClassificationStats(dataset14nm,7);
generateClassificationStats(dataset24nm,8);



%Find the mean of each directory for a specific scene/camera
meanMatchesScore = mean(stats(1:4,1))
meanNonMatchesScore = mean(stats(5:8,1))

matchesScoreMatrix = [meanMatchesScore meanNonMatchesScore]




