function [keypointSum, maxNumImages] = calculateMeanKeypoints(data)

%Find the maximum number of images
maxNumImages = max(data(:,4));

%Find the total number of images
totalNumImages = size(data,1);

%The detection matrix and extraction matrix
meanKeypointMatrix = [];

%The sum of the keypoints
keypointSum = 0;

for ii=1:maxNumImages
    
    if ii==1
        row1Match = strmatch([ii], data(:,5));
        numKeypoints = data(row1Match(1),7);
    else
        %Find all images equal to ii
        row1Match = strmatch([ii], data(:,4));
        numKeypoints = data(row1Match(1),6);
    end
    
    
    
    
    %Add the number of keypoints to the first image
    keypointSum = keypointSum + numKeypoints;
    
    
    % %Average all the keypoints for the specific image
    % averageKeypoints = mean(data(row1Match,7));
    
    % %Find the number of points
    % numPoints = size(row1Match,1);
    %
    % %The weighting of the detected points
    % weighting = numPoints/totalNumImages;
    %
    % %Calculate the final time
    % meanKeypointMatrix = [meanKeypointMatrix weighting*averageKeypoints];
    
    
end


% %Calculate the mean
% meanKeypoints = sum(meanKeypointMatrix);


