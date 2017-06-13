//
// ImageProcessor.cpp
// Hello-OpenCV
//
// Created by nsp on 23/5/17.
// Copyright © 2017 nspool. All rights reserved.
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
    
    cv::Mat a = cv::imread("a.jpg");
    cv::Mat b = cv::imread("a.jpg");
    
    double distance = cv::EMDL1(a,b);
    
    printf("Earth Mover's Distance: %f\n", distance);
}

cv::Mat ImageProcessor::backProjection() {
    
    std::vector<cv::Mat> images;
    cv::Mat rabbit = cv::imread("b.jpg");
    images.push_back(rabbit);
    
    // Canvas contains the color of the background canvas
    cv::Mat canvas = cv::imread("canvas.png");
    
    cv::Mat output, hist;
    int histSize[] = {16, 16, 16};
    int channels[] = {0, 1, 2};
    const float hRange[] = {0.f, 256.f};
    const float* ranges[] = {hRange, hRange, hRange};
    
    cv::calcHist(&canvas, 1, channels, cv::noArray(), hist, 2, histSize, ranges);
    cv::calcBackProject(&rabbit, 1, channels, hist, output, ranges);
    
    return output;
}
