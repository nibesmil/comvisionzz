#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 웹캡 캡처
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "웹캡을 열 수 없습니다." << endl;
        return -1;
    }

    // 얼굴, 눈, 입, 코 인식을 위한 Haar Cascade 로드
    CascadeClassifier face_cascade;
    CascadeClassifier eye_cascade;
    CascadeClassifier mouth_cascade;
    CascadeClassifier nose_cascade;

    // Haar Cascade XML 파일 경로 (필요한 XML 파일 경로로 수정)
    if (!face_cascade.load("haarcascade_frontalface_default.xml") ||
        !eye_cascade.load("haarcascade_eye.xml") ||
        !mouth_cascade.load("haarcascade_mcs_mouth.xml") ||
        !nose_cascade.load("haarcascade_mcs_nose.xml")) {
        cout << "Haar Cascade XML 파일을 로드할 수 없습니다." << endl;
        return -1;
    }

    Mat frame, gray;
    while (true) {
        cap >> frame;  // 웹캡으로부터 한 프레임 읽기

        if (frame.empty()) {
            break;
        }

        // 흑백 이미지로 변환
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        // 얼굴 인식
        vector<Rect> faces;
        face_cascade.detectMultiScale(gray, faces);

        for (size_t i = 0; i < faces.size(); i++) {
            // 얼굴에 사각형 그리기
            rectangle(frame, faces[i], Scalar(255, 0, 0), 2);

            // 얼굴 영역을 잘라서 눈, 입, 코 인식
            Mat faceROI = gray(faces[i]);

            // 눈 인식
            vector<Rect> eyes;
            eye_cascade.detectMultiScale(faceROI, eyes);
            for (size_t j = 0; j < eyes.size(); j++) {
                Point center(faces[i].x + eyes[j].x + eyes[j].width / 2,
                    faces[i].y + eyes[j].y + eyes[j].height / 2);
                int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
                circle(frame, center, radius, Scalar(0, 255, 0), 2);
            }

            // 입 인식
            vector<Rect> mouths;
            mouth_cascade.detectMultiScale(faceROI, mouths);
            for (size_t j = 0; j < mouths.size(); j++) {
                Point mouth_center(faces[i].x + mouths[j].x + mouths[j].width / 2,
                    faces[i].y + mouths[j].y + mouths[j].height / 2);
                rectangle(frame, faces[i].tl() + mouths[j].tl(), faces[i].tl() + mouths[j].br(), Scalar(0, 255, 255), 2);
            }

            // 코 인식
            vector<Rect> noses;
            nose_cascade.detectMultiScale(faceROI, noses);
            for (size_t j = 0; j < noses.size(); j++) {
                Point nose_center(faces[i].x + noses[j].x + noses[j].width / 2,
                    faces[i].y + noses[j].y + noses[j
                    ].height / 2);
                rectangle(frame, faces[i].tl() + noses[j].tl(), faces[i].tl() + noses[j].br(), Scalar(0, 0, 255), 2);
            }
        }

        // 결과 화면에 출력
        imshow("얼굴 인식", frame);

        // 'Esc' 키로 종료
        if (waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
