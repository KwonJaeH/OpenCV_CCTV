#pragma once
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class DetectLane {
private:
	
	Point pt1, pt2; // ���� �� �� ��ǥ�� ������ ����
	int frame_cnt; // 

	// ���� y = ax + b 
	double a, b;
	
public:
	DetectLane() {
		pt1 = Point(0, 0);
		pt2 = Point(0, 0);

		a = 0, b = 0;
		frame_cnt = 0;
	}

	Mat ColorFilter(Mat frame);
	Mat set_ROI(Mat img);
	void set_equation();
	void Trans_Hough(Mat frame);
	bool Done_detectLane();
	void DrawRoadLane(Mat img);
	int WhatLane(Rect rct);
};