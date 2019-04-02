#include "pch.h"
#include "CChrDecoder.h"

//����utf8����Ҫ���ֽ�
#define BYTE8 0xFF //����8���ֽڵ�ǰ�ã�һ�㲻�����������
#define BYTE7 0xFE //����7���ֽڵ�ǰ��
#define BYTE6 0xFC //����6���ֽڵ�ǰ��
#define BYTE5 0xF8 //����5���ֽڵ�ǰ��
#define BYTE4 0xF0 //����4���ֽڵ�ǰ��
#define BYTE3 0xE0 //����3���ֽڵ�ǰ��
#define BYTE2 0xC0 //����2���ֽڵ�ǰ��
#define BYTEPREFIX 0x80 //����utf8�������ֽ�ǰ��

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
  size_t ret = 0; //���淵�ص��ֽڳ�
  size_t bytesLen = 0; //����������ֽڳ�

  for (size_t i = 0; i < srcLen; i++)
  {
    chr = *(src + i);
    if ((chr & BYTEPREFIX) == BYTEPREFIX) //�����ASCII��Ͳ�����
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
      break; //�ݴ�
    }
    size_t calc = 0; //��¼��ʹ�õ�λ
    size_t byteIndex; //��¼��ʹ�õ��ֽ�
    byte used;
    byte *pDst = (byte *)dst;
    byte result[10] = { 0 }; //�����ҪС�������ô˱�������
    for (size_t j = bytesLen - 1; j > 0; j--, calc += 6)
    {
      byteIndex = calc / 8;
      used = 0x3f & src[i + j];
      pDst[byteIndex] |= (used << (calc % 8));
      pDst[byteIndex + 1] |= used >> (8 - calc % 8);
    }
    //�������һλ
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