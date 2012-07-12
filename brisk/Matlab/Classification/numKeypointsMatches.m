%Compare the number of keypoints found and the number of matches for all 3
%methods
clear all
clc
knn =1;

if knn==1
    %SBRISK UBRISK
    %load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
    load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'
    
    
    [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
    
    %For SBRISK
    %load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070321012_2310.mat'
    %load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_0220_30.mat'%mScore
    %load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_4625_mScoreFixed.mat'
    %load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_30_mScoreFixed_consistentThreshold.mat'
    
    %load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
    load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'
    
    
    [sb_overlappingMatchesMatrix, sb_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
    
    %For BRISK4
    %load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070421012_1341_51.mat'
    %load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2125_5125_mScoreFixed.mat'
    %load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2215_30_mScoreFixed_consistentThreshold.mat'
    
    %load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
    load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'
    
    
    [b4_overlappingMatchesMatrix, b4_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
    
    %For SBRISK SURF2D
    %load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_0220_45.mat'
    %load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_45_mScoreFixed.mat'
    %load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_30_mScoreFixed_consistentThreshold.mat'
    
    %load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
    load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'
    
    
    
    [sb_2d_overlappingMatchesMatrix, sb_2d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);

    
    
else
    
    %For SBRISK
    %load 'nonmatching_matching_Data__BRISK__BRISK_Hamming_070421012_1222_98_78.mat'
    %load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070421012_1922.mat'
    %%Latest
    load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070521012_2318_78_110_mScoreFixed.mat'
    
    [sb_total_matches, sb_valid_matches, sb_invalid_matches, sb_best_matches]  = calculateMatchingStatistics(data);
    
    %For BRISK4
    %load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070421012_1341.mat'
    %load 'g_matching_Data__BRISK4__BRISK4_Hamming_070421012_1922_88.mat'%Latest
    load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070521012_2318_85_12125_mScoreFixed.mat'
    
    [b4_total_matches, b4_valid_matches, b4_invalid_matches, b4_best_matches]  = calculateMatchingStatistics(data);
    
    %SBRISK SURF2D
    %load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_0250_33.mat'
    load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_2318_65_028_mScoreFixed.mat'
    
    [sb_2d_total_matches, sb_2d_valid_matches, sb_2d_invalid_matches, sb_2d_best_matches]  = calculateMatchingStatistics(data);
    
end
%Output the matching statistics for overlapping and non-overlapping
%datasets respectively
%The statistics are mean total matches, valid matches, invalid matches,
%mean number of keypoints
overlappingMatchingStats = [sb_overlappingMatchesMatrix;
    b4_overlappingMatchesMatrix;
    sb_2d_overlappingMatchesMatrix;
    ub_overlappingMatchesMatrix];

nonOverlappingMatchingStats = [sb_nonOverlappingMatchesMatrix;
    b4_nonOverlappingMatchesMatrix;
    sb_2d_nonOverlappingMatchesMatrix;
    ub_nonOverlappingMatchesMatrix];

overlappingMatchingStats = Roundoff(overlappingMatchingStats, 2);

nonOverlappingMatchingStats = Roundoff(nonOverlappingMatchingStats,2);



