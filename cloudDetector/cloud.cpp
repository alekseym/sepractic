#include "opencv2/opencv.hpp"
using namespace cv;
int main(int, char**)  {
    VideoCapture cap(0);                   // Open camera 0
    if(!cap.isOpened())                         // Check if we succeeded
        return -1;
    Mat edges;
    namedWindow("edges",1);
    while(1) {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);     
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);  
        Canny(edges, edges, 0, 30, 3);                              
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }    
    return 0;
}
