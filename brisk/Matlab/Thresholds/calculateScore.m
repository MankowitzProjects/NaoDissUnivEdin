function [dataset1] = calculateScore(dataset1, maxTime, maxNVM)
%The weights
global k1;
global k2;

%Now to work out the average for each datapoint
%nonZeroRows = strmatch([0], dataset1(:,9));
dataset1(:,19) = 0;
% %Create a column for the num of invalid matches
% dataset1(:,19) =  dataset1(:,10) - dataset1(:,11);
% 
% %Total number of invalid matches
% numInvalidMatches=  dataset1(:,19);

%dataset1(:,17) = (dataset1(:,9) - dataset1(:,16)).*...

%Calculate the score for each datapoint
 time = dataset1(:,18);
 numValidMatches = dataset1(:,11);
 totalMatches = dataset1(:,10);
 nonZeroNum = (totalMatches>0);
%Score calculation 1
% dataset1(:,18) = nonZeroNum.*(1./(time.*(numInvalidMatches + 0.01.*ones(size(dataset1(:,1))))));


%Create the time function normalised between [0.031,1]
f_t = abs(log10(0.9*time./(maxTime) + 0.1));
dataset1(:,20) = f_t;
%Create the valid matches function normalised between [0,1]
epsilon  = 0.1;
g_NVM = numValidMatches./(totalMatches + epsilon);
dataset1(:,21) = g_NVM;
%New scoring criteria
dataset1(:,19) = (nonZeroNum.*(k1.*f_t + k2.*g_NVM))./(k1 + k2);



