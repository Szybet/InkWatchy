#pragma once

#include "minimalDefines.h"
#if GSR_WATCHFACES && GSR_STARFIELD
#include "Watchy_GSR.h"

#include "WatchyStarfield.h"

RTC_DATA_ATTR int MyGSRStarfieldWatchFaceId; // Watchface ID #.
// Approx. 3035 bytes

class GSRWatchFaceStarfield : public WatchyGSR
{
public:
  GSRWatchFaceStarfield() : WatchyGSR() { initAddOn(this); } // *** DO NOT EDIT THE CONTENTS OF THIS FUNCTION ***

  WatchyStarfield StarfieldWatchFace;

  void RegisterWatchFaces()
  { // Add WatchStyles here (this is done pre-boot, post init)
    MyGSRStarfieldWatchFaceId = AddWatchStyle("Starfield", this);
  };

  // void InsertInitWatchStyle(uint8_t StyleID){
  //   if (StyleID == GSRStarfieldWatchFaceId){ // LCD
  //       Design.Menu.Top = 56;
  //       Design.Menu.Header = 25;
  //       Design.Menu.Data = 66;
  //       Design.Menu.Gutter = 3;
  //       Design.Menu.Font = &aAntiCorona12pt7b;
  //       Design.Menu.FontSmall = &aAntiCorona11pt7b;
  //       Design.Menu.FontSmaller = &aAntiCorona10pt7b;
  //       Design.Face.Bitmap = nullptr;
  //       Design.Face.SleepBitmap = nullptr;
  //       Design.Face.Gutter = 4;
  //       Design.Face.Time = 134;
  //       Design.Face.TimeHeight = 48;
  //       Design.Face.TimeColor = GSR_AutoFore;
  //       Design.Face.TimeFont = &TRAN48pt7b;
  //       Design.Face.TimeLeft = 0;
  //       Design.Face.TimeStyle = WatchyGSR::dCENTER;
  //       Design.Face.Day = 44;
  //       Design.Face.DayGutter = 4;
  //       Design.Face.DayColor = GSR_AutoFore;
  //       Design.Face.DayFont = &TRAN19pt7b;
  //       Design.Face.DayFontSmall = &TRAN19pt7b;
  //       Design.Face.DayFontSmaller = &TRAN19pt7b;
  //       Design.Face.DayLeft = 0;
  //       Design.Face.DayStyle = WatchyGSR::dCENTER;
  //       Design.Face.Date = 170;
  //       Design.Face.DateGutter = 4;
  //       Design.Face.DateColor = GSR_AutoFore;
  //       Design.Face.DateFont = &TRAN19pt7b;
  //       Design.Face.DateFontSmall = &TRAN19pt7b;
  //       Design.Face.DateFontSmaller = &TRAN19pt7b;
  //       Design.Face.DateLeft = 0;
  //       Design.Face.DateStyle = WatchyGSR::dLEFT;
  //       Design.Face.Year = 170;
  //       Design.Face.YearLeft = 99;
  //       Design.Face.YearColor = GSR_AutoFore;
  //       Design.Face.YearFont = &TRAN19pt7b;
  //       Design.Face.YearLeft = 0;
  //       Design.Face.YearStyle = WatchyGSR::dRIGHT;
  //       Design.Status.WIFIx = 5;
  //       Design.Status.WIFIy = 193;
  //       Design.Status.BATTx = 155;
  //       Design.Status.BATTy = 178;
  //    }
  // };

  void InsertDrawWatchStyle(uint8_t StyleID)
  {
    uint8_t X, Y;
    uint16_t A;
    if (StyleID == MyGSRStarfieldWatchFaceId)
    {
      if (SafeToDraw())
      {
        StarfieldWatchFace.drawWatchFace();
      }
    }
  };
};

GSRWatchFaceStarfield MyGSRWatchFaceStarfield;
#endif