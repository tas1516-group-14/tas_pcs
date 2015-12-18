#include <iostream>
#include <ncurses.h>


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "debugfunctions.h"
using namespace std;

struct Schnittpunkte{
    double contour;
    double line;
    double direction;
    std::vector<cv::Point> Points;

};

int main()
{

    cv::Mat InputImage, GrayscaleImage, BinaryImage, BinaryImageManipulated;
    DebugFunctions debug;

    InputImage = cv::imread("Map.png");
    debug.BildAnzeigen("Eingangsbild:", InputImage);



    cout << "In Graustufen konvertiert!" << endl;
    cv::cvtColor(InputImage, GrayscaleImage, CV_RGB2GRAY);



    cout << "Binärbild erstellt!" << endl;
    cv::inRange(GrayscaleImage, cv::Scalar(254,254,254), cv::Scalar(255,255,255), BinaryImage);
    debug.BildAnzeigen("Binärbild:", BinaryImage);

    int SizeForMorph = 2;
    cv::Mat element = cv::getStructuringElement(0, cv::Size(2*SizeForMorph, 2*SizeForMorph), cv::Point(SizeForMorph,SizeForMorph));
    cv::morphologyEx(BinaryImage, BinaryImageManipulated, CV_MOP_CLOSE , element);


    ///BinaryImageManipulated = BinaryImage;


    debug.BildAnzeigen("Vor der Bearbeitung:", BinaryImage);
    debug.BildAnzeigen("Nach der Bearbeitung:", BinaryImageManipulated);


    cout << "Contour erkannt!" << endl;
    vector<vector<cv::Point> > Contour;
    vector< cv::Vec4i > hierarchy;
    cv::findContours(BinaryImageManipulated, Contour, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cv::Point(0, 0) );

    vector<vector<cv::Point> > FoundContours;
    vector< cv::Vec4i > hierarchy2;
    for(int i = 0; i < Contour.size(); i++ ){
        static double NextContour;
        if(hierarchy[i][3] == -1 && hierarchy[i][2] > 0 ){
            FoundContours.push_back(Contour[i]);
            hierarchy2.push_back(hierarchy[i]);
            NextContour = hierarchy[i][2];
            i = 0;
        }
        if(NextContour >= 0){
            FoundContours.push_back(Contour[NextContour]);
            hierarchy2.push_back(hierarchy[NextContour]);
            NextContour = hierarchy[NextContour][0];
        }else{
            break;
        }
    }


    cv::Mat ContourImage;
    ContourImage = InputImage;
//    for( int i = 0; i< FoundContours.size(); i++ ){
//        if(i == 0){
//            cv::drawContours( ContourImage, FoundContours, i, cv::Scalar(255, 0,255), 2, 8, hierarchy, 0, cv::Point() );
//        }else{
//            cv::drawContours( ContourImage, FoundContours, i, cv::Scalar(0, 255,0), 2, 8, hierarchy, 0, cv::Point() );
//        }
//    }

    int LineSizeRow = ContourImage.rows/50;
    int LineSizeCol = ContourImage.cols/50;


    //Horizontale Linien
    std::vector<Schnittpunkte> FoundPointOnContour;
    for(int u = 0; u < FoundContours.size(); u++ ){
        for(int i = 0; i <= ContourImage.rows; i = i+ LineSizeRow){          //Für 100 Schnittlinien
            Schnittpunkte Schnittpunkt;
            for (int z = 0; z < FoundContours[u].size(); z++){
                if( FoundContours[u][z].y == i ){
                    Schnittpunkt.Points.push_back(FoundContours[u][z]);
                    Schnittpunkt.line = i;
                    Schnittpunkt.direction = 0;
                    Schnittpunkt.contour = u;
                }
            }
            if(Schnittpunkt.Points.size() > 0){
                FoundPointOnContour.push_back(Schnittpunkt);
            }
        }
    }


    //Vertikale Linien
    for(int u = 0; u < FoundContours.size(); u++ ){
        for(int i = 0; i <= ContourImage.cols; i = i+LineSizeCol){
            Schnittpunkte Schnittpunkt;
            for (int z = 0; z < FoundContours[u].size(); z++){
                if( FoundContours[u][z].x == i ){
                    Schnittpunkt.Points.push_back(FoundContours[u][z]);
                    Schnittpunkt.line = i;
                    Schnittpunkt.direction = 1;
                    Schnittpunkt.contour = u;
                }
            }
            if(Schnittpunkt.Points.size() > 0){
                FoundPointOnContour.push_back(Schnittpunkt);
            }
        }
    }




    for(int i = 0; i <  FoundPointOnContour.size(); i++){
        double direction0    =   FoundPointOnContour[i].direction;
        double line0         =   FoundPointOnContour[i].line;
        double contour0      =   FoundPointOnContour[i].contour;
        bool found = false;
        for(int u = 0; u < FoundPointOnContour.size(); u++){
            double direction1    =   FoundPointOnContour[u].direction;
            double line1         =   FoundPointOnContour[u].line;
            double contour1      =   FoundPointOnContour[u].contour;
            if(line0 == line1 && u != i && direction0 == direction1 && contour0 != contour1){
                found = true;
                break;
            }
        }
        if(!found){
            FoundPointOnContour.erase(FoundPointOnContour.begin()+i);
            i =  0;
        }
    }


    //Zeichnen der Punkte und Schnittlinien
    for(int u = 0; u < FoundPointOnContour.size(); u++){
        Schnittpunkte Schnittpunkt = FoundPointOnContour[u];
        for(int i = 0; i < Schnittpunkt.Points.size(); i++){
            if(Schnittpunkt.contour == 0){
            debug.DrawLines(ContourImage, Schnittpunkt.Points[i].x, Schnittpunkt.Points[i].y, Schnittpunkt.Points[i].x, Schnittpunkt.Points[i].y, cv::Scalar(255,0,255), 5);

            }else{
            debug.DrawLines(ContourImage, Schnittpunkt.Points[i].x, Schnittpunkt.Points[i].y, Schnittpunkt.Points[i].x, Schnittpunkt.Points[i].y, cv::Scalar(0,0,255), 5);

            }
            if(Schnittpunkt.direction == 0){
                debug.DrawLines(ContourImage, 0, Schnittpunkt.line,ContourImage.cols, Schnittpunkt.line, cv::Scalar(255,0,0), 1);
            }else{
                debug.DrawLines(ContourImage, Schnittpunkt.line, 0,Schnittpunkt.line, ContourImage.rows, cv::Scalar(255,0,0), 1);
            }

        }
    }



//    cout << '\a' << endl;

//    double MatrixDistance;

//    for(int i = 0; i < FoundPointOnContour.size(); i++){
//        std::vector<double> PointDistance0,PointDistance1, DistanceBetweenPoints;
//        std::vector<cv::Point> Point0, Point1;
//        Point0 = FoundPointOnContour[i].Points;
//        double line = FoundPointOnContour[i].line;
//        double direction = FoundPointOnContour[i].direction;
//        for(int u = 0; u < FoundPointOnContour.size(); u++){
//            if(line == FoundPointOnContour[u].line && 0 < FoundPointOnContour[u].contour && FoundPointOnContour[i].direction == FoundPointOnContour[u].direction){
//                Point1 = FoundPointOnContour[u].Points;
//            }
//        }
//        if(Point0.size() > 0 && Point1.size() > 0){


//        for(int i = 1; i < Point0.size(); i++){
//            for(int u = 1; u < Point1.size(); u++){
//                DistanceBetweenPoints.push_back(sqrt( pow( Point0[i].x-Point1[u].x, 2) + pow( Point0[i].x-Point1[u].y, 2)));
//            }
//        }
//        }
//       double test;
//    }





    debug.BildAnzeigen("Ergebnis:", ContourImage);



    cv::waitKey(0);
    return 0;
}

