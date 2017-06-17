//
//  ImageProcessor.hpp
//  Hello-OpenCV
//
//  Created by nsp on 23/5/17.
//  Copyright © 2017 nspool. All rights reserved.
//

#ifndef ImageProcessor_hpp
#define ImageProcessor_hpp

#include <stdio.h>
#include "opencv2/opencv.hpp"

#endif /* ImageProcessor_hpp */

class ImageProcessor
{
public:
    void histogramEMD();
    cv::Mat contourFinding();
    cv::Mat backProjection();
    cv::Mat templateMatching();
    cv::Mat histogramEqualize();
};
