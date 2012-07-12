function [knnValidMatches, knnInvalidMatches] = getKnnRatios(matches, knnInvalid)


%Find the number of images being compared
numImages1 = max(matches(:,3));
numImages2 = max(matches(:,4));

%Store the KNN distances
knnValidMatches = [];
knnInvalidMatches = [];
counter = 1;
stepSize = 2;
ii=1;
while(ii<size(matches(:,19),1))
   neighbor0 = matches(ii,19);
   neighbor1 = matches(ii + 1, 19);
   dist0 = matches(ii,20);
   dist1 = matches(ii+1,20);
   
   if (neighbor0 ~=neighbor1)
       knnValidMatches(counter, 1) = dist0;
       knnValidMatches(counter,2) = dist1;
       ii = ii+2;
       counter = counter+1;
   else
       ii=ii+1;
   end
    
end

if nargin>1
%Get all the invalid matches
jj=1;
counter = 1;

while(jj<size(knnInvalid(:,17),1))
   neighbor0 = knnInvalid(jj,17);
   neighbor1 = knnInvalid(jj + 1, 17);
   dist0 = knnInvalid(jj,18);
   dist1 = knnInvalid(jj+1,18);
   knnInvalidMatches(counter, 1) = dist0;
   knnInvalidMatches(counter,2) = dist1; 
   
   counter = counter +1;
   jj = jj + 2;
end
knnInvalidMatches(:, 3) = knnInvalidMatches(:, 1)./knnInvalidMatches(:, 2);
end
%Create the ratios
knnValidMatches(:, 3) = knnValidMatches(:, 1)./knnValidMatches(:, 2);

