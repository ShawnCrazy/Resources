#include "pch.h"
#include "CChrDecoder.h"

//定义utf8所需要的字节
#define BYTE8 0xFF //定义8个字节的前置，一般不存在这种情况
#define BYTE7 0xFE //定义7个字节的前置
#define BYTE6 0xFC //定义6个字节的前置
#define BYTE5 0xF8 //定义5个字节的前置
#define BYTE4 0xF0 //定义4个字节的前置
#define BYTE3 0xE0 //定义3个字节的前置
#define BYTE2 0xC0 //定义2个字节的前置
#define BYTEPREFIX 0x80 //定义utf8的引导字节前置

CChrDecoder::CChrDecoder()
{
  m_buffer = new byte[1024];
}


CChrDecoder::~CChrDecoder()
{
  delete [] m_buffer;
}

bool CChrDecoder::MultiByteToWideChar(byte *src, size_t srcLen, wchar_t *dst, size_t dstLen)
{
  byte chr = '\0';
  size_t ret = 0; //保存返回的字节长
  size_t bytesLen = 0; //保存待处理字节长

  for (size_t i = 0; i < srcLen; i++)
  {
    chr = *(src + i);
    if ((chr & BYTEPREFIX) == BYTEPREFIX) //如果非ASCII码就不处理
    {
      if ((chr & BYTE8) == BYTE8)
      {
        bytesLen = 8;
        chr &= ~BYTE8;
        goto _DEAL;
      }
      else if ((chr & BYTE7) == BYTE7)
      {
        bytesLen = 7;
        chr &= ~BYTE7;
        goto _DEAL;
      }
      else if ((chr & BYTE6) == BYTE6)
      {
        bytesLen = 6;
        chr &= ~BYTE6;
        goto _DEAL;
      }
      else if ((chr & BYTE5) == BYTE5)
      {
        bytesLen = 5;
        chr &= ~BYTE5;
        goto _DEAL;
      }
      else if ((chr & BYTE4) == BYTE4)
      {
        bytesLen = 4;
        chr &= ~BYTE4;
        goto _DEAL;
      }
      else if ((chr & BYTE3) == BYTE3)
      {
        bytesLen = 3;
        chr &= ~BYTE3;
        goto _DEAL;
      }
      else if ((chr & BYTE2) == BYTE2)
      {
        bytesLen = 2;
        chr &= ~BYTE2;
        goto _DEAL;
      }
    }
    continue;
  _DEAL:
    if (i + bytesLen >= srcLen)
    {
      break; //容错
    }
    size_t calc = 0; //记录已使用的位
    size_t byteIndex; //记录已使用的字节
    byte used;
    byte *pDst = (byte *)dst;
    byte result[10] = { 0 }; //如果需要小端序则用此变量缓存
    for (size_t j = bytesLen - 1; j > 0; j--, calc += 6)
    {
      byteIndex = calc / 8;
      used = 0x3f & src[i + j];
      pDst[byteIndex] |= (used << (calc % 8));
      pDst[byteIndex + 1] |= used >> (8 - calc % 8);
    }
    //计算最后一位
    byteIndex = calc / 8;
    if (calc % 8 != 0)
    {
      pDst[byteIndex] |= (chr << (calc % 8));
    }
    dst = dst + (byteIndex + 1) / sizeof(wchar_t);
    dstLen = dstLen - byteIndex / sizeof(wchar_t);
    i += bytesLen - 1;
  }
  return true;
}