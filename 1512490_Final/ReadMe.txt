ID: 1512490
Name: Nguyễn Hoài Thanh Tân
Email: nhthanhtan@gmail.com


Chức năng đã thực hiện được:
- Khi bật chương trình sẽ có chạy icon xuất hiện ở khung thông báo
- Chương trình sẽ lưu ghi chú của người dùng lại
- Chương trình cho phép người dùng xem lại ghi chú của mình
- Thống kê số ghi chú của từng tag và thống kê phân bố tag theo biểu đồ tròn
- Chương trình cho phép người dùng ẩn màn hình đi và dùng icon để:
+ Bật màn hình xem lại ghi chú
+ Bật màn hình thống kê
- Người dùng có thể nhấn tổ hợp phím ALT + SPACE để hiển thị màn hình cho phép người dùng thêm ghi chú

Một số hàm sử dụng:
-BOOL OnInitDialog(HWND hWnd);

-void ShowContextMenu(HWND hWnd);

-int CreateAddNotesWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd, LPWSTR WindowClass, int x, int y);

-int CreateListWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd, LPWSTR WindowClass, int x, int y);

-int CreateStatiticsWindow(HINSTANCE hInstance, int nCmdShow);

-int CreateSeparateListWindow(HINSTANCE hInstance, int nCmdShow);

-int CreateSeparateAddNoteWindow(HINSTANCE hInstance, int nCmdShow);

-int ChangeFont(HWND hwnd);

BOOL InitInstanceForListView(HINSTANCE hInstance, int nCmdShow);

-ATOM ListRegisterClass(HINSTANCE hInstance);

-LRESULT CALLBACK WndListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

-int loadTagToListView(HWND hwndListView);


-int InitPie(CPiePart * PiePart, int & index, float percentage);

-std::wstring AttatchResultString(std::wstring notificationString, float input);

-int drawPieChart(HINSTANCE hInstance, int nCmdShow, HWND hWnd, HDC & hdc, int x, int y);

-int CreatePiePart(CPiePart * PiePart, HINSTANCE hInstance, HWND hWnd, int x, int y);


-BOOL InitInstanceForStatitics(HINSTANCE hInstance, int nCmdShow);

-ATOM StatiticsRegisterClass(HINSTANCE hInstance);

-LRESULT CALLBACK WndStatiticsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


-CNote getNoteFromWindow(HWND hWnd);
std::vector<std::wstring> getWindowNoteTag(HWND hWnd);

-LPWSTR getWindowNoteContent(HWND hWnd);
LPWSTR getWindowNoteTitle(HWND hWnd);

-std::vector<std::wstring> separateTagFromText(LPWSTR TagsLine);

-std::vector<std::wstring> getAllPreviewNotesContent(std::wstring TagName);

-std::wstring getCurrentTag(HWND hWnd, int uID);

-int showPreviewNote(std::vector<std::wstring> nContents, HWND hWnd, WPARAM ID);

-std::vector<CNote> getPreviewNotes(std::wstring Tag);

-int getCurrentID(HWND hWnd, int wid, std::vector<CNote> PreviewNotes);

-int isTheSameString(LPWSTR fiStr, LPWSTR secString);


-int showNote(HWND hWnd, int nid, int ticounter, int ccounter, int tacounter);

-int AddTitle(HWND hWnd, std::wstring Title, int counter);

-int AddContent(HWND hWnd, std::wstring Content, int counter);

-int AddTags(HWND hWnd, std::vector<std::wstring> Tags, int counter);


-BOOL InitInstanceForAddNote(HINSTANCE hInstance, int nCmdShow);


-ATOM AddNoteRegisterClass(HINSTANCE hInstance);

link youtube: https://www.youtube.com/watch?v=8nMbO22GYdo&feature=youtu.be
