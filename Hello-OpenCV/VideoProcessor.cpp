//
//  VideoProcessor.cpp
//  Hello-OpenCV
//
//  Created by nsp on 23/5/17.
//  Copyright © 2017 nspool. All rights reserved.
//

#include <vector>

#include "VideoProcessor.hpp"

cv::Mat VideoProcessor::backgroundSubstitution() {

    cv::Mat image, IavgF, IdiffF, IprevF, IhiF, IlowF, tmp, tmp2, Imask, Imaskt;
    float Icount = 0.00001;
    
    std::vector<cv::Mat> Igray(3);
    std::vector<cv::Mat> Ilow(3);
    std::vector<cv::Mat> Ihi(3);
    
    bool isAllocated = false;
    
    cv::VideoCapture cap;
    if(!cap.open("trees.mp4")) {
        printf("Failed to open video file. Aborting\n");
        return image;
    }
    
    // Training
    
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
            
            tmp = cv::Mat::zeros(sz, CV_32FC3);
            tmp2 = cv::Mat::zeros(sz, CV_32FC3);
            Imaskt = cv::Mat(sz, CV_32FC1);
            
            // Seed the first iteration
            image.convertTo(tmp, CV_32F);
            IprevF = tmp;
            
            isAllocated = true;
            
            cap >> image;
        }
        
        // calculate the accumulated abs. value of frame-to-frame differences
        // (a parsimonous proxy for stddev)
        
        image.convertTo(tmp, CV_32F);
        IavgF += tmp;
        cv::absdiff(tmp, IprevF, tmp2);
        IdiffF += tmp2;
        Icount += 1.0;
        IprevF = tmp;
    }
    
    // calculate the model
    
    IavgF *= (1.0/Icount);
    IdiffF *= (1.0/Icount);
    IdiffF += cv::Scalar(1.0,1.0,1.0);
    
    // calculate thresholds
    
    IhiF = IavgF + (IdiffF * 100.0);
    cv::split(IhiF, Ihi);
    
    IlowF = IavgF - (IdiffF * 80.0);
    cv::split(IlowF, Ilow);
    
    // Testing
    
    if(!cap.open("trees.mp4")) {
        printf("Failed to open video file. Aborting\n");
        return image;
    }
    
    while(1) {
        cap >> image;
        if(!image.data) { break; }
        
        // segment the background
        image.convertTo(tmp, CV_32F);
        cv::split(tmp, Igray);
        
        cv::inRange(Igray[0], Ilow[0], Ihi[0], Imask);
        cv::inRange(Igray[1], Ilow[1], Ihi[1], Imaskt);
        Imask = cv::min(Imask, Imaskt);
        cv::inRange(Igray[2], Ilow[2], Ihi[2], Imaskt);
        Imask = cv::min(Imask, Imaskt);
        
        // Invert the mask
        Imask = 255 - Imask;
        
        
        // Visualise
        cv::split(image, Igray);
        Igray[2] = cv::max(Imask, Igray[2]);
        cv::merge(Igray, image);

        cv::imshow("title", image);
        cv::waitKey(100);
    }
    
    return IprevF;
}

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

void VideoProcessor::processVideo(std::string videoFilename) {
    
    cv::VideoCapture capture(videoFilename);
    
    if(!capture.isOpened()){

        std::cerr << "Unable to open video file: " << videoFilename << std::endl;
        exit(EXIT_FAILURE);
    }
    
    cv::Size s(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
    
    std::cerr << s.width << '\n';
    std::cerr << s.height << '\n';
    cv::VideoWriter writer ("Video.avi",CV_FOURCC('X','2','6','4'), 10, s, true);
    
    while(true){

        if(!capture.read(frame)) {
            std::cerr << "Unable to read next frame.\n";
            exit(EXIT_FAILURE);
        }
        
        //update the background model
        pMOG2->apply(frame, fgMaskMOG2);
        // apply the operation
        Mat element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(1,1) );
        Mat mask;
        morphologyEx(fgMaskMOG2, mask, CV_MOP_OPEN, element);
        dilate(mask, mask, element, Point(-1,-1), 5);
        //        morphologyEx(mask, mask, CV_MOP_CLOSE, element);
        
        
        //show the current frame and the fg masks
        Mat outputFrame;
        frame.copyTo(outputFrame, mask);
        writer << outputFrame;
    }

    writer.release();
    
    capture.release();
}
