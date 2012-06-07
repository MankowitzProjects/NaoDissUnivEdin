%Read in all the data using different thresholds

% files = dir('../data/*.txt');
% 
% for i=1:length(files)
% eval(['load ' files(i).name ' -ascii']);
% end

load 'changingThreshold2.mat'

%Compare the number of duplicate and non-duplicate matches
subplot(2,2,1)
plot(matchingData2(:,1),matchingData2(:,9))
hold on 
plot(matchingData2(:,1),matchingData2(:,8),'r')

%Compute a histogram of the non-duplicate matches
subplot(2,2,2)
hist(matchingData2(:,9),50);

%Plot the number of keypoints in both images
subplot(2,2,3)
plot(matchingData2(:,1),matchingData2(:,5));
hold on
plot(matchingData2(:,1),matchingData2(:,4),'r');

%Plot the matching scores
subplot(2,2,4)
plot(matchingData2(:,1),matchingData2(:,6));
hold on
plot(matchingData2(:,1),matchingData2(:,7),'r');
hold on
%Determine the intersection of the curves
[x0, y0, iout, jout] = intersections(matchingData2(:,1),matchingData2(:,6),matchingData2(:,1),matchingData2(:,7));
line

