//
//  main.cpp
//  Hello-OpenCV
//
//  Created by nsp on 13/2/17.
//  Copyright © 2017 nspool. All rights reserved.
//

#include <iostream>

#include "ImageProcessor.hpp"
#include "VideoProcessor.hpp"

std::string titleText = std::string("Hello, OpenCV");

int main(int argc, const char * argv[]) {
    
    // Display the window and draw the image
    if(cv::startWindowThread() != 0) {
        std::cerr << "Cannot start window thread. Continuing.." << std::endl;
    }
    
    // Image processing examples
    
    //    auto ip = ImageProcessor();
    //    auto imageBGR = ip.histogramEqualize();
    //    cv::Mat image = ip.templateMatching();
    //    ip.histogramEMD();
    //    cv::Mat image = ip.backProjection();
    //    cv::Mat image = ip.contourFinding();
    //    cv::Mat image = ip.connectedComponentAnalysis();
    //    removeBackground();
    //    houghLines();
    //    watershed();
    
    // Video processing examples

    // NB: create the window *before* opening the video, otherwise will encounter
    // "current event queue and the main event queue are not the same" bug.
    
    cv::namedWindow(titleText);
    
    auto vp = VideoProcessor();
    
//    cv::Mat image = vp.backgroundSubstitution();
    vp.processVideo("trees.mp4");

    cv::waitKey();
    
    cv::destroyAllWindows();
    
    return 0;
    
}

