In here will be Watchface AddOns, other people may make repositories of such, though the ones in here are designed to be #include'd at the top of your GSR.ino (just below the only include present).  Place them in the order you want them to show up in the Watchface list.

All of these files can contain 1 or more than one Watchface, currently a limit of 96 is present, sadly, there isn't sufficient RTC memory to add too many more without sacrificing space available for other things.

There is an "empty watchface" EmptyAddOn.h that you can use as a starting point.  Best to read the file and use a "Search" and "Replace" to quickly create your AddOn, place a copy of this file in your project folder and search & replace as explained in the top of the file, once that is done, save the file and edit with your favorite editor, it is actually a functional AddOn as is (copy of the base Classic GSR).

Also included for use with WatchFace AddOns is a WeatherIcons.h which has a function: `const unsigned char* getWeatherIcon(uint16_t Condition, bool Small = false)` that will return the weather icon from the OWM's current Conditions value which you supply from the WeatherData retrieved once Watchy_GSR has retrieved data, if one isn't found a nullptr is returned, so make sure you don't directly set that icon in a display.drawBitmap or unexpected results can happen.  Setting the bool Small to true will give versions capable of sitting over top of the Status area of Watchy_GSR.  There is also a second `function const unsigned char* getTemperatureScaleIcon(bool Metric, bool Small)` that will return the requested Temperature Scale Icon (used right after the temperature to show either °F or °C).

Default Watchy_GSR example of InsertDrawWeather (this works for using the drawStatus() if you put this inside your InsertDrawWeather function):

```
  const unsigned char *Cond, *Ico;
  uint16_t C = (Design.Status.Inverted ? BackColor() : ForeColor());
  byte X = Design.Status.WIFIx;
  byte Y = Design.Status.WIFIy + 2;
  int16_t  x1, y1;
  uint16_t w, h;
  String S;
  if (!Status || !IsWeatherAvailable()) return;
  Cond = getWeatherIcon(GetWeatherID(), Status);
  Ico = getTemperatureScaleIcon(IsMetric(), Status);
  if (Cond == nullptr || Ico == nullptr) return;
  display.drawBitmap(X, Y - 18, Cond, 26, 17, (Design.Status.Inverted ? BackColor() : ForeColor()), (Design.Status.Inverted ? ForeColor() : BackColor()));
  display.setFont(&Bronova_Regular10pt7b);
  if (IsBatteryHidden()){
    X = Design.Status.BATTx + 27 - Design.Face.Gutter;
    Y = Design.Status.BATTy + 17;
    S = String(GetWeatherTemperatureFeelsLike());
    display.getTextBounds(S, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(X - w, Y);
    display.print(S);
    X = display.getCursorX() + 1;
    display.drawBitmap(X, Y - 8, Ico, 13, 10, (Design.Status.Inverted ? BackColor() : ForeColor()), (Design.Status.Inverted ? ForeColor() : BackColor()));
  }
```
