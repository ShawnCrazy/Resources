#pragma once
#include <string>
#include <iostream>
#include <windows.h>
class CChrDecoder
{
public:
  CChrDecoder();
  ~CChrDecoder();
  bool MultiByteToWideChar(byte *src, size_t srcLen, wchar_t *dst, size_t dstLen);
private:
  byte *m_buffer;
};

