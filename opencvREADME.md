#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>）
using namespace cv;
using namespace std;

void getredColor(Mat imgHSV, Mat imgResize) {

	Scalar lower_red1(0, 50, 50);  
	Scalar upper_red1(10, 255, 255);
	Scalar lower_red2(160, 50, 50); 
	Scalar upper_red2(180, 255, 255);


	Mat mask1, mask2, mask;
	inRange(imgHSV, lower_red1, upper_red1, mask1);  
	inRange(imgHSV, lower_red2, upper_red2, mask2);  
	mask = mask1 | mask2;                        
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));  // 定义3x3矩形核
	morphologyEx(mask, mask, MORPH_OPEN, kernel, Point(-1, -1), 1);   //去噪
	morphologyEx(mask, mask, MORPH_CLOSE, kernel, Point(-1, -1), 1);  //填充

	vector<vector<Point>> redcontours;
	findContours(mask, redcontours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  // 查找外部轮廓
	for (size_t i = 0; i < redcontours.size(); i++) {
		vector<vector<Point>> conPoly(redcontours.size());
		vector<Rect> boundRect(redcontours.size());
		double area = contourArea(redcontours[i]);  // 计算轮廓面积
		float peri = arcLength(redcontours[i], true);
		approxPolyDP(redcontours[i], conPoly[i], 0.005 * peri, true);
		boundRect[i] = boundingRect(conPoly[i]);
		int objCor = (int)conPoly[i].size();
		Point2f center;
		float radius;
		minEnclosingCircle(redcontours[i], center, radius);
		float circle_area = CV_PI * radius * radius;
		float area_ratio = area / circle_area;
		float circularity = (4 * CV_PI * area) / (peri * peri);
		if (area > 5000 && objCor > 10 && circularity >0.3&&area_ratio >0.7) { 
			Rect bbox = boundingRect(redcontours[i]);  // 获取最小外接矩形
			rectangle(imgResize, bbox, Scalar(0, 255, 0), 2); 
			putText(imgResize, "red", Point(boundRect[i].x, boundRect[i].y - 10), FONT_HERSHEY_DUPLEX, 0.8, Scalar(255, 0, 0), 2);
		}
	}
}
void getgreenColor(Mat imgHSV,Mat imgResize){
	Mat imgBlur;
	GaussianBlur(imgHSV, imgBlur, Size(5, 5), 0);
	Scalar lower_green(35, 50, 50); 
	Scalar upper_green(85, 255, 255); 
	Mat maskgreen;
	inRange(imgBlur, lower_green, upper_green, maskgreen);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));
	morphologyEx(maskgreen, maskgreen, MORPH_OPEN, kernel, Point(-1, -1), 1);   // 开运算（去噪）
	morphologyEx(maskgreen, maskgreen, MORPH_OPEN, kernel, Point(-1, -1), 1);   // 开运算（去噪）
	morphologyEx(maskgreen, maskgreen, MORPH_CLOSE, kernel, Point(-1, -1), 1);  // 闭运算（填充）

	vector<vector<Point>> greencontours;
	findContours(maskgreen, greencontours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // 提取外部轮廓
	
	for (size_t i = 0; i < greencontours.size(); i++) {
		vector<Point> conPoly;
		Rect bbox;
		vector<Rect> boundRect(greencontours.size());
		double area = contourArea(greencontours[i]);  // 计算轮廓面积
		float peri = arcLength(greencontours[i], true);
					approxPolyDP(greencontours[i], conPoly, 0.001 * peri, true);
				/*	drawContours(imgResize, conPoly, i, Scalar(255, 0, 255), 3);*/
				/*	cout << conPoly[i].size() << endl;*/
					bbox = boundingRect(greencontours[i]);
					int objCor = (int)conPoly.size();
					Point2f center;
					float radius;
					minEnclosingCircle(greencontours[i], center, radius);
					float circle_area = CV_PI * radius * radius;
					float area_ratio = area / circle_area;
					float circularity = (4 * CV_PI * area) / (peri * peri);
		if (area >2300 && objCor > 5 &&circularity > 0.21&&area_ratio > 0.1) {  
			
			rectangle(imgResize,bbox, Scalar(0, 255, 0), 2);  
			Point textPos(bbox.x, bbox.y - 10);
			putText(imgResize, "green", textPos, FONT_HERSHEY_DUPLEX, 0.8, Scalar(255, 0, 0), 2);
		
		}
	}
}
int main() {
	string path = "resource/TrafficLight.mp4";
	VideoCapture cap(path);
	string output_folder = "resource/";
	string output_path = output_folder + "output.avi";
	double fps = cap.get(CAP_PROP_FPS);  // 视频帧率
	Mat temp_frame;
	cap.read(temp_frame);  // 读一帧原始帧
	if (temp_frame.empty()) {
		cout << "视频为空！" << endl;
		return -1;
	}
	resize(temp_frame, temp_frame, Size(), 0.7, 0.7);  // 和你的resize逻辑一致（0.7缩放）
	Size frame_size(temp_frame.cols, temp_frame.rows);  // 处理后的帧尺寸

	// (3) 初始化VideoWriter（四字符编解码器选XVID，对应.avi格式）
	VideoWriter writer(
		output_path,                // 输出路径
		VideoWriter::fourcc('X', 'V', 'I', 'D'),  // 编解码器（XVID是常用AVI编解码器）
		fps,                      // 帧率（与原视频一致）
		frame_size,               // 帧尺寸（与处理后的帧一致）
		true                      // 是否为彩色视频
	);
	if (!writer.isOpened()) {
		cout << "VideoWriter初始化失败！请检查路径或编解码器。" << endl;
		return -1;
	}
	Mat img, imgResize, imgGray, imgBlur, imgCanny, imgDil, imgErode, imgHSV;
	while (true) {
		cap.read(img);
		if (img.empty()) break;
		resize(img, imgResize, Size(),0.7, 0.7);
		cvtColor(imgResize, imgGray, COLOR_BGR2GRAY);
		cvtColor(imgResize, imgHSV,COLOR_BGR2HSV);
		GaussianBlur(imgGray, imgBlur, Size(5,5),5, 0);
		Canny(imgBlur, imgCanny, 10, 5);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));
		dilate(imgCanny, imgDil, kernel);
		erode(imgDil, imgErode, kernel);
		getredColor(imgHSV, imgResize);
		getgreenColor(imgHSV, imgResize);
		writer.write(imgResize);
		imshow("Image", imgResize);
		/*writer.write(imgResize);*/
		//imshow("ImageErode", imgErode);
		/*imshow("ImageDil", imgDil);*/
		waitKey(15);

	}
	writer.release();  // 释放VideoWriter
	cap.release();
	destroyAllWindows();

	return 0;
}
