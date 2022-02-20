#pragma once
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class DetectVehicle {
private :
	vector<pair<Rect, Scalar>> vehicle;

public :
	void DrawAndEraseVehicle(vector<vector<Point>> contours,Mat frame);
	Mat set_ROI(Mat img);
	double euclidean_dist(double x1, double y1, double x2, double y2);
	int road_same_vehicle(Rect rct, vector<bool> exist);
};