# AMPT Data Processing and Analysis

This codebase is used to process and analyze AMPT data root files.

## Testing
To test the code, start with **CEN = 5**, mid-central events, 30-40% centrality first.

## Running the Code
To run the code, use the following command:

```bash
./run.sh <energy> <pathtodata>
Example: ./run.sh 19 /media/Disk_YIN/AMPT-27GeV/27GeV-10M-2/
```
To run it in the background and pipe output to nohup.out, use this command (this may be useful if your dataset is very large):
```bash
nohup bash ./run.sh <energy> <pathtodata> & 
```

This will produce root files, organized by centrality, inside the directory New_Cuts.

## Merging
To merge, you can run ./merge.sh in /Merge/ to combine root files.
Within combineResults_simple.pl, just change the energy variable to your desired energy.

## Plotting
After merging, use plot_multiple_energies.c to create a plot for these energies. You can use plot_single_energy.c to plot a single energy for testing. plot_multiple_energies.c is currently configured to plot v2 vs. centrality but there is commented out code to plot the v2(data)/v2(ampt) ratio vs. centrality as well. 

## Centrality Definitions
To produce centrality definitions, you can run Centrality.C using root -b -q Centrality.c and change the file path in line #45 to a small sample of data. The code will then output the centrality definitions in stdout. Be careful with how centrality is defined in terms of % as the output will be all the centrality definitions from 0-100% in increments of 5% but the centralities 1-9 are not defined by these increments.
