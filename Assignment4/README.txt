The problem statement is as follows:

Regression
Identify the threshold voltage for the semiconductor device, from its I-V characteristics

Read the data from the datafile. 
Start with one curve (Vg11), and find Vth. You can use any suitable method 
linear fits to the low and high voltage parts of the graph
start with some value of Vth and try to minimize the RMS error from the linear fits by search for a better Vth
fitting all the data to a model curve e.g.  I = I0*(1-exp(-V/V0)
Repeat the exercise for the different data columns in the datafile and finally plot Vth versus Vg
