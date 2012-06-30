function [dataset1] = calculateScore(dataset1)
%Now to work out the average for each datapoint
%nonZeroRows = strmatch([0], dataset1(:,9));
dataset1(:,18) = 0;
%Create a column for the num of invalid matches
dataset1(:,19) =  dataset1(:,10) - dataset1(:,11);

%Total number of invalid matches
numInvalidMatches=  dataset1(:,19);

%dataset1(:,17) = (dataset1(:,9) - dataset1(:,16)).*...

%Calculate the score for each datapoint
time = dataset1(:,17);
numValidMatches = dataset1(:,11);
totalMatches = dataset1(:,10);
nonZeroNum = (totalMatches>0);
dataset1(:,18) = nonZeroNum.*(1./(time.*(numInvalidMatches + 0.01.*ones(size(dataset1(:,1))))));

%New scoring criteria
%Create the functions normalised between [0,1]
% g_t = time./1000;
% h_NIM = numInvalidMatches./(totalMatches + ones(size(totalMatches)));
% %Initialise the weights
% k2 = totalMatches;
% k1 = floor(totalMatches./4) + 1;
% 
% dataset1(:,17) = (nonZeroNum.*(k1.*g_t + k2.*h_NIM))/(k1 + k2);



