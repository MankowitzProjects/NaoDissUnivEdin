#ifndef FEATUREEXTRACTION_H
#define FEATUREEXTRACTION_H

#include <opencv2/opencv.hpp>
#include "brisk.h"
#include <fstream>
#include <iostream>
#include <list>
#include <iomanip>
#include <math.h>

class FeatureExtraction
{
    public:
        FeatureExtraction();
        virtual ~FeatureExtraction();
        //Functions for gathering data
        cv::Ptr<cv::FeatureDetector> getDetector(int argc, char** argv, cv::Ptr<cv::FeatureDetector> detector, int threshold, int testThreshold, int testFlag);
        cv::Ptr<cv::DescriptorExtractor> getExtractor(int argc,char** argv,bool hamming, cv::Ptr<cv::DescriptorExtractor> descriptor );


        //Override functions for actual implementation
        cv::Ptr<cv::DescriptorExtractor> getExtractor(int argc, string feat_descriptor,bool hamming, cv::Ptr<cv::DescriptorExtractor> descriptorExtractor);
        cv::Ptr<cv::FeatureDetector> getDetector(int argc, string feat_detector, cv::Ptr<cv::FeatureDetector> detector, int threshold, int testThreshold, int testFlag);

        //Verfify that the matches are indeed correct
        bool verifyMatch(const cv::Mat &image, cv::KeyPoint keypoint1, cv::KeyPoint keypoint2);
        void verifyMatchingOrder(const cv::Mat &image, cv::KeyPoint keypoint1, cv::KeyPoint keypoint2);
        void removeInvalidMatches();
    protected:
    private:
};

#endif // FEATUREEXTRACTION_H
