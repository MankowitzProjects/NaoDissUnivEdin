function [maxNZM1, maxNZM2, maxNZM3, maxNZM4] = calcNumZeroMatchesKNN(dataset1, dataset2, dataset3, dataset4)

threshold = 20;

    row1Match = [];
    row2Match = [];
    row3Match = [];
    row4Match = [];
    
    %Variables storing the max number of non-zero matches
    maxNZM1 = 0;
    maxNZM2 = 0;
    maxNZM3 = 0;
    maxNZM4 = 0;
    
   for thresholdCounter = 1:19
    %For Knn Neighbors
    row1Match = strmatch([threshold], dataset1(:,3));
    row2Match = strmatch([threshold], dataset2(:,3));
    row3Match = strmatch([threshold], dataset3(:,3));
    row4Match = strmatch([threshold], dataset4(:,3));
    
    %take zero valid matches into account
    numZeroMatches1 = sum(strmatch([0], dataset1(row1Match, 11))>0);
    numZeroMatches2 = sum(strmatch([0], dataset2(row2Match, 11))>0);
    numZeroMatches3 = sum(strmatch([0], dataset3(row3Match, 11))>0);
    numZeroMatches4 = sum(strmatch([0], dataset4(row4Match, 11))>0);
    
    %Find the maximum number of zero matches for each dataset
    if (numZeroMatches1>maxNZM1)
        maxNZM1 = numZeroMatches1;
    end
    if (numZeroMatches2>maxNZM2)
        maxNZM2 = numZeroMatches2;
    end
    if (numZeroMatches3>maxNZM3)
        maxNZM3 = numZeroMatches3;
    end
    if (numZeroMatches4>maxNZM4)
        maxNZM4 = numZeroMatches4;
    end
        
    
    %Increment the threshold
    threshold = threshold + 5;
   end
