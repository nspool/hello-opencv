//
//  VideoProcessor.cpp
//  Hello-OpenCV
//
//  Created by nsp on 23/5/17.
//  Copyright © 2017 nspool. All rights reserved.
//

#include "VideoProcessor.hpp"

cv::Mat VideoProcessor::backgroundSubstitution() {

    cv::Mat image, IavgF, IdiffF, IprevF, IhiF, IlowF, Icount, tmp, tmp2, Imaskt;
    bool isAllocated = false;
    
    cv::VideoCapture cap;
    if(!cap.open("1.mp4")) {
        printf("Failed to open video file. Aborting\n");
        return image;
    }
    
    while(1) {
        
        cap >> image;
        if(!image.data) { break; }
        
        // Initial allocation of intermediate images
        if(!isAllocated) {
            cv::Size sz = image.size();
            IavgF = cv::Mat::zeros(sz, CV_32FC3);
            IdiffF = cv::Mat::zeros(sz, CV_32FC3);
            IprevF = cv::Mat::zeros(sz, CV_32FC3);
            IhiF = cv::Mat::zeros(sz, CV_32FC3);
            IlowF = cv::Mat::zeros(sz, CV_32FC3);
            Icount = 0.00001;
            tmp = cv::Mat::zeros(sz, CV_32FC3);
            tmp2 = cv::Mat::zeros(sz, CV_32FC3);
            Imaskt = cv::Mat(sz, CV_32FC1);
            
            // Seed the first iteration
            image.convertTo(tmp, CV_32F);
            IprevF = tmp;
            
            isAllocated = true;
            
            cap >> image;
        }
        
        image.convertTo(tmp, CV_32F);
        IavgF += tmp;
        cv::absdiff(tmp, IprevF, tmp2);
        IdiffF += tmp2;
        Icount += 1.0;
        IprevF = tmp;
    }
    
    return IprevF;
}
