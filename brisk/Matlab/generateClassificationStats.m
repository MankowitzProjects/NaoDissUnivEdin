function [] = generateClassificationStats(dataset1, index)
%Stats matrix
global stats;


%Now determine the average matching score for matching set
meanBestMatchingScore1 = mean(dataset1(:,7));
meanGeneralMatchingScore1 = mean(dataset1(:,7));

%Now determine the standard deviation for matching set
stdMeanBestMatchingScore1 =  std(dataset1(:,7));

%Update outputs
stats(index, 1) = meanBestMatchingScore1;
stats(index, 2) = meanGeneralMatchingScore1;
stats(index, 3) = stdMeanBestMatchingScore1;
stats(index, 4) = max(dataset1(:,7));
