%Find the number of zero matches
clear all
clc
knn =-1;
knnThreshold =0 ;

if knn==1

%For SBRISK

    
%SBRISK UBRISK
% load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'
    
%calculate the mean number of zero matches
[ub_mean_zero_matches] = calculateMeanZeroMatches(data);

%For SBRISK
% load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'

[sb_mean_zero_matches] = calculateMeanZeroMatches(data)



%For BRISK4
% load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'

[b4_mean_zero_matches] = calculateMeanZeroMatches(data)


%For SBRISK SURF2D
% load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'

[sb_2d_mean_zero_matches] = calculateMeanZeroMatches(data)


elseif knn==0
    
%For BRISK0 UBRISK
 load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'

 [ub_mean_zero_matches] = calculateMeanZeroMatches(data)

%For SBRISK
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070521012_2318_78_110_mScoreFixed.mat'
load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'

[sb_mean_zero_matches] = calculateMeanZeroMatches(data)
%For BRISK4
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070521012_2318_85_12125_mScoreFixed.mat'
load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'

[b4_mean_zero_matches] = calculateMeanZeroMatches(data)

%SBRISK SURF2D
load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'
[sb_2d_mean_zero_matches] = calculateMeanZeroMatches(data)

end

if knnThreshold==1
    %SBRISK - UBRISK
%load 'threshold_SBRISK_UBRISK_KNN_threshold_09072012_0935_NewTimes.mat'
load 'threshold_SBRISK_UBRISK_KNN_threshold_10072012_1652.mat'
[ub_mean_zero_matches] = calculateMeanZeroMatches(data,1)


%SBRISK
%load '/thresholds/threshold_BRISK_BRISK_KNN_03072012_2202.mat'
%load 'threshold_SBRISK_SBRISK_KNN_threshold_05072012_1829_FixedMatchScore.mat'
%load 'threshold_SBRISK_SBRISK_KNN_threshold_09072012_0935_NewTimes.mat'
load 'threshold_SBRISK_SBRISK_KNN_threshold_10072012_1636.mat'
[sb_mean_zero_matches] = calculateMeanZeroMatches(data,1)

%BRISK4
%load '/thresholds/threshold_BRISK4_BRISK4_KNN_threshold_04072012_1041.mat'
%load 'threshold_BRISK4_BRISK4_KNN_threshold_05072012_1857_FixedMatchScore.mat'
load 'threshold_BRISK4_BRISK4_KNN_threshold_10072012_1745.mat'
[b4_mean_zero_matches] = calculateMeanZeroMatches(data,1)


%SBRISK SURF2D
%load '/thresholds/threshold_SBRISK_SURF2D_KNN_threshold_04072012_1502.mat'
%load 'threshold_SBRISK_SURF2D_KNN_threshold_05072012_1933_FixedMatchScore.mat'
load 'threshold_SBRISK_SURF2D_KNN_threshold_10072012_1748.mat'
[sb_2d_mean_zero_matches] = calculateMeanZeroMatches(data,1)

    
elseif knnThreshold==0
    
    %U-BRISK
load 'threshold_SBRISK_UBRISK_Hamming_threshold_12072012_0055.mat'
[ub_mean_zero_matches] = calculateMeanZeroMatches(data,1)

%SBRISK
load 'threshold_SBRISK_SBRISK_Hamming_threshold_12072012_0055.mat'
[sb_mean_zero_matches] = calculateMeanZeroMatches(data,1)

%BRISK4
load 'threshold_BRISK4_BRISK4_Hamming_threshold_11072012_1013.mat'
[b4_mean_zero_matches] = calculateMeanZeroMatches(data,1)

%SBRISK SURF2D
load 'threshold_SBRISK_SURF2D_Hamming_threshold_11072012_0149.mat'
[sb_2d_mean_zero_matches] = calculateMeanZeroMatches(data,1)

end

meanOverlappingMatchesMatrix = Roundoff([sb_mean_zero_matches;
                                b4_mean_zero_matches;
                                sb_2d_mean_zero_matches;
                                ub_mean_zero_matches],2)
                                


