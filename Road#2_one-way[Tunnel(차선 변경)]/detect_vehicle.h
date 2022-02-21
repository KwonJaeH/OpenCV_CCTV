#pragma once
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "detect_lane.h"

using namespace cv;
using namespace std;

class DetectVehicle {
private:

	typedef struct {
		Rect rct;
		Scalar color;
		int pos_of_lane;
	}obj;

	vector<obj> vehicle;

public:

	Mat set_ROI(Mat img);
	double euclidean_dist(double x1, double y1, double x2, double y2);
	int road_same_vehicle(Rect rct, vector<bool> exist,int LanePos);
	void DrawAndEraseVehicle(vector<vector<Point>> contours, Mat frame, DetectLane dl);
};