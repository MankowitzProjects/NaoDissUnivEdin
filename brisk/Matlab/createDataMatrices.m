function [matchingScoreMatrix, meanTotalMatchesMatrix, meanBestMatchesMatrix, meanOverallTimeMatrix, meanScoreMatrix] = createDataMatrices(dataset, dim, displayStats, plot, matchType)

%Create the various Matrices
matchingScoreMatrix = zeros(dim,dim);
meanTotalMatchesMatrix =zeros(dim,dim);
meanBestMatchesMatrix = zeros(dim,dim);
meanOverallTimeMatrix = zeros(dim,dim);
meanScoreMatrix = zeros(dim,dim);

%Set the counters
distanceCounter = 1;
thresholdCounter=1;

%distance = 0.05 %BRISK SURF
distance = 10; %BRISK BRISK
threshold = 20;

for distanceCounter=1:19
    
    row1Match = [];
    threshold = 20;
    
   for thresholdCounter = 1:19
    
    number = sum(find(dataset(:,2)==distance)>0);
    distance  = Roundoff(distance,2);
    
    if matchType==1
    %For matching using Hamming distance or Euclidean Distance
    row1Match = strmatch([distance threshold], dataset(:,2:3));
    elseif matchType ==2
    %For Knn Neighbors
    row1Match = strmatch([threshold], dataset(:,3));
    end
    
    numberofPtsFound = sum(row1Match>0);
    %radius = radius
    %threshold = threshold
    sizeDataset = size(dataset(:,1));
    
    %Find the mean number of overall keypoints;
    meanKeypointsImage1  = mean(dataset(row1Match,6));
    meanKeypointsImage2  = mean(dataset(row1Match,7));
    %Find the mean best matching score
    meanMatchingScore1 = mean(dataset(row1Match,8));
    %Mean number of total matches
    meanTotalMatches1 = mean(dataset(row1Match,10));
    %Mean number of best matches (I.e. valid matches with repititions)
    meanBestMatches1 = mean(dataset(row1Match,11));
    
    
    %find the mean overallTime
    if matchType==1
    meanOverallTime = mean(dataset(row1Match, 15));
    elseif matchType==2
    %For KNN
    meanOverallTime = mean(dataset(row1Match, 17));
    end
    %Find the score for the particular threshold and radius
    summedScore = sum(dataset(row1Match,17));
    %take zero valid matches into account
    numZeroMatches = sum(strmatch([0], dataset(row1Match, 10))>0)
    %Find the number of zero matches
    passRatio = 1 - numZeroMatches/size(row1Match,1);
    %Output the final mean score
    summedScore  = summedScore/(numZeroMatches + 0.1);
    
    if(passRatio<0.6)
        summedScore = 0;
    end
    
    if(displayStats)
    disp('-----------------Stats---------------')
    meanKeypointsImage1 = meanKeypointsImage1
    meanKeypointsImage2 = meanKeypointsImage2
    meanMatchingScore1 = meanMatchingScore1
    meanTotalMatches1 = meanTotalMatches1
    meanBestMatches1 = meanBestMatches1
    meanOverallTime = meanOverallTime
    summedScore = summedScore
    disp('---------------------------------------')
    end

    %Update matrices
    matchingScoreMatrix(distanceCounter,thresholdCounter) = meanMatchingScore1;
    meanTotalMatchesMatrix(distanceCounter,thresholdCounter) = meanTotalMatches1;
    meanBestMatchesMatrix(distanceCounter,thresholdCounter) = meanBestMatches1;
    meanOverallTimeMatrix(distanceCounter,thresholdCounter) = meanOverallTime;
    meanScoreMatrix(distanceCounter, thresholdCounter) = summedScore;


    threshold = threshold +5;
   end
    %distance = distance + 0.01 BRISK SURF
    distance = distance+5; %BRISK BRISK
end

if(plot)
%Plot the various graphs
subplot(2,2,1);
%The matching score plot
surf(1:19,1:19,matchingScoreMatrix);
title('Matching Score');
xlabel('Threshold');
ylabel('Distance');

subplot(2,2,2);
%The mean number of total matches matrix
surf(1:19,1:19,meanTotalMatchesMatrix);
title('Mean Number of total matches');
xlabel('Threshold');
ylabel('Distance');

subplot(2,2,3)
%The mean number of best matches matrix
surf(1:19,1:19,meanBestMatchesMatrix);
title('Mean Number of best matches');
xlabel('Threshold');
ylabel('Distance');

subplot(2,2,4)
%The mean overall time matrix
surf(1:19,1:19,meanOverallTimeMatrix);
title('Mean Overall time (ms)');
xlabel('Threshold');
ylabel('Distance');
end
