#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#pragma comment(lib, "freetype.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID OnPaint(HDC);
VOID InitFreetype();
VOID UninitFreetype();

int APIENTRY wWinMain(_In_ HINSTANCE    hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR        lpCmdLine,
                     _In_ int           nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    InitFreetype();

    wndClass.style          = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc    = WndProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = hInstance;
    wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName   = NULL;
    wndClass.lpszClassName  = TEXT("Freetype+Gdiplus");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("Freetype+Gdiplus"), // window class name
        TEXT("Freetype+Gdiplus"), // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        500,                      // initial x size
        450,                      // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    UninitFreetype();

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

FT_Library ftLib;
FT_Face    ftFace;

VOID InitFreetype()
{
    int error;
    
    error = FT_Init_FreeType(&ftLib);
    if (error) { /* TODO */ }

    char fontFile[MAX_PATH] = {0};
    GetWindowsDirectoryA(fontFile, MAX_PATH);
    strcat(fontFile, "\\fonts\\calibri.ttf");
    error = FT_New_Face(ftLib, fontFile, 0, &ftFace);
    if (error) { /* TODO */ }

    error = FT_Set_Char_Size(
        ftFace,
        0,         /* char_width in 1/64th of points  */
        100 * 64,  /* char_height in 1/64th of points */
        96,        /* horizontal device resolution    */
        96);       /* vertical device resolution      */
    if (error) { /* TODO */ }
}

VOID UninitFreetype()
{
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);
}

struct DrawContext
{
    PointF glyphOrigin;
    PointF lastPt;
    GraphicsPath path;
    bool figureStarted;
};

static PointF pt_to_device(const FT_Vector* ft_pt, const PointF& origin)
{
    return PointF(
        origin.X + ft_pt->x / 64.0f, 
        origin.Y - ft_pt->y / 64.0f);
}

static int move_to(const FT_Vector* to, void* user)
{
    DrawContext* context = (DrawContext*)user;
    if (context->figureStarted)
        context->path.CloseFigure();
    context->path.StartFigure();
    context->figureStarted = true;
    PointF pt = pt_to_device(to, context->glyphOrigin);
    context->lastPt = pt;
    return 0;
}

static int line_to(const FT_Vector* to, void* user)
{
    DrawContext* context = (DrawContext*)user;
    PointF pt = pt_to_device(to, context->glyphOrigin);
    context->path.AddLine(context->lastPt, pt);
    context->lastPt = pt;
    return 0;
}

static int conic_to(const FT_Vector* control, const FT_Vector* to, void* user)
{
    DrawContext* context = (DrawContext*)user;
    PointF ptControl = pt_to_device(control, context->glyphOrigin);
    PointF ptTo = pt_to_device(to, context->glyphOrigin);
    // https://stackoverflow.com/questions/3162645/convert-a-quadratic-bezier-to-a-cubic-one
    PointF pts[4];
    pts[0] = context->lastPt;
    pts[3] = ptTo;
    pts[1].X = (pts[0].X + ptControl.X + ptControl.X) / 3;
    pts[1].Y = (pts[0].Y + ptControl.Y + ptControl.Y) / 3;
    pts[2].X = (pts[3].X + ptControl.X + ptControl.X) / 3;
    pts[2].Y = (pts[3].Y + ptControl.Y + ptControl.Y) / 3;
    context->path.AddBezier(pts[0], pts[1], pts[2], pts[3]);
    context->lastPt = ptTo;
    return 0;
}

static int cubic_to(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user)
{
    DrawContext* context = (DrawContext*)user;
    PointF ptControl1 = pt_to_device(control1, context->glyphOrigin);
    PointF ptControl2 = pt_to_device(control2, context->glyphOrigin);
    PointF ptTo = pt_to_device(to, context->glyphOrigin);
    context->path.AddBezier(context->lastPt, ptControl1, ptControl2, ptTo);
    context->lastPt = ptTo;
    return 0;
}

static void DrawCharPath(Graphics& graphics, const Color& color, const PointF& pt, char c)
{
    int error = FT_Load_Char(ftFace, c, FT_LOAD_DEFAULT);
    if (error) { /* TODO */ }

    FT_Outline_Funcs func_interface = {0};
    func_interface.move_to  = move_to;
    func_interface.line_to  = line_to;
    func_interface.conic_to = conic_to;
    func_interface.cubic_to = cubic_to;

    DrawContext context;
    context.glyphOrigin = pt;

    error = FT_Outline_Decompose(&ftFace->glyph->outline, &func_interface, &context);
    if (error) { /* TODO */ }

    context.path.CloseAllFigures();
    SolidBrush brush(color);
    graphics.FillPath(&brush, &context.path);
}

static void DrawCharBitmap(Graphics& graphics, const Color& color, const PointF& pt, char c)
{
    int error = FT_Load_Char(ftFace, c, FT_LOAD_DEFAULT);
    if (error) { /* TODO */ }
    
    error = FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);
    if (error) { /* TODO */ }

    const FT_Bitmap* glyphBitmap = &ftFace->glyph->bitmap;
    const unsigned char* pixelBuffer = glyphBitmap->buffer;
    Bitmap bitmap(glyphBitmap->width, glyphBitmap->rows, &graphics);
    for (unsigned int y = 0; y < glyphBitmap->rows; y++)
    {
        for (unsigned int x = 0; x < glyphBitmap->width; x++)
        {
            Color pixelColor(
                (BYTE)((int)(*pixelBuffer) * color.GetA() / 255),
                color.GetR(), color.GetG(), color.GetB());
            bitmap.SetPixel(x, y, pixelColor);
            pixelBuffer++;
        }
    }

    PointF origin(
        pt.X + ftFace->glyph->bitmap_left, 
        pt.Y - ftFace->glyph->bitmap_top);
    graphics.DrawImage(&bitmap, origin);
}

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    Color color(255, 0, 0, 0);

    DrawCharPath(graphics, color, PointF(20.0f, 120.0f), 'A');
    DrawCharPath(graphics, color, PointF(120.0f, 120.0f), 'B');

    graphics.SetSmoothingMode(SmoothingModeHighQuality);
    DrawCharPath(graphics, color, PointF(20.0f, 240.0f), 'A');
    DrawCharPath(graphics, color, PointF(120.0f, 240.0f), 'B');
    graphics.SetSmoothingMode(SmoothingModeDefault);

    DrawCharBitmap(graphics, color, PointF(20.0f, 360.0f), 'A');
    DrawCharBitmap(graphics, color, PointF(120.0f, 360.0f), 'B');
}
