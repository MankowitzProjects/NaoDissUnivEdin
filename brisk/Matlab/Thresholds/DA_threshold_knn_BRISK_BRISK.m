%To determine the radius and threshold to utilise
%**************************************************
clear all
clc

%SBRISK - UBRISK
%load 'threshold_SBRISK_UBRISK_KNN_threshold_09072012_0935_NewTimes.mat'
%load 'threshold_SBRISK_UBRISK_KNN_threshold_10072012_1652.mat'


%SBRISK
%load '/thresholds/threshold_BRISK_BRISK_KNN_03072012_2202.mat'
%load 'threshold_SBRISK_SBRISK_KNN_threshold_05072012_1829_FixedMatchScore.mat'
%load 'threshold_SBRISK_SBRISK_KNN_threshold_09072012_0935_NewTimes.mat'
%load 'threshold_SBRISK_SBRISK_KNN_threshold_10072012_1636.mat'

%BRISK4
%load '/thresholds/threshold_BRISK4_BRISK4_KNN_threshold_04072012_1041.mat'
%load 'threshold_BRISK4_BRISK4_KNN_threshold_05072012_1857_FixedMatchScore.mat'
%load 'threshold_BRISK4_BRISK4_KNN_threshold_10072012_1745.mat'


%SBRISK SURF2D
%load '/thresholds/threshold_SBRISK_SURF2D_KNN_threshold_04072012_1502.mat'
%load 'threshold_SBRISK_SURF2D_KNN_threshold_05072012_1933_FixedMatchScore.mat'
%load 'threshold_SBRISK_SURF2D_KNN_threshold_10072012_1748.mat'


%1D SURF
%load '../../../openSurf/Matlab/threshold_SURF1D_thresholds_09072012_2034.mat'

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
distance = 10;
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
dataset1Processed =  calculateScoreKNN(dataset1, maxTime1, maxNVM1);
dataset2Processed =  calculateScoreKNN(dataset2, maxTime2, maxNVM2);
dataset3Processed =  calculateScoreKNN(dataset3, maxTime3, maxNVM3);
dataset4Processed =  calculateScoreKNN(dataset4, maxTime4, maxNVM4);

%Find the maximum Number of zero valid matches
[maxNZM1, maxNZM2, maxNZM3, maxNZM4] = calcMaxZeroMatchesKNN(dataset1, dataset2, dataset3, dataset4);

%Generate the statistics for the current dataset
[msm, mtmm, mbmm, motm, mScore] = createDataMatricesKNN(dataset1Processed,maxNZM1, 19,0 , 0);
[msm2, mtmm2, mbmm2, motm2, mScore2] = createDataMatricesKNN(dataset2Processed, maxNZM2, 19,0 , 0);
[msm3, mtmm3, mbmm3, motm3, mScore3] = createDataMatricesKNN(dataset3Processed, maxNZM3, 19,0 , 0);
[msm4, mtmm4, mbmm4, motm4, mScore4] = createDataMatricesKNN(dataset4Processed, maxNZM4, 19,0 , 0);


%Calculate the final scores for each dataset
[finalDistance(1,1)] = getThresholdKNN(mScore, threshold);
[finalDistance(1,2)] = getThresholdKNN(mScore2,threshold);
[finalDistance(1,3)] = getThresholdKNN(mScore3,threshold);
[finalDistance(1,4)] = getThresholdKNN(mScore4, threshold);

%The radius and threshold to use are:
%distance = mean(finalDistance)
finalThreshold = mean(finalDistance)

%Generate the mean score matrix
mScoreSum = (mScore + mScore2 + mScore3 + mScore4)./4;

%Find the most consistent threshold
[maxScore, index] = max(mScoreSum);
consistentThreshold = (threshold - 5) + 5*index

figure(5)
%The mean overall time matrix
plot(mScoreSum);
title('Mean Overall score');
xlabel('Threshold');
ylabel('Score');
%axis([20 120 0 1.1*maxScore]);
set(gca,'Xtick',1:size(mScoreSum,2),'XTickLabel',{'20', '25', '30', '35', '40', '45', '50', '55', '60', '65', '70', '75', '80', '85', '90', '95','100', '105', '110'})
hold on
line([index index], [0 mScoreSum(index)],'Color','r','LineWidth',2);
%line([0, index], [mScoreSum(index), mScoreSum(index)],'Color','r','LineWidth',2);
text(index,mScoreSum(index)/2 , strcat({mScoreSum(index),', ',consistentThreshold}));

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





