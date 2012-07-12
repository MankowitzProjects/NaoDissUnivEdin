function [matchingScoreMatrix, meanTotalMatchesMatrix, meanBestMatchesMatrix, meanOverallTimeMatrix, meanScoreMatrix] = createDataMatricesKNN(dataset, maxNZM, dim, displayStats, plot)
global k3;
global k4;
global statsTable;

statsTable = zeros(8, 19);
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
    statsTable(thresholdCounter,1) = threshold;
    numberofPtsFound = sum(row1Match>0);
    statsTable(thresholdCounter,2) = numberofPtsFound;
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
    %Mean number of valid matches (I.e. valid matches with repititions)
    meanBestMatches1 = mean(dataset(row1Match,11));
    %find the mean overallTime
    meanOverallTime = mean(dataset(row1Match, 18));
    %Find the score for the particular threshold and radius
    summedScore = sum(dataset(row1Match,19));
    statsTable(thresholdCounter,3) = summedScore;
    %take zero valid matches into account
    numZeroMatches = sum(strmatch([0], dataset(row1Match, 11))>0);
    statsTable(thresholdCounter,4) = numZeroMatches;
    %Find the number of zero matches
    passRatio = 1 - numZeroMatches/size(row1Match,1);
    statsTable(thresholdCounter,5) = passRatio;
    %Normalise the summedScore
    normalisedSummedScore = summedScore/numberofPtsFound;
    statsTable(thresholdCounter,6) = normalisedSummedScore; 
    %Create the function k_nzm normalised between [0,1]
    h_NZM = abs(log10(0.9*numZeroMatches/(maxNZM) +0.1));
    statsTable(thresholdCounter,7) = h_NZM;
    %Output the final mean score
    FinalScore  = k4*normalisedSummedScore + k3*h_NZM;
    statsTable(thresholdCounter,8) = FinalScore;
    %Normalise the final score between [0,1]
    normalisedFinalScore = FinalScore/(k3 + k4);
    
    if(passRatio<0.3)
        normalisedFinalScore = 0;
    end
    
    if(displayStats)
    disp('-----------------Stats---------------')
    meanKeypointsImage1 = meanKeypointsImage1
    meanKeypointsImage2 = meanKeypointsImage2
    meanMatchingScore1 = meanMatchingScore1
    meanTotalMatches1 = meanTotalMatches1
    meanBestMatches1 = meanBestMatches1
    meanOverallTime = meanOverallTime
    normalisedFinalScore = normalisedFinalScore
    disp('---------------------------------------')
    end

    %Update matrices
    matchingScoreMatrix(1,thresholdCounter) = meanMatchingScore1;
    meanTotalMatchesMatrix(1, thresholdCounter) = meanTotalMatches1;
    meanBestMatchesMatrix(1, thresholdCounter) = meanBestMatches1;
    meanOverallTimeMatrix(1,thresholdCounter) = meanOverallTime;
    meanScoreMatrix(1,thresholdCounter) = normalisedFinalScore;


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