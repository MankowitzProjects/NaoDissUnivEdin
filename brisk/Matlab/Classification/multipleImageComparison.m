%Overall Image statistics
%SBRISK SURF2D
%load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_0250_33.mat'

%SBRISK
load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_0220_30.mat'%mScore
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






