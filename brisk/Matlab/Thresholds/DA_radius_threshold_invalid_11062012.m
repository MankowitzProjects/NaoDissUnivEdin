%To determine the radius and threshold to utilise
%**************************************************
clear all
clc
load 'DA_Radius_Threshold_11062012_4_directories.mat'
%load 'DA_Radius_Threshold_14062012_BRISK_BRISK.mat'
%Initial values:
%Initialise the weights used for scoring
global k1;
global k2;
global k3;
global k4;
k1 = 0.7;
k2 = 0.3; 
k3 = 0.5;
k4 = 0.5;

%If data in the old format is being used
oldData = 1;

%For BRISK SURF
distance = 0.05;
%For BRISK BRISK
%distance = 10;
threshold = 20;

%Separate the datasets
[row1,col1] = find(data(:,1)==1);
[row2,col2] = find(data(:,1)==2);
[row3,col3] = find(data(:,1)==3);
[row4,col4] = find(data(:,1)==4);
%  [row5,col5] = find(data(:,1)==5);
%  [row6,col6] = find(data(:,1)==6);
%  [row7,col7] = find(data(:,1)==7);
%  [row8,col8] = find(data(:,1)==8);

%We have filtered the datasets by directory
dataset1 = data(row1,:);
dataset2 = data(row2,:);
dataset3 = data(row3,:);
dataset4 = data(row4,:);
%  dataset5 = data(row5,:);
%  dataset6 = data(row6,:);

%  dataset7 = data(row7,:);
%  dataset8 = data(row8,:);

%Append 0's to row 17 to make up for calculations
if oldData
dataset1(:,17) = dataset1(:,16);
dataset1(:,16) = 0;
dataset2(:,17) = dataset2(:,16);
dataset2(:,16) = 0;
dataset3(:,17) = dataset3(:,16);
dataset3(:,16) = 0;
dataset4(:,17) = dataset4(:,16);
dataset4(:,16) = 0;
end


%Find the maximum time for calculating the score
[maxTime1,maxTime2, maxTime3, maxTime4]  = calcMaxTime(dataset1, dataset2, dataset3, dataset4);

%Find the maximum number of valid matches
[maxNVM1, maxNVM2, maxNVM3, maxNVM4] = calcMaxValidMatchesKNN(dataset1, dataset2, dataset3, dataset4);


%Calculate the score for each dataset for processing
dataset1Processed =  calculateScore(dataset1, maxTime1, maxNVM1);
dataset2Processed =  calculateScore(dataset2, maxTime2, maxNVM2);
dataset3Processed =  calculateScore(dataset3, maxTime3, maxNVM3);
dataset4Processed =  calculateScore(dataset4, maxTime4, maxNVM4);

%Find the maximum Number of zero valid matches
[maxNZM1, maxNZM2, maxNZM3, maxNZM4] = calcMaxZeroMatchesKNN(dataset1, dataset2, dataset3, dataset4);

%Generate the statistics for the current dataset
[msm, mtmm, mbmm, motm, mScore] = createDataMatrices(dataset1Processed, maxNZM1, 19,0 , 1,1);
[msm2, mtmm2, mbmm2, motm2, mScore2] = createDataMatrices(dataset2Processed,maxNZM2, 19,0 , 1,1);
[msm3, mtmm3, mbmm3, motm3, mScore3] = createDataMatrices(dataset3Processed, maxNZM3,19,0 , 1,1);
[msm4, mtmm4, mbmm4, motm4, mScore4] = createDataMatrices(dataset4Processed, maxNZM4,19,0 , 1,1);


%Calculate the final scores for each dataset
[finalDistance(1,1), finalThreshold(1,2)] = getRadiusThreshold(mScore,distance, threshold);
[finalDistance(2,1), finalThreshold(2,2)] = getRadiusThreshold(mScore2,distance, threshold);
[finalDistance(3,1), finalThreshold(3,2)] = getRadiusThreshold(mScore3,distance, threshold);
[finalDistance(4,1), finalThreshold(4,2)] = getRadiusThreshold(mScore4,distance, threshold);

%The radius and threshold to use are:
distance = mean(finalDistance)
threshold = mean(finalThreshold)

%Generate the mean score matrix
mScoreSum = (mScore + mScore2 + mScore3 + mScore4)./4;

figure(5)
%The mean overall time matrix
surf(1:19,1:19,mScoreSum);
title('Mean Overall score');
xlabel('Threshold');
ylabel('Distance');




%qIndex  = strmatch([counter+1 nextCandidateRank], Qsa(:,1:2));

%writeFile <<tempDir<<", "<<radius<<", "<<testThreshold<<", "
%<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()
%<<", "<<imageMatchingScoreBest<<", "<<imageMatchingScore<<","
%<<totalNumMatches<<", "<<",",<<totalNumInvalidMatches<<totalNumBestMatches<<", "<<detectionTime<<
%", "<<extractionTime<<", "<<matchingTime<<", "<<overallTime<<"\n";

%Generate the statistics
if 0
    [msm1, mtmm1, mbmm1, motm1] = createDataMatrices(dataset1, 19, 0, 0);
    [msm2, mtmm2, mbmm2, motm2] = createDataMatrices(dataset2, 19, 0, 0);
    [msm3, mtmm3, mbmm3, motm3] = createDataMatrices(dataset3, 19, 0, 0);
    [msm4, mtmm4, mbmm4, motm4] = createDataMatrices(dataset4, 19, 0, 0);
    [msm5, mtmm5, mbmm5, motm5] = createDataMatrices(dataset5, 19, 0, 0);
    [msm6, mtmm6, mbmm6, motm6] = createDataMatrices(dataset6, 19, 0, 0);
    
    msmMean = msm1+ msm2+ msm3+ msm4;
    mtmmMean = mtmm1+ mtmm2+ mtmm3+ mtmm4;
    mbmmMean = mbmm1 +mbmm2+ mbmm3+ mbmm4;
    motmMean = motm1+motm2+ motm3+motm4;
    
    msmMean = msmMean./4;
    mtmmMean = mtmmMean./4;
    mbmmMean = mbmmMean./4;
    motmMean = motmMean./4;
    
    %Plot the various graphs
    subplot(2,2,1);
    %The matching score plot
    surf(1:19,1:19,msmMean);
    title('Matching Score');
    xlabel('Threshold');
    ylabel('Radius');
    
    subplot(2,2,2);
    %The mean number of total matches matrix
    surf(1:19,1:19,mtmmMean);
    title('Mean Number of total matches');
    xlabel('Threshold');
    ylabel('Radius');
    
    subplot(2,2,3)
    %The mean number of best matches matrix
    surf(1:19,1:19,mbmmMean);
    title('Mean Number of best matches');
    xlabel('Threshold');
    ylabel('Radius');
    
    subplot(2,2,4)
    %The mean overall time matrix
    surf(1:19,1:19,motmMean);
    title('Mean Overall time (ms)');
    xlabel('Threshold');
    ylabel('Radius');
end





