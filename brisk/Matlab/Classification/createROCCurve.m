%Determining whether or not matching scores are useful for classification
function [fpRateMatrix, tpRateMatrix, statsMatrix, thresholdsMatrix] = createROCCurve(data, step, datasetType, usingBriskorSURF)
global stats;

%Separate the datasets

[row1,col1] = find(data(:,1)==1 & data(:,2)==1);
[row2,col2] = find(data(:,1)==2 & data(:,2)==2);
[row3,col3] = find(data(:,1)==3 & data(:,2)==3);
[row4,col4] = find(data(:,1)==4 & data(:,2)==4);


[row13nm,col13nm] = find(data(:,1)==1 & data(:,2)==3);
[row14nm,col14nm] = find(data(:,1)==1 & data(:,2)==4);
[row23nm,col23nm] = find(data(:,1)==2 & data(:,2)==3);
[row24nm,col24nm] = find(data(:,1)==2 & data(:,2)==4);

%2 additional overlapping datasets
[row5,col5] = find(data(:,1)==5 & data(:,2)==5);
[row6,col6] = find(data(:,1)==6 & data(:,2)==6);

%1 additional non-overlapping dataset
[row56nm,col56nm] = find(data(:,1)==5 & data(:,2)==6);


%We have filtered the datasets by directory
dataset1 = data(row1,:);
dataset2 = data(row2,:);
dataset3 = data(row3,:);
dataset4 = data(row4,:);


dataset13nm = data(row13nm,:);
dataset23nm = data(row23nm,:);
dataset14nm = data(row14nm,:);
dataset24nm = data(row24nm,:);

%Dataset 2
dataset5 = data(row5,:);
dataset6 = data(row6,:);

%Dataset 2
dataset56nm = data(row56nm,:);


if datasetType ==1 || datasetType ==3
    %Generate the statistics for each matching dataset
    generateClassificationStats(dataset1,1);
    generateClassificationStats(dataset2,2);
    generateClassificationStats(dataset3,3);
    generateClassificationStats(dataset4,4);
    
    %Generate stats for each non-matching dataset
    generateClassificationStats(dataset13nm,5);
    generateClassificationStats(dataset23nm,6);
    generateClassificationStats(dataset14nm,7);
    generateClassificationStats(dataset24nm,8);
    
    %Find the mean of each directory for a specific scene/camera
    %Directory 1 to 4 is for the new Nao camera
    %Directory 5 to 8 is for the old Nao camera
    meanMatchesScore = mean(stats(1:4,1))
    meanNonMatchesScore = mean(stats(5:8,1))
    
elseif datasetType==2 || datasetType==21
    %Overlapping dataset
    generateClassificationStats(dataset5,1);
    generateClassificationStats(dataset6,2);
    %Non-overlapping dataset
    generateClassificationStats(dataset56nm,3);
    
    meanMatchesScore = mean(stats(1:2,1))
    meanNonMatchesScore = mean(stats(3,1))
end

if usingBriskorSURF
    stepValue = 0.01;
else
    stepValue = 1;
end

%Set the interval 
interval =1;

%Counter
counter = 1;
%To generate values for the ROC Curve
largestThreshold = ceil(max(stats(:,4)));

tpRateMatrix = zeros(1,largestThreshold/stepValue);
fpRateMatrix = zeros(1,largestThreshold/stepValue);

%Create a matrix to store the thresholds at each point
thresholdsMatrix = [];
thresholdCounter = 1;

for ii=largestThreshold:-stepValue:0

    classificationBoundary = meanMatchesScore - ((meanMatchesScore - meanNonMatchesScore)/1.02);
    %classificationBoundary =meanNonMatchesScore*2;
    
    if (counter == 300)
      counter;  
    end
    
    %Now to perform classification. We use the matching score
    %defined as the inverse hamming distance between 2 vectors
    %for BRISK or the inverse euclidean distance for SURF
    data(:,19) = data(:,8)>=ii;
    
    %Find the number of matches for matching pairs
    if datasetType == 1 || datasetType ==3
        [matchingStats] = calculateMatchingStats(data, row1,row2,row3,row4);
        [matchingStats1] = calculateMatchingStats(data, row13nm, row23nm, row14nm, row24nm);
    elseif datasetType==2 || datasetType==21
        [matchingStats] = calculateMatchingStats(data, row5,row6);
        [matchingStats1] = calculateMatchingStats(data, row56nm);
    end
    %The overall mean matching stat is:
    meanMatchesStat = mean(matchingStats);
    meanNonMatchesStat = mean(matchingStats1);
    
    %True positive rate
    TPRate = meanMatchesStat;
    %False Positive rate
    FPRate = meanNonMatchesStat;
    
    %Update the vector with the TP and FP rate respectively
    thresholdsMatrix(counter,1) = ii;
    thresholdsMatrix(counter,2) = FPRate;
    thresholdsMatrix(counter,3) = TPRate;
    
    %Store the values to plot on the ROC Curve
    tpRateMatrix(1,counter) = TPRate;
    fpRateMatrix(1,counter) = FPRate;
    counter  = counter+1;
end



AUC = trapz(fpRateMatrix, tpRateMatrix)*100

%The average time for computing each match
averageDetectionTime1 = mean(data(:,14))
averageExtractionTime1 = mean(data(:,15))

[averageDetectionTimeMatrix, averageExtractionTimeMatrix] = calculateMeanTimes(data);
averageDetectionTime = sum(averageDetectionTimeMatrix)
averageExtractionTime = sum(averageExtractionTimeMatrix)
averageMatchingTime = mean(data(:,16))
averageVerificationTime = mean(data(:,17))
averageOverallTime = mean(data(:,18))

%Compute the mean number of FP matches
fpRate = mean(fpRateMatrix)*100

statsMatrix = [AUC averageDetectionTime averageExtractionTime averageMatchingTime averageVerificationTime averageOverallTime];


