function [matchingScoreMatrix, meanTotalMatchesMatrix, meanBestMatchesMatrix, meanOverallTimeMatrix] = createDataMatrices(dataset, dim, displayStats, plot)

%Create the various Matrices
matchingScoreMatrix = zeros(dim,dim);
meanTotalMatchesMatrix =zeros(dim,dim);
meanBestMatchesMatrix = zeros(dim,dim);
meanOverallTimeMatrix = zeros(dim,dim);

%Set the counters
radiusCounter = 1;
thresholdCounter=1;

radius = 0.05;
threshold = 20;

for radiusCounter=1:19
    
    row1Match = [];
    threshold = 20;
    
   for thresholdCounter = 1:19
    
    number = sum(find(dataset(:,2)==radius)>0);
    radius  = Roundoff(radius,2);
    row1Match = strmatch([radius threshold], dataset(:,2:3));

    numberofPtsFound = sum(row1Match>0);
    %radius = radius
    %threshold = threshold
    sizeDataset = size(dataset(:,1));
    
    %Find the mean number of overall keypoints;
    meanKeypointsImage1  = mean(dataset(row1Match,6));
    meanKeypointsImage2  = mean(dataset(row1Match,7));
    %Find the mean matching score
    meanMatchingScore1 = mean(dataset(row1Match,8));
    %Mean number of total matches
    meanTotalMatches1 = mean(dataset(row1Match,10));
    %Mean number of best matches
    meanBestMatches1 = mean(dataset(row1Match,11));
    %find the mean overallTime
    meanOverallTime = mean(dataset(row1Match, 15));
    if(displayStats)
    disp('-----------------Stats---------------')
    meanKeypointsImage1 = meanKeypointsImage1
    meanKeypointsImage2 = meanKeypointsImage2
    meanMatchingScore1 = meanMatchingScore1
    meanTotalMatches1 = meanTotalMatches1
    meanBestMatches1 = meanBestMatches1
    meanOverallTime = meanOverallTime
    disp('---------------------------------------')
    end

    %Update matrices
    matchingScoreMatrix(radiusCounter,thresholdCounter) = meanMatchingScore1;
    meanTotalMatchesMatrix(radiusCounter,thresholdCounter) = meanTotalMatches1;
    meanBestMatchesMatrix(radiusCounter,thresholdCounter) = meanBestMatches1;
    meanOverallTimeMatrix(radiusCounter,thresholdCounter) = meanOverallTime;



    threshold = threshold +5;
    end
    radius = radius+0.01;
end

if(plot)
%Plot the various graphs
subplot(2,2,1);
%The matching score plot
surf(1:19,1:19,matchingScoreMatrix);
title('Matching Score');
xlabel('Threshold');
ylabel('Radius');

subplot(2,2,2);
%The mean number of total matches matrix
surf(1:19,1:19,meanTotalMatchesMatrix);
title('Mean Number of total matches');
xlabel('Threshold');
ylabel('Radius');

subplot(2,2,3)
%The mean number of best matches matrix
surf(1:19,1:19,meanBestMatchesMatrix);
title('Mean Number of best matches');
xlabel('Threshold');
ylabel('Radius');

subplot(2,2,4)
%The mean overall time matrix
surf(1:19,1:19,meanOverallTimeMatrix);
title('Mean Overall time (ms)');
xlabel('Threshold');
ylabel('Radius');
end
