#include "minimalDefines.h"
#if GSR_WATCHFACES && GSR_STATIONARY
// TextualTimeGSR.h

#include "Watchy_GSR.h"
#include "stationaryfonts/NunitoSans_Bold28pt7b.h"
#include "stationaryfonts/NunitoSans_Light28pt7b.h"

RTC_DATA_ATTR uint8_t TextualTimeStyle;
int TextualTimeState = 0;

class TextualTimeGSR : public WatchyGSR {
  public:
    TextualTimeGSR() : WatchyGSR() { initAddOn(this); }

    void RegisterWatchFaces(){
      TextualTimeStyle = AddWatchStyle("Textual Time", this);
    }

    void InsertInitWatchStyle(uint8_t StyleID){
      if (StyleID == TextualTimeStyle){
          Design.Face.Bitmap = nullptr;
          Design.Face.SleepBitmap = nullptr;
          Design.Face.Gutter = 0;
          Design.Status.Inverted = false;
          Design.Face.TimeFont = &NunitoSans_Bold28pt7b;
      }
    }

    void InsertDrawWatchStyle(uint8_t StyleID){
      if (StyleID == TextualTimeStyle){
        if (!SafeToDraw()) return;

        display.fillScreen(GxEPD_BLACK);
        display.setTextColor(GxEPD_WHITE);
        display.setTextWrap(false);

        

        const char *lows[10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
        const char *teensone[11] = {"", "ten", "eleven", "twelve", "thir", "four", "fif", "six", "seven", "eight", "nine"};
        const char *teenstwo[11] = {"", "", "", "teen", "teen", "teen", "teen", "teen", "teen", "teen", "teen"};
        const char *tens[10] = {"zero", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

        uint16_t lines = 0;
        display.setFont(&NunitoSans_Bold28pt7b);

        display.setCursor(8, lines * 47 + 42);
        if (WatchTime.Local.Hour == 0) {
          display.print(tens[2]);
          lines++;
          display.setCursor(8, lines * 47 + 42);
          display.print(lows[4]);
        } else if (WatchTime.Local.Hour < 10) {
          display.print(lows[WatchTime.Local.Hour]);
        } else if (WatchTime.Local.Hour < 20) {
          display.print(teensone[WatchTime.Local.Hour - 9]);
          if (WatchTime.Local.Hour > 12) {
            lines++;
            display.setCursor(8, lines * 47 + 42);
            display.print(teenstwo[WatchTime.Local.Hour % 10]);
          }
        } else {
          display.print(tens[WatchTime.Local.Hour / 10]);
          if (WatchTime.Local.Hour % 10 > 0) {
            lines++;
            display.setCursor(8, lines * 47 + 42);
            display.print(lows[WatchTime.Local.Hour % 10]);
          }
        }

        lines++;
        display.setCursor(8, lines * 47 + 42);
        display.setFont(&NunitoSans_Light28pt7b);

        if (WatchTime.Local.Minute == 0) {
          display.print("o'clock");
        } else if (WatchTime.Local.Minute < 10) {
          display.print("oh");
          lines++;
          display.setCursor(8, lines * 47 + 42);
          display.print(lows[WatchTime.Local.Minute]);
        } else if (WatchTime.Local.Minute < 20) {
          display.print(teensone[WatchTime.Local.Minute - 9]);
          if (WatchTime.Local.Minute > 12) {
            lines++;
            display.setCursor(8, lines * 47 + 42);
            display.print(teenstwo[WatchTime.Local.Minute % 10]);
          }
        } else {
          display.println(tens[WatchTime.Local.Minute / 10]);
          if (WatchTime.Local.Minute % 10 > 0) {
            lines++;
            display.setCursor(8, lines * 47 + 42);
            display.print(lows[WatchTime.Local.Minute % 10]);
          }
        }
      }
    }
};

TextualTimeGSR TextualTimeGSRLoader;
#endif