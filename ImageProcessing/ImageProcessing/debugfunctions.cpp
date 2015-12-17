#include "debugfunctions.h"

DebugFunctions::DebugFunctions()
{
}


void DebugFunctions::BildAnzeigen(const std::string& Eingangstring, cv::Mat InputImage){
    cv::namedWindow(Eingangstring, cv::WINDOW_AUTOSIZE);
    cv::imshow(Eingangstring, InputImage);
}
cv::Mat DebugFunctions::DrawLines(cv::Mat InputImage, double Punkt1X, double Punkt1Y, double Punkt2X, double Punkt2Y, cv::Scalar Color, double Size){
    cv::Point Punkt1, Punkt2;
    Punkt1.x = Punkt1X;
    Punkt1.y = Punkt1Y;
    Punkt2.x = Punkt2X;
    Punkt2.y = Punkt2Y;
    cv::line(InputImage, Punkt1, Punkt2, Color,Size,8,0);
    return InputImage;
}
