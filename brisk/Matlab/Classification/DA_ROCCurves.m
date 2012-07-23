%Generate the ROC Curves
clear all
clc
addpath('../Lighting')
addpath('../Data3');
addpath('../Data2');
%Choose the dataset type.
% 1 - means using structure of 1-4
% 2 - means using structure 1-2
% 3 - varying lighting
datasetType = 21;

%Choose if we are comparing knn or hamming/euclidean distance
% 1 - KNN
% 2 - Hamming/Euclidean Distance
knn =2;

if knn==1
    
    if datasetType == 1
        %BRISK0 UBRISK
        %         load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
        load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'
        
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %For BRISK0
        %         load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
        load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'
        
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For BRISK4
        %         load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
        load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'
        
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
        %         load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
        load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'
        
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        
    elseif datasetType ==2
        %BRISK0 - U-BRISK
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_60__max.mat'
        %load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_75__consistent.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_35__consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_55__max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %BRISK0
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_45__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_40__consistent.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_30__consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_46.25__max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %BRISK4
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_47.5__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_40__consistent.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_30__consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_51.25__max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
        % load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_45__max.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_40__consistent.mat'
        %         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_30__consistent.mat'
        load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_43.75__max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset2_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        
    elseif datasetType==21
        %BRISK0 - U-BRISK
                load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_35__consistent.mat'
%         load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_KNN_22072012_2151_55__max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %BRISK0
                load dataset3_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_30__consistent.mat''
%         load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_KNN_22072012_2151_46.25__max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %BRISK4
                load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_30__consistent.mat'
%         load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_KNN_22072012_2151_51.25__max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
                load 'dataset3_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_30__consistent.mat'
%         load 'dataset3_nonmatching_matching_Data__BRISK_SURF_KNN_22072012_2151_43.75__max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset3_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        
        
    end%datasetType
    
elseif knn==2 %Hamming/Euclidean distance
    
    if datasetType ==1
        %For BRISK0 UBRISK
        %         load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'
        load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_14072012_1140_75_130_consistent.mat'
        
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK
        %         load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'
        load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_14072012_1104_75_115_consistent.mat'
        
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For BRISK4
        %         load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'
        load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_14072012_1140_65_130_consistent.mat'
        
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %SBRISK SURF2D
        %         load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'
        load 'nonmatching_matching_Data__BRISK_SURF_Hamming_14072012_1140_60_0_consistent.mat'
        
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF
        load 'nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
    elseif datasetType == 2
        %Hamming/Euclidean distance on dataset 2
        %BRISK0 - U-BRISK
%         load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_130_consistent.mat'
                load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_121_max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %BRISK0
%         load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_75_115_consistent.mat'
                load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_77.5_107_max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %BRISK4
%         load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_65_130_consistent.mat'
                load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_80_120_max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
%         load 'dataset2_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_60_0_consistent.mat'
                load 'dataset2_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_65_0_max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset2_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
    elseif datasetType ==21
        %BRISK0 - U-BRISK
%         load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_130_consistent.mat'
                load 'dataset3_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_22072012_2151_75_121_max.mat'
        [fpUBRISK, tpUBRISK, ub_statsMatrix, ub_thresholdsMatrix] = createROCCurve(data, 0.01,datasetType, 1);
        
        %BRISK0
%         load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_75_115_consistent.mat'
                load 'dataset3_nonmatching_matching_Data__BRISK_BRISK_Hamming_22072012_2151_77.5_107_max.mat'
        [fpSBRISK, tpSBRISK, sb_statsMatrix, sb_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %BRISK4
%         load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_65_130_consistent.mat'
                load 'dataset3_nonmatching_matching_Data__BRISK4_BRISK4_Hamming_22072012_2151_80_120_max.mat'
        [fpBRISK4, tpBRISK4, b4_statsMatrix, b4_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %For SBRISK SURF2D
%         load 'dataset3_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_60_0_consistent.mat'
                load 'dataset3_nonmatching_matching_Data__BRISK_SURF_Hamming_22072012_2151_65_0_max.mat'
        [fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix, sb_2d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
        %1D SURF - This test does not use KNN and is only used for comparison
        load 'dataset3_nonmatching_matching_Data__SURF1D_Euclidean_22072012_2151_325.125__given.mat'
        [fpSURF1D, tpSURF1D, s1d_statsMatrix, s1d_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 0);
        
    elseif datasetType == 3 %For varying illumination
        
        %All these methods use BRISK0 - UBRISK
        %1. Original Dataset Left light off
        load 'dataLighting_left_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
        [fpLeft, tpLeft, left_statsMatrix, left_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %2. Original Dataset Right light off
        load 'dataLighting_right_light_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
        [fpRight, tpRight, right_statsMatrix, right_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
        %3. Original dataset both lights off
        load 'dataLighting_both_lights_off_nonmatching_matching_Data__BRISK_U-BRISK_Hamming_21072012_1317_75_121_consistent.mat'
        [fpBoth, tpBoth, both_statsMatrix, both_thresholdsMatrix] = createROCCurve(data, 0.01, datasetType, 1);
        
    end %End dataset type
    
end %End KNN neighbors

if datasetType ==1 || datasetType ==2 || datasetType == 21
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
    thresholdsMatrix = [sb_thresh;
        b4_thresh;
        sb_2d_thresh;
        ub_thresh;
        s1d_thresh;]
    
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
    title('ROC Curve using KNN Consistent');
    hleg1 = legend('Left Light off','Right Light and Main Light off', 'Left and Right Lights off');
    set(hleg1,'Location','SouthEast')
    
    overallStatsMatrix = [left_statsMatrix;
        right_statsMatrix;
        both_statsMatrix];
end

overallStatsMatrix = Roundoff(overallStatsMatrix,3)