#include <windows.h>
#include <shellapi.h>

// 定义托盘消息和菜单ID
#define WM_TRAYICON (WM_USER + 1)
#define IDM_EXIT 1001

// 定义颜色选项ID
#define IDM_COLOR_RED 2001
#define IDM_COLOR_GREEN 2002
#define IDM_COLOR_CYAN 2003
#define IDM_COLOR_YELLOW 2004
#define IDM_COLOR_WHITE 2005
#define IDM_COLOR_MAGENTA 2006

// 全局变量
HWND g_hwndOverlay = NULL;
COLORREF g_crosshairColor = RGB(255, 0, 0); // 默认准星颜色：红色

// 声明窗口过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void AddTrayIcon(HWND hwnd) {
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));
    lstrcpy(nid.szTip, TEXT("FPS 准星大师")); // 悬停提示
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon(HWND hwnd) {
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

// 核心绘图逻辑
void DrawCrosshair(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // 填充纯黑背景 (将被系统透明化)
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // 创建画笔绘制十字
    HPEN hPen = CreatePen(PS_SOLID, 2, g_crosshairColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int size = 40;
    int center = size / 2;
    int length = 10;

    // 画水平和垂直线
    MoveToEx(hdc, center - length, center, NULL);
    LineTo(hdc, center + length, center);
    MoveToEx(hdc, center, center - length, NULL);
    LineTo(hdc, center, center + length);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    EndPaint(hwnd, &ps);
}

// 显示带颜色选项的托盘右键菜单
void ShowTrayMenu(HWND hwnd) {
    POINT pt;
    GetCursorPos(&pt); 
    
    // 创建主菜单和子菜单
    HMENU hMenu = CreatePopupMenu();
    HMENU hColorMenu = CreatePopupMenu();
    
    // 添加颜色子菜单项
    AppendMenu(hColorMenu, MF_STRING, IDM_COLOR_RED, TEXT("红色 (Red)"));
    AppendMenu(hColorMenu, MF_STRING, IDM_COLOR_GREEN, TEXT("绿色 (Green)"));
    AppendMenu(hColorMenu, MF_STRING, IDM_COLOR_CYAN, TEXT("青色 (Cyan)"));
    AppendMenu(hColorMenu, MF_STRING, IDM_COLOR_YELLOW, TEXT("黄色 (Yellow)"));
    AppendMenu(hColorMenu, MF_STRING, IDM_COLOR_WHITE, TEXT("白色 (White)"));
    AppendMenu(hColorMenu, MF_STRING, IDM_COLOR_MAGENTA, TEXT("紫红色 (Magenta)"));

    // 将子菜单挂载到主菜单
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hColorMenu, TEXT("选择准星颜色"));
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL); // 分割线
    AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("退出程序"));

    SetForegroundWindow(hwnd);
    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
    DestroyMenu(hMenu);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "CrosshairOverlayClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClass(&wc)) return 0;

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int size = 40;
    int x = (screenW - size) / 2;
    int y = (screenH - size) / 2;

    g_hwndOverlay = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        CLASS_NAME, "FPS Crosshair", WS_POPUP,
        x, y, size, size, NULL, NULL, hInstance, NULL
    );

    if (g_hwndOverlay == NULL) return 0;

    SetLayeredWindowAttributes(g_hwndOverlay, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(g_hwndOverlay, SW_SHOW);
    AddTrayIcon(g_hwndOverlay);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT:
            DrawCrosshair(hwnd);
            return 0;

        case WM_TRAYICON:
            if (lParam == WM_RBUTTONUP) {
                ShowTrayMenu(hwnd);
            }
            return 0;

        case WM_COMMAND:
            // 处理菜单点击事件
            switch (LOWORD(wParam)) {
                case IDM_EXIT:
                    DestroyWindow(hwnd);
                    break;
                // 切换颜色并强制重新绘制窗口
                case IDM_COLOR_RED: g_crosshairColor = RGB(255, 0, 0); InvalidateRect(hwnd, NULL, TRUE); break;
                case IDM_COLOR_GREEN: g_crosshairColor = RGB(0, 255, 0); InvalidateRect(hwnd, NULL, TRUE); break;
                case IDM_COLOR_CYAN: g_crosshairColor = RGB(0, 255, 255); InvalidateRect(hwnd, NULL, TRUE); break;
                case IDM_COLOR_YELLOW: g_crosshairColor = RGB(255, 255, 0); InvalidateRect(hwnd, NULL, TRUE); break;
                case IDM_COLOR_WHITE: g_crosshairColor = RGB(255, 255, 255); InvalidateRect(hwnd, NULL, TRUE); break;
                case IDM_COLOR_MAGENTA: g_crosshairColor = RGB(255, 0, 255); InvalidateRect(hwnd, NULL, TRUE); break;
            }
            return 0;

        case WM_DESTROY:
            RemoveTrayIcon(hwnd);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}