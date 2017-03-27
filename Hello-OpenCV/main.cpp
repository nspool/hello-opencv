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

int main(int argc, const char * argv[]) {
    
//    createSparse();

    cv::Mat imgBackground = cv::imread("a.jpg");
    cv::Mat imgSprite = cv::imread("b.jpg");
    
    if(imgBackground.empty() || imgSprite.empty()){
        return -1;
    }
    
    cv::Mat roi1(imgBackground, cv::Rect(10,10,258,259));
    cv::Mat roi2(imgSprite, cv::Rect(0,0,258,259));
    
    cv::addWeighted(roi1, 0.5, roi2, 0.5, 0.0, roi1);
    
    cv::imshow("Example", imgBackground);
    cv::waitKey();
    cv::destroyWindow("Example");
    
    return 0;
}




