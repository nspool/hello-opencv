//
// ImageProcessor.cpp
// Hello-OpenCV
//
// Created by nsp on 23/5/17.
// Copyright © 2017 nspool. All rights reserved.
//

#include <vector>
#include "ImageProcessor.hpp"

void ImageProcessor::removeBackground() {
    
    // load test image
    cv::Mat image = cv::imread("b.jpg", cv::IMREAD_GRAYSCALE);
    //    cv::Mat colourImage = cv::imread("b.jpg");
    
    if(image.empty()){
        std::cerr << "Cannot load images. Aborting." << std::endl;
        return;
    }
    
    // Set a threshold that leaves only the outline of the foreground objects
    //    cv::threshold(image, image, 10, 255, CV_THRESH_BINARY);
    cv::adaptiveThreshold(image, image, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 21, 50);
    //    cv::fastNlMeansDenoising(image, image);
    cv::Mat k = cv::Mat::eye(2, 2, 0);
    cv::dilate(image, image, k);
    
    // Flood Fill?
    
    cv::imshow(titleText_, image);
    cv::waitKey();
}


// Hough Lines

void ImageProcessor::houghLines() {
    
    cv::Mat imageIn, imageOut;
    
    imageIn = cv::imread("a.jpg", cv::IMREAD_COLOR);
    
    cv::cvtColor(imageIn, imageOut, cv::COLOR_BGR2GRAY);
    
    std::vector<cv::Vec4i> lines;
    
    cv::Canny(imageOut, imageOut, 50, 200, 3);
    
    cv::HoughLinesP(imageOut, lines, 1, CV_PI/180, 10, 10, 0 );
    
    for(auto &l: lines) {
        cv::line(imageIn, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 1, cv::LineTypes::LINE_4);
    }
    
    cv::imshow(titleText_, imageIn);

}


void ImageProcessor::watershed() {
    
    cv::Mat imageIn, imageProc, markers;
    
    imageIn = cv::imread("a.jpg");
    
    imageProc.create(imageIn.rows, imageIn.cols, CV_8UC3);
    
    markers.create(imageIn.rows, imageIn.cols, CV_32SC1);
    
    cv::cvtColor(imageIn, imageProc, CV_8UC3);
    
    cv::watershed(imageIn, markers);
    
    cv::imshow(titleText_, markers);
}

void ImageProcessor::mouseCallback(int event, int x, int y, int flags, void* param) {
    switch(event) {
        case cv::EVENT_LBUTTONUP:
            return;
        default:
            break;
    }
}

void ImageProcessor::openImages() {
    
    // Colours
    cv::Mat imgBackground = cv::imread("a.jpg");
    cv::Mat imgSprite = cv::imread("b.jpg");
    if(imgBackground.empty() || imgSprite.empty()){
        std::cerr << "Cannot load images. Aborting." << std::endl;
        return;
    }
    
    // For borders
    auto white = cv::Scalar(255,255,255,0);
    auto red = cv::Scalar(0,0,255,0);
    
    cv::Mat imgOutput;
    
    cv::transpose(imgSprite, imgOutput);
    cv::Mat roi1(imgBackground, cv::Rect(10,10,259,258));
    cv::Mat roi2(imgOutput, cv::Rect(0,0,259,258));
    
    // Blend the sprite onto the background image
    
    cv::addWeighted(roi1, 0.5, roi2, 0.5, 0.0, roi1);
    
    // Put a border around the transposed sprite
    
    cv::rectangle(imgBackground, cv::Point(10,10), cv::Point(269,268), white);
    
    // Draw the title text and place a red border around it
    
    cv::Point textPoint = cv::Point(300,30);
    cv::putText(imgBackground, titleText_, textPoint, 0, 1, 0, 2, 4, false);
    int textBaseline = 0;
    cv::Size textSize = cv::getTextSize(titleText_, 0, 1, 2, &textBaseline);
    textPoint.y = textPoint.y + textBaseline;
    cv::rectangle(imgBackground, textPoint, cv::Point(textPoint.x + textSize.width, textPoint.y - textBaseline - textSize.height - 2), red);
    
    cv::imshow(titleText_, imgBackground);
    
    // Mouse callback
    cv::setMouseCallback(titleText_, mouseCallback, nullptr);
    
    // Wait for any keypress and then close
    cv::waitKey();
}

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
