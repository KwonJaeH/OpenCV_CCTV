#include "detect_vehicle.h"

Mat DetectVehicle::set_ROI(Mat img) {

	Mat output;
	Mat mask = Mat::zeros(img.rows, img.cols, CV_8UC1);

	// ��ٸ��� ���ɿ��� ���� ( �ܼ��� �����ӿ��� ��ǥ�� ã�� �̿� )
	Point point[4]{ Point(80,140),Point(120,130),
				Point(170,232),Point(90,258) };

	fillConvexPoly(mask, point, 4, Scalar(255, 0, 0));

	bitwise_and(img, mask, output);

	return output;
}


double DetectVehicle::euclidean_dist(double x1, double y1, double x2, double y2) {
	double x_dif = abs(x2 - x1), y_dif = abs(y2 - y1);
	return sqrt(x_dif * x_dif + y_dif * y_dif);
}

int DetectVehicle::road_same_vehicle(Rect rct, vector<bool> exist) {

	double cur_x = (rct.x + rct.width) / 2, cur_y = (rct.y + rct.height) / 2;

	int pos = -1;
	double dist = 10000;

	for (int i = 0; i < vehicle.size(); i++) {
		Rect pre = vehicle[i].first;
		double pre_x = (pre.x + pre.width) / 2, pre_y = (pre.y + pre.height) / 2;

		double result = euclidean_dist(pre_x, pre_y, cur_x, cur_y);
		if ((result <= 20 && result < dist)) {
			dist = result;
			pos = i;
		}
	}

	if (pos == -1)
		return -1;

	// ���ο� ���� ��ǥ �� ũ�� ã���� ����
	vehicle[pos].first.x = rct.x;
	vehicle[pos].first.y = rct.y;
	vehicle[pos].first.width = max(vehicle[pos].first.width, rct.width);
	vehicle[pos].first.height = max(vehicle[pos].first.height, rct.height);
	return pos;
}

void DetectVehicle::DrawAndEraseVehicle(vector<vector<Point>> contours,Mat frame) {
	vector<bool> exist(100, false); // ���� �����ӿ��� ���� ����

	for (int i = 0; i < contours.size(); i++) {
		Rect rct = boundingRect(contours[i]);

		
		if ((contourArea(contours[i]) >= 100)) { // contour ���̰� 100 �̻�

			int vehicle_idx;
			vehicle_idx = road_same_vehicle(rct, exist);

			if (vehicle_idx >= 0) { 
				// ���� ���� ����

				exist[vehicle_idx] = true;
				rectangle(frame, vehicle[vehicle_idx].first, vehicle[vehicle_idx].second, 1, 4, 0);

			}
			else { 
				// ���������ʴ´ٸ� 
				// random ����� ��ǥ ���� ����

				Scalar color(rand() & 255, rand() & 255, rand() & 255);
				rectangle(frame, Rect(rct.x, rct.y, rct.width, rct.height), color, 1, 4, 0);

				vehicle.push_back(make_pair(rct, color));
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