%Generate the ROC Curves
clear all
clc
knn =1;

if knn==1
%For SBRISK
%load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070321012_2310.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_0220_30.mat'%mScore
%load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_4625_mScoreFixed.mat'
load 'nonmatching_matching_Data__SBRISK__SBRISK_KNN_070521012_2057_30_mScoreFixed_consistentThreshold.mat'

[fpSBRISK, tpSBRISK] = createROCCurve(data, 0.01);


%For BRISK4
%load 'nonmatching_matching_Data__BRISK__BRISK_KNN_070421012_1341_51.mat'
%load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2125_5125_mScoreFixed.mat'
load 'nonmatching_matching_Data__BRISK4__BRISK4_KNN_070521012_2215_30_mScoreFixed_consistentThreshold.mat'

[fpBRISK4, tpBRISK4] = createROCCurve(data, 0.01);

%For SBRISK SURF2D
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_0220_45.mat'
%load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_45_mScoreFixed.mat'
load 'nonmatching_matching_Data__SBRISK__SURF2D_KNN_070521012_2239_30_mScoreFixed_consistentThreshold.mat'

[fpSBRISKSURF2D, tpSBRISKSURF2D] = createROCCurve(data, 1);


else

%For SBRISK
%load 'nonmatching_matching_Data__BRISK__BRISK_Hamming_070421012_1222_98_78.mat'
%load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070421012_1922.mat'
%%Latest
load 'nonmatching_matching_Data__SBRISK__SBRISK_Hamming_070521012_2318_78_110_mScoreFixed.mat'

[fpSBRISK, tpSBRISK] = createROCCurve(data, 0.01);

%For BRISK4
%load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070421012_1341.mat'
%load 'g_matching_Data__BRISK4__BRISK4_Hamming_070421012_1922_88.mat'%Latest
load 'nonmatching_matching_Data__BRISK4__BRISK4_Hamming_070521012_2318_85_12125_mScoreFixed.mat'

[fpBRISK4, tpBRISK4] = createROCCurve(data, 0.01);
%SBRISK SURF2D
%load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_0250_33.mat'
load 'nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_2318_65_028_mScoreFixed.mat'
[fpSBRISKSURF2D, tpSBRISKSURF2D] = createROCCurve(data, 1);

end

%Plot the ROC Curve
plot(fpSBRISK,tpSBRISK, 'ro-', fpBRISK4,tpBRISK4, 'bs-', fpSBRISKSURF2D,tpSBRISKSURF2D, 'gd-')
hold on
%Plot the random curve
xrand = [0:0.01:1];
yrand = [0:0.01:1];
plot(xrand, yrand, 'r--');
xlabel('False Positive rate');
ylabel('True positive rate');
title('ROC Curve using KNN Consistent');
hleg1 = legend('SBRISK','BRISK4', 'SBRISK-SURF2D');
set(hleg1,'Location','SouthEast')


