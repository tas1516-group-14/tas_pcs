#ifndef DEBUGFUNCTIONS_H
#define DEBUGFUNCTIONS_H

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class DebugFunctions
{
public:
    DebugFunctions();
    void BildAnzeigen(const std::string& Eingangstring, cv::Mat InputImage);
};

#endif // DEBUGFUNCTIONS_H
