/*
Wichtiges Kriterium für die Bildanalyse ist die Betrachtung der Wegbreite,
welche als bekannt vorrausgesetzt wird!

*/
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

struct Waypoint{
    cv::Point Point1;
    cv::Point Point2;
    cv::Point AveragePoint;
    double Distance;

};

int main()
{

    cv::Mat InputImage, GrayscaleImage, BinaryImage, BinaryImageManipulated;
    DebugFunctions debug;

    InputImage = cv::imread("test.png");
    if(InputImage.empty()){
        cout<< "Bild konnte nicht geladen werden!";
        cv::waitKey(0);
        return 0;

    }
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
        static bool FoundOuterContour = false;
        if(hierarchy[i][3] == -1 && hierarchy[i][2] > 0 && !FoundOuterContour){
            FoundContours.push_back(Contour[i]);
            hierarchy2.push_back(hierarchy[i]);
            NextContour = hierarchy[i][2];
            i = 0;
            FoundOuterContour = true;
        }
        if(NextContour >= 0 && FoundOuterContour){
            FoundContours.push_back(Contour[NextContour]);
            hierarchy2.push_back(hierarchy[NextContour]);
            NextContour = hierarchy[NextContour][0];
        }else if(FoundOuterContour){
            break;
        }
    }


    cv::Mat ContourImage;
    ContourImage = InputImage;

    int LineSizeRow = ContourImage.rows/100;
    int LineSizeCol = ContourImage.cols/100;


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



    //Bereinignung
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



    std::vector<Waypoint> Waypoints;
    for(int i = 0; i < FoundPointOnContour.size(); i++){
        std::vector<cv::Point> Points1 = FoundPointOnContour[i].Points;
        double contour1 = FoundPointOnContour[i].contour;
        for(int u = 0; u < FoundPointOnContour.size(); u++){
            double contour2 = FoundPointOnContour[u].contour;
            if(contour1 != contour2){
                std::vector<cv::Point> Points2 = FoundPointOnContour[u].Points;
                for(int z = 0; z < Points1.size(); z++){
                    for(int t = 0; t < Points2.size(); t++){
                        double Distance = sqrt( pow((Points1[z].x - Points2[t].x), 2) + pow((Points1[z].y - Points2[t].y), 2) );
                        if(Distance > 30 && Distance < 50){
                            cv::Point AveragePoint;
                            AveragePoint.x = (Points1[z].x + Points2[t].x)/2;
                            AveragePoint.y = (Points1[z].y + Points2[t].y)/2;


                            Waypoint FoundWayPoint;
                            FoundWayPoint.Point1 = Points1[z];
                            FoundWayPoint.Point2 = Points2[t];
                            FoundWayPoint.Distance = Distance;
                            FoundWayPoint.AveragePoint = AveragePoint;

                            Waypoints.push_back(FoundWayPoint);

                        }
                    }
                }
            }
        }
    }

    //Zeichnen von Weglinien und Punkten
    cv::Mat WaypointImage( ContourImage.cols, ContourImage.cols, CV_8UC3 );
    for(int i = 0; i < Waypoints.size(); i++){
        double Point1x, Point1y, Point2x, Point2y, AveragePointx, AveragePointy;
        Point1x = Waypoints[i].Point1.x;
        Point1y = Waypoints[i].Point1.y;
        Point2x = Waypoints[i].Point2.x;
        Point2y = Waypoints[i].Point2.y;

        AveragePointx = Waypoints[i].AveragePoint.x;
        AveragePointy = Waypoints[i].AveragePoint.y;

        debug.DrawLines(ContourImage, Point1x, Point1y, Point2x, Point2y, cv::Scalar(0,0,255), 1);
        debug.DrawLines(ContourImage, AveragePointx, AveragePointy, AveragePointx, AveragePointy, cv::Scalar(0,255,0), 3);


        ///debug.DrawLines(WaypointImage, Point1x, Point1y, Point2x, Point2y, cv::Scalar(0,0,255), 1);
        debug.DrawLines(WaypointImage, AveragePointx, AveragePointy, AveragePointx, AveragePointy, cv::Scalar(0,255,0), 1);
    }
    debug.BildAnzeigen("Before morph:WaypointImage:", WaypointImage);
    // Morphology
    cv::Mat WaypointImageBinary;
    int SizeForMorph2 = 2;
    cv::Mat element2 = cv::getStructuringElement(0, cv::Size(2*SizeForMorph2, 2*SizeForMorph2), cv::Point(SizeForMorph2,SizeForMorph2));
    cv::morphologyEx(WaypointImage, WaypointImage, CV_MOP_CLOSE , element2);

    
    cv::cvtColor(WaypointImage, WaypointImageBinary, CV_RGB2GRAY);



    
    cv::inRange(WaypointImageBinary, cv::Scalar(254,254,254), cv::Scalar(255,255,255), BinaryImage);
    debug.BildAnzeigen("After inrange: WaypointImage:", WaypointImageBinary);

    std::vector<cv::Vec4f> lines;
    vector<vector<cv::Point> > ContourWayPoint;
    vector< cv::Vec4i > hierarchyWayPoint;
    cv::findContours(WaypointImageBinary, ContourWayPoint, hierarchyWayPoint, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cv::Point(0, 0) );



    for(int i = 0; i < ContourWayPoint.size(); i++){
        cv::drawContours(WaypointImage, ContourWayPoint, i, cv::Scalar(0,0,255), 1);
    }
    debug.BildAnzeigen("After contour: WaypointImage:", WaypointImage);

    for(int i = 0; i < ContourWayPoint.size(); i++){
        //Bereinigen von kleinen Contouren
        if(ContourWayPoint[i].size() < 20){
            ContourWayPoint.erase(ContourWayPoint.begin() +i);
            i = i -1;
        }
    }

    for(int i = 0; i < ContourWayPoint.size(); i++){
        cv::Vec4f line;
        cv::fitLine(cv::Mat(ContourWayPoint[i]),line,2,0,0.01,0.01);
        lines.push_back(line);

    }

