#pragma once

// Coordinates definition for MSMConstants
struct PxXY
{
  int X;
  int Y;
  unsigned char R;
  unsigned char G;
  unsigned char B;
  bool Response;

  PxXY() :
    X(0), Y(0), R(0), G(0), B(0), Response(false) {}

  PxXY(int x, int y, unsigned long rgb, bool response = false) :
    X(x), Y(y),
    R(rgb & 0x000000FF),
    G((rgb & 0x0000FF00) >> 8),
    B((rgb & 0x00FF0000) >> 16),
    Response(response) {}
  
  PxXY(int x, int y, unsigned char r, unsigned char g, unsigned char b, 
       bool response = false) :
    X(x), Y(y), R(r), G(g), B(b), Response(response) {}

  bool operator==(const PxXY pxXy)
  {
    unsigned int error_count = 0;
    if ((pxXy.R != this->R) || (pxXy.G != this->G) || (pxXy.B != this->B))
    {
      error_count = sqrt(pow(pxXy.R - this->R, 2)) + 
                    sqrt(pow(pxXy.G - this->G, 2)) +
                    sqrt(pow(pxXy.B - this->B, 2));
    }

    return (10 >= error_count);
  }

  bool operator!=(const PxXY pxXy)
  {
    return (false == this->operator==(pxXy));
  }
};