#pragma once
#include "Coreminimal.h"

void SaveBitMapImage(HBITMAP hBmp, string filename)
{
    filename += ".bmp";
    LONG imageSize;       // �摜�T�C�Y
    BITMAPFILEHEADER fh;  // �r�b�g�}�b�v�t�@�C���w�b�_
    BITMAPINFO* pbi;      // �r�b�g�}�b�v���
    BITMAP bmp = { 0 };    // �r�b�g�}�b�v�\����
    LONG bpp;             // ��f��
    LPBYTE bits;          // �摜�r�b�g
    HDC hdc;              // �f�o�C�X�R���e�L�X�g
    HDC hdc_mem;          // �f�o�C�X�R���e�L�X�g�E������
    HANDLE hFile;         // �t�@�C���n���h��
    DWORD writeSize;      // �������񂾃T�C�Y

    // BITMAP�����擾����
    GetObject(hBmp, sizeof(bmp), &bmp);
    hdc = GetDC(0);
    hdc_mem = CreateCompatibleDC(hdc);
    ReleaseDC(0, hdc);
    HBITMAP hBmpOld = (HBITMAP)SelectObject(hdc_mem, hBmp);

    // �t�@�C���T�C�Y�v�Z
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

    // BITMAPFILEHEADER�w�b�_�[�o��
    ZeroMemory(&fh, sizeof(fh));
    memcpy(&fh.bfType, "BM", 2);
    fh.bfSize = imageSize;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
        + sizeof(RGBQUAD) * bpp;

    // BITMAPINFOHEADER�w�b�_�[�o��
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

    // �摜�f�[�^�𓾂�
    bits = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];
    GetDIBits(hdc_mem, hBmp, 0, bmp.bmHeight, bits, pbi, DIB_RGB_COLORS);

    // �t�@�C���ɏ�������
    hFile = CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, &fh, sizeof(fh), &writeSize, NULL);
    WriteFile(hFile, pbi,
        sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bpp, &writeSize, NULL);
    WriteFile(hFile, bits, bmp.bmWidthBytes * bmp.bmHeight, &writeSize, NULL);
    CloseHandle(hFile);

    // �J��
    delete[] pbi;
    delete[] bits;
    SelectObject(hdc_mem, hBmpOld);
    DeleteDC(hdc_mem);
}