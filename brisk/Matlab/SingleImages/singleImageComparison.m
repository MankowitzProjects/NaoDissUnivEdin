%Single image comparison function
clear all
clc
%Load all the comparison files into matlab
load 'singleImages_SBRISK_SBRISK_07052012_1812_46.mat'

%Compute Matching Statistics

%The matching vector
matchVector = [];

%Find the number of images being compared
numImages1 = max(matches(:,3));
numImages2 = max(matches(:,4));

% numDirectories = max(matches(:,1));
% for kk=1:numDirectories
%     for ii=1:numImages1
%         for jj=1:ii-1
%             %Determine the number of matches for a pair of images in a
%             %specific directory
%             imagePairIndices = strmatch([kk, ii, jj], matches(:,[1, 3:4]));
%             matchesData = matches(imagePairIndices, :);
%             
%             %Find the maximum query index
%             maxIndex = max(matchesData(:,5));
%             
%             %Now determine if two matches were found
%             for ll=1:maxIndex
%                 
%                 %Calculate the number of matches
%                 numMatches = sum(strmatch([ll], matchesData(:,5))>0);
%       
%                 
%                 if (numMatches ==2)
%                     [rows, cols] = find (matchesData(:,5) == ll);
%                     num = matchesData(rows(1,1), 20)
%                     den = matchesData(rows(2,1), 20)
%                     ll
%                     %add this condition to prevent non negative values
%                     if(matchesData(rows(2,1)>0))
%                     ratio = (num/den)
%                     
%                     %ratio = matchesData(:,
%                     matchVector = [matchVector ratio];
%                     end
%                 end
%                 
%             end
%             
%             %numMatches = sum(strmatch([ii, jj], matches(:,3))>0)
%         end
%     end
% end


%Now to calculate the average size, response etc of invalid matches

%Find the average angle, size and response of each invalid keypoint
kp1_angle = angle(:,5);
kp1_angle_std = std(angle(:,5));

kp1_size = angle(:,6);
kp1_size_std = std(angle(:,6));

kp1_response = angle(:,8);
kp1_response_std = std(angle(:,8));

kp2_angle = angle(:,11);
kp2_angle_std = std(angle(:,11));

kp2_size = angle(:,12);
kp2_size_std = std(angle(:,12));

kp2_response = angle(:,14);
kp2_response_std = std(angle(:,14));

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

%Keypoints
%-------------------------------------------------
%Find the average response

%Find the average size

%Find the average octave

%Find the average angle


%Matches
%-------------------------------------------------
%Find the average difference in angle between matches
averageAngleDiff = mean((matches(:,13) - matches(:,7)))
%Find the average difference in size between matches
averageSizeDiff = mean((matches(:,14) - matches(:,8)))
%Find the average difference in octaves between matches
averageOctaveDiff = mean((matches(:,15) - matches(:,9)))
%Find the average difference in response between matches
averageResponseDiff = mean((matches(:,16) - matches(:,10)))
%Find the average distance between matches
averageDistance = mean(matches(:,18))



