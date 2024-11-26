// 메인 프레임 클래스 정의
class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    wxBitmapButton* startButton;    // 얼굴 인식 버튼
    wxBitmapButton* openFileButton; // 파일에서 사진 찾기 버튼
    wxBitmapButton* teamButton;     // 'Team asdf' 버튼
    wxBitmapButton* exitButton;     // 종료 버튼

    void OnStart(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnTeamButton(wxCommandEvent& event);  // 'Team asdf' 버튼 이벤트 핸들러
    void OnExit(wxCommandEvent& event);

    void RunFaceDetection();  // 얼굴 인식 실행 함수

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(1001, MainFrame::OnStart)
EVT_BUTTON(1002, MainFrame::OnOpenFile)
EVT_BUTTON(1004, MainFrame::OnTeamButton)
EVT_BUTTON(1003, MainFrame::OnExit)
wxEND_EVENT_TABLE()

// 메인 프레임 생성자
MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {

    // 아이콘 설정 (아이콘 파일 경로를 설정)
    wxIcon icon;
    icon.LoadFile("iconzz.ico", wxBITMAP_TYPE_ICO); // 아이콘 파일 경로 설정
    SetIcon(icon);  // 윈도우 아이콘으로 설정

    // 배경색 설정
    SetBackgroundColour(*wxBLACK);

    // 레이아웃 설정
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* logoText = new wxStaticText(this, wxID_ANY, wxT("관觀\n상相"));
    logoText->SetPosition(wxPoint(10, 10));  // (10, 10) 위치에 배치
    wxFont logoFont(50, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("궁서"));
    logoText->SetFont(logoFont);
    logoText->SetForegroundColour(*wxWHITE); // 텍스트 색상 흰색으로 설정

    wxImage::AddHandler(new wxPNGHandler()); // PNG 파일 지원 핸들러 추가
    // 버튼 이미지 생성 및 리사이즈
    wxImage startImage(wxT("cam.png"), wxBITMAP_TYPE_PNG);
    wxImage openFileImage(wxT("photo.png"), wxBITMAP_TYPE_PNG);
    wxImage teamImage(wxT("team.png"), wxBITMAP_TYPE_PNG);
    wxImage exitImage(wxT("exit.png"), wxBITMAP_TYPE_PNG);

    startImage.Rescale(100, 100);
    openFileImage.Rescale(100, 100);
    teamImage.Rescale(100, 100);
    exitImage.Rescale(100, 100);

    // 버튼 생성
    startButton = new wxBitmapButton(this, 1001, wxBitmap(startImage), wxDefaultPosition, wxSize(100, 100));
    openFileButton = new wxBitmapButton(this, 1002, wxBitmap(openFileImage), wxDefaultPosition, wxSize(100, 100));
    teamButton = new wxBitmapButton(this, 1004, wxBitmap(teamImage), wxDefaultPosition, wxSize(100, 100));
    exitButton = new wxBitmapButton(this, 1003, wxBitmap(exitImage), wxDefaultPosition, wxSize(100, 100));

    // 텍스트 추가
    wxStaticText* startText = new wxStaticText(this, wxID_ANY, wxT("얼굴 인식"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxStaticText* openFileText = new wxStaticText(this, wxID_ANY, wxT("파일에서 사진 찾기"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxStaticText* teamText = new wxStaticText(this, wxID_ANY, wxT("Team asdf"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxStaticText* exitText = new wxStaticText(this, wxID_ANY, wxT("종료"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);

    // 폰트 설정
    wxFont textFont(10, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("궁서"));
    startText->SetFont(textFont);
    openFileText->SetFont(textFont);
    teamText->SetFont(textFont);
    exitText->SetFont(textFont);

    // 텍스트 색상 설정 (흰색)
    startText->SetForegroundColour(*wxWHITE);
    openFileText->SetForegroundColour(*wxWHITE);
    teamText->SetForegroundColour(*wxWHITE);
    exitText->SetForegroundColour(*wxWHITE);

    // 버튼과 텍스트를 묶을 BoxSizer 생성
    wxBoxSizer* startSizer = new wxBoxSizer(wxVERTICAL);
    startSizer->Add(startButton, 0, wxALIGN_CENTER);
    startSizer->Add(startText, 0, wxALIGN_CENTER | wxTOP, 5);

    wxBoxSizer* openFileSizer = new wxBoxSizer(wxVERTICAL);
    openFileSizer->Add(openFileButton, 0, wxALIGN_CENTER);
    openFileSizer->Add(openFileText, 0, wxALIGN_CENTER | wxTOP, 5);

    wxBoxSizer* teamSizer = new wxBoxSizer(wxVERTICAL);
    teamSizer->Add(teamButton, 0, wxALIGN_CENTER);
    teamSizer->Add(teamText, 0, wxALIGN_CENTER | wxTOP, 5);

    wxBoxSizer* exitSizer = new wxBoxSizer(wxVERTICAL);
    exitSizer->Add(exitButton, 0, wxALIGN_CENTER);
    exitSizer->Add(exitText, 0, wxALIGN_CENTER | wxTOP, 5);

    // 버튼과 텍스트를 포함하는 Sizer를 설정
    wxGridSizer* buttonSizer = new wxGridSizer(2, 2, 10, 10);
    buttonSizer->Add(startSizer, 0, wxALIGN_CENTER);
    buttonSizer->Add(openFileSizer, 0, wxALIGN_CENTER);
    buttonSizer->Add(teamSizer, 0, wxALIGN_CENTER);
    buttonSizer->Add(exitSizer, 0, wxALIGN_CENTER);

    // 레이아웃 설정
    mainSizer->Add(logoText, 0, wxALIGN_CENTER | wxTOP, 20);   // 텍스트 상단에 배치
    mainSizer->AddStretchSpacer(1);                           // 로고와 버튼 사이의 여백 추가
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 20); // 버튼을 중앙에 배치
    mainSizer->AddStretchSpacer(1);                           // 버튼 하단 여유 공간 추가
    // 버튼 하단 여유 공간 추가

    SetSizer(mainSizer);
    Centre(); // 화면 중앙에 배치
}
