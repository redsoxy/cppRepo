// TongQi.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "TongQi.h"
#include <time.h>
#include <windowsx.h>
#include <vector>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名


#define EMPTYMJ 34

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



struct MJINFO {
    int h;
    int l;

    int s0;
    int s1;

    int x0;
    int x1;

    int z0;
    int z1;

    int y0;
    int y1;
};


struct CanXiao {
    MJINFO m1;
    MJINFO m2;
    int fangxiang; //m1 上下左右, m2上下左右
    int step;
};

std::vector<CanXiao> ok;

HDC hImageDC;

int MJ[8][17] = {
    23,2,24,5,27,0,34,34,24,8,16,30,31,13,18,4,1,

};


int curMoveH = -1, curMoveL = -1;


static int p1 = 498 / 9, p2 = 320 / 4;

//0-33

const WCHAR MJName [35][100] = {
    L"一万0", L"二万1", L"三万2", L"四万3", L"五万4", L"六万5", L"七万6", L"八万7", L"九万8",  //0-8
    L"一硕9", L"二硕10", L"三硕11", L"四硕12", L"五硕13", L"六硕14", L"七硕15", L"八硕16", L"九硕17",  //9-17
    L"一饼18", L"二饼19", L"三饼20", L"四饼21", L"五饼22", L"六饼23", L"七饼24", L"八饼25", L"九饼26",  //18-26
    L"北27", L"白板28", L"南29", L"中30", L"发31", L"东32", L"西33", L"空34" //27-34
};

void initMJPaintDC() {
    //  0 - 33
    //  500 / 9 = 55.5  300 /4 = 74
    //  136 = 17 * 8

    HBITMAP hMajiang = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MAJIANG));
    //BITMAP bitmap;
    //GetObject(hMajiang, sizeof(BITMAP), &bitmap);
    hImageDC = CreateCompatibleDC(NULL);
    SelectObject(hImageDC, hMajiang);
}

void initMJ() {
    for (int i = 0; i <= 33; ++i) {
        for (int j = 0; j < 4; ++j) {
            *((int*)(MJ)+i * 4 + j) = i;
        }
    }

    /*
    for (int i = 0; i < 40; ++i) {
        int k1 = rand() % 8;
        int k2 = rand() % 17;
        MJ[k1][k2] = EMPTYMJ;
    }*/

    
}


void render(HDC hdc) {
    
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hbitMap = CreateCompatibleBitmap(hdc, p1*17, p2*8);
    SelectObject(hMemDC, hbitMap);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 17; ++j) {
            int v = MJ[i][j];

            int h = v / 9;
            int w = v % 9;
     
            //55.5
            //75
            StretchBlt(hMemDC, j* p1, i* p2, p1, p2, hImageDC, w* p1, h* p2, p1, p2, SRCCOPY);
        }
    }
    for (unsigned int i = 0; i < ok.size(); ++i) {
        MJINFO one = ok[i].m1;
        MJINFO two = ok[i].m2;

        MoveToEx(hMemDC, one.l * p1 + p1 / 2, one.h * p2 + p2 / 2, NULL);
        LineTo(hMemDC, two.l * p1 + p1 / 2, two.h * p2 + p2 / 2);
    }

    //HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    HDC hSelectDC = CreateCompatibleDC(hdc);
    HBITMAP hbits = CreateCompatibleBitmap(hdc, p1, p2);
    //SelectObject(hSelectDC, brush);
    SelectObject(hSelectDC, hbits);
    RECT rc = { 0, 0, p1, p2 };
    FillRect(hSelectDC, &rc, CreateSolidBrush(RGB(100, 0, 0)));

    if (curMoveH >= 0 && curMoveL >= 0) {
        StretchBlt(hMemDC, curMoveL * p1, curMoveH * p2, p1, p2, hSelectDC, 0, 0, p1, p2, SRCINVERT);
    }

    StretchBlt(hdc, 0, 0, p1 * 17, p2 * 8, hMemDC, 0, 0, p1 * 17, p2 * 8, SRCCOPY);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
   

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TONGQI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TONGQI));

    srand((unsigned int)time(NULL));
    initMJPaintDC();
    initMJ();

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TONGQI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TONGQI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, p1*18, p2*9, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//






