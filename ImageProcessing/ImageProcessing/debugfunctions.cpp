#include "debugfunctions.h"

DebugFunctions::DebugFunctions()
{
}


void DebugFunctions::BildAnzeigen(const std::string& Eingangstring, cv::Mat InputImage){
    cv::namedWindow(Eingangstring, cv::WINDOW_AUTOSIZE);
    cv::imshow(Eingangstring, InputImage);
}
