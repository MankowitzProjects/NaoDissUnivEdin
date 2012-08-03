%Generate the ROC Curves
clear all
clc
addpath('../Lighting')
addpath('../DataValidation');
addpath('../Data3');
addpath('../Data2');
addpath('../DataCamera');
addpath('../DataStreet');
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

%Whether we plot the ROC Curve or not
plotROC = 0;

if matchingMethod==1
    
    if datasetType == 1
        %BRISK0 UBRISK
        %         load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
        %load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'
        
        
%         load 'nonmatching_matching_Data__BRISK_U-BRISK_KNN_02082012_1033_75__consistent.mat'
        load 'nonmatching_matching_Data__BRISK_U-BRISK_KNN_02082012_1033_60__max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %For BRISK0
        %         load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
%         load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'
        
        
%         load 'nonmatching_matching_Data__BRISK_BRISK_KNN_02082012_1033_40__consistent.mat'
        load 'nonmatching_matching_Data__BRISK_BRISK_KNN_02082012_1033_45__max.mat'

        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For BRISK4
        %         load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
%         load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'
        
%         load 'nonmatching_matching_Data__BRISK4_BRISK4_KNN_02082012_1033_40__consistent.mat'
        load 'nonmatching_matching_Data__BRISK4_BRISK4_KNN_02082012_1033_47.5__max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
        %         load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
%         load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'
        

%         load 'nonmatching_matching_Data__BRISK_SURF_KNN_02082012_1033_40__consistent.mat'
        load 'nonmatching_matching_Data__BRISK_SURF_KNN_02082012_1033_45__max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
%         load 'nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        load 'nonmatching_matching_Data__SURF1D_Euclidean_02082012_1523_325.125__given.mat'
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        
    elseif datasetType ==2
        %BRISK0 - U-BRISK
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_60__max.mat'
        %load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_75__consistent.mat'
        
                load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_35__consistent.mat'
%         load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_55__max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %BRISK0
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_45__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_40__consistent.mat'
        
                load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_30__consistent.mat'
%         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_46.25__max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %BRISK4
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_47.5__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_40__consistent.mat'
        
                load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_30__consistent.mat'
%         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_51.25__max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_45__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_40__consistent.mat'
        
                load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_30__consistent.mat'
%         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_43.75__max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset2_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        
    elseif datasetType==21
        %BRISK0 - U-BRISK
        load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_35__consistent.mat'
%                 load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_55__max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %BRISK0
        load dataset3_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_30__consistent.mat''
%                 load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_46.25__max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %BRISK4
        load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_30__consistent.mat'
%                 load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_51.25__max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
        load 'dataset3_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_30__consistent.mat'
%                 load 'dataset3_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_43.75__max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset3_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        
        
    end%datasetType
    
elseif matchingMethod==2 %Hamming/Euclidean distance
    
    if datasetType ==1
        %For BRISK0 UBRISK
%                 load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'
%         load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_14072012_1140_75_130_consistent.mat'
        
%         load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_130_consistent.mat'
        load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK
%                 load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'
%         load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_14072012_1104_75_115_consistent.mat'
        
%         load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_02082012_1033_75_115_consistent.mat'
        load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_02082012_1033_77.5_107_max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For BRISK4
%                 load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'
%         load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_14072012_1140_65_130_consistent.mat'
        
%         load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_02082012_1033_65_130_consistent.mat'
        load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_02082012_1033_80_120_max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %SBRISK SURF2D
%                 load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'
%         load 'nonmatching_matching_Data__BRISK_SURF_Hamming_14072012_1140_60_0_consistent.mat'
        

%         load 'nonmatching_matching_Data__BRISK_SURF_Hamming_02082012_1033_60_0_consistent.mat'
        load 'nonmatching_matching_Data__BRISK_SURF_Hamming_02082012_1033_65_0_max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF
        %load 'nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        load 'nonmatching_matching_Data__SURF1D_Euclidean_02082012_1523_325.125__given.mat'
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
    elseif datasetType == 2
        %Hamming/Euclidean distance on dataset 2
        %BRISK0 - U-BRISK
                load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_130_consistent.mat'
