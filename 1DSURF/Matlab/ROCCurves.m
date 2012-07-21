
clear all
clc
%Load rUNSWifts ROC Curve on their images
load 'rocCurve.mat'

%Declare a filenames matrix
%fileNamesMatrix = zeros
datasetA = 1;


%The number of files
if datasetA==1
numFiles = 108;
else
numFiles = 50;
end
%Set the left and right horizon pixel rows
left = 120;
right = 120;

leftMatrix = repmat(left,numFiles,1)
%leftMatrix = num2str(leftMatrix)

rightMatrix = repmat(right,numFiles,1)
%rightMatrix = num2str(rightMatrix)
%Set the angles (corresponding to directions)
if datasetA == 1
directions(1:26,:) = 10;
directions(27:54,:) = 100;
directions(55:85,:) = 190;
directions(86:108,:) = 280;
else
directions(1:25,:) = 10;
directions(26:50,:) = 100;   
end

%Declare the file names
fileNum = [1:numFiles]
extension = '.jpg'

%Create the final file extension
filename = num2str(fileNum(:))
fileNamesMatrix = strcat(filename, extension)

dataMatrix = [leftMatrix, rightMatrix, directions];

%print to file
if datasetA==1
fid = fopen('dataFile.txt','w');
else
    fid = fopen('dataFile2.txt','w');
end
for ii = 1:numFiles
fprintf(fid, '%s \t %3.1d \t %3.1d \t %3.1d \n',fileNamesMatrix(ii,:), dataMatrix(ii,:))

end

fclose(fid);





