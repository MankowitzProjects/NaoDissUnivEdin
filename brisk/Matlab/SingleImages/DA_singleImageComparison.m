%Single image comparison function
clear all
clc
%Load all the comparison files into matlab
%load 'singleImages_SBRISK_SBRISK_07052012_1812_46.mat'

%Compute Matching Statistics

%BRISK4
%load 'singleImageData_BRISK4_BRISK4_KNN_11072012_2300_keypointStats.mat'

%SBRISK
%load 'singleImageData_BRISK_BRISK_KNN_11072012_2300_keypointStats.mat'

%SBRISK SURF2D
load 'singleImageData_BRISK_SURF2D_KNN_11072012_2300_keypointStats.mat'

%SBRISK UBRISK
%load 'singleImageData_BRISK_UBRISK_KNN_11072012_2300_keypointStats.mat'


%The matching vector
matchVector = [];

%Find the number of images being compared
numImages1 = max(matches(:,3));
numImages2 = max(matches(:,4));

%Get the knn ratios
[knnValidMatches, knnInvalidMatches] = getKnnRatios(matches, knnInvalid)

%Find the average angle, size and response of each invalid keypoint
kp1_angle_knn_invalid = knnInvalid(:,7);
kp1_angle_std_knn_invalid = std(knnInvalid(:,7));

kp1_size_knn_invalid = knnInvalid(:,8);
kp1_size_std_knn_invalid = std(knnInvalid(:,8));

kp1_response_knn_invalid = knnInvalid(:,10);
kp1_response_std_knn_invalid = std(knnInvalid(:,10));

kp2_angle_knn_invalid = knnInvalid(:,13);
kp2_angle_std_knn_invalid = std(knnInvalid(:,13));

kp2_size_knn_invalid = knnInvalid(:,14);
kp2_size_std_knn_invalid = std(knnInvalid(:,14));

kp2_response_knn_invalid = knnInvalid(:,16);
kp2_response_std_knn_invalid = std(knnInvalid(:,16));

%Find the difference between the two plots
kp12_angle_mean_knn_invalid =  abs(mean(kp1_angle_knn_invalid - kp2_angle_knn_invalid));
kp12_angle_std_knn_invalid = abs(kp1_angle_std_knn_invalid-kp2_angle_std_knn_invalid);
kp12_size_mean_knn_invalid = abs(mean(kp1_size_knn_invalid - kp2_size_knn_invalid));
kp12_size_std_knn_invalid = abs(kp1_size_std_knn_invalid - kp2_size_std_knn_invalid);
kp12_response_mean_knn_invalid = abs(mean(kp1_response_knn_invalid - kp2_response_knn_invalid));
kp12_response_std_knn_invalid = abs(kp1_response_std_knn_invalid - kp2_response_std_knn_invalid);


% plot(knnValidMatches(:,3), 'g')
% hold on
% plot(knnInvalidMatches(:,3), 'r')


%Now to calculate the average size, response etc of invalid matches

%Find the average angle, size and response of each invalid keypoint
kp1_angle = angle(:,7);
kp1_angle_std = std(angle(:,7));

kp1_size = angle(:,8);
kp1_size_std = std(angle(:,8));

kp1_response = angle(:,10);
kp1_response_std = std(angle(:,10));

kp2_angle = angle(:,13);
kp2_angle_std = std(angle(:,13));

kp2_size = angle(:,14);
kp2_size_std = std(angle(:,14));

kp2_response = angle(:,16);
kp2_response_std = std(angle(:,16));

%Find the difference between the two plots
kp12_angle_mean =  abs(mean(kp1_angle - kp2_angle));
kp12_angle_std = abs(kp1_angle_std-kp2_angle_std);
kp12_size_mean = abs(mean(kp1_size - kp2_size));
kp12_size_std = abs(kp1_size_std - kp2_size_std);
kp12_response_mean = abs(mean(kp1_response - kp2_response));
kp12_response_std = abs(kp1_response_std - kp2_response_std);

%Find the average angle, size and response of each valid keypoint match
kp1_angle_valid = matches(:,9);
kp1_angle_std_valid = std(matches(:,9));

kp1_size_valid = matches(:,10);
kp1_size_std_valid = std(matches(:,10));

kp1_response_valid = matches(:,12);
kp1_response_std_valid = std(matches(:,12));

kp2_angle_valid = matches(:,15);
kp2_angle_std_valid = std(matches(:,15));

kp2_size_valid = matches(:,16);
kp2_size_std_valid = std(matches(:,16));

kp2_response_valid = matches(:,18);
kp2_response_std_valid = std(matches(:,18));

%Find the difference between the two plots
kp12_angle_mean_valid =  abs(mean(kp1_angle_valid - kp2_angle_valid));
kp12_angle_std_valid = (kp1_angle_std_valid -kp2_angle_std_valid);
kp12_size_mean_valid = (mean(kp1_size_valid - kp2_size_valid));
kp12_size_std_valid = (kp1_size_std_valid - kp2_size_std_valid);
kp12_response_mean_valid = (mean(kp1_response_valid - kp2_response_valid));
kp12_response_std_valid = (kp1_response_std_valid - kp2_response_std_valid);

%Relative conclusions:
%The angle is difference is far smaller for valid matches
%The response difference is smaller for valid matches
%The std deviation for both quatities are smaller for both matches


keypointMatrix = Roundoff([kp12_angle_mean, kp12_angle_mean_knn_invalid, kp12_angle_mean_valid;
                  kp12_angle_std, kp12_angle_std_knn_invalid, kp12_angle_std_valid;
                  kp12_response_mean, kp12_response_mean_knn_invalid, kp12_response_mean_valid;
                  kp12_response_std, kp12_response_std_knn_invalid, kp12_response_std_valid;
                  kp12_size_mean, kp12_size_mean_knn_invalid, kp12_size_mean_valid;
                  kp12_size_std, kp12_size_std_knn_invalid, kp12_size_std_valid]',2)
                  
                 


%Keypoints
%-------------------------------------------------
%Find the average response

%Find the average size

%Find the average octave

%Find the average angle


%Matches
%-------------------------------------------------
%Find the average difference in angle between matches
% averageAngleDiff = mean((matches(:,13) - matches(:,7)))
% %Find the average difference in size between matches
% averageSizeDiff = mean((matches(:,14) - matches(:,8)))
% %Find the average difference in octaves between matches
% averageOctaveDiff = mean((matches(:,15) - matches(:,9)))
% %Find the average difference in response between matches
% averageResponseDiff = mean((matches(:,16) - matches(:,10)))
% %Find the average distance between matches
% averageDistance = mean(matches(:,18))



