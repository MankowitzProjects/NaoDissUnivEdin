%Single image comparison function

load 'DA_SI_BRISK_100_BRISK_KNN_Im2_Im12.mat'
%Keypoints
%-------------------------------------------------
%Find the average response

%Find the average size

%Find the average octave

%Find the average angle


%Matches
%-------------------------------------------------
%Find the average difference in angle between matches
averageAngleDiff = mean(abs(matches(:,13) - matches(:,7)))
%Find the average difference in size between matches
averageSizeDiff = mean(abs(matches(:,14) - matches(:,8)))
%Find the average difference in octaves between matches
averageOctaveDiff = mean(abs(matches(:,15) - matches(:,9)))
%Find the average difference in response between matches
averageResponseDiff = mean(abs(matches(:,16) - matches(:,10)))
%Find the average distance between matches
averageDistance = mean(matches(:,18))



