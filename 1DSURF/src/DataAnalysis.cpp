#include "DataAnalysis.h"

DataAnalysis::DataAnalysis()
{
	//ctor
}

DataAnalysis::~DataAnalysis()
{
	//dtor
}

int DataAnalysis::getNumImagesInDirectory(string *dir, bool terminal)
{
	//Read in the number of images from a directory
	//************************************
	ifstream fin;
	string filepath;
	int num;
	DIR *dp;
	struct dirent *dirp;
	struct stat filestat;
	int jpegCounter = 0;

	string mgMatchLeft;
	string mgMatchRight;
	string ogMatchLeft;
	string ogMatchRight;

	string mgLeftMatchLeft;
	string mgLeftMatchRight;
	string ogLeftMatchLeft;
	string ogLeftMatchRight;

	string mgRightMatchLeft;
	string mgRightMatchRight;
	string ogRightMatchLeft;
	string ogRightMatchRight;

	string mgBothMatchLeft;
	string mgBothMatchRight;
	string ogBothMatchLeft;
	string ogBothMatchRight;

	string dataset2_1;
	string dataset2_2;

	string dataset3_1;
	string dataset3_2;

	//There directories where the files can be found
	//The original dataset (Testing Set)
	mgMatchLeft = "../images/PicsMGValidation/Matching_Pics_Left_Overlapping";
	mgMatchRight = "../images/PicsMGValidation/Matching_Pics_Right_Overlapping";
	ogMatchLeft = "../images/PicsOGValidation/Matching_Pics_Left_Overlapping";
	ogMatchRight = "../images/PicsOGValidation/Matching_Pics_Right_Overlapping";


	//The original dataset with lighting variation - LEFT LIGHT OFF (MAIN LIGHT ON)
	mgLeftMatchLeft = "../images/Pics1MGLeftLightOff/Matching_Pics_Left_Overlapping";
	mgLeftMatchRight = "../images/Pics1MGLeftLightOff/Matching_Pics_Right_Overlapping";
	ogLeftMatchLeft = "../images/Pics1OGLeftLightOff/Matching_Pics_Left_Overlapping";
	ogLeftMatchRight = "../images/Pics1OGLeftLightOff/Matching_Pics_Right_Overlapping";

	//The original dataset with lighting variation - RIGHT LIGHT OFF (MAIN LIGHT OFF)
	mgRightMatchLeft = "../images/Pics2MGRightLightOff/Matching_Pics_Left_Overlapping";
	mgRightMatchRight = "../images/Pics2MGRightLightOff/Matching_Pics_Right_Overlapping";
	ogRightMatchLeft = "../images/Pics2OGRightLightOff/Matching_Pics_Left_Overlapping";
	ogRightMatchRight = "../images/Pics2OGRightLightOff/Matching_Pics_Right_Overlapping";

	//The original dataset with lighting variation - BOTH LIGHTS OFF (MAIN LIGHT ON)
	mgBothMatchLeft = "../images/Pics3MGBothLightsOff/Matching_Pics_Left_Overlapping";
	mgBothMatchRight = "../images/Pics3MGBothLightsOff/Matching_Pics_Right_Overlapping";
	ogBothMatchLeft = "../images/Pics3OGBothLightsOff/Matching_Pics_Left_Overlapping";
	ogBothMatchRight = "../images/Pics3OGBothLightsOff/Matching_Pics_Right_Overlapping";

	//The first additional dataset
	dataset2_1 = "../images/Dataset2_Overlapping_1";
	dataset2_2 = "../images/Dataset2_Overlapping_2";
	//The second additional dataset
	dataset3_1 = "../images/Dataset3_Overlapping_1";
	dataset3_2 = "../images/Dataset3_Overlapping_2";

	//DATASETS****************************
	//ORIGINAL DATASET: 1-4
	//DATASET LEFT LIGHT OFF: 5-8
	//DATASET RIGHT LIGHT OFF: 9-12
	//DATASET BOTH LIGHTS OFF: 13-16

	//ADDITIONAL DATASET 1: 17-18
	//ADDITIONAL DATASET 2: 19-20

	int value = atoi((*dir).c_str());
	switch(value){
	case 1:
		*dir = mgMatchLeft;
		break;
	case 2:
		*dir = mgMatchRight;
		break;
	case 3:
		*dir = ogMatchLeft;
		break;
	case 4:
		*dir = ogMatchRight;
		break;
	case 5:
		*dir = mgLeftMatchLeft;
		break;
	case 6:
		*dir = mgLeftMatchRight;
		break;
	case 7:
		*dir = ogLeftMatchLeft;
		break;
	case 8:
		*dir = ogLeftMatchRight;
		break;
	case 9:
		*dir = mgRightMatchLeft;
		break;
	case 10:
		*dir = mgRightMatchRight;
		break;
	case 11:
		*dir = ogRightMatchLeft;
		break;
	case 12:
		*dir = ogRightMatchRight;
		break;
	case 13:
		*dir = mgBothMatchLeft;
		break;
	case 14:
		*dir = mgBothMatchRight;
		break;
	case 15:
		*dir = ogBothMatchLeft;
		break;
	case 16:
		*dir = ogBothMatchRight;
		break;
	case 17:
		*dir = dataset2_1;
		break;
	case 18:
		*dir = dataset2_2;
		break;
	case 19:
		*dir = dataset3_1;
		break;
	case 20:
		*dir = dataset3_2;
		break;
	default:
		std::cout<<"Nothing found"<<endl;
		break;

	}


	dp = opendir( (*dir).c_str() );
	if (dp == NULL)
	{
		//cout << "Error(" << errno << ") opening " << dir << endl;
		return 0;//errno;
	}

	while ((dirp = readdir( dp )))
	{
		filepath = *dir + "/" + dirp->d_name;

		// If the file is a directory (or is in some way invalid) we'll skip it
		if (stat( filepath.c_str(), &filestat )) continue;
		if (S_ISDIR( filestat.st_mode ))         continue;

		jpegCounter = jpegCounter +1;
	}

	closedir( dp );

	return jpegCounter;

}

void DataAnalysis::help(char** argv){
	std::cout << "This command line tool lets you evaluate different keypoint "
			<< "detectors, descriptors and matchers." << std::endl
			<< "usage:" << std::endl
			<< argv[0] << " <dataset> <2nd> <detector> <descriptor> [descFile1 descFile1]" << std::endl
			<< "    " << "dataset:    Folder containing the images. The images must be of .ppm "<< std::endl
			<< "    " << "            format. They must be named img#.ppm, and there must be "<< std::endl
			<< "    " << "            corresponding homographies named H1to#." << std::endl
			<< "    " << "            You can also use the prefix rot-, then 2nd will be the" << std::endl
			<< "    " << "            rotation in degrees." << std::endl
			<< "    " << "2nd:        Number of the 2nd image (the 1st one is always img1.ppm)"<< std::endl
			<< "    " << "            or the rotation in degrees, if rot- used." << std::endl
			<< "    " << "detector:   Feature detector, e.g. AGAST, or BRISK. You can add the "<< std::endl
			<< "    " << "            threshold, e.g. BRISK80 or SURF2000"<< std::endl
			<< "    " << "descriptor: Feature descriptor, e.g. SURF, BRIEF, BRISK or U-BRISK."<< std::endl
			<< "    " << "[descFile]: Optional: files with descriptors to act as detected points."<< std::endl;
}
