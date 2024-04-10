# InkWatchy
A new **firmware** written  from the ground up for the [watchy](https://watchy.sqfmi.com/), written by the second developer of [Quill OS](https://github.com/Quill-OS/quill) 

<sub>Quill OS was named InkBox OS which explains the name of this project but it was rebranded and now I'm stuck with this name :D</sub>

### [Outdated demo](https://www.youtube.com/watch?v=gFOCqalJidQ)

<details> 
<summary><h2>Code features</h2></summary>

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

</details>

## User features
Apart from code things, here are the key features for now:
- Full weather info via open weather api in charts, 5 day forecast
- A book reader, with pages changable by moving your hand
- Working, uncomplete watchface
   - Watchface modules (Apps which are shown on the watchface directly). COnfigured to be shown on the bottom of the watchface. They can be configured as "dissmissable" so
      - Bitcoin module, which shows some data, here is an example:
        
        ![image](https://github.com/Szybet/InkWatchy/assets/53944559/01a01c95-f797-44e5-aac1-7a8f9458c5a6)
      - A simple net module which just shows wifi status and SSID. It's more of an example imlementation than a serious module (for now)
- Debug menus to check on things on the fly
- Battery life features:
   - Configurable wake up every x minutes at night
   - A menu to manually, or after certain battery percantage drop enable these features:
      - Disable wake up - The watch will sleep until you press a button
      - Disable all vibrations (For button clicks) - Spikes caused by the motor are heavy for the battery
- A "vault" which allows to save encrypted images to the watchy and view them if provided the correct PIN. All generated dynamically via a script. Uses AES128 cbc/ecb. Example use is a backup phone book
- I integrated https://github.com/risinek/esp32-wifi-penetration-tool to this firmware as a service. I won't provide support for it or give any advice how to use it. Use for your own responsibility, don't do illegall stuff. Seriously.
- https://github.com/ckcr4lyf/EvilAppleJuice-ESP32 too. the same story as above.

<details> 
<summary><h2>Compiling</h2></summary>
   
1. You need to run the `generate.sh` in the `resources` folder. As mentioned before it will convert images and fonts on the fly to header files
   - Run it from the same directory it is placed in via, for example `cd resources && ./generate.sh`
   - Look at the output if you need to install any packages
      - Don't assumie pio will download them. no.
   - Version 6.9.11-60 of ImageMagick (`convert` command) doesn't work, gives black squares as a result. Version 7.1.1-27 works for me.
   - You need also `xxd`, newer is better
3. Run `generate_config.sh` also in the `resources` folder. After that you can ajust values in `config.h` and `confidential.h` (in `/src/defines/` directory) to your needs.
   - DEBUG and various other debugging options are helpfull to debug things but eat battery. On the final compilation disable them
   - To enable all bitcoin data, you need a coinlib api key. If you can't create an account there because of no verification email, just create an account directly with google. I have already emailed them about the issue. If no api key is supplied, It will only show the block clock.
4. Read platformio.ini and choose your env. Don't use the default one for reasons. You propably want to select the `min` one. Remember to do that too if you use platformio on the terminal

<!-- **Also latest stable commit: None** Go back to it if you encounter problems. It's also possible this commit is the latest commit in this repo -->

<details> 
<summary><h2>So you are a interested developer?</h2></summary>

Some informations which you could find helpfull:
- Feel free to ask questions
- For writing your own watchface you could just modify watchFaceDraw.cpp file. If you write one, You can make it turn on off via config.h and merge it to the main repo :D
- Submodules are written relatively to the main coordinate so you can eassly move them arround, but the original size must remain. Or you can not implement them at all too
- I listed many things in `Code features` that make my code *good*. But as the project expanded and my brain failed from time to time because coding at 2 is actually a bad idea, there are some things that even I know about that are bad:
   - Submodules should be pure OOP and not retarded OOP structures.
 
They need to be ramede, so before you touch it, repair them or I will do it

 Obiously feel free to complain and even help!

</details>

</details>

## Donations
This was the first thing I have written and asked myself if I should release it for free. Well here we are - I also write other open source things - check my profile for more eink stuff. Here is [my libera pay](https://liberapay.com/Szybet/). Direct paypall is also a option. 

Oh and as it seems bitcoin is also preferred, just contact me and ask me for an address because creating a new address for every transaction is more secure, right?

## Sister projects
- https://github.com/Szybet/E-Watch-FOSS
- https://github.com/Szybet/watchy-scom

## Some credits
- [GuruSR](https://github.com/GuruSR/Watchy_GSR) - I used some of his awesome libraries and looked at the code for how the watchy hardware should be handled. Also answered a few questions on discord. Thanks!
- [Prokuon](https://github.com/Prokuon/watchy-starfield/) - I really liked the design of this watchface and almost coppied it. Thanks! - well the code is completly diffrent

## Questions
Create a github issue or get in touch with me on Watchy or Quill OS discord server ( Or other discord communication channels )

or via matrix: @szybet:matrix.org
