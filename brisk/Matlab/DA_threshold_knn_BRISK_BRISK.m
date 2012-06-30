%To determine the radius and threshold to utilise
%**************************************************
clear all
clc
%load 'DA_Radius_Threshold_11062012_4_directories.mat'
load 'DA_threshold_knn_BRISK_BRISK.mat'
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

%Calculate the score for each dataset for processing
dataset1Processed =  calculateScore(dataset1);
dataset2Processed =  calculateScore(dataset2);
dataset3Processed =  calculateScore(dataset3);
dataset4Processed =  calculateScore(dataset4);

%Generate the statistics for the current dataset
[msm, mtmm, mbmm, motm, mScore] = createDataMatricesKNN(dataset1Processed, 19,0 , 1);
[msm2, mtmm2, mbmm2, motm2, mScore2] = createDataMatricesKNN(dataset2Processed, 19,0 , 1);
[msm3, mtmm3, mbmm3, motm3, mScore3] = createDataMatricesKNN(dataset3Processed, 19,0 , 1);
[msm4, mtmm4, mbmm4, motm4, mScore4] = createDataMatricesKNN(dataset4Processed, 19,0 , 1);


%Calculate the final scores for each dataset
[finalDistance(1,1)] = getThresholdKNN(mScore, threshold);
[finalDistance(1,2)] = getThresholdKNN(mScore2,threshold);
[finalDistance(1,3)] = getThresholdKNN(mScore3,threshold);
[finalDistance(1,4)] = getThresholdKNN(mScore4, threshold);

%The radius and threshold to use are:
%distance = mean(finalDistance)
threshold = mean(finalDistance)

%Generate the mean score matrix
mScoreSum = (mScore + mScore2 + mScore3 + mScore4)./4;

figure(5)
%The mean overall time matrix
plot(mScoreSum);
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





