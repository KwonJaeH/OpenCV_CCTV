#include "detect_lane.h"

Mat DetectLane::ColorFilter(Mat frame) {

	Mat white_mask, white_image;

	Scalar upper_white = Scalar(255, 255, 255);
	Scalar lower_white = Scalar(125, 125, 125);

	inRange(frame, lower_white, upper_white, white_mask);
	bitwise_and(frame, frame, white_image, white_mask);
	
	return white_image;
}


Mat DetectLane::set_ROI(Mat img) {

	Mat output;
	Mat mask = Mat::zeros(img.rows, img.cols, CV_8UC1);


	// 사다리꼴 관심영역 설정 ( 단순히 프레임에서 좌표를 찾아 이용 )

	Point point[4]{ Point(395, 295), Point(415, 290),
					 Point(485, 390), Point(465, 395) };

	fillConvexPoly(mask, point, 4, Scalar(255, 0, 0));
	bitwise_and(img, mask, output);
	
	return output;
}

void DetectLane::Trans_Hough(Mat frame,Mat img) {
	vector<Vec4i> lines;
	HoughLinesP(frame, lines, 1, (CV_PI / 180), 50, 50, 50);

	for (int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		pt1 += Point(l[0], l[1]);
		pt2 += Point(l[2], l[3]);
		line_cnt++;
	}
}


void DetectLane::set_equation() {

	// y = (기울기)x - (기울기*x1) + y1
	// y = ax + b

	// pt2.x == pt1.x 인 경우에는 사용 못함
	// 현재 영상에서는 대각선이기 때문에 사용
	
	double incli = double((pt2.y - pt1.y)) / double((pt2.x - pt1.x));

	a = incli;
	b = (-1) * (incli * double(pt1.x)) + double(pt1.y);
}

bool DetectLane::Done_detectLane() {
	
	// 다 더한 x, y 좌표를 frame수로 나눠서 평균 좌표를 구함
	if (line_cnt) {

		pt1.x = pt1.x / line_cnt; pt1.y = pt1.y / line_cnt;
		pt2.x = pt2.x / line_cnt; pt2.y = pt2.y / line_cnt;

		set_equation();

		return true;
	}
	return false;
}


void DetectLane::DrawRoadLane(Mat img) {

	// 적당한 범위의 임의의 x값을 앞서 구한 방정식에 삽입해 y값 도출
	// 두개의 좌표를 만들어 차선 line 생성 

	double y1 = a * 400 + b;
	double y2 = a * 500 + b;

	line(img, Point(400, y1), Point(500, y2), Scalar(0, 200, 200), 2, 8, 0);
}


int DetectLane::WhatLane(Rect rct) {

	// 중심 좌표
	double cur_x = (rct.x + (rct.x+rct.width)) / 2, cur_y = (rct.y + (rct.y+rct.height)) / 2;
	
	double y1 = a * cur_x + b;

	if (y1 > cur_y) { // 화면 기준 오른쪽 차선
		return 1;
	}
	else if (y1 < cur_y) { // 왼쪽 차선
		return 2;
	}
	  
	return 0;
}