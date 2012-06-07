%Radius and thresholds taken into account
clear all
clc
load 'DA_radius_threshold_110_thresh.mat'

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


%qIndex  = strmatch([counter+1 nextCandidateRank], Qsa(:,1:2));

%writeFile <<tempDir<<", "<<radius<<", "<<testThreshold<<", "
%<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()
%<<", "<<imageMatchingScoreBest<<", "<<imageMatchingScore<<","
%<<totalNumMatches<<", "<<totalNumBestMatches<<", "<<detectionTime<<
%", "<<extractionTime<<", "<<matchingTime<<", "<<overallTime<<"\n";

%Generate the statistics
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






