//
//  Helper.cpp
//  Hello-OpenCV
//
//  Created by nsp on 15/9/17.
//  Copyright © 2017 nspool. All rights reserved.
//

#include "Helper.hpp"

//cv::Vec small, fixed vectors
//cv::Matx small, fixed matrices
//cv::Point (x & y) can be cast to Vec but not derived from it. Dot & cross product.
//cv::Size (width & height), area computation
//cv::Rect (x & y & width & height), area, upper-left and bottom-right, intersections
//cv::RotatedRect
//cv::Scalar a four-dimensional point class, element-wise multiplication, quaternion conjection

void Helper::createSparse() {
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

void Helper::createMat() {
    cv::Mat m;
    m.create(3, 10, CV_32FC3); // Create data area for 3 rows and 10 columns of 3-channel 32-bit floats
    m.setTo(cv::Scalar(1.0f, 0.0f, 1.0f)); // Set the value of 1st and 3rd channels to 1.0, 2nd channel to 0
    // Equivalent to:
    cv::Mat m2(3,10,CV_32FC3,cv::Scalar(1.0f, 0.0f, 1.0f));
}

void Helper::scalarPlay() {
    
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
