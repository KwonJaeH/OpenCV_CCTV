#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>
#include "detect_vehicle.h"
#include "detect_lane.h"

//터널 2
#define URL "http://211.252.223.177:1935/live/985.stream/playlist.m3u8"

int main(void) {

	DetectVehicle DV;
	DetectLane DL;

	Mat frame, background;

	VideoCapture cap(URL);
	if (!cap.isOpened())
	{
		cerr << "카메라를 열 수 없습니다." << endl;
		return -1;
	}

	// 차선 추출 했는지

	bool lane_detect = false;
	
	while (1)
	{

		cap.read(frame);
		if (frame.empty())
		{
			cerr << "캡쳐 실패" << endl;
			break;
		}


		if (!lane_detect) {
			//차선 검출

			Mat white_image;

			// 1. 흰색 도출
			white_image = DL.ColorFilter(frame);
		
			// 2. gray 변환
			cvtColor(white_image, white_image, COLOR_BGR2GRAY);

			// 3. 관심 영역 설정
			white_image = DL.set_ROI(white_image);
			
			// 4. Blur 처리
			GaussianBlur(white_image, white_image, Size(7, 7), 0);
			
			// 5. Canny Edge 추출
			Canny(white_image, white_image, 50, 250);

			// 6. HoughLinesP 직선 추출
			DL.Trans_Hough(white_image);

			// * 50프레임 이상 검사해서 추출했는지
			lane_detect = DL.Done_detectLane();

		}
		else {
			// 이동체 추적

			Mat diff;

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
				erode(diff, diff, Mat::ones(Size(3, 3), CV_8UC1), Point(-1, -1), 1);

				// * 프레임 위에 선(범위)을 그려주는 것
				line(frame, Point(320, 50), Point(400, 50), Scalar(0, 0, 255), 1, 8, 0);
				line(frame, Point(30, 350), Point(580, 350), Scalar(0, 255, 0), 1, 8, 0);
				

				// * 차선 그리기
				DL.DrawRoadLane(frame);

				// 6. contours 찾기
				vector<vector<Point>> contours;
				vector<Vec4i> hierarchy;
				findContours(diff, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

				// 7. 같은 차 인식 및 지우기
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