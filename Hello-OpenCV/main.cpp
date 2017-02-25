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


//cv::Vec small, fixed vectors
//cv::Matx small, fixed matrices
//cv::Point (x & y) can be cast to Vec but not derived from it. Dot & cross product.
//cv::Size (width & height), area computation
//cv::Rect (x & y & width & height), area, upper-left and bottom-right, intersections
//cv::RotatedRect
//cv::Scalar a four-dimensional point class, element-wise multiplication, quaternion conjection

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
