%To determine the radius and threshold to utilise
%**************************************************
clear all
clc
%Datasets 1-4
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

%Datasets 5-6
%UBRISK
load 'dataset2_threshold_BRISK_U-BRISK_KNN__threshold_14072012_1018.mat'
%load 'dataset2_threshold_BRISK_U-BRISK_KNN__threshold_14072012_1534.mat'

%BRISK0
%load 'dataset2_threshold_BRISK_BRISK_KNN__threshold_14072012_0945.mat'
%load 'dataset2_threshold_BRISK_BRISK_KNN__threshold_14072012_1534.mat'

%BRISK4
%load 'dataset2_threshold_BRISK4_BRISK4_KNN__threshold_14072012_1006.mat'
%load 'dataset2_threshold_BRISK4_BRISK4_KNN__threshold_14072012_1534.mat'


%BRISK0 SURF2D
%load 'dataset2_threshold_BRISK_SURF_KNN__threshold_14072012_1018.mat'
%load 'dataset2_threshold_BRISK_SURF_KNN__threshold_14072012_1534.mat'




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

datasetA =0;

%Separate the datasets
[row1,col1] = find(data(:,1)==1);
[row2,col2] = find(data(:,1)==2);
[row3,col3] = find(data(:,1)==3);
[row4,col4] = find(data(:,1)==4);
[row5,col5] = find(data(:,1)==5);
[row6,col6] = find(data(:,1)==6);
%  [row7,col7] = find(data(:,1)==7);
%  [row8,col8] = find(data(:,1)==8);

%We have filtered the datasets by directory
dataset1 = data(row1,:);
dataset2 = data(row2,:);
dataset3 = data(row3,:);
dataset4 = data(row4,:);
dataset5 = data(row5,:);
dataset6 = data(row6,:);

%  dataset7 = data(row7,:);
%  dataset8 = data(row8,:);

%Find the maximum time for calculating the score
[maxTime1,maxTime2, maxTime3, maxTime4, maxTime5, maxTime6]  = calcMaxTime(dataset1, dataset2, dataset3, dataset4, dataset5, dataset6);

%Find the maximum number of valid matches
[maxNVM1, maxNVM2, maxNVM3, maxNVM4, maxNVM5, maxNVM6] = calcMaxValidMatchesKNN(dataset1, dataset2, dataset3, dataset4, dataset5, dataset6);


%Calculate the score for each dataset for processing
dataset1Processed =  calculateScoreKNN(dataset1, maxTime1, maxNVM1);
dataset2Processed =  calculateScoreKNN(dataset2, maxTime2, maxNVM2);
dataset3Processed =  calculateScoreKNN(dataset3, maxTime3, maxNVM3);
dataset4Processed =  calculateScoreKNN(dataset4, maxTime4, maxNVM4);

dataset5Processed =  calculateScoreKNN(dataset5, maxTime5, maxNVM5);
dataset6Processed =  calculateScoreKNN(dataset6, maxTime6, maxNVM6);

%Find the maximum Number of zero valid matches
[maxNZM1, maxNZM2, maxNZM3, maxNZM4, maxNZM5, maxNZM6] = calcMaxZeroMatchesKNN(dataset1, dataset2, dataset3, dataset4, dataset5, dataset6);

%Generate the statistics for the current dataset
[msm, mtmm, mbmm, motm, mScore] = createDataMatricesKNN(dataset1Processed,maxNZM1, 19,0 , 0);
[msm2, mtmm2, mbmm2, motm2, mScore2] = createDataMatricesKNN(dataset2Processed, maxNZM2, 19,0 , 0);
[msm3, mtmm3, mbmm3, motm3, mScore3] = createDataMatricesKNN(dataset3Processed, maxNZM3, 19,0 , 0);
[msm4, mtmm4, mbmm4, motm4, mScore4] = createDataMatricesKNN(dataset4Processed, maxNZM4, 19,0 , 0);
[msm5, mtmm5, mbmm5, motm5, mScore5] = createDataMatricesKNN(dataset5Processed, maxNZM5, 19,0 , 0);
[msm6, mtmm6, mbmm6, motm6, mScore6] = createDataMatricesKNN(dataset6Processed, maxNZM6, 19,0 , 0);


%Calculate the final scores for each dataset
if datasetA
    [finalDistance(1,1)] = getThresholdKNN(mScore, threshold);
    [finalDistance(1,2)] = getThresholdKNN(mScore2,threshold);
    [finalDistance(1,3)] = getThresholdKNN(mScore3,threshold);
    [finalDistance(1,4)] = getThresholdKNN(mScore4, threshold);
else
    [finalDistance(1,1)] = getThresholdKNN(mScore5, threshold);
    [finalDistance(1,2)] = getThresholdKNN(mScore6,threshold);
end
%The radius and threshold to use are:
%distance = mean(finalDistance)
finalThreshold = mean(finalDistance)

if datasetA
    %Generate the mean score matrix
    mScoreSum = (mScore + mScore2 + mScore3 + mScore4)./4;
else
    mScoreSum = (mScore5 + mScore6)./2;
end
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

