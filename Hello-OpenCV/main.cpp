//
//  main.cpp
//  Hello-OpenCV
//
//  Created by nsp on 13/2/17.
//  Copyright © 2017 nspool. All rights reserved.
//

// # TODO
// - Put each of the examples in their own classes
// - 1.mp4: tell exactly on what frame it is different using histogram methods


// Because the opencv framework headers don't play well with clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <iostream>
#include "ImageProcessor.hpp"

std::string titleText = std::string("Hello, OpenCV");

//cv::Vec small, fixed vectors
//cv::Matx small, fixed matrices
//cv::Point (x & y) can be cast to Vec but not derived from it. Dot & cross product.
//cv::Size (width & height), area computation
//cv::Rect (x & y & width & height), area, upper-left and bottom-right, intersections
//cv::RotatedRect
//cv::Scalar a four-dimensional point class, element-wise multiplication, quaternion conjection

void createSparse() {
    int size[] = {10,10};
    cv::SparseMat sm(2,size, CV_32F);
    
    for(int i=0; i<10; i++) {
        int idx[2];
        idx[0] = size[0] * rand();
        idx[1] = size[1] * rand();
        sm.ref<float>(idx) += 1.0f;
    }
    
    // cv::SparseMatConstIterator_<float> it = sm.begin<float>();
    // cv::SparseMatConstIterator_<float> it_end = sm.end<float>();
    auto it = sm.begin<float>();
    auto it_end = sm.end();
    for(;it != it_end; ++it) {
        const cv::SparseMat::Node* node = it.node();
        printf("(%3d,%3d) %f\n", node->idx[0],node->idx[1], *it);
    }
}

void createMat() {
    cv::Mat m;
    m.create(3, 10, CV_32FC3); // Create data area for 3 rows and 10 columns of 3-channel 32-bit floats
    m.setTo(cv::Scalar(1.0f, 0.0f, 1.0f)); // Set the value of 1st and 3rd channels to 1.0, 2nd channel to 0
    // Equivalent to:
    cv::Mat m2(3,10,CV_32FC3,cv::Scalar(1.0f, 0.0f, 1.0f));
}


// Do something when the window is clicked
void mouseCallback(int event, int x, int y, int flags, void* param) {
    switch(event) {
        case cv::EVENT_LBUTTONUP:
            return;
        default:
            break;
    }
}

// Open a video and read it frame-by-frame
void openVideo() {
    
    cv::Mat frame, prevFrame;
    
    int frameCount = 0;
    
    // NB: create the window *before* opening the video, otherwise will encounter
    // "current event queue and the main event queue are not the same" bug.
    cv::namedWindow(titleText);
    
    // VideoCapture functor
    cv::VideoCapture cap("1.mp4");
    
    cap >> frame;
    
    std::cout << "rows: " << frame.rows << " cols: " << frame.cols << std::endl;
    
    // Print out the location of the pixels that have changed between frames
    
    while(!frame.empty()) {
        
        frameCount++;
        
        cv::Mat deltaFrame;
        
        if(!prevFrame.empty()) {
            
            cv::cvtColor(prevFrame - frame, deltaFrame, CV_BGR2GRAY);
            
            int c = cv::countNonZero(deltaFrame);
            
            if(c > 0) {
                std::cout << frameCount << std::endl;
                std::vector<cv::Point> locations;
                cv::findNonZero(deltaFrame, locations);
                for(cv::Point& l : locations) {
                    std::cout << l << std::endl;
                }
            }
            cv::imshow(titleText, deltaFrame);
            cv::waitKey();
        }
        
        frame.copyTo(prevFrame);
        cap >> frame;
    }
    
    std::cout << frameCount << std::endl;
    
}


void openImages() {
    
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
    cv::putText(imgBackground, titleText, textPoint, 0, 1, 0, 2, 4, false);
    int textBaseline = 0;
    cv::Size textSize = cv::getTextSize(titleText, 0, 1, 2, &textBaseline);
    textPoint.y = textPoint.y + textBaseline;
    cv::rectangle(imgBackground, textPoint, cv::Point(textPoint.x + textSize.width, textPoint.y - textBaseline - textSize.height - 2), red);
    
    cv::imshow(titleText, imgBackground);
    
    // Mouse callback
    cv::setMouseCallback(titleText, mouseCallback, nullptr);
    
    // Wait for any keypress and then close
    cv::waitKey();
}

void scalarPlay() {

    // convert to a mar
    cv::Mat red = cv::Mat(cv::Scalar(0,0,255,0));
    cv::Mat blue = cv::Mat(cv::Scalar(255,0,0,0));
    cv::Mat green = cv::Mat(cv::Scalar(0,255,0,0));

    // Determine which of these scalars is blueish

    cv::Mat v1 = cv::Mat(cv::Scalar(254, 1,1,0 ));
    cv::Mat v2 = cv::Mat(cv::Scalar(1, 254,1,0));
    cv::Mat v3 = cv::Mat(cv::Scalar(1, 1,254,0));

    cv::Mat diff = cv::Mat_<double>(4,1);

    // determined emperically
    double threshold = 5;
    
    for(auto &v: {v1, v2, v3}){
        cv::absdiff(v, blue, diff);
        auto norm = cv::norm(diff);
        
        if(norm < threshold) {
            std::cout << "blueish (" << norm << ")" << std::endl;
        } else {
            std::cout << "not blueish (" << norm << ")" << std::endl;
        }
    }
}

void removeBackground() {
    
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
    
    cv::imshow(titleText, image);
    cv::waitKey();
}


// Hough Lines

void houghLines() {
    
    cv::Mat imageIn, imageOut;
    
    imageIn = cv::imread("a.jpg", cv::IMREAD_COLOR);
    
    cv::cvtColor(imageIn, imageOut, cv::COLOR_BGR2GRAY);
    
    std::vector<cv::Vec4i> lines;
    
    cv::Canny(imageOut, imageOut, 50, 200, 3);
    
    cv::HoughLinesP(imageOut, lines, 1, CV_PI/180, 10, 10, 0 );
    
    for(auto &l: lines) {
        cv::line(imageIn, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 1, cv::LineTypes::LINE_4);
    }
    
    cv::imshow(titleText, imageIn);

}

void watershed() {
    
    cv::Mat imageIn, imageProc, markers;

    imageIn = cv::imread("a.jpg");
    
    imageProc.create(imageIn.rows, imageIn.cols, CV_8UC3);
    
    markers.create(imageIn.rows, imageIn.cols, CV_32SC1);
    
    cv::cvtColor(imageIn, imageProc, CV_8UC3);

    cv::watershed(imageIn, markers);
    
    cv::imshow(titleText, markers);
}


int main(int argc, const char * argv[]) {
    
    // Display the window and draw the image
    if(cv::startWindowThread() != 0) {
        std::cerr << "Cannot start window thread. Continuing.." << std::endl;
    }
    
    // openImages();
    // openVideo();
    // removeBackground();
//    houghLines();
//    watershed();
    
    auto ip = ImageProcessor();
    
//    auto imageBGR = ip.histogramEqualize();
    
//    cv::Mat image = ip.templateMatching();

//    ip.histogramEMD();
    
//    cv::Mat image = ip.backProjection();
    
    cv::Mat image = ip.contourFinding();
    
//    cv::Mat image = ip.connectedComponentAnalysis();
    
    cv::imshow("title", image);

    cv::waitKey();
    
    cv::destroyAllWindows();
    
    return 0;
    
}

