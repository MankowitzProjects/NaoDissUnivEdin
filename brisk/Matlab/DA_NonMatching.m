%Data Analysis for non-matching data
load 'nonMatching.mat'

clc;
histplotflag=0;

%Calculate the mean matching score, including all matches
meanGeneralMatch = mean(nonmatchingData(:,7))
%Standard Deviation
stdDeviationGeneralMatch  =std(nonmatchingData(:,7))

%Calculate the mean matching score, including only best matches
meanBestMatch = mean(nonmatchingData(:,6))
%Standard Deviation
stdDeviationBestMatch = std(nonmatchingData(:,6))

%Mean number of matches (duplicates)
duplicateMatches = floor(mean(nonmatchingData(:,8)))

%Mean number of matches (no duplicates)
noDuplicateMatches = floor(mean(nonmatchingData(:,9)))

%Mean number of interest points detected (Image 1)
interestPoints1 = floor(mean(nonmatchingData(:, 4)))
%Mean number of interest points detected (Image 2)
interestPoints2 = floor(mean(nonmatchingData(:, 5)))


if histplotflag
%Create a histogram of the interest points in image 2
subplot(2,2,1)
hist(matchingData(:,5),10)
xlabel('Interest points');

%Create a histogram of non-duplicate matching score
subplot(2,2,2)
hist(matchingData(:,6),10)
xlabel('Non-duplicate Matching Score');

%Create a histogram of duplicate matching score
subplot(2,2,3)
hist(matchingData(:,7),10)
xlabel('Duplicate Matching score');

%Create a histogram of non-duplicate matches
subplot(2,2,4)
hist(matchingData(:,9),10)
xlabel('Number of Non-duplicate matches');
end
%----------------------------------------
%Determine the number of zero matches
numZeroMatches = sum(find(nonmatchingData(:,8)==0)>0)

[row,col] = find(nonmatchingData(:,8)==0);

%Percentage of zero matches @40 threshold
totalImages = size(nonmatchingData,1);

percentageZeroMatches = numZeroMatches/totalImages*100

%images(:,1) = nonmatchingData(row, 2);
%images(:,2) = nonmatchingData(row, 3);