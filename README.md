<h1 align="center">
  InkWatchy
</h1>

<p align="center">
<img src="https://github.com/user-attachments/assets/848177ae-2931-4a7a-bb6a-09255235c6c0" alt="02-demo-10mb" width=200px height=200px />
</p>
<p align="center">
A new innovative <b>firmware</b> written from the ground up for the <a href="https://watchy.sqfmi.com">watchy</a> and now also <a href="https://github.com/Szybet/Yatchy">yatchy</a>
</p>
<p align="center">
Now getting a bit <a href="https://github.com/Szybet/InkWatchy/tree/master/components/rusty/inkrusty">rusty</a>🦀
</p>
  
<details>
<summary>Demo</summary>
  
<sub>This demo was heavily compressed, so it looks like it looks. The speed is also messed up because of missing frames</sub>
<p align="center">
 
https://github.com/user-attachments/assets/91d169da-298a-425a-859c-2c9bc7fb6cf7
  
</p>

</details>
  
## User features
- Easy to install [demo](https://github.com/Szybet/InkWatchy/wiki/Trying-out-the-demo-firmware) (no compiling!)
- [Modular and configurable](https://github.com/Szybet/InkWatchy/blob/master/src/defines/templates/gifnoc-template.h), a lot - if you disable a thing, it's not being compiled, it's like it was never there
- A lot of [watchfaces](https://github.com/Szybet/InkWatchy/wiki/Watchfaces)
- Full weather info via open meteo api in charts, 16 day forecast + On watchface
- Calendar show of events, their time and description from an ICS file (So any calendar should sync)
- Alarms
  - Basically unlimited ammount of them
  - Quick alarms setting
  - Pomodoro timer
- A book reader
   - With pages changable by moving your hand
   - Support for many books
- Interactive watchface by using watchface modules (Apps which are shown on the watchface directly).
    - Bitcoin module, with a "Big" mode to be truly a "bitclock"
    - A simple network module
    - A small conway game of life module
    - Image gallery module
    - Calendar module, to view the nearest event
    - Book module, to view the book text all the time
    - Alarm module, to see when the next alarm rings
    - Api module, to send button presses over the network (experimental)
- Debug menus to check on things on the fly (for example, a 3D demo of the accelerometer axis, to check if it works)
- Battery life features:
   - Configurable wake up every x minutes at night, at default it's between 23-06 every 45 minutes
   - A menu to manually, or after certain battery percantage drop enable these features:
      - Disable wake up - The watch will sleep until you press a button, this extremely improves battery life
      - Disable all vibrations - The vibration motor is heavy for the battery
- A "vault" which allows to save encrypted images to the watchy and view them if provided the correct PIN. All generated dynamically via a script. Uses AES128 cbc/ecb. Example use is a backup phone book, bitcoin key backup in the form of a QR code
- Games / Fun
   - Blockchy, a tetris like game
   - Pong game
   - Conway game of life
   - Small video player
- Experimental heart rate monitor (Just a proof of concept, works best on Yatchy)
- Reset cause shower. If your battery will be the cause of a reset, you will be informed about it on boot.
- ~~I integrated https://github.com/risinek/esp32-wifi-penetration-tool to this firmware as a service. I won't provide support for it or give any advice how to use it. Use for your own responsibility, don't do illegall stuff. Seriously.~~
- ~~https://github.com/ckcr4lyf/EvilAppleJuice-ESP32 too. the same story as above.~~ <sub>Those 2 options were removed because I updated the build system and didn't bother to make it work with it, with some cmake you should be able to easily re-add those features, I you can't use cmake, then you shouldn't probably use either of those features anyway</sub>

<details> 
<summary><h2>Code features</h2></summary>

This list is outdated ;)

It doesn't use the watchy sub-class but it uses the same libraries. With that in mind, those are further diffrences from other firmwares:
<sub>Every small text is a reason why I was mad enough to rewrite everything myself...</sub>
- Welcome to contributions!
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
- LittleFS is used instead of NVS. Which means better code, logs in file system. Resources not uploaded on every program change. Awesome
- Various tasks which speed up the debugging / development process (`resources/tools/other/tasks`)

</details>

## Devices supported
Supported devices:
- [Yatchy](https://github.com/Szybet/Yatchy)
- Watchy 2.0
- Watchy 3.0 thanks to the community :)
- Watchy 1.0, 1.5 too but less used, less tested

## Easy to install demo, Compiling, Some instructions, Further informations
Don't try anything by yourself without the wiki, you will probably fail.

Head out to the [wiki](https://github.com/Szybet/InkWatchy/wiki)

## Donations
This is a passion project at this point. To help an open source developer and motivate him to create this software feel free to donate

For paypall you need to contact me

Bitcoin address: `bc1qm2d3w6a3qd93zrfuz4l6rs9yr30dmkl2t2z3y5`

For a small coffe I can manage your firmware and provide you personalised binaries, if you can't compile it yourself, even add some features for you

## Sister projects
- https://github.com/Szybet/WatchySourcingHub
- https://github.com/Szybet/watchy-scom
- https://github.com/Szybet/Yatchy

## Questions / Contact
Create a github issue or get in touch with me on Quill OS or <a href="https://discord.gg/6PUmRXZRGD">*atchy community</a> discord servers

or via matrix: @szybet:matrix.org

<details>
<summary><h2>Outdated demos</h2></summary>

[Outdated demo of version 0.1](https://www.youtube.com/watch?v=gFOCqalJidQ)

<details>
<summary>Demo of version 0.4</summary>
  
<sub>This 3> minute demo was heavily compressed, so it looks like it looks. The speed is also messed up because of missing frames</sub>
<p align="center">
  <img src="https://github.com/Szybet/InkWatchy/assets/53944559/0c85d516-ff50-4440-9351-32780107e998" alt="02-demo-10mb""/>
</p>

</details>

</details>

## Some credits
- [Can in a hoodie](https://www.youtube.com/@cat_in_a_hoodie_27) - Many awesome graphics, the logo
- [GuruSR](https://github.com/GuruSR/Watchy_GSR) - I used some of his awesome libraries and looked at the code for how the watchy hardware should be handled. Also answered a few questions on discord. Thanks!
- [Prokuon](https://github.com/Prokuon/watchy-starfield/) - I really liked the design of this watchface and almost coppied it. Thanks! - well the code is completly diffrent
- [Jason2866](https://github.com/Jason2866) - Help with solving platformio hackery
- C4nn0n from discord - Testing a lot for the watchy v3 and beyond
- [invpt](https://github.com/invpt) (slabeb on discord) - Huge thanks for pointing out the direction for a potential fix for screen ghosting, I easily have spend 2 weeks on it ;o 

and many other contributors, whom you can see on the panel on the right

Thanks!
