# Root_Data_Processing

To use, run ./run.sh <energy> <pathtodata>
Example: ./run.sh 19 /media/Disk_YIN/AMPT-27GeV/27GeV-10M-2/

This will produce root files, organized by centrality, inside the directory New_Cuts.
Then, you can run ./merge.sh in /Merge/ to combine root files.
Within combineResults_simple.pl, just change the energy variable to your desired energy.

After merging, use plot_multiple_energies.c to create a v2 vs. centrality plot for these energies.

You can modify Sample_code.cpp to change the actual data processing (histograms, v2, cuts, etc).

Also you can run nohup bash ./run.sh <energy> <pathtodata> & to run it in the background and pipe output to nohup.out as this process may take a long time depending on how large your dataset is. 
