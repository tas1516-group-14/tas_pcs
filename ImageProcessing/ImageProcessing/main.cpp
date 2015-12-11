#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;

void BildAnzeigen(const string& Eingangstring, cv::Mat InputImage){

    cv::namedWindow(Eingangstring, cv::WINDOW_AUTOSIZE);
    cv::imshow(Eingangstring, InputImage);
}

int main()
{
    cv::Mat InputImage, GrayscaleImage, BinaryImage;


    InputImage = cv::imread("Map.png");
    BildAnzeigen("Eingangsbild:", InputImage);

    cout << "In Graustufen konvertiert!" << endl;
    cv::cvtColor(InputImage, GrayscaleImage, CV_RGB2GRAY);



    cout << "Binärbild erstellt!" << endl;
    cv::inRange(GrayscaleImage, cv::Scalar(254,254,254), cv::Scalar(255,255,255), BinaryImage);
    BildAnzeigen("Binärbild:", BinaryImage);


    cout << "Kannten erkannt!" << endl;
    vector<vector<cv::Point> > Kannten;
    vector<cv::Vec4i> hierarchy;
    cv::findContours(BinaryImage, Kannten, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );


    for( int i = 0; i< Kannten.size(); i++ )
         {
           cv::Scalar color = cv::Scalar(0, 0,255);
           cv::drawContours( InputImage, Kannten, i, color, 2, 8, hierarchy, 0, cv::Point() );
         }
    BildAnzeigen("Ergebnis:", InputImage);


    cv::waitKey(0);
    return 0;
}

