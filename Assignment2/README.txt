The problem statement is as follows : 
Normal distributions
Generate a sine wave x  as an array of points x[N]
Take in 3 arguments as argv: amplitude, freq, number of cycles (M)
Choose 3<P<10 number of points per cycle such that N = M*P
Generate and add Gaussian noise to create the noisy sine wave y[N]
Choose mean = 0, stdev = 0.1 Vpp
Use the Box-Muller transform to convert from a uniform distribution to a normal distribution
Estimate the error e = y - x  for all N points and determine
Mean and stdev (sigma) of the error distribution
Tabulate the statistics: what % of points lie within 2*sigma, 4*sigma and 6*sigma of the mean?
Choose a random mean,  |mu| ~ 0.1*Vpp,  stdev = 0.2 Vpp and repeats steps 1 to 3
How do your choices of P and mu affect the observed statistics of e?
