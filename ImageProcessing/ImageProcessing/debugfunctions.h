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
    cv::Mat DrawLines(cv::Mat InputImage, double Punkt1X, double Punkt1Y, double Punkt2X, double Punkt2Y, cv::Scalar Color, double Size);
};

#endif // DEBUGFUNCTIONS_H
