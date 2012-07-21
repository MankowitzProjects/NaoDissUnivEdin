%Generate the ROC Curves
clear all
clc
knn =1;

datasetA = 1;

if knn==1

%For SBRISK

    
%SBRISK UBRISK
if datasetA
load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_35.mat'
%load 'nonmatching_matching_Data__SBRISK__UBRISK_KNN_100521012_1736_55_max.mat'
else
% load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_60__max.mat'   
load 'dataset2_nonmatching_matching_Data__BRISK_U-BRISK_KNN_14072012_1639_75__consistent.mat'
end
[fpUBRISK, tpUBRISK, ub_statsMatrix] = createROCCurve(data, 0.01,datasetA);

%For SBRISK
if datasetA
%load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070321012_2310.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_0220_30.mat'%mScore
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_4625_mScoreFixed.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_30_mScoreFixed_consistentThreshold.mat'

load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_30.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_100521012_1712_4625_max.mat'
else
% load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_45__max.mat'    
load 'dataset2_nonmatching_matching_Data__BRISK_BRISK_KNN_14072012_1639_40__consistent.mat'
end

[fpSBRISK, tpSBRISK, sb_statsMatrix] = createROCCurve(data, 0.01, datasetA);


%For BRISK4
if datasetA
%load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070421012_1341_51.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2125_5125_mScoreFixed.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2215_30_mScoreFixed_consistentThreshold.mat'

load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_30.mat'
% load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_100521012_1736_51.25_max.mat'
else
% load 'dataset2_dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_47.5__max.mat'    
load 'dataset2_nonmatching_matching_Data__BRISK4_BRISK4_KNN_14072012_1639_40__consistent.mat'
end

[fpBRISK4, tpBRISK4, b4_statsMatrix] = createROCCurve(data, 0.01, datasetA);

%For SBRISK SURF2D
if datasetA
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_0220_45.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_45_mScoreFixed.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_30_mScoreFixed_consistentThreshold.mat'

load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_30.mat'
% load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_100521012_1829_4375_max.mat'
else
% load 'dataset2_dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_45__max.mat'   
load 'dataset2_nonmatching_matching_Data__BRISK_SURF_KNN_14072012_1639_40__consistent.mat'
end


[fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix] = createROCCurve(data, 0.01, datasetA);

%1D SURF
if datasetA
load 'nonmatching_matching_SURF1D_19072012_1151.mat' 
else
load 'dataset2_nonmatching_matching_SURF1D_19072012_1151.mat'    
end

[fpSURF1D, tpSURF1D, s1d_statsMatrix] = createROCCurve(data, 0.01, datasetA);

else
    
%For BRISK0 UBRISK
load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_12072012_1009_75_121_max.mat'
% load 'nonmatching_matching_Data__BRISK_U-BRISK_Hamming_14072012_1140_75_130_consistent.mat'

 [fpUBRISK, tpUBRISK, ub_statsMatrix] = createROCCurve(data, 0.01);

%For SBRISK
load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_12072012_1009_77.5_107_max.mat'
% load 'nonmatching_matching_Data__BRISK_BRISK_Hamming_14072012_1104_75_115_consistent.mat'

[fpSBRISK, tpSBRISK, sb_statsMatrix] = createROCCurve(data, 0.01);

%For BRISK4
load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_120521012_0055_80_120_max.mat'
% load 'nonmatching_matching_Data__BRISK4_BRISK4_Hamming_14072012_1140_65_130_consistent.mat'

[fpBRISK4, tpBRISK4, b4_statsMatrix] = createROCCurve(data, 0.01);

%SBRISK SURF2D
load 'nonmatching_matching_Data__BRISK_SURF_Hamming_12072012_1009_65_0_max.mat'
% load 'nonmatching_matching_Data__BRISK_SURF_Hamming_14072012_1140_60_0_consistent.mat'

[fpSBRISKSURF2D, tpSBRISKSURF2D, sb_2d_statsMatrix] = createROCCurve(data, 0.01);

%1D SURF
load 'dataset2_nonmatching_matching_SURF1D_19072012_1151.mat'    

[fpSURF1D, tpSURF1D, s1d_statsMatrix] = createROCCurve(data, 0.01, datasetA);



end

%Plot the ROC Curve
plot(fpSBRISK,tpSBRISK, 'ro-', fpBRISK4,tpBRISK4, 'bs-', fpSBRISKSURF2D,tpSBRISKSURF2D, 'g-', fpUBRISK, tpUBRISK, 'k*-', fpSURF1D, tpSURF1D, 'm-')
hold on
%Plot the random curve
xrand = [0:0.01:1];
yrand = [0:0.01:1];
plot(xrand, yrand, 'r--');
xlabel('False Positive rate');
ylabel('True positive rate');
title('ROC Curve using KNN Consistent');
hleg1 = legend('SBRISK','BRISK4', 'SBRISK-SURF2D', 'SBRISK-UBRISK', 'SURF1D');
set(hleg1,'Location','SouthEast')

overallStatsMatrix = [sb_statsMatrix;
                      b4_statsMatrix;
                      sb_2d_statsMatrix;
                      ub_statsMatrix;
                      s1d_statsMatrix];
overallStatsMatrix = Roundoff(overallStatsMatrix,3)