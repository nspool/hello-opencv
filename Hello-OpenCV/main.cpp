//
//  main.cpp
//  Hello-OpenCV
//
//  Created by nsp on 13/2/17.
//  Copyright © 2017 nspool. All rights reserved.
//

// Because the opencv framework headers don't play well with clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <iostream>
#include "opencv2/opencv.hpp"

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
    
    // VideoCapture functor
    cv::namedWindow(titleText);
    cv::VideoCapture cap("1.mp4");
    
    cap.read(frame);
    
    std::cout << "rows: " << frame.rows << " cols: " << frame.cols << std::endl;

    while(!frame.empty()) {
        
        frameCount++;
        
        cv::imshow(titleText, frame);
        
        std::cout << frameCount << std::endl;
        
        cv::waitKey();
        
        prevFrame = frame;
        
        cap.read(frame);
    }
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


int main(int argc, const char * argv[]) {
    
    // Display the window and draw the image
    if(cv::startWindowThread() != 0) {
        std::cerr << "Cannot start window thread. Continuing.." << std::endl;
    }
    
    //    openImages();
    openVideo();
    
    cv::destroyAllWindows();
    
    return 0;
    
}

