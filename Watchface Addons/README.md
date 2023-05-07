In here will be Watchface AddOns, other people may make repositories of such, though the ones in her are designed to be #include'd at the top of your GSR.ino (just below the only include present).  Place them in the order you want them to show up in the Watchface list.

All of these files can contain 1 or more than one Watchface, currently a limit of 32 is present, if people really want more, just put in an Issue asking for more.

There is also (going to be) an "empty watchface" EmptyAddOn.h that you can use as a starting point.

Also included for use with WatchFace AddOns is a WeatherIcons.h which has a function: const unsigned char* getWeatherIcon(uint16_t Condition, bool Small = false) that will return the weather icon from the OWM's current Conditions value, if one isn't found a nullptr is returned.  Setting the bool Small to true will give versions capable of sitting over top of the Status area of Watchy_GSR.
