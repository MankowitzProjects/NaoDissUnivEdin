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
    
   %For BRISK0 UBRISK
 load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'
 [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);


%For SBRISK
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070521012_2318_78_110_mScoreFixed.mat'
load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'

[sb_overlappingMatchesMatrix, sb_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);

%For BRISK4
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070521012_2318_85_12125_mScoreFixed.mat'
load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'

[b4_overlappingMatchesMatrix, b4_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);

%SBRISK SURF2D
load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'

[sb_2d_overlappingMatchesMatrix, sb_2d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);

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



