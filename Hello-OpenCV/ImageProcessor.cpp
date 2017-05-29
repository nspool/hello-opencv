//
//  ImageProcessor.cpp
//  Hello-OpenCV
//
//  Created by nsp on 23/5/17.
//  Copyright © 2017 nspool. All rights reserved.
//

#include <vector>
#include "ImageProcessor.hpp"

// Histograms
// Equalise the histogram of a channel of a colour image to normalise brightness

cv::Mat ImageProcessor::histogramEqualize() {
    
    cv::Mat imageBGR, imageYUV;
    std::vector<cv::Mat> channels;
    
    imageBGR = cv::imread("a.jpg");
    
    // Convert the image to YUV
    
    cv::cvtColor(imageBGR, imageYUV, CV_BGR2YUV);
    cv::split(imageYUV, channels);
    
    // The first channel in YUV is 'luma' or brightness
    
    cv::equalizeHist(channels[0], channels[0]);
    
    // Convert back to BGR
    cv::merge(channels, imageYUV);
    cv::cvtColor(imageYUV, imageBGR, CV_YUV2BGR);
    
    return imageBGR;
}

cv::Mat ImageProcessor::templateMatching() {
    
    cv::Mat t = cv::imread("template.png");
    cv::Mat rabbit = cv::imread("b.jpg");
    cv::Mat output;
    
    cv::matchTemplate(rabbit, t, output, cv::TemplateMatchModes::TM_SQDIFF);
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(output, NULL, NULL, &minLoc, &maxLoc);
    
    cv::rectangle(rabbit, minLoc, maxLoc, 0);
    
    return rabbit;
}

// TODO: Example of using the Earth Mover's Distance to compare histograms
void ImageProcessor::histogramEMD() {
//    
//    cv::Mat a = cv::imread("a.jpg");
//    cv::Mat b = cv::imread("a.jpg");
//    = cv::EMD(a, b, 2, nullptr, 0, nullptr);
//    
    double distance = 0;
    
    printf("Earth Mover's Distance: %f\n", distance);
}


// TODO: Back Projection example
void ImageProcessor::backProjection() {
    
    std::vector<cv::Mat> inputs;
    std::vector<int> channels;
    std::vector<float> ranges;
    cv::Mat backProject;

    // use the canvas as the background
    cv::Mat canvas = cv::imread("canvas.png");
    cv::Mat rabbit = cv::imread("b.jpg");
    
    inputs.push_back(canvas);
    ranges.push_back(10);
    channels.push_back(canvas.channels());
    
//    cv::calcBackProject(inputs, &channels, rabbit, backProject, &ranges);
    
}

























