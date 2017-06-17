//
// ImageProcessor.cpp
// Hello-OpenCV
//
// Created by nsp on 23/5/17.
// Copyright © 2017 nspool. All rights reserved.
//

#include <vector>
#include "ImageProcessor.hpp"

cv::Mat ImageProcessor::contourFinding() {
    
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat output;
    cv::Mat rabbitColour = cv::imread("b.jpg");
    cv::Mat rabbitGrey = cv::imread("b.jpg", cv::IMREAD_GRAYSCALE);
    auto red = cv::Scalar(0,0,255,0);

    cv::Canny(rabbitGrey, output, 120, 240, 7);
    
    // RETR_EXTERNAL should only give the major 'external' contours
    cv::findContours(output, contours, hierarchy,  cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    
    cv::Mat drawing = cv::Mat::zeros(output.size(), CV_8UC3);
    for( int i = 0; i< contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar(255,0,0);
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
        
        // The discovered contours can be used to determine object geometry
        cv::Rect box = cv::boundingRect(contours[i]);
        cv::rectangle(rabbitColour, box, red);
    }
    

    
    return rabbitColour;
}

// Fast connected component analysis
cv::Mat ImageProcessor::connectedComponentAnalysis() {

    cv::Mat rabbit_thresh, labels, stats, centroids;
    cv::Mat rabbitGrey = cv::imread("b.jpg", cv::IMREAD_GRAYSCALE);
    
    cv::threshold(rabbitGrey, rabbit_thresh, 128, 255, cv::THRESH_BINARY);
    
    int componentCount = cv::connectedComponentsWithStats(rabbit_thresh, labels, stats, centroids);

    std::cout << "Number of components detected: " << componentCount << std::endl;
    
    return rabbit_thresh;
}

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
