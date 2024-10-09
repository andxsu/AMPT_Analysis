#include <iostream>
using namespace std;

void plot_multiple_energies() {
    TCanvas *canvas = new TCanvas("canvas", "Mean v2 vs Centralities", 800, 600);
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9); 
    legend->SetFillColor(0); 
    legend->SetBorderSize(0); 

    const double fdata7GeV[] = {0.0228231, 0.029682, 0.0364254, 0.0412119, 0.0424986, 0.0395516, 0.0315448, 0.0217375, 0.0124565}; // 7 GeV
    const double fdata14GeV[] = { 0.05499,0.110591,0.178004,0.23887,0.272136,0.260886,0.187453,0.0934281,0.0272566}; // 14 GeV
    const double fdata19GeV[] = {0.0237424, 0.0325764, 0.0417857, 0.0472937, 0.0486548, 0.0448375, 0.0355971, 0.0246454, 0.0150614}; // 19 GeV
    const double fdata27GeV[] = {0.0221558, 0.0346134, 0.0433496, 0.0486536, 0.0497235, 0.0455847, 0.0357431, 0.0250085, 0.0152213};  // 27 GeV
    const double fdata200Gev[] = {0.0413077,0.0575181,0.0730605,0.0831016,0.0846779,0.0756208,0.0537518,0.0296554,0.0112468}; // 20 GeV

    const double fdata_err7GeV[] = {0.00098296, 0.000347636, 0.000160024, 8.8207e-05, 6.2222e-05, 5.29589e-05, 5.88684e-05, 0.000131415, 0.000353551}; // 7 GeV
    const double fdata_err14Gev[] = {0.000128851,0.000133232,0.000131624,0.000125947,0.000125044,0.000124418,0.00012925,0.000186579,0.000186369,}; // 14 Gev
    const double fdata_err19GeV[] = {0.000606642, 0.000239973, 0.000103824, 5.9053e-05, 3.90414e-05, 3.2346e-05, 3.57789e-05, 8.89287e-05, 0.000276607}; // 19 GeV
    const double fdata_err27GeV[] = {0.00109819, 0.000375481, 0.000159769, 8.48198e-05, 5.38648e-05, 4.34593e-05, 4.71431e-05, 0.00011781, 0.000332189};  // 27 GeV
    const double fdata_err200GeV[] = {4.42892e-05,4.33883e-05,4.37164e-05,4.48032e-05,4.54317e-05,4.59594e-05,4.61996e-05,6.66937e-05,6.46083e-05}; //200 GeV

    // Loop over energies
    int colors[] = {kBlue, kRed, kBlack, kGreen, kViolet}; 
    int marker_style = 20;
    int color_index = 0;
    // string centralityLabels[] = {"70-80", "60-70", "50-60", "40-50", "30-40", "20-30", "10-20", "5-10", "0-5"};
    for (int energy : {7,14, 19, 27}) { 
        TGraphErrors *graph = new TGraphErrors();

        for (int centrality = 1; centrality <= 9; ++centrality) {
            TString filename = Form("/media/Disk_YIN/Andrew/Merge/cen%d_%dGeV.merged.root", centrality, energy);
            TFile *file = TFile::Open(filename);

            if (!file || file->IsZombie()) {
                cerr << "Error: Unable to open file " << filename << endl;
                continue;
            }

            TProfile *p_v2 = (TProfile*)file->Get("p_v2");
            if (!p_v2) {
                cerr << "Error: Unable to retrieve TProfile p_v2 from file " << filename << endl;
                continue;
            }


            // Extract mean v2 and its error for this centrality
            double meanV2 = p_v2->GetMean(2); // Getting the mean y-value
            double meanV2Error = p_v2->GetMeanError(2); // Getting the error of the mean y-value
            // double meanV3 = p_v3->GetMean(2); // Getting the mean y-value
            // double meanV3Error = p_v3->GetMeanError(2); // Getting the error of the mean y-value
            double datav2[9];
            const double* index;
            if (energy == 200) index = fdata200Gev;
            else if (energy == 27) index = fdata27GeV; 
            else if (energy == 19) index = fdata19GeV;
            else if (energy == 14) index = fdata14GeV;
            else if (energy == 7) index = fdata7GeV;
            else std::cout << "Invalid energy" << std::endl;
            copy(index, index+9, datav2);

            double datav2_err[9];
            if (energy == 200) index = fdata_err200GeV;
            else if (energy == 27) index = fdata_err27GeV; 
            else if (energy == 19) index = fdata_err19GeV;
            else if (energy == 14) index = fdata_err14Gev;
            else if (energy == 7) index = fdata_err7GeV;
            else std::cout << "Invalid energy" << std::endl;
            copy(index, index+9, datav2_err);

            // double v2ratio = meanV2/(datav2[centrality-1]);
            // double v2err_ratio = meanV2Error/(datav2_err[centrality-1]);

            // Add the data point to the graph
            // cout<<"v2err_ratio: "<<v2err_ratio<<" energy: "<<energy<<" centrality: "<<centrality<< endl;
            graph->SetPoint(centrality - 1, centrality, meanV2);
            graph->SetPointError(centrality - 1, 0, meanV2Error);
            // graph->SetPoint(centrality - 1, centrality, v2ratio);
            // graph->SetPointError(centrality - 1, 0, v2err_ratio);
            // graph->SetPoint(centrality - 1, centrality, meanV3);
            // graph->SetPointError(centrality - 1, 0, meanV3Error);
            file->Close();
        }

        graph->SetTitle("v2 vs Centrality");
        graph->GetXaxis()->SetTitle("Centrality");
        // graph->GetYaxis()->SetTitle("v2(AMPT)/V2(Data)");
        graph->GetYaxis()->SetTitle("v2(AMPT)");
        graph->GetYaxis()->SetRangeUser(0, 6); // Adjust the range as needed
        graph->SetMarkerStyle(marker_style);
        graph->SetMarkerColor(colors[color_index]);
        graph->SetMarkerSize(1.5);
        graph->SetLineColor(colors[color_index]); 

        canvas->cd();
        if (energy == 7) {
            graph->Draw("AP"); 
        } else {
            graph->Draw("P SAME"); 
        }
        legend->AddEntry(graph, Form("%d GeV", energy), "lp");
        marker_style++;
        color_index++;
    }

    legend->Draw();
    // canvas->Print("v2ratiovsCentrality.png");
    canvas->Print("v2vsCentrality(test with 7,19,27-kaon).png");
}
