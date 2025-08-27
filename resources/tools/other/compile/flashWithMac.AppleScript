-- you need to install esptool on your mac with "pip install esptool" or "brew install esptool"
-- you need to install brew on your mac if not already done https://brew.sh/index_de
-- you need to install docker on your mac if not already done https://docs.docker.com
-- you need to open the container. If you opened the container with VSCode, you are fine.


set envNames to {"Watchy_1", "Watchy_1_5", "Watchy_2", "Watchy_3", "Yatchy"}
set actions to {"flash all", "flash firmware", "flash fs"}
set partitionTableAddress to "0x8000"
set firmwareAddress to "0x10000"


set username to (do shell script "echo $USER")

display dialog "Connect your Watchy to USB"
set serialPorts to paragraphs of (do shell script "ls /dev/tty.* | grep tty")
set serialPort to choose from list serialPorts with prompt "Select your serial port:"

set envName to choose from list envNames with prompt "Select your Enviroment:" default items {"Watchy_2"}
set action to choose from list actions with prompt "Select your Flash mode:" default items {"flash fs"}

-- if the script stops here you shuld try to replace this part and hard code the containerName  with "set containerName to "name of the container"
set dockerContainers to paragraphs of (do shell script "PATH=$PATH:/usr/local/bin
 docker container ls | grep inkwatchy") -- this grep is working, maybe for ever. As long as we have "inkwatchy" in the image name of the devcontainer
set containerNameLong to choose from list dockerContainers with prompt "Select your container:" --ther could be mor then one devcontainer running
set containerName to (do shell script "echo " & containerNameLong & " | grep -o '[^ ]*_[^ ]*'") -- this grep-regex is working, maybe for ever. As long as Docker gives names to the containers like "wizardly_williams"

display dialog "Your selected container: " & containerName & "
Your Watchy is type: " & envName & "
On on serial port: " & serialPort & "
to: " & action

if "Watchy_1" contains envName then
	
	set bootloaderAddress to "0x1000"
	
else if "Watchy_1_5" contains envName then
	
	set bootloaderAddress to "0x1000"
	
else if "Watchy_2" contains envName then
	
	set bootloaderAddress to "0x1000"
	
else if "Watchy_3" contains envName then
	
	set bootloaderAddress to "0x0"
	
else if "Yatchy" contains envName then
	
	set bootloaderAddress to "0x0"
	
else
	display dialog "Do not know where to flash the bootloader, exiting"
end if



-- this copies the needed files from the devcontainer to your may take care about whiespaces
do shell script "PATH=$PATH:/usr/local/bin
docker container start " & containerName & "
docker cp " & containerName & ":/workspaces/InkWatchy/resources/tools/fs/in/offset.txt /Users/$USER/Downloads/
docker cp " & containerName & ":/workspaces/InkWatchy/resources/tools/fs/out/fs.bin  /Users/$USER/Downloads/
docker cp " & containerName & ":/workspaces/InkWatchy/.pio/build/" & envName & "/firmware.bin  /Users/$USER/Downloads/
docker cp " & containerName & ":/workspaces/InkWatchy/.pio/build/" & envName & "/partitions.bin  /Users/$USER/Downloads/
docker cp " & containerName & ":/workspaces/InkWatchy/.pio/build/" & envName & "/bootloader.bin  /Users/$USER/Downloads/
"


-- read the offset.txt file to get the resourceAddress to flash the filesystem




set theFile to ("Macintosh HD:Users:" & username & ":Downloads:offset.txt")
set {resourceAddress} to paragraphs of (read file theFile)

if "flash all" contains action then
	try
		do shell script "PATH=$PATH:/usr/local/bin
		lsesptool --chip esp32 --port " & serialPort & " --baud 921600 write-flash " & bootloaderAddress & " /Users/$USER/Downloads/bootloader.bin " & partitionTableAddress & " /Users/$USER/Downloads/partitions.bin " & firmwareAddress & " /Users/$USER/Downloads/firmware.bin " & resourceAddress & " /Users/$USER/Downloads/fs.bin"
	on error
		display dialog "Flashed all, now wait for reset!"
	end try
else if "flash firmware" contains action then
	try
		do shell script "/opt/homebrew/bin/esptool --chip esp32 --port " & serialPort & " --baud 921600 write-flash " & firmwareAddress & " /Users/$USER/Downloads/firmware.bin"
	on error
		display dialog "Flashed firmware, now wait for reset!"
	end try
else if "flash fs" contains action then
	try
		do shell script "/opt/homebrew/bin/esptool --chip esp32 --port " & serialPort & " --baud 921600 write-flash " & resourceAddress & " /Users/$USER/Downloads/fs.bin"
	on error
		display dialog "Flashed filesystem, now wait for reset!"
	end try
end if


display dialog "Well, now 
...  what time is it "