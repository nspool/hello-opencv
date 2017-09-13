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
#include "main.hpp"

#include "opencv2/opencv.hpp"

class ImageProcessor
{
public:
    void removeBackground();
    void houghLines();
    void watershed();
    void openImages();
    void histogramEMD();
    cv::Mat contourFinding();
    cv::Mat backProjection();
    cv::Mat templateMatching();
    cv::Mat histogramEqualize();
    cv::Mat connectedComponentAnalysis();
private:
    static void mouseCallback(int event, int x, int y, int flags, void* param);
    std::string titleText_ = std::string("Hello, OpenCV");
};

#endif /* ImageProcessor_hpp */
