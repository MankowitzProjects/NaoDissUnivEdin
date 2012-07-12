%Determining whether or not matching scores are useful for classification
clear all
clc
%Are we using BRISK or SURF based methods
usingBrisk = 1;

%For BRISK0 UBRISK
load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'

%For SBRISK
%load 'nonmatching_matching_Data__BRISK__BRISK_Hamming_070421012_1222_98_78.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070421012_1922.mat'
%%Latest
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070521012_2318_78_110_mScoreFixed.mat'
%load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'

%For BRISK4
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070421012_1341.mat'
%load 'g_matching_Data__BRISK4__BRISK4_Hamming_070421012_1922_88.mat'%Latest
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070521012_2318_85_12125_mScoreFixed.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'

%SBRISK SURF2D
%load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_0250_33.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_2318_65_028_mScoreFixed.mat'
%load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'
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


% [row5,col5] = find(data(:,1)==5 & data(:,2)==5);
% [row5nm,col5nm] = find(data(:,1)==5 & data(:,2)==1);
% 
% [row6,col6] = find(data(:,1)==6 & data(:,2)==6);
% [row6nm,col6nm] = find(data(:,1)==6 & data(:,2)==1);
% 
% [row7,col7] = find(data(:,1)==7 & data(:,2)==7);
% [row7nm,col7nm] = find(data(:,1)==7 & data(:,2)==1);
% 
% [row8,col8] = find(data(:,1)==8 & data(:,2)==8);
% [row8nm,col8nm] = find(data(:,1)==8 & data(:,2)==1);

%rows = [row1, row2, row3,  row4, row13nm, row23nm, row14nm, row24nm];

%We have filtered the datasets by directory
dataset1 = data(row1,:);
dataset2 = data(row2,:);
dataset3 = data(row3,:);
dataset4 = data(row4,:);


dataset13nm = data(row13nm,:);
dataset23nm = data(row23nm,:);
dataset14nm = data(row14nm,:);
dataset24nm = data(row24nm,:);

% dataset5 = data(row5,:);
% dataset5nm = data(row5nm,:);
% dataset6 = data(row6,:);
% dataset6nm = data(row6nm,:);
% dataset7 = data(row7,:);
% dataset7nm = data(row7nm,:);
% dataset8 = data(row8,:);
% dataset8nm = data(row8nm,:);

%testing

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

%[rowt, colt] = find(dataset14nm(:,7)>0);
%dataset14nm(rowt,7)
%[a,b] = find(dataset14nm(:,7)==155.8340)

% generateClassificationStats(dataset5, dataset5nm,5);
% generateClassificationStats(dataset6, dataset6nm,6);
% generateClassificationStats(dataset7, dataset7nm,7);
% generateClassificationStats(dataset8, dataset8nm,8);

%Find the mean of each directory for a specific scene/camera
%Directory 1 to 4 is for the new Nao camera
%Directory 5 to 8 is for the old Nao camera
meanMatchesScore = mean(stats(1:4,1))
meanNonMatchesScore = mean(stats(5:8,1))


%Counter
counter = 1;
%To generate values for the ROC Curve
largestThreshold = ceil(max(stats(:,1)));

if usingBrisk
stepValue = 0.01;
else
   stepValue = 1;
end

tpRateMatrix = zeros(1,largestThreshold/stepValue);
fpRateMatrix = zeros(1,largestThreshold/stepValue);

%for ii=1000:-1:0 %BRISK SURF
for ii=largestThreshold:-stepValue:0 %BRISK BRISK
%for ii=1:-0.001:0

%classificationBoundary = meanMatchesScore - ((meanMatchesScore - meanNonMatchesScore)/1.02);
%classificationBoundary =meanNonMatchesScore*2;

%Now to perform classification. We use the matching score
%defined as the inverse hamming distance between 2 vectors
%for BRISK or the inverse euclidean distance for SURF
data(:,19) = data(:,8)>=ii; 

% dataset1nm(:,16) = dataset1nm(:,7)>0; 
% dataset2(:,16) = dataset2(:,7)>0; 
% dataset2nm(:,16) = dataset2nm(:,7)>0; 
% dataset3(:,16) = dataset3(:,7)>0; 
% dataset3nm(:,16) = dataset3nm(:,7)>0; 
% dataset4(:,16) = dataset4(:,7)>0; 
% dataset4nm(:,16) = dataset4nm(:,7)>0;

%Find the number of matches for matching pairs
[matchingStats] = calculateMatchingStats(data, row1,row2,row3,row4);
[matchingStats1] = calculateMatchingStats(data, row13nm, row23nm, row14nm, row24nm);

%The overall mean matching stat is:
meanMatchesStat = mean(matchingStats);
meanNonMatchesStat = mean(matchingStats1);

%True positive rate
TPRate = meanMatchesStat;
%False Positive rate
FPRate = meanNonMatchesStat;

%Store the values to plot on the ROC Curve
tpRateMatrix(1,counter) = TPRate;
fpRateMatrix(1,counter) = FPRate;
counter  = counter+1;
end

%Plot the ROC Curve
plot(fpRateMatrix,tpRateMatrix)
hold on
%Plot the random curve
xrand = [0:stepValue:1];
yrand = [0:stepValue:1];
plot(xrand, yrand, 'r--');
xlabel('False Positive rate');
ylabel('True positive rate');
title('ROC Curve');

AUC = trapz(fpRateMatrix, tpRateMatrix)*100

%The average time for computing each match
averageDetectionTime = mean(data(:,14))
averageExtractionTime = mean(data(:,15))
averageMatchingTime = mean(data(:,16))./1000
averageVerificationTime = mean(data(:,17))./1000
averageOverallTime = mean(data(:,18))

%Compute the mean number of FP matches
fpRate = mean(fpRateMatrix)*100



%writeFile <<tempDir<<", "<<tempDir1<<", "
%<<name1<<", "<<name2<<", "<<keypoints.size()<<
%", "<<keypoints2.size()<<", "<<imageMatchingScoreBest
%<<", "<<imageMatchingScore<<","<<totalNumMatches<<
%", "<<totalNumValidMatches<<", "<<totalNumBestMatches
%<<", "<<detectionTime<<", "<<extractionTime<<", "
%<<matchingTime<<", "<<overallTime<<"\n";


