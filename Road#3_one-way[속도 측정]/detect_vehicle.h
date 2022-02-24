#pragma once
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include "detect_lane.h"

using namespace cv;
using namespace std;

class DetectVehicle {
private:

	typedef struct {
		Rect rct;
		int pos_of_lane;
		clock_t start_time;
	}obj;
	

	vector<obj> vehicle;

	// CCTV 화면에 맞게, 차선에 맞게 재설정

	const double DISTANCE = 0.07;  // 70m , 0.07km
	const int start_y = 297, end_y = 391;


public:


	double left_lane_speed, right_lane_speed;

	Mat set_ROI(Mat img);
	double euclidean_dist(double x1, double y1, double x2, double y2);
	int road_same_vehicle(Rect rct, vector<bool> exist,int LanePos, DetectLane& dl);
	void DrawAndEraseVehicle(vector<vector<Point>> contours, Mat frame, DetectLane& dl);
	void Estimate_Speed(Mat frame,int idx,clock_t end_time);
};