Each input data has roughly 2000 events, each event represents Au+Au collision
at different centrality (a random beam impactor of b); Thus, the output files
are categorized by the "centrality" = 0-5% (cen=9) means b=0 fm, most central events;
centrality = 70-80% (cen=1) means b ~ 70%*2*Radius of Au, most peripheral case.

To test the code, run on cen = 5, mid-central events, 30-40% centrality first.

To test the code:
root -b -q Sample_code.cpp

If you're happy with the code, run ./run.sh <energy> <pathtodata>
Example: ./run.sh 19 /media/Disk_YIN/AMPT-27GeV/27GeV-10M-2/

This will produce root files, organized by centrality, inside the directory New_Cuts.
Then, you can run ./merge.sh in /Merge/ to combine root files.
Within combineResults_simple.pl, just change the energy variable to your desired energy.

After merging, use plot_multiple_energies.c to create a v2 vs. centrality plot for these energies. You can use plot_single_energy.c to plot a single energy for testing.

You can modify Sample_code() function to change the actual data processing (histograms, v2, cuts, etc).

You can also run nohup bash ./run.sh <energy> <pathtodata> & to run it in the background and pipe output to nohup.out as this process may take a long time depending on how large your dataset is. 
