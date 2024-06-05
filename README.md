
SPCCP-Miner
Introduction
SPCCP-Miner is an interactive system to mine the congested road junctions. This system include data layer, algorithm layer, and display layer. These layers are mainly implemented in /Sources/mainwindow.cpp. All source codes were divided into the following folders:

./Headers: the header files of SPCCP-Miner, these files are used to store all predefined functions.
./Sources: the most important files in this system, all functions are implemented in this folder.
./Forms: The interface design scheme of the system and icons are stored in this folder.
./Datas: Some of the data files for testing are stored in this folder.
./Proj: Some project configuration files.
demo.exe: The executable program, users can open it to start SPCCP-Miner.
Usage
how to download the SPCCP-Miner from this web.
We can click the top right corner of the code->download ZIP to download the zip package, unzip it and open demo.exe to start this system.

The data format in SPCCP-Miner:
Whether the user inputs the data themselves or reads it from /data folder, the data should follow the following format:

To begin with, the first few rows show the neighbor relationship between roads. We indicate the road name in the first position of each row, followed by the neighboring road of that road. For example, this row indicate that the road of A has two neighbor roads: C and I.

A C I    
After that, we should input the congestion instance of road, which represents the time period at which road is congested. For example, this row indicate that road A is congested at first time span, road C is congested at third time span.

A1 C3   
Finally, we can get a complete data set, which can upload to system to generate complete and correct SPCCPs.

More infomation about SPCCP-Miner:
Watch the video for more detailed instruction: https://youtu.be/1RuCg0LegbM