MJINFO getMJInfo(int pos) {
    MJINFO info;
    memset(&info, 0, sizeof(info));

    info.h = pos / 17;
    info.l = pos % 17;

    int i, j;

    for (i = info.l-1; i >= 0; --i) {
        if (MJ[info.h][i] != EMPTYMJ) {
            info.z0++;
        }
        else {
            break;
        }
    }
    for (j = i; j >= 0; --j) {
        if (MJ[info.h][j] == EMPTYMJ) {
            info.z1++;
        }
        else {
            break;
        }
    }


    for (i = info.l + 1; i < 17; ++i) {
        if (MJ[info.h][i] != EMPTYMJ) {
            info.y0++;
        }
        else {
            break;
        }
    }
    for (j = i; j < 17; ++j) {
        if (MJ[info.h][j] == EMPTYMJ) {
            info.y1++;
        }
        else {
            break;
        }
    }


    for (i = info.h - 1; i >= 0; --i) {
        if (MJ[i][info.l] != EMPTYMJ) {
            info.s0++;
        }
        else {
            break;
        }
    }
    for (j = i; j >= 0; --j) {
        if (MJ[j][info.l] == EMPTYMJ) {
            info.s1++;
        }
        else {
            break;
        }
    }


    for (i = info.h + 1; i < 8; ++i) {
        if (MJ[i][info.l] != EMPTYMJ) {
            info.x0++;
        }
        else {
            break;
        }
    }
    for (j = i; j < 8; ++j) {
        if (MJ[j][info.l] == EMPTYMJ) {
            info.x1++;
        }
        else {
            break;
        }
    }

    return info;
}

MJINFO getMJInfo(int h, int l) {
    return getMJInfo(h * 17 + l);
}




std::vector<CanXiao> checkCanMove(MJINFO m1, MJINFO m2) {
    //高度差
    int g = m2.h - m1.h;
    int s = m2.l - m1.l;
    int i;
    bool allEmpty;

    std::vector<CanXiao> v;

    CanXiao x;
   
    if (g == 0) {
        allEmpty = true;
        for (i = m1.l + 1; i < m2.l; ++i) {
            if (MJ[m2.h][i] != EMPTYMJ) {
                allEmpty = false;
                break;
            }
        }
        if (allEmpty) {
            x.m1 = m1;
            x.m2 = m2;
            x.fangxiang = 3;
            x.step = s;
            v.push_back(x);
        }
        return v;
    }

    if (s == 0) {
        allEmpty = true;
        for (i = m1.h + 1; i < m2.h; ++i) {
            if (MJ[i][m2.l] != EMPTYMJ) {
                allEmpty = false;
                break;
            }
        }
        if (allEmpty) {
            x.m1 = m1;
            x.m2 = m2;
            x.fangxiang = 1;
            x.step = g;
            v.push_back(x);
        }
        return v;
    }

    //m1往下
    if (m1.x1 >= g) {
        allEmpty = true;
        for (i = min(m1.l, m2.l)+1; i < max(m1.l,m2.l); ++i) {
            if (MJ[m2.h][i] != EMPTYMJ) {
                allEmpty = false;
                break;
            }
        }
        if (allEmpty) {
            x.m1 = m1;
            x.m2 = m2;
            x.fangxiang = 1;
            x.step = g;
            v.push_back(x);
        }
    }

    if (m2.s1 >= g) {
        allEmpty = true;
        for (i = min(m1.l, m2.l) + 1; i < max(m1.l, m2.l); ++i) {
            if (MJ[m1.h][i] != EMPTYMJ) {
                allEmpty = false;
                break;
            }
        }
        if (allEmpty) {
            x.m1 = m2;
            x.m2 = m1;
            x.fangxiang = 4;
            x.step = g;
            v.push_back(x);
        }
    }
    
    //m1在左边
    if (s >= 0) {

        if (m1.y1 >= s) { //m1向右
            allEmpty = true;
            for (i = m1.h + 1; i < m2.h; ++i) {
                if (MJ[i][m2.l] != EMPTYMJ) {
                    allEmpty = false;
                    break;
                }
            }
            if (allEmpty) {
                x.m1 = m1;
                x.m2 = m2;
                x.fangxiang = 3;
                x.step = s;
                v.push_back(x);
            }
        }

        if (m2.z1 >= s) { //m2向左
            allEmpty = true;
            for (i = m1.h + 1; i < m2.h; ++i) {
                if (MJ[i][m1.l] != EMPTYMJ) {
                    allEmpty = false;
                    break;
                }
            }
            if (allEmpty) {
                x.m1 = m2;
                x.m2 = m1;
                x.fangxiang = 6;
                x.step = s;
                v.push_back(x);
            }
        }
    }

    //m1在右边
    if (s <= 0) {
        if (m1.z1 >= (-s)) {
            allEmpty = true;
            for (i = m1.h + 1; i < m2.h; ++i) {
                if (MJ[i][m2.l] != EMPTYMJ) {
                    allEmpty = false;
                    break;
                }
            }
            if (allEmpty) {
                x.m1 = m1;
                x.m2 = m2;
                x.fangxiang = 2;
                x.step = -s;
                v.push_back(x);
            }
        }

        if (m2.y1 >= (-s)) {
            allEmpty = true;
            for (i = m1.h + 1; i < m2.h; ++i) {
                if (MJ[i][m1.l] != EMPTYMJ) {
                    allEmpty = false;
                    break;
                }
            }
            if (allEmpty) {
                x.m1 = m2;
                x.m2 = m1;
                x.fangxiang = 7;
                x.step = -s;
                v.push_back(x);
            }
        }
    }

    return v;
}


