#include "detect_vehicle.h"
#include "detect_lane.h"

Mat DetectVehicle::set_ROI(Mat img) {

	Mat output;
	Mat mask = Mat::zeros(img.rows, img.cols, CV_8UC1);

	// ��ٸ��� ���ɿ��� ���� ( �ܼ��� �����ӿ��� ��ǥ�� ã�� �̿� )

	Point point[4]{ Point(360, 270), Point(470, 270),
					 Point(570, 410) , Point(410, 410) };

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

	vehicle[pos].rct.x = rct.x;
	vehicle[pos].rct.y = rct.y;
	vehicle[pos].rct.width = max(vehicle[pos].rct.width, rct.width);
	vehicle[pos].rct.height = max(vehicle[pos].rct.height, rct.height);
	return pos;
}

void DetectVehicle::DrawAndEraseVehicle(vector<vector<Point>> contours, Mat frame, DetectLane& dl) {
	vector<bool> exist(100, false); // ���� �����ӿ��� ���� ����

	for (int i = 0; i < contours.size(); i++) {
		Rect rct = boundingRect(contours[i]);


		if ((contourArea(contours[i]) >= 600)) {

			// ��� Lane���� �Ǻ�
			int LanePos = dl.WhatLane(rct);

			int vehicle_idx;
			vehicle_idx = road_same_vehicle(rct, exist, LanePos,dl);

			double cur_y = (rct.y + (rct.y + rct.height)) / 2;

			if (vehicle_idx >= 0) {
				// ���� ���� ����

				if (cur_y > end_y) {
					// ������ ������ ����� ������ �ڽ� ����

					Scalar color(0, 0, 255);
					rectangle(frame, vehicle[vehicle_idx].rct, color, 1, 4, 0);
				}else
					exist[vehicle_idx] = true;
				
			}
			else {
				if (cur_y >= start_y && cur_y < end_y) {
					obj e;
					Scalar color(0, 255, 0);
					clock_t st = clock();

					rectangle(frame, Rect(rct.x, rct.y, rct.width, rct.height), color, 1, 4, 0);
					e = { rct,LanePos,st};

					vehicle.push_back(e);
					exist[vehicle.size() - 1] = true;
				}
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

	clock_t end_time = clock();
	for (int i = 0; i < clear_veh.size(); i++) {
		int idx = clear_veh[i] - i;
		Estimate_Speed(frame, idx,end_time);
		vehicle.erase(vehicle.begin() + (clear_veh[i] - i));
	}
}


void DetectVehicle::Estimate_Speed(Mat frame,int idx, clock_t end_time) {
	
	double drive_time_sec = double(end_time - vehicle[idx].start_time)/1000;
	
	double div = 3600 / drive_time_sec;
	
	if (vehicle[idx].pos_of_lane == 2)
		left_lane_speed = DISTANCE * div;
	else
		right_lane_speed = DISTANCE * div;
	

}