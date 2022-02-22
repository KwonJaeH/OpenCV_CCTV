#include "detect_vehicle.h"
#include "detect_lane.h"

Mat DetectVehicle::set_ROI(Mat img) {

	Mat output;
	Mat mask = Mat::zeros(img.rows, img.cols, CV_8UC1);

	// ��ٸ��� ���ɿ��� ���� ( �ܼ��� �����ӿ��� ��ǥ�� ã�� �̿� )

	Point point[4]{ Point(320,50),Point(400,50),
				Point(580,350),Point(30,350) };

	fillConvexPoly(mask, point, 4, Scalar(255, 0, 0));
	bitwise_and(img, mask, output);


	return output;
}


double DetectVehicle::euclidean_dist(double x1, double y1, double x2, double y2) {
	double x_dif = abs(x2 - x1), y_dif = abs(y2 - y1);
	return sqrt(x_dif * x_dif + y_dif * y_dif);
}

int DetectVehicle::road_same_vehicle(Rect rct, vector<bool> exist, int LanePos, DetectLane& dl) {

	double cur_x = (rct.x + (rct.x + rct.width)) / 2, cur_y = (rct.y + (rct.y + rct.height)) / 2;

	int pos = -1;
	double dist = 10000;

	for (int i = 0; i < vehicle.size(); i++) {
		Rect before = vehicle[i].rct;
		double bf_x = (before.x + (before.x + before.width)) / 2, bf_y = (before.y + (before.y + before.height)) / 2;

		double result = euclidean_dist(bf_x, bf_y, cur_x, cur_y);

		if (result <= 20 && result < dist) {
			dist = result;
			pos = i;
		}

	}

	if (pos == -1)
		return -1;

	// ����Ǿ��ִ� lane�� �ٸ��� ���� ���� & ���������� ����
	if (vehicle[pos].pos_of_lane != LanePos) {
		vehicle[pos].pos_of_lane = LanePos;
		vehicle[pos].color = { 0, 0, 255 };
		dl.Add_ChangeVehicle();
	}

	vehicle[pos].rct.x = rct.x;
	vehicle[pos].rct.y = rct.y;
	vehicle[pos].rct.width = min(vehicle[pos].rct.width, rct.width);
	vehicle[pos].rct.height = min(vehicle[pos].rct.height, rct.height);
	return pos;
}

void DetectVehicle::DrawAndEraseVehicle(vector<vector<Point>> contours, Mat frame, DetectLane& dl) {
	vector<bool> exist(100, false); // ���� �����ӿ��� ���� ����

	for (int i = 0; i < contours.size(); i++) {
		Rect rct = boundingRect(contours[i]);


		if ((contourArea(contours[i]) >= 800)) {


			// ��� Lane���� �Ǻ�

			int LanePos = dl.WhatLane(rct);

			int vehicle_idx;
			vehicle_idx = road_same_vehicle(rct, exist, LanePos,dl);

			if (vehicle_idx >= 0) {
				// ���� ���� ����

				exist[vehicle_idx] = true;
				rectangle(frame, vehicle[vehicle_idx].rct, vehicle[vehicle_idx].color, 1, 4, 0);

			}
			else {
				// Lane ���� ���� �ٸ��� ����

				obj e;
				Scalar color;
				if (LanePos == 1) {
					color = { 255, 0, 0 }; // �Ķ�
				}
				else if (LanePos == 2) {
					color = { 0, 255, 0 }; // �ʷ�
				}
				else {
					color = { 0, 0, 255 }; // ����
					dl.Add_ChangeVehicle();
				}

				rectangle(frame, Rect(rct.x, rct.y, rct.width, rct.height), color, 1, 4, 0);
				e = { rct,color,LanePos };

				vehicle.push_back(e);
				exist[vehicle.size() - 1] = true;
			}

		}
	}

	// �� ������ ������ 
	// �������� �ʴ� vehicle�� ���ɿ����� ���Դٰ� ��� ���̹Ƿ�
	// �����ִ� ����
	vector<int> clear_veh;
	for (int i = 0; i < vehicle.size(); i++) {
		if (!exist[i])
			clear_veh.push_back(i);
	}

	for (int i = 0; i < clear_veh.size(); i++) {
		vehicle.erase(vehicle.begin() + (clear_veh[i] - i));
	}
}