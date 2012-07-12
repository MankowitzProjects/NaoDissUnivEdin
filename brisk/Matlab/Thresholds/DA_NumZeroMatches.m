%Find the number of zero matches
clear all
clc
knn =0;

if knn==1

%For SBRISK

    
%SBRISK UBRISK
load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
%load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'
    

%For SBRISK
%load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070321012_2310.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_0220_30.mat'%mScore
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_4625_mScoreFixed.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_30_mScoreFixed_consistentThreshold.mat'

load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'





%For BRISK4
%load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070421012_1341_51.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2125_5125_mScoreFixed.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2215_30_mScoreFixed_consistentThreshold.mat'

load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
% load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'




%For SBRISK SURF2D
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_0220_45.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_45_mScoreFixed.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_30_mScoreFixed_consistentThreshold.mat'

load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
% load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'




else
    
%For BRISK0 UBRISK
 load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'

 

%For SBRISK
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070521012_2318_78_110_mScoreFixed.mat'
load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'


%For BRISK4
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070521012_2318_85_12125_mScoreFixed.mat'
load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'



%SBRISK SURF2D
load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'

end




