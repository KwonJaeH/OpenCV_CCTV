#pragma once
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class DetectLane {
private:
	
	Point pt1, pt2; // 차선 양 끝 좌표를 저장할 변수
	int line_cnt; // 차선을 구할 때, 허프변환 끝나고 좌표 평균을 구하는 용도  

	// 차선 y = ax + b 
	double a, b;
public:
	DetectLane() {
		pt1 = Point(0, 0);
		pt2 = Point(0, 0);

		a = 0, b = 0;
		line_cnt = 0;
	}

	Mat ColorFilter(Mat frame);
	Mat set_ROI(Mat img);
	void set_equation();
	void Trans_Hough(Mat frame,Mat img);
	bool Done_detectLane();
	void DrawRoadLane(Mat img);
	int WhatLane(Rect rct);
};