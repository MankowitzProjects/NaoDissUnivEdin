%Compare the number of keypoints found and the number of matches for all 3
%methods
clear all
clc
addpath('../Lighting')
addpath('../Data3');
addpath('../Data2');
addpath('../DataCamera');
addpath('../DataStreet');
addpath('../DataValidation');
%Choose the dataset type.
% 1 - means using structure of 1-4
% 2 - means using structure 1-2 (Office Environment)
% 21 - means using structure 1-2 (Large Hall)
% 3 - varying lighting
% 4 - means matching Nao dataset with camera for Robocup
% 5 - means matching Nao dataset with camera for Large Hall
% 6 - means matching Nao dataset with Google Street View
datasetType = 4;

%Choose if we are comparing knn or hamming/euclidean distance
% 1 - KNN
% 2 - Radius Match
matchingMethod =2;

if matchingMethod==1
    
    if datasetType == 1
        %BRISK0 UBRISK
        %         load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
        %         load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'
        
%               load 'nonmatching_matching_Data__BRISK_U-BRISK_KNN_02082012_1033_75__consistent.mat'
        load 'nonmatching_matching_Data__BRISK_U-BRISK_KNN_02082012_1033_60__max.mat'
    
        [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %For BRISK0
        %         load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
        %         load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'
        
%         load 'nonmatching_matching_Data__BRISK_BRISK_KNN_02082012_1033_40__consistent.mat'
        load 'nonmatching_matching_Data__BRISK_BRISK_KNN_02082012_1033_45__max.mat'
        
        [sb_overlappingMatchesMatrix, sb_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %For BRISK4
        %         load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
        %         load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'
        
%          load 'nonmatching_matching_Data__BRISK4_BRISK4_KNN_02082012_1033_40__consistent.mat'
        load 'nonmatching_matching_Data__BRISK4_BRISK4_KNN_02082012_1033_47.5__max.mat'
        
        [b4_overlappingMatchesMatrix, b4_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %For SBRISK SURF2D
        %         load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
        %         load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'
        
%           load 'nonmatching_matching_Data__BRISK_SURF_KNN_02082012_1033_40__consistent.mat'
        load 'nonmatching_matching_Data__BRISK_SURF_KNN_02082012_1033_45__max.mat'      
        [sb_2d_overlappingMatchesMatrix, sb_2d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        %         load 'nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        
        load 'nonmatching_matching_Data__SURF1D_Euclidean_02082012_1523_325.125__given.mat'
        [s1d_overlappingMatchesMatrix, s1d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        
    elseif datasetType ==2
        %BRISK0 - U-BRISK
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_60__max.mat'
        %load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_75__consistent.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_35__consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_55__max.mat'
        [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %BRISK0
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_45__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_40__consistent.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_30__consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_46.25__max.mat'
        [sb_overlappingMatchesMatrix, sb_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data,2);
        
        %BRISK4
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_47.5__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_40__consistent.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_30__consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_51.25__max.mat'
        [b4_overlappingMatchesMatrix, b4_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %For SBRISK SURF2D
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_45__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_40__consistent.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_30__consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_43.75__max.mat'
        [sb_2d_overlappingMatchesMatrix, sb_2d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset2_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [s1d_overlappingMatchesMatrix, s1d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        
    elseif datasetType==21
        %BRISK0 - U-BRISK
        %         load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_35__consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_55__max.mat'
        [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %BRISK0
        %         load dataset3_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_30__consistent.mat''
        load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_46.25__max.mat'
        [sb_overlappingMatchesMatrix, sb_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data,2 );
        
        %BRISK4
        %         load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_30__consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_51.25__max.mat'
        [b4_overlappingMatchesMatrix, b4_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %For SBRISK SURF2D
        %         load 'dataset3_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_30__consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_43.75__max.mat'
        [sb_2d_overlappingMatchesMatrix, sb_2d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset3_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [s1d_overlappingMatchesMatrix, s1d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        
        
    end%datasetType
    
elseif matchingMethod==2 %Hamming/Euclidean distance
    
    if datasetType ==1
        %For BRISK0 UBRISK
        %                 load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'
        %         load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_14072012_1140_75_130_consistent.mat'
        
%         load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_130_consistent.mat'
                load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
        
        
        [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %For SBRISK
        %                 load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'
        %         load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_14072012_1104_75_115_consistent.mat'
        
%         load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_02082012_1033_75_115_consistent.mat'
                load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_02082012_1033_77.5_107_max.mat'
        
        [sb_overlappingMatchesMatrix, sb_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %For BRISK4
        %load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'
        %         load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_14072012_1140_65_130_consistent.mat'
        %         load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_29072012_1248_80_120_max.mat'
        
%         load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_02082012_1033_65_130_consistent.mat'
                load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_02082012_1033_80_120_max.mat'
        
        [b4_overlappingMatchesMatrix, b4_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %SBRISK SURF2D
        %                 load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'
        %         load 'nonmatching_matching_Data__BRISK_SURF_Hamming_14072012_1140_60_0_consistent.mat'
        
        
%         load 'nonmatching_matching_Data__BRISK_SURF_Hamming_02082012_1033_60_0_consistent.mat'
                load 'nonmatching_matching_Data__BRISK_SURF_Hamming_02082012_1033_65_0_max.mat'
        
        [sb_2d_overlappingMatchesMatrix, sb_2d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %1D SURF
        %         load 'nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        load 'nonmatching_matching_Data__SURF1D_Euclidean_02082012_1523_325.125__given.mat'
        
        [s1d_overlappingMatchesMatrix, s1d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
    elseif datasetType == 2
        %Hamming/Euclidean distance on dataset 2
        %BRISK0 - U-BRISK
        %         load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_130_consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_121_max.mat'
        [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %BRISK0
        %         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_75_115_consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_77.5_107_max.mat'
        [sb_overlappingMatchesMatrix, sb_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %BRISK4
        %         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_65_130_consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_80_120_max.mat'
        [b4_overlappingMatchesMatrix, b4_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %For SBRISK SURF2D
        %         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_60_0_consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_65_0_max.mat'
        [sb_2d_overlappingMatchesMatrix, sb_2d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset2_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        
        [s1d_overlappingMatchesMatrix, s1d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
    elseif datasetType ==21
        %BRISK0 - U-BRISK
        %         load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_130_consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_121_max.mat'
        [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %BRISK0
        %         load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_75_115_consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_77.5_107_max.mat'
        [sb_overlappingMatchesMatrix, sb_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %BRISK4
        %         load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_65_130_consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_80_120_max.mat'
        [b4_overlappingMatchesMatrix, b4_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %For SBRISK SURF2D
        %         load 'dataset3_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_60_0_consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_65_0_max.mat'
        [sb_2d_overlappingMatchesMatrix, sb_2d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset3_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [s1d_overlappingMatchesMatrix, s1d_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2);
        
    elseif datasetType == 3 %For varying illumination
        
        %All these methods use BRISK0 - UBRISK
        %1. Original Dataset Left light off
        %load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%         load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2038_75_121_max.mat'
%       load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_130_consistent.mat'  

load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
[left_overlappingMatchesMatrix, left_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data,2,4);
        
        %2. Original Dataset Right light off
        %load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%         load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2038_75_121_max.mat'
%        load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_130_consistent.mat' 

load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
[right_overlappingMatchesMatrix, right_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data,2,4);
        
        %3. Original dataset both lights off
        %load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%         load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2038_75_121_max.mat'
%     load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_130_consistent.mat'     

load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
[both_overlappingMatchesMatrix, both_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data,2,4);
        
    elseif datasetType==4 %For comparing pics with the camera
        %For BRISK0 UBRISK
%         load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'
        %         load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_14072012_1140_75_130_consistent.mat'
        
        load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
        [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data);
        
        %Compare the Nao pics with pics taken from a Nikon camera
        %load 'BRISK_U-BRISK_Hamming_25072012_2038_75_121_max.mat'
%         load 'camera_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2232_75_121_max.mat'
        
load 'camera_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_03082012_1033_75_121_max.mat'
[camera_overlappingMatchesMatrix, camera_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data, 2, 3);
        
    elseif datasetType==5
        %BRISK0 - U-BRISK
        %         load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_130_consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_121_max.mat'
        [ub_overlappingMatchesMatrix, ub_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data,2);
        
        
        %Comparing Nao pics with large hall
        load 'camera_large_hall_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2232_75_121_max.mat'
        [hall_overlappingMatchesMatrix, hall_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data,2,3,4);
        
    elseif datasetType == 6
        %Comparing Nao Pics with Google Street View
        load 'streetview_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2232_75_121_max.mat'
        [street_overlappingMatchesMatrix, street_nonOverlappingMatchesMatrix]  = calculateMatchingStatistics(data,2,3,4);
        
    end%End dataset type
    
end %End KNN neighbors

%----------------------------------------------------------
%Output the matching statistics for overlapping and non-overlapping
%datasets respectively
%The statistics are mean total matches, valid matches, invalid matches,
%mean number of keypoints



if datasetType ==1 || datasetType ==2 || datasetType == 21
    overlappingMatchingStats = [sb_overlappingMatchesMatrix;
        b4_overlappingMatchesMatrix;
        sb_2d_overlappingMatchesMatrix;
        ub_overlappingMatchesMatrix;
        s1d_overlappingMatchesMatrix]
    
    nonOverlappingMatchingStats = [sb_nonOverlappingMatchesMatrix;
        b4_nonOverlappingMatchesMatrix;
        sb_2d_nonOverlappingMatchesMatrix;
        ub_nonOverlappingMatchesMatrix;
        s1d_nonOverlappingMatchesMatrix]
    
elseif datasetType==3
    overlappingMatchingStats = [left_overlappingMatchesMatrix;
        right_overlappingMatchesMatrix;
        both_overlappingMatchesMatrix]
    
    nonOverlappingMatchingStats = [left_nonOverlappingMatchesMatrix;
        right_nonOverlappingMatchesMatrix;
        both_nonOverlappingMatchesMatrix]
    
elseif datasetType==4
    overlappingMatchingStats = [ub_overlappingMatchesMatrix;
        camera_overlappingMatchesMatrix]
    
    nonOverlappingMatchingStats = [ub_nonOverlappingMatchesMatrix;
        camera_nonOverlappingMatchesMatrix]
    
    
elseif datasetType==5
    overlappingMatchingStats = [ub_overlappingMatchesMatrix;
        hall_overlappingMatchesMatrix]
    
    nonOverlappingMatchingStats = [ub_nonOverlappingMatchesMatrix;
        hall_nonOverlappingMatchesMatrix]
    
elseif datasetType==6
    overlappingMatchingStats = [street_overlappingMatchesMatrix]
    
    nonOverlappingMatchingStats = [street_nonOverlappingMatchesMatrix]
    
end

overlappingMatchingStats = Roundoff(overlappingMatchingStats, 2)

nonOverlappingMatchingStats = Roundoff(nonOverlappingMatchingStats,2)
