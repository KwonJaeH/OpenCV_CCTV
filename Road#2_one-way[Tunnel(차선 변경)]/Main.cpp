#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>
#include "detect_vehicle.h"
#include "detect_lane.h"

//�ͳ� 2
#define URL "http://211.252.223.177:1935/live/985.stream/playlist.m3u8"

int main(void) {

	DetectVehicle DV;
	DetectLane DL;

	Mat frame, background;

	VideoCapture cap(URL);
	if (!cap.isOpened())
	{
		cerr << "ī�޶� �� �� �����ϴ�." << endl;
		return -1;
	}

	// ���� ���� �ߴ���

	bool lane_detect = false;
	
	while (1)
	{

		cap.read(frame);
		if (frame.empty())
		{
			cerr << "ĸ�� ����" << endl;
			break;
		}


		if (!lane_detect) {
			//���� ����

			Mat white_image;

			// 1. ��� ����
			white_image = DL.ColorFilter(frame);
		
			// 2. gray ��ȯ
			cvtColor(white_image, white_image, COLOR_BGR2GRAY);

			// 3. ���� ���� ����
			white_image = DL.set_ROI(white_image);
			
			// 4. Blur ó��
			GaussianBlur(white_image, white_image, Size(7, 7), 0);
			
			// 5. Canny Edge ����
			Canny(white_image, white_image, 50, 250);

			// 6. HoughLinesP ���� ����
			DL.Trans_Hough(white_image);

			// * 50������ �̻� �˻��ؼ� �����ߴ���
			lane_detect = DL.Done_detectLane();

		}
		else {
			// �̵�ü ����

			Mat diff;

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
				erode(diff, diff, Mat::ones(Size(3, 3), CV_8UC1), Point(-1, -1), 1);

				// * ������ ���� ��(����)�� �׷��ִ� ��
				line(frame, Point(320, 50), Point(400, 50), Scalar(0, 0, 255), 1, 8, 0);
				line(frame, Point(30, 350), Point(580, 350), Scalar(0, 255, 0), 1, 8, 0);
				

				// * ���� �׸���
				DL.DrawRoadLane(frame);

				// 6. contours ã��
				vector<vector<Point>> contours;
				vector<Vec4i> hierarchy;
				findContours(diff, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

				// 7. ���� �� �ν� �� �����
				DV.DrawAndEraseVehicle(contours, frame, DL);

				imshow("Live", frame);
			}
		

			cap.read(background);
		}

		//int fps = cap.get(CAP_PROP_FPS);

		int wait = int(1.0 / 100 * 1000);

		if (waitKey(wait) >= 0)
			break;

		
	}

	return 0;
}