#include "../include/brisk/FeatureExtraction.h"

#define PI 3.14159265
#define FEATURE_DEBUG_MODE 1

FeatureExtraction::FeatureExtraction()
{
    //ctor
}

FeatureExtraction::~FeatureExtraction()
{
    //dtor
}

cv::Ptr<cv::FeatureDetector> FeatureExtraction::getDetector(int argc, char ** argv, cv::Ptr<cv::FeatureDetector> detector, int threshold, int testThreshold, int testFlag){

if(argc==1){
		detector = new cv::BriskFeatureDetector(60,4);
	}
	else{
		if(strncmp("FAST", argv[3], 4 )==0){
			threshold = atoi(argv[3]+4);
			if(threshold==0)
				threshold = 30;
			detector = new cv::FastFeatureDetector(threshold,true);
		}
		else if(strncmp("AGAST", argv[3], 5 )==0){
			threshold = atoi(argv[3]+5);
			if(threshold==0)
				threshold = 30;
			detector = new cv::BriskFeatureDetector(threshold,0);
		}
		else if(strncmp("BRISK", argv[3], 5 )==0){

			if(testFlag==1)
			threshold = atoi(argv[3]+5);
			else
            threshold = testThreshold;

			if(threshold==0)
				threshold = 30;
			detector = new cv::BriskFeatureDetector(threshold,4);
		}
		else if(strncmp("SURF", argv[3], 4 )==0){
			threshold = atoi(argv[3]+4);
			if(threshold==0)
				threshold = 400;
			detector = new cv::SurfFeatureDetector(threshold);
		}
		else if(strncmp("SIFT", argv[3], 4 )==0){
			float thresh = 0.04 / cv::SIFT::CommonParams::DEFAULT_NOCTAVE_LAYERS / 2.0;
			float edgeThreshold=atof(argv[3]+4);
			if(edgeThreshold==0)
				thresh = 10.0;
			detector = new cv::SiftFeatureDetector(thresh,edgeThreshold);
		}
		else{
			detector = cv::FeatureDetector::create( argv[3] );
		}
		if (detector.empty()){
			std::cout << "Detector " << argv[3] << " not recognized. Check spelling!" << std::endl;
			return detector;
		}
	}

	return detector;

}

cv::Ptr<cv::DescriptorExtractor> FeatureExtraction::getExtractor(int argc, char** argv,bool hamming, cv::Ptr<cv::DescriptorExtractor> descriptorExtractor)
{
if(argc==1){
		descriptorExtractor = new cv::BriskDescriptorExtractor();
	}
	else{
		if(std::string(argv[4])=="BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor();
		}
		else if(std::string(argv[4])=="U-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(false);
		}
		else if(std::string(argv[4])=="SU-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(false,false);
		}
		else if(std::string(argv[4])=="S-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(true,false);
		}
		else if(std::string(argv[4])=="BRIEF"){
			descriptorExtractor = new cv::BriefDescriptorExtractor(64);
		}
		else if(std::string(argv[4])=="CALONDER"){
			descriptorExtractor = new cv::CalonderDescriptorExtractor<float>("current.rtc");
			hamming=false;
		}
		else if(std::string(argv[4])=="SURF"){
			descriptorExtractor = new cv::SurfDescriptorExtractor();
			hamming=false;
		}
		else if(std::string(argv[4])=="SIFT"){
			descriptorExtractor = new cv::SiftDescriptorExtractor();
			hamming=false;
		}
		else{
			descriptorExtractor = cv::DescriptorExtractor::create( argv[4] );
		}
		if (descriptorExtractor.empty()){
			hamming=false;
			std::cout << "Descriptor " << argv[4] << " not recognized. Check spelling!" << std::endl;
			return descriptorExtractor;
		}
	}
    return descriptorExtractor;



}