//    for( int i = 0; i < lines.size(); i++){
//        cv::Vec4f line = lines[i];
//        int lefty = (-line[2]*line[1]/line[0])+line[3];
//        int righty = ((WaypointImage.cols-line[2])*line[1]/line[0])+line[3];

//        cv::line(WaypointImage,cv::Point(WaypointImage.cols-1,righty),cv::Point(0,lefty),cv::Scalar(0,255,0),2);
//    }
//    debug.BildAnzeigen("After clearing line: WaypointImage:", WaypointImage);




    //bereinigen der linen, suche nach 2 orthogonalen schnittlinien
    for(int i = 0; i < lines.size(); i++){
        std::vector<float> Angles;
        cv::Vec4f Line1 = lines[i];
        for(int u = 0; u < lines.size(); u++){
            cv::Vec4f Line2 = lines[u];
            float Scalar = Line1[0]*Line2[0]+Line1[1]*Line2[1];
            float Abs1 = abs(Line1[0])+abs(Line1[1]);
            float Abs2 = abs(Line2[0])+abs(Line2[1]);
            float Alpha = (acos(abs(Scalar)/(Abs1*Abs2)))*57.2958;//1 grad -> 57.2958 deg
            Angles.push_back(Alpha);
        }
        bool FoundOrthogonalLine1 = false;
        bool FoundOrthogonalLine2 = false;
        for(int z = 0; z < Angles.size(); z++){
            float Alpha = Angles[z];
            if(FoundOrthogonalLine1 == true && Alpha > 89 && Alpha < 91){
                FoundOrthogonalLine2 = true;
                break;
            }
            if(Alpha > 89 && Alpha < 91){
                FoundOrthogonalLine1 = true;
            }

        }
        if(!FoundOrthogonalLine2){
            lines.erase(lines.begin()+i);
            i = i-1;
        }
    }


    cv::Mat LinesImage( ContourImage.cols, ContourImage.cols, CV_8UC3 );

    for( int i = 0; i < lines.size(); i++){
        cv::Vec4f line = lines[i];
        int lefty = (-line[2]*line[1]/line[0])+line[3];
        int righty = ((WaypointImage.cols-line[2])*line[1]/line[0])+line[3];

        cv::line(LinesImage,cv::Point(WaypointImage.cols-1,righty),cv::Point(0,lefty),cv::Scalar(255,0,0),2);
    }
    debug.BildAnzeigen("After clearing line: LinesImage:", LinesImage);





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
    debug.BildAnzeigen("Ergebnis:", ContourImage);



    cv::waitKey(0);
    return 0;
}

