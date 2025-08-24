-- you need to create the folder /Users/yourusername/Documents/InkWatchyScriptFlash/bin/ manually
-- you need to install esptool on your mac with "pip install esptool" or "brew install esptool"
-- you need to install brew on your mac if not already done https://brew.sh/index_de
-- you need to install docker on your mac if not already done https://docs.docker.com
-- you need to start docker  on your mac. The container will be startet by script if not already done
-- you need to change the variables below to your needs



set serialPort to "/dev/tty.usbserial-59680179551" -- change this to your serial port, get it with "ls /dev/tty.*" in terminal
set username to "meffi" -- change this to your mac username, get it with "echo $USER" in terminal
set containerName to "wizardly_williams" -- change this to your container name, get it with "docker ps" in terminal

-- this values are fixed for the watchy but may change in future. look at /workspaces/InkWatchy/resources/tools/other/compile/demoMaker.sh near line 166
set bootloaderAddress to "0x1000"
set partitionTableAddress to "0x8000"
set firmwareAddress to "0x10000"

set envNames to {"Watchy_1", "Watchy_1_5", "Watchy_2", "Watchy_3", "Yatchy"}
set actions to {"flash all", "flash firmware", "flash fs"}


set envName to choose from list envNames with prompt "Select your Enviroment:" default items {"Watchy_2"}

set action to choose from list actions with prompt "Select your Flash mode:" default items {"flash fs"}

-- this copies the needed files from the devcontainer to your mac take care about whiespaces
do shell script "cd /../Applications/Docker.app/Contents/Resources/bin
 export PATH='$PATH:/Applications/Docker.app/Contents/Resources/bin/'
 docker container start " & containerName & "
 docker cp " & containerName & ":/workspaces/InkWatchy/resources/tools/fs/in/offset.txt /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/
 docker cp " & containerName & ":/workspaces/InkWatchy/resources/tools/fs/out/fs.bin  /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/
 docker cp " & containerName & ":/workspaces/InkWatchy/.pio/build/" & envName & "/firmware.bin  /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/
 docker cp " & containerName & ":/workspaces/InkWatchy/.pio/build/" & envName & "/partitions.bin  /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/
 docker cp " & containerName & ":/workspaces/InkWatchy/.pio/build/" & envName & "/bootloader.bin  /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/
"

-- read the offset.txt file to get the resourceAddress to flash the filesystem
set theFile to ("Macintosh HD:Users:" & username & ":Documents:InkWatchyScriptFlash:bin:offset.txt")
set {resourceAddress} to paragraphs of (read file theFile)

if "flash all" contains action then
	try
		do shell script "/opt/homebrew/bin/esptool --chip esp32 --port " & serialPort & " --baud 921600 write-flash " & bootloaderAddress & " /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/bootloader.bin " & partitionTableAddress & " /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/partitions.bin " & firmwareAddress & " /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/firmware.bin " & resourceAddress & " /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/fs.bin"
	on error
		display dialog "Flashed all, now wait for reset!"
	end try
else if "flash firmware" contains action then
	try
		do shell script "/opt/homebrew/bin/esptool --chip esp32 --port " & serialPort & " --baud 921600 write-flash " & firmwareAddress & " /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/firmware.bin"
	on error
		display dialog "Flashed firmware, now wait for reset!"
	end try
else if "flash fs" contains action then
	try
		do shell script "/opt/homebrew/bin/esptool --chip esp32 --port " & serialPort & " --baud 921600 write-flash " & resourceAddress & " /Users/" & username & "/Documents/InkWatchyScriptFlash/bin/fs.bin"
	on error
		display dialog "Flashed filesystem, now wait for reset!"
	end try
end if


display dialog "Well, now 
...  what time is it "