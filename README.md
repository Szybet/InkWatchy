# InkWatchy
A new **firmware** written for the [watchy](https://watchy.sqfmi.com/) written by the second developer of [InkBox OS](https://inkbox.ddns.net/)

### Outdated demo
https://www.youtube.com/watch?v=gFOCqalJidQ

## Code features
It doesn't use the watchy sub-class but it uses the same libraries. With that in mind, those are further diffrences from other firmwares:
<sub>Every small text is a reason why I was mad enough to rewrite everything myself...</sub>
- It's splitted into files and folders <sub>In my opinion a file that has 5k lines is heresy</sub>
- It's function - not object based for the reason above and to make it easier for beginners to use
- It uses free rtos tasks where needed
- All resources like images, fonts, books are converted into variables dynamically via scripts. Editing images, changing font spacing is just one click. <sub>I can't believe I was the first to make this that way</sub>
- The Ui is dynamically written. There are functions to do it eassly. Adding a new menu is just a few lines for example<sub>It's not just a collection of drawBitmap</sub>
- It has a "manager" and design for various apps to run eassly
- Has logs via serial. They can be disabled that they don't get compiled with changing one define - and the code doesn't look bad because it's a macro. Amazing <sub>Yea, this is a feature compared to other ones I have looked at</sub>
- Uses libraries instead of pure calls to NTP or open weather
- Many configurable values via defines in config.h
- Many debugging tools in config.h
- Most UI is rendered only when needed / values it's showing changed. Good for battery life
- <sub> Proper variable naming and camelCase everywhere</sub>

## User features
Apart from code things, here are the key features for now:
- Full weather info via open weather api in charts, 5 day forecast
- A book reader, with pages changable by moving your hand
- Working, uncomplete watchface
- Debug menus to check on things on the fly
- Battery life features:
   - Configurable wake up every x minutes at night
   - A menu to manually, or after certain battery percantage drop enable these features:
      - Disable wake up - The watch will sleep until you press a button
      - Disable all vibrations (For button clicks) - Spikes caused by the motor are heavy for the battery
- A "vault" which allows to save encrypted images to the watchy and view them if provided the correct PIN. All generated dynamically via a script. Uses AES128 cbc/ecb. Example use is a backup phone book
- I integrated https://github.com/risinek/esp32-wifi-penetration-tool to this firmware as a service. I won't provide support for it or give any advice how to use it. Use for your own responsibility, don't do illegall stuff. Seriously.
- https://github.com/ckcr4lyf/EvilAppleJuice-ESP32 too. the same story as above.

## Compiling
1. You need to run the `generate.sh` in the `resources` folder. As mentioned before it will convert images and fonts on the fly to header files
   - Run it from the same directory it is placed in via, for example `cd resources && ./generate.sh`
   - Look at the output if you need to install any packages
      - Don't assumie pio will download them. no.
3. Run `generate_config.sh` also in the `resources` folder. After that you can ajust values in `config.h` and `confidential.h` (in `/src/defines/` directory) to your needs.
   - DEBUG and various other debugging options are helpfull to debug things but eat battery. On the final compilation disable them
4. Read platformio.ini and choose your env. Don't use the default one for reasons. You propably want to select the `min` one.

## Donations
This was the first thing I have written and asked myself if I should release it for free. Well here we are - I also write other open source things - check my profile for more eink stuff. Here is [my libera pay](https://liberapay.com/Szybet/). Direct paypall is also a option.

## Some credits
- [GuruSR](https://github.com/GuruSR/Watchy_GSR) - I used some of his awesome libraries and looked at the code for how the watchy hardware should be handled. Also answered a few questions on discord. Thanks!
- [Prokuon](https://github.com/Prokuon/watchy-starfield/) - I really liked the design of this watchface and almost coppied it. Thanks! - well the code is completly diffrent

