function [detectionTimeMatrix, extractionTimeMatrix] = calculateMeanTimes(data)

%Find the maximum number of images
maxNumImages = max(data(:,5));

%Find the total number of images
totalNumImages = size(data,1);

%The detection matrix and extraction matrix
detectionTimeMatrix = [];
extractionTimeMatrix = [];

for ii=1:maxNumImages
    
%Find all images equal to ii
row1Match = strmatch([ii], data(:,5));

%Average all the times for the specific image
averageDetectionTime = mean(data(row1Match,14));
averageExtractionTime = mean(data(row1Match,15));

%Find the number of points
numPoints = size(row1Match,1);

%The weighting of the detected points
weighting = numPoints/totalNumImages;

%Calculate the final time
detectionTimeMatrix = [detectionTimeMatrix weighting*averageDetectionTime];

extractionTimeMatrix = [extractionTimeMatrix weighting*averageExtractionTime];

end
