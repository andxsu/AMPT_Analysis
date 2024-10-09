This codebase is used to process and analyze AMPT data root files.

To test the code, run on cen = 5, mid-central events, 30-40% centrality first.

To run, run the command: ./run.sh <energy> <pathtodata>
Example: ./run.sh 19 /media/Disk_YIN/AMPT-27GeV/27GeV-10M-2/

This will produce root files, organized by centrality, inside the directory New_Cuts.
Then, you can run ./merge.sh in /Merge/ to combine root files.
Within combineResults_simple.pl, just change the energy variable to your desired energy.

After merging, use plot_multiple_energies.c to create a plot for these energies. You can use plot_single_energy.c to plot a single energy for testing. plot_multiple_energies.c is currently configured to plot v2 vs. centrality but there is commented out code to plot the v2(data)/v2(ampt) ratio vs. centrality as well. 

You can modify Sample_code() function to change the actual data processing (histograms, v2, cuts, etc).

You can also run nohup bash ./run.sh <energy> <pathtodata> & to run it in the background and pipe output to nohup.out as this process may take a long time depending on how large your dataset is. 

To produce centrality definitions, you can run Centrality.C using root -b -q Centrality.c and change the file path in line #45 to a small sample of data. The code will then output the centrality definitions in stdout. Be careful with how centrality is defined in terms of % as the output will be all the centrality definitions from 0-100% in increments of 5% but the centralities 1-9 are not defined by these increments.
