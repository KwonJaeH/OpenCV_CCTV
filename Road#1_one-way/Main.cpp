#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "detect_vehicle.h"
#include <iostream>

//����������
#define URL "https://5fc0975374737.streamlock.net/live/video33.stream/playlist.m3u8"

using namespace cv;
using namespace std;


int main(void) {

	DetectVehicle DV;

	Mat frame, background;

	VideoCapture cap(URL);
	if (!cap.isOpened())
	{
		cerr << "ī�޶� �� �� �����ϴ�." << endl;
		return -1;
	}

	Mat diff;


	while (1)
	{

		cap.read(frame);
		if (frame.empty())
		{
			cerr << "ĸ�� ����" << endl;
			break;
		}

		// 1. gray ��ȯ
		Mat gray_frame, gray_bg;
		cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
		if (!background.empty()) {
			cvtColor(background, gray_bg, COLOR_BGR2GRAY);

			
			// 2. ���� �����Ӱ��� ���� ����
			absdiff(gray_frame, gray_bg, diff);

			// 3. �������Ϸ� ��ȯ
			threshold(diff, diff, 50, 255, THRESH_BINARY);

			// 4. ���ɿ��� ����
			diff = DV.set_ROI(diff);

			// 5. ��â -> ħ��, closing ����
			dilate(diff, diff, Mat::ones(Size(7, 7), CV_8UC1), Point(-1, -1), 2);
			erode(diff, diff, Mat::ones(Size(7, 7), CV_8UC1), Point(-1, -1), 2);

			// * ������ ���� ���� �����ִ� ��
			line(frame, Point(80, 140), Point(120, 130), Scalar(0, 0, 255), 5, 8, 0);
			line(frame, Point(90, 258), Point(170, 232), Scalar(0, 0, 255), 5, 8, 0);


			// 6. contours ã��
			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;
			findContours(diff, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

			// 7. ���� �� �ν� �� �����
			DV.DrawAndEraseVehicle(contours, frame);


			imshow("Live", diff);
			imshow("Live2", frame);
		}

		int fps = cap.get(CAP_PROP_FPS);

		int wait = int(1.0 / fps * 1000);

		if (waitKey(wait) >= 0)
			break;

		cap.read(background);
	}

	return 0;
}