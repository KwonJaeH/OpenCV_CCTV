#include "detect_lane.h"

Mat DetectLane::ColorFilter(Mat frame) {

	Mat white_mask, white_image;

	// �ͳ� ����, ȭ���� ���� ���� ��� 255,255,255 �ƴ�
	Scalar upper_white = Scalar(150, 150, 150);
	Scalar lower_white = Scalar(80, 80, 80);

	inRange(frame, lower_white, upper_white, white_mask);
	bitwise_and(frame, frame, white_image, white_mask);
	
	return white_image;
}


Mat DetectLane::set_ROI(Mat img) {

	Mat output;
	Mat mask = Mat::zeros(img.rows, img.cols, CV_8UC1);

	// ��ٸ��� ���ɿ��� ���� ( �ܼ��� �����ӿ��� ��ǥ�� ã�� �̿� )
	Point point[4]{ Point(330,50),Point(370,50),
					Point(320,350),Point(250,350) };

	fillConvexPoly(mask, point, 4, Scalar(255, 0, 0));
	bitwise_and(img, mask, output);
	
	return output;
}

void DetectLane::Trans_Hough(Mat frame) {
	vector<Vec4i> lines;
	HoughLinesP(frame, lines, 1, (CV_PI / 180), 20, 100, 10);

	for (int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		pt1 += Point(l[0], l[1]);
		pt2 += Point(l[2], l[3]);
		frame_cnt++;
	}
}

void DetectLane::set_equation() {

	// y = (����)x - (����*x1) + y1
	// y = ax + b

	// pt2.x == pt1.x �� ��쿡�� ��� ����
	// ���� ���󿡼��� �밢���̱� ������ ���
	
	double incli = (pt2.y - pt1.y) / (pt2.x - pt1.x);

	a = incli;
	b = (-1) * (incli * pt1.x) + pt1.y;
	cout << a << " "<< b << "\n";
}

bool DetectLane::Done_detectLane() {
	
	// frame 50�� �̻󿡼� �����ߴٸ�
	// �� ���� x, y ��ǥ�� frame���� ������ ��� ��ǥ�� ����

	if (frame_cnt >= 50) {
		pt1.x = pt1.x / frame_cnt; pt1.y = pt1.y / frame_cnt;
		pt2.x = pt2.x / frame_cnt; pt2.y = pt2.y / frame_cnt;

		set_equation();

		return true;
	}
	return false;
}


void DetectLane::DrawRoadLane(Mat img) {

	// ������ ������ ������ x���� �ռ� ���� �����Ŀ� ������ y�� ����
	// �ΰ��� ��ǥ�� ����� line ���� 

	double y1 = a * 250 + b;
	double y2 = a * 400 + b;

	line(img, Point(250, y1), Point(400, y2), Scalar(0, 200, 200), 3, 8, 0);
}


int DetectLane::WhatLane(Rect rct) {

	// �߽� ��ǥ
	double cur_x = (rct.x + (rct.x+rct.width)) / 2, cur_y = (rct.y + (rct.y+rct.height)) / 2;
	
	double y1 = a * cur_x + b;

	if (y1 > cur_y) { // ȭ�� ���� ���� ����
		return 1;
	}
	else if (y1 < cur_y) { // ������ ����
		return 2;
	}
	  
	return 0;
}
