The problem statement is as follows : 

Data analysis
A flow cytometer detects the fluorescence signal from cells that are flowing past a laser-detector. The data is arriving at regular intervals. Often, these cells are encapsulated within droplets  and the task is to identify 

the location (record number) of  droplet and the width of the droplet
the mean arrival time, and its std dev, between droplets
The data file has 2 columns of data

column one: forward scatter (noisier, simple photodetector)
column two: side scatter (cleaner, sensitive photomultiplier tube)
First work with column two to determine the location and arrival time distribution, create your reference, and then 
compare the location and arrival time distribution against using data from only column one. 
Through this assignment, you should demonstrate an understanding of

moving average with a good choice of a window
thresholding (p*sigma band about the moving average)
peak detection