void check() {
    ok.clear();
    for (int cur = 0; cur < 17 * 8; ++cur) {
        int v = *((int*)(MJ)+cur);
        if (v != EMPTYMJ) {
            for (int next = cur + 1; next < 17 * 8; ++next) {
                int n = *((int*)(MJ)+next);
                if (n == v) {
                    //find same MJ
                    MJINFO curInfo = getMJInfo(cur);
                    MJINFO nextInfo = getMJInfo(next);
                    std::vector<CanXiao> v = checkCanMove(curInfo, nextInfo);
                    ok.insert(ok.end(), v.begin(), v.end());
                }
            }
        }
    }

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_CHONGZHI:
                ok.clear();
                curMoveH = curMoveL = -1;
                for (int i = 0; i < 1000; ++i) {
                    int k1 = rand() % 8;
                    int k2 = rand() % 17;
                    int v1 = rand() % 8;
                    int v2 = rand() % 17;
                    
                    int g = MJ[k1][k2];
                    MJ[k1][k2] = MJ[v1][v2];
                    MJ[v1][v2] = g;
                }
                RECT rc;
                GetClientRect(hWnd, &rc);
                InvalidateRect(hWnd, &rc, TRUE);
                break;
            case ID_CHECK:
                check();
                GetClientRect(hWnd, &rc);
                InvalidateRect(hWnd, &rc, TRUE);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        render(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
    {
        curMoveL = GET_X_LPARAM(lParam) / p1;
        curMoveH = GET_Y_LPARAM(lParam) / p2;
        if (MJ[curMoveH][curMoveL] == EMPTYMJ) {
            curMoveH = curMoveL = -1;
        }
        RECT rc;
        GetClientRect(hWnd, &rc);
        InvalidateRect(hWnd, &rc, TRUE);
    }
    break;
    case WM_RBUTTONDOWN :
    {
        int xPos = GET_X_LPARAM(lParam) / p1;
        int yPos = GET_Y_LPARAM(lParam) / p2;
        MJ[yPos][xPos] = EMPTYMJ;
        RECT rc;
        GetClientRect(hWnd, &rc);
        InvalidateRect(hWnd, &rc, TRUE);
        curMoveH = curMoveL = -1;
    }
    break;
    case WM_KEYDOWN:
    {
        if (wParam == 0x51) {
            check();
            RECT rc;
            GetClientRect(hWnd, &rc);
            InvalidateRect(hWnd, &rc, TRUE);
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        if (curMoveH < 0 || curMoveL < 0) {
            break;
        }
        MJINFO info = getMJInfo(curMoveH, curMoveL);
        int i;
        switch (wParam) {
        case 0x57:
            if (info.s1 > 0) {
                for (i = info.h - info.s0 - 1; i < info.h; ++i) {
                    MJ[i][info.l] = MJ[i + 1][info.l];
                }
                MJ[info.h][info.l] = EMPTYMJ;
                curMoveH--;
            }
            break;
        case 0x53:
            if (info.x1 > 0) {
                for (i = info.h + info.x0 + 1; i > info.h; --i) {
                    MJ[i][info.l] = MJ[i - 1][info.l];
                }
                MJ[info.h][info.l] = EMPTYMJ;
                curMoveH++;
            }
            break;
        case 0x41:
            if (info.z1 > 0) {
                for (i = info.l - info.z0 - 1; i < info.l; ++i) {
                    MJ[info.h][i] = MJ[info.h][i + 1];
                }
                MJ[info.h][info.l] = EMPTYMJ;
                curMoveL--;
            }
            break;
        case 0x44:
            if (info.y1 > 0) {
                for (i = info.l + info.y0 + 1; i > info.l; --i) {
                    MJ[info.h][i] = MJ[info.h][i - 1];
                }
                MJ[info.h][info.l] = EMPTYMJ;
                curMoveL++;
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        RECT rc;
        GetClientRect(hWnd, &rc);
        InvalidateRect(hWnd, &rc, TRUE);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}




