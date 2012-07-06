function [maxNVM1, maxNVM2, maxNVM3, maxNVM4] = calcMaxValidMatchesKNN(dataset1, dataset2, dataset3, dataset4)

    %Variables storing the max number of valid matches for each dataset
    maxNVM1 = max(dataset1(:, 11));
    maxNVM2 = max(dataset2(:, 11));
    maxNVM3 = max(dataset3(:, 11));
    maxNVM4 = max(dataset4(:, 11));

