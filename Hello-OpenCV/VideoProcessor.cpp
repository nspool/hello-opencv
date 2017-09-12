//
//  VideoProcessor.cpp
//  Hello-OpenCV
//
//  Created by nsp on 23/5/17.
//  Copyright © 2017 nspool. All rights reserved.
//

#include "VideoProcessor.hpp"

void VideoProcessor::openVideo() {
    
    std::string titleText = std::string("Hello, OpenCV");
 
    cv::Mat frame, prevFrame;
    
    int frameCount = 0;
    
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
