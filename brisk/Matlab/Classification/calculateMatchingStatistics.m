
%Calculate the matching statistics
function [total_matches, valid_matches, invalid_matches, best_matches]  = calculateMatchingStatistics(data)

total_matches = mean(data(:,10));

valid_matches = mean(data(:,11));

invalid_matches = mean(data(:,12));

best_matches = mean(data(:,13));