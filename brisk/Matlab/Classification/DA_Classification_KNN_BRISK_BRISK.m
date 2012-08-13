%Determining whether or not matching scores are useful for classification
clear all
clc

load 'runswiftROC.mat'

addpath('../Varying_Lighting_Datasets')
addpath('../Main_Robocup_Testing_Datasets');
addpath('../Large_Hall_Datasets');
addpath('../Office_Environment_Datasets');
addpath('../Nikon_Camera_Datasets');
addpath('../Google_Street_View_Datasets');

%SBRISK UBRISK
%load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
%load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'

%For SBRISK
%load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070321012_2310.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_0220_30.mat'%mScore
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_4625_mScoreFixed.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_30_mScoreFixed_consistentThreshold.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'

%For BRISK4
%load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070421012_1341_51.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2125_5125_mScoreFixed.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2215_30_mScoreFixed_consistentThreshold.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'


%For SBRISK SURF2D
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_0220_45.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_45_mScoreFixed.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_30_mScoreFixed_consistentThreshold.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'


%1D SURF
%load 'nonmatching_matching_SURF1D_18072012_1151.mat'
load 'nonmatching_matching_SURF1D_19072012_1151.mat'

%----------------------------------------------------------------
%Datasets 5-6
%UBRISK
%load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1140_52.5__consistent.mat'
%load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_60__max.mat'

%BRISK0
%load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1140_57.5__consistent.mat'
%load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_45__max.mat'

%BRISK4
%load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1140_50__consistent.mat'
%load 'dataset2_dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_47.5__max.mat'


%BRISK0 SURF2D
%load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1140_47.5__consistent.mat'
%load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_45__max.mat'

%1D SURF
%load 'dataset2_nonmatching_matching_SURF1D_18072012_1151.mat'
%load 'dataset2_nonmatching_matching_SURF1D_19072012_1151.mat'

global stats;

%The method being used
usingBrisk = 0;

%Choose the dataset to use
datasetA = 0;

%Separate the datasets

%4 overlapping datasets
[row1,col1] = find(data(:,1)==1 & data(:,2)==1);
[row2,col2] = find(data(:,1)==2 & data(:,2)==2);
[row3,col3] = find(data(:,1)==3 & data(:,2)==3);
[row4,col4] = find(data(:,1)==4 & data(:,2)==4);

%4 non-overlapping data sets
[row13nm,col13nm] = find(data(:,1)==1 & data(:,2)==3);
[row14nm,col14nm] = find(data(:,1)==1 & data(:,2)==4);
[row23nm,col23nm] = find(data(:,1)==2 & data(:,2)==3);
[row24nm,col24nm] = find(data(:,1)==2 & data(:,2)==4);


%2 additional overlapping datasets
[row5,col5] = find(data(:,1)==5 & data(:,2)==5);
[row6,col6] = find(data(:,1)==6 & data(:,2)==6);

%1 additional overlapping dataset
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
    meanMatchesScore = mean(stats(1:4,4))
    meanNonMatchesScore = mean(stats(5:8,4))
    
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
largestThreshold = ceil(max(stats(:,4)));%4

if usingBrisk
    stepValue = 0.01;
else
    stepValue = 1;
end
tpRateMatrix = zeros(1,largestThreshold/stepValue);
fpRateMatrix = zeros(1,largestThreshold/stepValue);


%Sort all of the datasets in descending order

sortedData  =sortrows(data,-8);

thresholdArray = [];

for jj=1:size(sortedData,1)
   
    if mod(jj,20)==1
     thresholdArray = [thresholdArray sortedData(jj,8)];   
    end
    
end

thresholdArray'

for ii=largestThreshold:-stepValue:0 
    
    classificationBoundary = meanMatchesScore - ((meanMatchesScore - meanNonMatchesScore)/1.02);
    %classificationBoundary =meanNonMatchesScore*2;
    
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

%Plot with their classification algorithm

% thresholdCompare = [thresholdArray', curve(:,3)]

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
%This must be a weighted average as some images are computed more
%frequently than others

[detectionTimeMatrix, extractionTimeMatrix] = calculateMeanTimes(data);

averageDetectionTime = sum(detectionTimeMatrix)
averageExtractionTime = sum(extractionTimeMatrix)
averageMatchingTime = mean(data(:,16))./1000
averageVerificationTime = mean(data(:,17))./1000
averageOverallTime = mean(data(:,18))

%Compute the mean number of FP matches
fpRate = mean(fpRateMatrix)*100
%Compute the mean number of TP matches
tpRate = mean(tpRateMatrix)*100

statsMatrix = [AUC averageDetectionTime averageExtractionTime averageMatchingTime averageVerificationTime averageOverallTime]

statsMatrix = Roundoff(statsMatrix,3);
%writeFile <<tempDir<<", "<<tempDir1<<", "
%<<name1<<", "<<name2<<", "<<keypoints.size()<<
%", "<<keypoints2.size()<<", "<<imageMatchingScoreBest
%<<", "<<imageMatchingScore<<","<<totalNumMatches<<
%", "<<totalNumValidMatches<<", "<<totalNumBestMatches
%<<", "<<detectionTime<<", "<<extractionTime<<", "
%<<matchingTime<<", "<<overallTime<<"\n";


