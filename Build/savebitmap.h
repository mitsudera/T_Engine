#pragma once
#include "Coreminimal.h"

void SaveBitMapImage(HBITMAP hBmp, string filename)
{
    filename += ".bmp";
    LONG imageSize;       // 画像サイズ
    BITMAPFILEHEADER fh;  // ビットマップファイルヘッダ
    BITMAPINFO* pbi;      // ビットマップ情報
    BITMAP bmp = { 0 };    // ビットマップ構造体
    LONG bpp;             // 画素数
    LPBYTE bits;          // 画像ビット
    HDC hdc;              // デバイスコンテキスト
    HDC hdc_mem;          // デバイスコンテキスト・メモリ
    HANDLE hFile;         // ファイラハンドル
    DWORD writeSize;      // 書き込んだサイズ

    // BITMAP情報を取得する
    GetObject(hBmp, sizeof(bmp), &bmp);
    hdc = GetDC(0);
    hdc_mem = CreateCompatibleDC(hdc);
    ReleaseDC(0, hdc);
    HBITMAP hBmpOld = (HBITMAP)SelectObject(hdc_mem, hBmp);

    // ファイルサイズ計算
    imageSize = bmp.bmWidthBytes * bmp.bmHeight
        + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    switch (bmp.bmBitsPixel)
    {
    case 2:
        bpp = 2;
        break;
    case 4:
        bpp = 16;
        break;
    case 8:
        bpp = 256;
        break;
    default:
        bpp = 0;
    }
    imageSize += (sizeof(RGBQUAD) * bpp);

    // BITMAPFILEHEADERヘッダー出力
    ZeroMemory(&fh, sizeof(fh));
    memcpy(&fh.bfType, "BM", 2);
    fh.bfSize = imageSize;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
        + sizeof(RGBQUAD) * bpp;

    // BITMAPINFOHEADERヘッダー出力
    pbi = new BITMAPINFO[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp];
    ZeroMemory(pbi, sizeof(BITMAPINFOHEADER));
    pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbi->bmiHeader.biWidth = bmp.bmWidth;
    pbi->bmiHeader.biHeight = bmp.bmHeight;
    pbi->bmiHeader.biPlanes = 1;
    pbi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    pbi->bmiHeader.biCompression = BI_RGB;
    if (bpp != 0)
    {
        GetDIBColorTable(hdc_mem, 0, bpp, pbi->bmiColors);
    }

    // 画像データを得る
    bits = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];
    GetDIBits(hdc_mem, hBmp, 0, bmp.bmHeight, bits, pbi, DIB_RGB_COLORS);

    // ファイルに書き込む
    hFile = CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, &fh, sizeof(fh), &writeSize, NULL);
    WriteFile(hFile, pbi,
        sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp, &writeSize, NULL);
    WriteFile(hFile, bits, bmp.bmWidthBytes * bmp.bmHeight, &writeSize, NULL);
    CloseHandle(hFile);

    // 開放
    delete[] pbi;
    delete[] bits;
    SelectObject(hdc_mem, hBmpOld);
    DeleteDC(hdc_mem);
}