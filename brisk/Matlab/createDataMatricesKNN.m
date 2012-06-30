function [matchingScoreMatrix, meanTotalMatchesMatrix, meanBestMatchesMatrix, meanOverallTimeMatrix, meanScoreMatrix] = createDataMatricesKNN(dataset, dim, displayStats, plot)

%Create the various Matrices
matchingScoreMatrix = zeros(1,dim);
meanTotalMatchesMatrix =zeros(1,dim);
meanBestMatchesMatrix = zeros(1,dim);
meanOverallTimeMatrix = zeros(1,dim);
meanScoreMatrix = zeros(1,dim);

%Set the counters
distanceCounter = 1;
thresholdCounter=1;

%distance = 0.05 %BRISK SURF
%distance = 10; %BRISK BRISK
threshold = 20;

    row1Match = [];
    
   for thresholdCounter = 1:19
    
    %number = sum(find(dataset(:,2)==distance)>0);
    %distance  = Roundoff(distance,2);
    
    %For Knn Neighbors
    row1Match = strmatch([threshold], dataset(:,3));
    
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
    meanOverallTime = mean(dataset(row1Match, 17));
    %Find the score for the particular threshold and radius
    summedScore = sum(dataset(row1Match,18));
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
    matchingScoreMatrix(1,thresholdCounter) = meanMatchingScore1;
    meanTotalMatchesMatrix(1, thresholdCounter) = meanTotalMatches1;
    meanBestMatchesMatrix(1, thresholdCounter) = meanBestMatches1;
    meanOverallTimeMatrix(1,thresholdCounter) = meanOverallTime;
    meanScoreMatrix(1,thresholdCounter) = summedScore;


    threshold = threshold +5;
   end

% if(plot)
% %Plot the various graphs
% subplot(2,2,1);
% %The matching score plot
% surf(1:19,1:19,matchingScoreMatrix);
% title('Matching Score');
% xlabel('Threshold');
% ylabel('Distance');
% 
% subplot(2,2,2);
% %The mean number of total matches matrix
% surf(1:19,1:19,meanTotalMatchesMatrix);
% title('Mean Number of total matches');
% xlabel('Threshold');
% ylabel('Distance');
% 
% subplot(2,2,3)
% %The mean number of best matches matrix
% surf(1:19,1:19,meanBestMatchesMatrix);
% title('Mean Number of best matches');
% xlabel('Threshold');
% ylabel('Distance');
% 
% subplot(2,2,4)
% %The mean overall time matrix
% surf(1:19,1:19,meanOverallTimeMatrix);
% title('Mean Overall time (ms)');
% xlabel('Threshold');
% ylabel('Distance');
% end