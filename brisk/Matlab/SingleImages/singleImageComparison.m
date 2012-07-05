%Single image comparison function

%Load all the comparison files into matlab
load 'singleImages_SBRISK_SBRISK_07052012_1812_46.mat'

%Compute Matching Statistics

%The matching vector
matchVector = [];

%Find the number of images being compared
numImages1 = max(matches(:,3));
numImages2 = max(matches(:,4));

numDirectories = max(matches(:,1));
for kk=1:numDirectories
    for ii=1:numImages1
        for jj=1:ii-1
            %Determine the number of matches for a pair of images in a
            %specific directory
            imagePairIndices = strmatch([kk, ii, jj], matches(:,[1, 3:4]));
            matchesData = matches(imagePairIndices, :);
            
            %Find the maximum query index
            maxIndex = max(matchesData(:,5));
            
            %Now determine if two matches were found
            for ll=1:maxIndex
                
                %Calculate the number of matches
                numMatches = sum(strmatch([ll], matchesData(:,5))>0);
      
                
                if (numMatches ==2)
                    [rows, cols] = find (matchesData(:,5) == ll);
                    num = matchesData(rows(1,1), 20)
                    den = matchesData(rows(2,1), 20)
                    ll
                    %add this condition to prevent non negative values
                    if(matchesData(rows(2,1)>0))
                    ratio = abs(num/den)
                    
                    %ratio = matchesData(:,
                    matchVector = [matchVector ratio];
                    end
                end
                
            end
            
            %numMatches = sum(strmatch([ii, jj], matches(:,3))>0)
        end
    end
end
%matches(:,19) =



%Keypoints
%-------------------------------------------------
%Find the average response

%Find the average size

%Find the average octave

%Find the average angle


%Matches
%-------------------------------------------------
%Find the average difference in angle between matches
averageAngleDiff = mean(abs(matches(:,13) - matches(:,7)))
%Find the average difference in size between matches
averageSizeDiff = mean(abs(matches(:,14) - matches(:,8)))
%Find the average difference in octaves between matches
averageOctaveDiff = mean(abs(matches(:,15) - matches(:,9)))
%Find the average difference in response between matches
averageResponseDiff = mean(abs(matches(:,16) - matches(:,10)))
%Find the average distance between matches
averageDistance = mean(matches(:,18))



