%To determine the radius and threshold to utilise
%**************************************************
clear all
clc
%load 'DA_Radius_Threshold_11062012_4_directories.mat'

%Initialises which method is being used. I.e. BRISK or SURF-based
usingBrisk = 1;

%U-BRISK
load 'threshold_SBRISK_UBRISK_Hamming_threshold_12072012_0055.mat'

%SBRISK
%load '/thresholds/threshold_BRISK_BRISK_Hamming_threshold_04072012_1041.mat'
%load '/thresholds/threshold_SBRISK_SBRISK_Hamming_threshold_04072012_1814.mat'%Fixed
%load 'threshold_SBRISK_SBRISK_Hamming_threshold_05072012_1825_AllImages.mat'
%load 'threshold_SBRISK_SBRISK_Hamming_threshold_12072012_0055.mat'

%BRISK4
%load '/thresholds/threshold_BRISK4_BRISK4_Hamming_threshold_04072012_1352'
%load '/thresholds/threshold_BRISK4_BRISK4_Hamming_threshold_04072012_1840'
%Fixed
%load 'threshold_BRISK4_BRISK4_Hamming_threshold_05072012_2250_AllImages.mat'
%load 'threshold_BRISK4_BRISK4_Hamming_threshold_11072012_1013.mat'

%SBRISK SURF2D
%load 'threshold_SBRISK_SURF2D_Hamming_threshold_04072012_1939.mat'
%load 'threshold_SBRISK_SURF2D_Hamming_threshold_05072012_2304_AllImages.mat'
%load 'threshold_SBRISK_SURF2D_Hamming_threshold_11072012_0149.mat'

%Initialise the weights used for scoring
global k1;
global k2;
global k3;
global k4;
k1 = 0.6;
k2 = 0.4;
k3 = 0.4;
k4 = 0.6;

%Create the stats table
global statsTable;
statsTable = [];

%Initial values:
%radius = 0.05;
%For BRISK BRISK

if usingBrisk
    distance = 40;
else
    distance = 0.1;
end
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
[msm, mtmm, mbmm, motm, mScore] = createDataMatrices(dataset1Processed, distance, maxNZM1, 19,0 , 0, usingBrisk);
[msm2, mtmm2, mbmm2, motm2, mScore2] = createDataMatrices(dataset2Processed, distance, maxNZM2, 19,0 , 0, usingBrisk);
[msm3, mtmm3, mbmm3, motm3, mScore3] = createDataMatrices(dataset3Processed, distance, maxNZM3, 19,0 , 0, usingBrisk);
[msm4, mtmm4, mbmm4, motm4, mScore4] = createDataMatrices(dataset4Processed, distance, maxNZM4, 19,0 , 0, usingBrisk);


%Calculate the final scores for each dataset
[Parameters(1,1),Parameters(1,2), row, col ] = getThreshold(mScore, threshold, distance, usingBrisk);
[Parameters(2,1),Parameters(2,2), row, col ] = getThreshold(mScore2,threshold, distance, usingBrisk);
[Parameters(3,1),Parameters(3,2), row, col ] = getThreshold(mScore3,threshold, distance, usingBrisk);
[Parameters(4,1),Parameters(4,2), row, col ] = getThreshold(mScore4, threshold, distance, usingBrisk);

%The radius and threshold to use are:
finalDistance = mean(Parameters(:,1))
finalThreshold = mean(Parameters(:,2))

%Generate the mean score matrix
mScoreSum = (mScore + mScore2 + mScore3 + mScore4)./4;
[consistentDistance,consistentThreshold, row, col ] = getThreshold(mScoreSum, threshold, distance, usingBrisk);
mScoreSum(row, col)

figure(5)
%The mean overall time matrix
surf(1:19,1:19,mScoreSum);
title('Mean Overall score');
xlabel('Threshold'); %Column is threshold (x axis)
ylabel('Distance'); %Row is distance (y axis)
hold on
line([col, col ], [row, row], [0, mScoreSum(row, col)],'Color','r','LineWidth',2);
line([col, col ], [row, 0], [0, 0],'Color','r','LineWidth',2);
line([col, 0 ], [row, row], [0, 0],'Color','r','LineWidth',2);
text(col, row, 0.1, strcat({mScoreSum(row, col)}));
text(col, row/2, 0.1, strcat({consistentThreshold }));
text(col/2, row, 0.1, strcat({consistentDistance}));




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





