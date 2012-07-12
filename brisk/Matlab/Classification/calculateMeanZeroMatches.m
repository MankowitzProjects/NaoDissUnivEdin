function [mean_zero_matches] = calculateMeanZeroMatches(data, test)

if nargin ==1
    %Find the overlapping image pairs
    %Separate the datasets
    
    [row1] = strmatch([1,1], data(:,1:2));
    [row2] = strmatch([2,2], data(:,1:2));
    [row3] = strmatch([3,3], data(:,1:2));
    [row4] = strmatch([4,4], data(:,1:2));
    
    %Create the overlapping dataset
    overlappingDataset = [data(row1,:); data(row2,:); data(row3,:); data(row4,:)];
    
    [row13nm] = strmatch([1,3], data(:,1:2));
    [row14nm] = strmatch([1,4], data(:,1:2));
    [row23nm] = strmatch([2,3], data(:,1:2));
    [row24nm] = strmatch([2,4], data(:,1:2));
    
    %Create the non-overlapping dataset
    nonOverlappingDataset = [data(row13nm,:); data(row14nm,:); data(row23nm,:); data(row24nm,:)];
    
    
    %Find the mean number of zero matches
    nonZeroMatches = sum(strmatch([0], overlappingDataset(:,11))>0);
    
elseif nargin >1
    
    overlappingDataset = data;
    %Find the mean number of zero matches
    nonZeroMatches = sum(strmatch([0], overlappingDataset(:,11))>0);
end


mean_zero_matches = nonZeroMatches/size(overlappingDataset,1)*100;
