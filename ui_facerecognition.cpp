#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <thread>

using namespace cv;
using namespace std;

// 메인 프레임 클래스 정의
class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    wxButton* startButton;    // 얼굴 인식 버튼
    wxButton* openFileButton; // 파일에서 사진 찾기 버튼
    wxButton* exitButton;     // 종료 버튼

    void OnStart(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    void RunFaceDetection();  // 얼굴 인식 실행 함수

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(1001, MainFrame::OnStart)
EVT_BUTTON(1002, MainFrame::OnOpenFile)
EVT_BUTTON(1003, MainFrame::OnExit)
wxEND_EVENT_TABLE()

// 메인 프레임 생성자
MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    // 배경색 설정
    SetBackgroundColour(*wxBLACK);

    // 레이아웃 설정
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // 상단 텍스트: "당신의 觀相을 봐드립니다." (흰색 텍스트)
    wxStaticText* logoText = new wxStaticText(this, wxID_ANY, wxT("당신의 觀相을 봐드립니다."),
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxFont logoFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    logoText->SetFont(logoFont);
    logoText->SetForegroundColour(*wxWHITE); // 텍스트 색상을 흰색으로 설정

    // 로고 이미지 추가
    wxImage::AddHandler(new wxPNGHandler()); // PNG 파일 지원 핸들러 추가
    wxImage logoImage;
    if (!logoImage.LoadFile("관 상.png", wxBITMAP_TYPE_PNG)) {
        wxMessageBox("로고 이미지를 불러올 수 없습니다. 파일 경로를 확인해주세요.", "오류", wxOK | wxICON_ERROR);
    }
    else {
        logoImage = logoImage.Scale(200, 100); // 로고 크기를 200x100으로 조정
    }
    wxStaticBitmap* logoBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(logoImage));

    // 버튼 생성 (크기 조정)
    startButton = new wxButton(this, 1001, wxT("얼굴 인식하기"), wxDefaultPosition, wxSize(200, 50));
    openFileButton = new wxButton(this, 1002, wxT("파일에서 사진 찾기"), wxDefaultPosition, wxSize(200, 50));
    exitButton = new wxButton(this, 1003, wxT("나가기"), wxDefaultPosition, wxSize(200, 50));

    // 하단 텍스트: "by. asdf" (왼쪽 하단)
    wxStaticText* footerText = new wxStaticText(this, wxID_ANY, wxT("by. asdf"),
        wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    wxFont footerFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
    footerText->SetFont(footerFont);
    footerText->SetForegroundColour(*wxWHITE); // 텍스트 색상을 흰색으로 설정

    // 요소들을 레이아웃에 추가
    mainSizer->Add(logoText, 0, wxALIGN_CENTER | wxTOP, 20);   // 텍스트 상단에 배치
    mainSizer->Add(logoBitmap, 0, wxALIGN_CENTER | wxTOP, 10); // 로고를 중앙 상단에 배치
    mainSizer->AddStretchSpacer(1);                           // 로고와 버튼 사이의 여백 추가
    mainSizer->Add(startButton, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(openFileButton, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(exitButton, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->AddStretchSpacer(1);                           // 버튼 하단 여유 공간 추가
    mainSizer->Add(footerText, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 10); // 하단에 텍스트 추가

    SetSizer(mainSizer);
    Centre(); // 화면 중앙에 배치
}

// 얼굴 인식 시작 함수
void MainFrame::OnStart(wxCommandEvent& event) {
    // 안내 문구 띄우기
    wxMessageBox("얼굴 인식을 시작합니다. 얼굴 정면을 잘 나오게 비춰주세요!", "안내", wxOK | wxICON_INFORMATION);

    // 얼굴 인식을 시작하기 전에 잠시 기다리기 (2초 대기)
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 별도의 스레드로 얼굴 인식 실행
    std::thread(&MainFrame::RunFaceDetection, this).detach();
}

// 파일 열기 처리 함수
void MainFrame::OnOpenFile(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("사진 파일 열기"), "", "",
        "이미지 파일 (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return; // 사용자가 파일 선택 취소
    }

    // 파일 확장자 검사
    wxString filePath = openFileDialog.GetPath();
    wxString fileExt = filePath.AfterLast('.').Lower();

    if (fileExt != "jpg" && fileExt != "png" && fileExt != "bmp") {
        wxBell(); // Windows 에러 효과음
        wxMessageBox("사진 파일만 첨부 가능합니다.", "오류", wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox("선택된 파일: " + filePath, "관상 확인", wxOK | wxICON_INFORMATION);
}

// 종료 함수
void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

// 얼굴 인식 실행 함수
void MainFrame::RunFaceDetection() {
    // 웹캡 캡처
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        wxMessageBox("웹캡을 열 수 없습니다.", "오류", wxOK | wxICON_ERROR);
        return;
    }

    // 얼굴, 눈, 입, 코 인식을 위한 Haar Cascade 로드
    CascadeClassifier face_cascade;
    CascadeClassifier eye_cascade;
    CascadeClassifier mouth_cascade;
    CascadeClassifier nose_cascade;

    // Haar Cascade XML 파일 경로
    if (!face_cascade.load("haarcascade_frontalface_default.xml") ||
        !eye_cascade.load("haarcascade_eye.xml") ||
        !mouth_cascade.load("haarcascade_mcs_mouth.xml") ||
        !nose_cascade.load("haarcascade_mcs_nose.xml")) {
        wxMessageBox("Haar Cascade XML 파일을 로드할 수 없습니다.", "오류", wxOK | wxICON_ERROR);
        return;
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
                    faces[i].y + noses[j].y + noses[j].height / 2);
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
}

// 애플리케이션 실행
class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MainFrame* frame = new MainFrame("얼굴 인식 프로그램");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
