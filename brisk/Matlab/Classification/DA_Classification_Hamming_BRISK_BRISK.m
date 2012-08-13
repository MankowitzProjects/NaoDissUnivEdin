%Determining whether or not matching scores are useful for classification
clear all
clc

addpath('../Varying_Lighting_Datasets')
addpath('../Main_Robocup_Testing_Datasets');
addpath('../Large_Hall_Datasets');
addpath('../Office_Environment_Datasets');
addpath('../Nikon_Camera_Datasets');
addpath('../Google_Street_View_Datasets');

%For BRISK0 UBRISK
%load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'
%load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_14072012_1140_75_130_consistent.mat'


%For SBRISK
%load 'nonmatching_matching_Data__BRISK__BRISK_Hamming_070421012_1222_98_78.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070421012_1922.mat'
%%Latest
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070521012_2318_78_110_mScoreFixed.mat'

%load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'
%load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_14072012_1104_75_115_consistent.mat'


%For BRISK4
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070421012_1341.mat'
%load 'g_matching_Data__BRISK4__BRISK4_Hamming_070421012_1922_88.mat'%Latest
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070521012_2318_85_12125_mScoreFixed.mat'

%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'
%load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_14072012_1140_65_130_consistent.mat'

%SBRISK SURF2D
%load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_0250_33.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_2318_65_028_mScoreFixed.mat'
%load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'
%load 'nonmatching_matching_Data__BRISK_SURF_Hamming_14072012_1140_60_0_consistent.mat'

%----------VARYING LIGHTING CONDITIONS--------------
%All these methods use BRISK0 - UBRISK
%1. Original Dataset Left light off
%load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%2. Original Dataset Right light off
%load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%3. Original dataset both lights off 
load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%4. Dataset3 (The Forum)
%load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_60_130_consistent.mat'

%------------------------------------------


%Are we using BRISK or SURF based methods
usingBrisk = 1;

%The dataset being used
datasetA = 1;

global stats;

%Separate the datasets

[row1,col1] = find(data(:,1)==1 & data(:,2)==1);
[row2,col2] = find(data(:,1)==2 & data(:,2)==2);
[row3,col3] = find(data(:,1)==3 & data(:,2)==3);
[row4,col4] = find(data(:,1)==4 & data(:,2)==4);

%From the alternative dataset
[row5,col5] = find(data(:,1)==5 & data(:,2)==5);
[row6,col6] = find(data(:,1)==6 & data(:,2)==6);

[row13nm,col13nm] = find(data(:,1)==1 & data(:,2)==3);
[row14nm,col14nm] = find(data(:,1)==1 & data(:,2)==4);
[row23nm,col23nm] = find(data(:,1)==2 & data(:,2)==3);
[row24nm,col24nm] = find(data(:,1)==2 & data(:,2)==4);

%From the alternative dataset
[row56nm,col56nm] = find(data(:,1)==5 & data(:,2)==6);


%We have filtered the datasets by directory
dataset1 = data(row1,:);
dataset2 = data(row2,:);
dataset3 = data(row3,:);
dataset4 = data(row4,:);

%Dataset 2
dataset5 = data(row5,:);
dataset6 = data(row6,:);

dataset13nm = data(row13nm,:);
dataset23nm = data(row23nm,:);
dataset14nm = data(row14nm,:);
dataset24nm = data(row24nm,:);

%Dataset 2
dataset56nm = data(row56nm,:);

%testing
if datasetA
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
    %Directory 1 to 4 is for the new Nao camera
    %Directory 5 to 8 is for the old Nao camera
    meanMatchesScore = mean(stats(1:4,1))
    meanNonMatchesScore = mean(stats(5:8,1))
else
    %Overlapping dataset
    generateClassificationStats(dataset5,1);
    generateClassificationStats(dataset6,2);
    %Non-overlapping dataset
    generateClassificationStats(dataset56nm,3);
    
    meanMatchesScore = mean(stats(1:2,1))
    meanNonMatchesScore = mean(stats(3,1))
end


%Counter
counter = 1;
%To generate values for the ROC Curve
largestThreshold = ceil(max(stats(:,4)));

if usingBrisk
    stepValue = 0.01;
else
    stepValue = 1;
end

tpRateMatrix = zeros(1,largestThreshold/stepValue);
fpRateMatrix = zeros(1,largestThreshold/stepValue);

for ii=largestThreshold:-stepValue:0 
    
    %Now to perform classification. We use the matching score
    %defined as the inverse hamming distance between 2 vectors
    %for BRISK or the inverse euclidean distance for SURF
    data(:,19) = data(:,8)>=ii;
    
    %Find the number of matches for matching pairs
    if datasetA
        [matchingStats] = calculateMatchingStats(data, row1,row2,row3,row4);
        [matchingStats1] = calculateMatchingStats(data, row13nm, row23nm, row14nm, row24nm);
    else
        [matchingStats] = calculateMatchingStats(data, row5,row6);
        [matchingStats1] = calculateMatchingStats(data, row56nm);
    end
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
