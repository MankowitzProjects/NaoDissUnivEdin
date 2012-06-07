%Data analysis for Master's Thesis
clc;
clear all

load 'mgMatchLeft.mat'

matchingData;

histplotflag=0;

%Calculate the mean matching score, including all matches
meanGeneralMatch = mean(matchingData(:,7))
%Standard Deviation
stdDeviationGeneralMatch  =std(matchingData(:,7))

%Calculate the mean matching score, including only best matches
meanBestMatch = mean(matchingData(:,6))
%Standard Deviation
stdDeviationBestMatch = std(matchingData(:,6))

%Mean number of matches (duplicates)
duplicateMatches = floor(mean(matchingData(:,8)))

%Mean number of matches (no duplicates)
noDuplicateMatches = floor(mean(matchingData(:,9)))

%Mean number of interest points detected (Image 2)
interestPoints = floor(mean(matchingData(:, 5)))

if histplotflag
%Create a histogram of the interest points
subplot(3,2,1)
%hist(matchingData(:,5),10)
plot(matchingData(:,5))
title('Interest points');

%Create a histogram of non-duplicate matching score
subplot(3,2,2)
hist(matchingData(:,6),10)
title('Non-duplicate Matching Score');

%Create a histogram of duplicate matching score
subplot(3,2,3)
hist(matchingData(:,7),10)
title('Duplicate Matching score');

%Create a histogram of non-duplicate matches
subplot(3,2,4)
hist(matchingData(:,9),10)
title('Number of Non-duplicate matches');
end
%----------------------------------------
%Determine the number of zero matches
numZeroMatches = sum(find(matchingData(:,8)==0)>0)

[row,col] = find(matchingData(:,8)==0);

images(:,1) = matchingData(row, 2);
images(:,2) = matchingData(row, 3);

numImages = max(images(:,1));
zeroMatches = zeros(1, numImages);
%Determine how many images
for ii=1:numImages
    zeroMatches(1,ii) = sum(find(images(:,1)==ii)>0);
end
%Plot the images with zero matches
figure;
bar(zeroMatches)

%The mean of the zero matches is
zeroMatchMean = mean(zeroMatches);

%Percentage of zero matches @40 threshold
totalImages = size(matchingData,1);

percentageZeroMatches = numZeroMatches/totalImages*100


