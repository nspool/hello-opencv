//
//  VideoProcessor.hpp
//  Hello-OpenCV
//
//  Created by nsp on 23/5/17.
//  Copyright © 2017 nspool. All rights reserved.
//

#ifndef VideoProcessor_hpp
#define VideoProcessor_hpp

#include <stdio.h>
#include "main.hpp"


class VideoProcessor
{
public:
    cv::Mat backgroundSubstitution();
    void openVideo();
private:
};

#endif /* VideoProcessor_hpp */