cv::Ptr<cv::FeatureDetector> FeatureExtraction::getDetector(int argc, string feat_detector, cv::Ptr<cv::FeatureDetector> detector, int threshold, int testThreshold, int testFlag){


		if("FAST" == feat_detector){
			if(threshold==0)
				threshold = 30;
			detector = new cv::FastFeatureDetector(threshold,true);
		}
		else if("AGAST"== feat_detector){
			if(threshold==0)
				threshold = 30;
			detector = new cv::BriskFeatureDetector(threshold,0);
		}
		else if("BRISK"== feat_detector){
			cout<<"Reached Here"<<endl;
			if(threshold==0)
				threshold = 30;
			detector = new cv::BriskFeatureDetector(threshold,4);
		}
		else if("SURF"== feat_detector){
			if(threshold==0)
				threshold = 400;
			detector = new cv::SurfFeatureDetector(threshold);
		}
		else if("SIFT"== feat_detector){
			float thresh = 0.04 / cv::SIFT::CommonParams::DEFAULT_NOCTAVE_LAYERS / 2.0;
			float edgeThreshold=threshold;
			if(edgeThreshold==0)
				thresh = 10.0;
			detector = new cv::SiftFeatureDetector(thresh,edgeThreshold);
		}
		else{
			detector = cv::FeatureDetector::create( feat_detector );
		}
		if (detector.empty()){
			std::cout << "Detector " << feat_detector << " not recognized. Check spelling!" << std::endl;
			return detector;
		}

	return detector;

}

cv::Ptr<cv::DescriptorExtractor> FeatureExtraction::getExtractor(int argc, string feat_descriptor,bool hamming, cv::Ptr<cv::DescriptorExtractor> descriptorExtractor)
{

		if(feat_descriptor=="BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor();
		}
		else if(feat_descriptor=="U-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(false);
		}
		else if(feat_descriptor=="SU-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(false,false);
		}
		else if(feat_descriptor=="S-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(true,false);
		}
		else if(feat_descriptor=="BRIEF"){
			descriptorExtractor = new cv::BriefDescriptorExtractor(64);
		}
		else if(feat_descriptor=="CALONDER"){
			descriptorExtractor = new cv::CalonderDescriptorExtractor<float>("current.rtc");
			hamming=false;
		}
		else if(feat_descriptor=="SURF"){
			cout<<"Reached Here SURF"<<endl;
			descriptorExtractor = new cv::SurfDescriptorExtractor();
			hamming=false;
		}
		else if(feat_descriptor=="SIFT"){
			descriptorExtractor = new cv::SiftDescriptorExtractor();
			hamming=false;
		}
		else{
			descriptorExtractor = cv::DescriptorExtractor::create( feat_descriptor );
		}
		if (descriptorExtractor.empty()){
			hamming=false;
			std::cout << "Descriptor " << feat_descriptor << " not recognized. Check spelling!" << std::endl;
			return descriptorExtractor;
		}

    return descriptorExtractor;



}

//Verfify that a match is indeed correct
//Needs the image size and the keypoints
bool FeatureExtraction::verifyMatch(const cv::Mat & image,cv::KeyPoint keypoint1, cv::KeyPoint keypoint2)
{
	
	//Store the keypoint coordinates
	//Image Left
	int x1 = keypoint1.pt.x;//col
	int y1 = keypoint1.pt.y;//row

	
	//Image Right
	int x2 = keypoint2.pt.x;//col
	int y2 = keypoint2.pt.y;//row
	
	
	#if (FEATURE_DEBUG_MODE)
	cout<<"rowR, colR: "<<y1<<", "<<x1<<endl;
	cout<<"rowL, colL: "<<y2<<", "<<x2<<endl;
	#endif
	
	//Store the image col and rows
	int rows = image.rows;
	int cols = image.cols;
	
	#if (FEATURE_DEBUG_MODE)
	cout<<"image rows: "<<rows<<endl;
	cout<<"image cols: "<<cols<<endl;
	#endif
		
	//Find the absolute distance between the two coordinates if the images were placed together
	double xdistance, ydistance;
	xdistance = (cols - x1) + x2;//cols
	ydistance = y2-y1;//rows
	
	#if (FEATURE_DEBUG_MODE)
	cout<<"col distance: "<<xdistance<<endl;
	cout<<"row distance: "<<ydistance<<endl;
	#endif
	
	//Calculate the angle of the line
	double angle = 0;
	angle = atan2(ydistance, xdistance)*180/PI;
	
	#if (FEATURE_DEBUG_MODE)
	cout<<"angle: "<<angle<<endl;
	//cout<<"Negative angle: "<<atan2(-10, 50)*180/PI<<endl;
	#endif
	
	//If the angle is greater than a threshold, then invalid match
	if((angle<-10 || angle > 10) || (xdistance>(640+200) || xdistance<(640-200)))
		return false;
	else
		return true;

}

void FeatureExtraction::removeInvalidMatches(){
	
}


