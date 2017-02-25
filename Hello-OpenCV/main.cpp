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

int main(int argc, const char * argv[]) {

    cv::Mat img = cv::imread(argv[1], -1);
    if(img.empty()){
        return -1;
    }
    
    cv::namedWindow("Example", cv::WINDOW_AUTOSIZE);
    
    cv::Mat out;
    
    cv::imshow("Example", img);
    cv::waitKey();

    cv::pyrDown(img,out);
    cv::imshow("Example", out);
    cv::waitKey();
    cv::destroyWindow("Example");
    
    return 0;
}