%         load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_121_max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %BRISK0
                load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_75_115_consistent.mat'
%         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_77.5_107_max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %BRISK4
                load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_65_130_consistent.mat'
%         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_80_120_max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
                load 'dataset2_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_60_0_consistent.mat'
%         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_65_0_max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset2_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
    elseif datasetType ==21
        %BRISK0 - U-BRISK
                load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_130_consistent.mat'
%         load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_121_max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %BRISK0
                load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_75_115_consistent.mat'
%         load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_77.5_107_max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %BRISK4
                load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_65_130_consistent.mat'
%         load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_80_120_max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
                load 'dataset3_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_60_0_consistent.mat'
%         load 'dataset3_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_65_0_max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset3_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
    elseif datasetType == 3 %For varying illumination
        
        %All these methods use BRISK0 - UBRISK
        %1. Original Dataset Left light off
        %load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%         load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2038_75_121_max.mat'
        
% load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_130_consistent.mat'
        
load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
[fpLeft, tpLeft, left_statsMatrix, left_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %2. Original Dataset Right light off
        %load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%         load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2038_75_121_max.mat'
%         load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_130_consistent.mat'
        
load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
[fpRight, tpRight, right_statsMatrix, right_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %3. Original dataset both lights off
        %load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
%         load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2038_75_121_max.mat'
%         load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_130_consistent.mat'
        
load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
[fpBoth, tpBoth, both_statsMatrix, both_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
    elseif datasetType==4 %For comparing pics with the camera
        %For BRISK0 UBRISK
%                 load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'
%         load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_14072012_1140_75_130_consistent.mat'
        
load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_02082012_1033_75_121_max.mat'
[fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01, 1, 1);
        
        %Compare the Nao pics with pics taken from a Nikon camera
        %load 'BRISK_U-BRISK_Hamming_25072012_2038_75_121_max.mat'
%         load 'camera_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2232_75_121_max.mat'
        
load 'camera_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_03082012_1033_75_121_max.mat'
[fpCamera, tpCamera, camera_statsMatrix, camera_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
    elseif datasetType==5
        %BRISK0 - U-BRISK
        %         load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_130_consistent.mat'
        load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_121_max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,21, 1);
        
        
        %Comparing Nao pics with large hall
        load 'camera_large_hall_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2232_75_121_max.mat'
        [fpHall, tpHall, hall_statsMatrix, hall_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
    elseif datasetType == 6
        %Comparing Nao Pics with Google Street View
        load 'streetview_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_25072012_2232_75_121_max.mat'
        [fpStreet, tpStreet, street_statsMatrix, street_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
    end%End dataset type
    
end %End KNN neighbors

if datasetType ==1 || datasetType ==2 || datasetType == 21
    
    if plotROC
    %Plot the ROC Curve
    plot(fpSBRISK,tpSBRISK, 'ro-', fpBRISK4,tpBRISK4, 'bs-', fpSBRISKSURF2D,tpSBRISKSURF2D, 'g-', fpUBRISK, tpUBRISK, 'k*-', fpSURF1D, tpSURF1D, 'm-')
    hold on
    %Plot the random curve
    xrand = [0:0.01:1];
    yrand = [0:0.01:1];
    plot(xrand, yrand, 'r--');
    xlabel('False Positive rate');
    ylabel('True positive rate');
    title('ROC Curve');
    hleg1 = legend('SBRISK','BRISK4', 'SBRISK-SURF2D', 'SBRISK-UBRISK', 'SURF1D');
    set(hleg1,'Location','SouthEast')
    end
    
    overallStatsMatrix = [sb_statsMatrix;
        b4_statsMatrix;
        sb_2d_statsMatrix;
        ub_statsMatrix;
        s1d_statsMatrix];
    
    [sb_thresh] = findThresholds(sb_thresholdsMatrix);
    [b4_thresh] = findThresholds(b4_thresholdsMatrix);
    [sb_2d_thresh] = findThresholds(sb_2d_thresholdsMatrix);
    [ub_thresh] = findThresholds(ub_thresholdsMatrix);
    [s1d_thresh] = findThresholds(s1d_thresholdsMatrix);
    
    %Create the thresholds matrix
    %[threshold TP]
    thresholdsMatrix = Roundoff([sb_thresh;
        b4_thresh;
        sb_2d_thresh;
        ub_thresh;
        s1d_thresh],3)
    
elseif datasetType==3
    %Plot the ROC Curve
    plot(fpLeft,tpLeft, 'ro-', fpRight,tpRight, 'bs-', fpBoth,tpBoth, 'g-')
    hold on
    %Plot the random curve
    xrand = [0:0.01:1];
    yrand = [0:0.01:1];
    plot(xrand, yrand, 'r--');
    xlabel('False Positive rate');
    ylabel('True positive rate');
    title('ROC Curve');
    hleg1 = legend('Left Light off','Right Light and Main Light off', 'Left and Right Lights off');
    set(hleg1,'Location','SouthEast')
    
    overallStatsMatrix = [left_statsMatrix;
        right_statsMatrix;
        both_statsMatrix];
    
    [left_thresh] = findThresholds(left_thresholdsMatrix);
    [right_thresh] = findThresholds(right_thresholdsMatrix);
    [both_thresh] = findThresholds(both_thresholdsMatrix);
    
    thresholdsMatrix = Roundoff([left_thresh;
                        right_thresh;
                        both_thresh],3)
    
elseif datasetType==4
    %Plot the ROC Curve
    plot(fpCamera,tpCamera,'bs-', fpUBRISK, tpUBRISK, 'ro-');
    hold on
    %Plot the random curve
    xrand = [0:0.01:1];
    yrand = [0:0.01:1];
    plot(xrand, yrand, 'r--');
    xlabel('False Positive rate');
    ylabel('True positive rate');
    title('ROC Curve for Robocup dataset');
    hleg1 = legend('Nikon Camera', 'Nao Images');
    set(hleg1,'Location','SouthEast')
    
    overallStatsMatrix = [camera_statsMatrix;
        ub_statsMatrix]
    
    [ub_thresh] = findThresholds(ub_thresholdsMatrix);
    [camera_thresh] = findThresholds(camera_thresholdsMatrix);
    
    thresholdsMatrix = Roundoff([ub_thresh;
                        camera_thresh],3)
    
elseif datasetType==5
    %Plot the ROC Curve
    plot(fpHall,tpHall, 'bs-', fpUBRISK, tpUBRISK, 'ro-');
    hold on
    %Plot the random curve
    xrand = [0:0.01:1];
    yrand = [0:0.01:1];
    plot(xrand, yrand, 'r--');
    xlabel('False Positive rate');
    ylabel('True positive rate');
    title('ROC Curve for Large Hall Dataset');
    hleg1 = legend('Nikon Camera', 'Nao Images');
    set(hleg1,'Location','SouthEast')
    
    overallStatsMatrix = [hall_statsMatrix;
                          ub_statsMatrix]
                      
    [ub_thresh] = findThresholds(ub_thresholdsMatrix);
    [hall_thresh] = findThresholds(hall_thresholdsMatrix);
    
    thresholdsMatrix = Roundoff([ub_thresh;
                        hall_thresh],3)
    
elseif datasetType==6
    %Plot the ROC Curve
    plot(fpStreet,tpStreet, 'ro-');
    hold on
    %Plot the random curve
    xrand = [0:0.01:1];
    yrand = [0:0.01:1];
    plot(xrand, yrand, 'r--');
    xlabel('False Positive rate');
    ylabel('True positive rate');
    title('ROC Curve for Google Street View dataset');
    hleg1 = legend('Camera Comparison');
    set(hleg1,'Location','SouthEast')
    
    overallStatsMatrix = [street_statsMatrix]
    
    [street_thresh] = findThresholds(street_thresholdsMatrix);
    
    thresholdsMatrix = Roundoff([street_thresh], 3)
    
end

overallStatsMatrix = Roundoff(overallStatsMatrix,3)