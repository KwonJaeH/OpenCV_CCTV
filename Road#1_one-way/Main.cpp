#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "detect_vehicle.h"
#include <iostream>

//덕포교차로
#define URL "CCTV 영상"

using namespace cv;
using namespace std;


int main(void) {

	DetectVehicle DV;

	Mat frame, background;

	VideoCapture cap(URL);
	if (!cap.isOpened())
	{
		cerr << "카메라를 열 수 없습니다." << endl;
		return -1;
	}

	Mat diff;


	while (1)
	{

		cap.read(frame);
		if (frame.empty())
		{
			cerr << "캡쳐 실패" << endl;
			break;
		}

		// 1. gray 변환
		Mat gray_frame, gray_bg;
		cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
		if (!background.empty()) {
			cvtColor(background, gray_bg, COLOR_BGR2GRAY);

			
			// 2. 이전 프레임과의 차이 저장
			absdiff(gray_frame, gray_bg, diff);

			// 3. 이진파일로 변환
			threshold(diff, diff, 50, 255, THRESH_BINARY);

			// 4. 관심영역 설정
			diff = DV.set_ROI(diff);

			// 5. 팽창 -> 침식, closing 연산
			dilate(diff, diff, Mat::ones(Size(7, 7), CV_8UC1), Point(-1, -1), 2);
			erode(diff, diff, Mat::ones(Size(7, 7), CV_8UC1), Point(-1, -1), 2);

			// * 프레임 위에 선을 구려주는 것
			line(frame, Point(80, 140), Point(120, 130), Scalar(0, 0, 255), 5, 8, 0);
			line(frame, Point(90, 258), Point(170, 232), Scalar(0, 0, 255), 5, 8, 0);


			// 6. contours 찾기
			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;
			findContours(diff, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

			// 7. 같은 차 인식 및 지우기
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
