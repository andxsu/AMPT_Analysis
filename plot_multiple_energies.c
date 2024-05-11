void plot_multiple_energies() {
    TCanvas *canvas = new TCanvas("canvas", "Mean v2 vs Centralities", 800, 600);
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9); // Add a legend
    legend->SetFillColor(0); // Set legend fill color to white
    legend->SetBorderSize(0); // Remove legend border

    // Loop over energies
    int colors[] = {kBlue, kRed, kBlack}; // Add more colors as needed
    int marker_style = 20;
    int color_index = 0;
    for (int energy : {7, 19, 27}) { // Add more energies as needed
        TGraphErrors *graph = new TGraphErrors();

        for (int centrality = 1; centrality <= 9; ++centrality) {
            TString filename = Form("Merge/cen%d_%dGeV.merged.root", centrality, energy);
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

            // Add the data point to the graph
            graph->SetPoint(centrality - 1, centrality, meanV2);
            graph->SetPointError(centrality - 1, 0, meanV2Error);

            file->Close();
        }

        // Customize the graph appearance
        graph->SetTitle("v2 vs Centrality");
        graph->GetXaxis()->SetTitle("Centrality");
        graph->GetYaxis()->SetTitle("v2");
        graph->SetMarkerStyle(marker_style);
        graph->SetMarkerColor(colors[color_index]);
        graph->SetMarkerSize(1.5);
        graph->SetLineColor(colors[color_index]); // Different colors for each energy

        // Draw the graph
        canvas->cd();
        if (energy == 7) {
            graph->Draw("AP"); // Draw first graph with axes
        } else {
            graph->Draw("P SAME"); // Draw subsequent graphs with the SAME option
        }

        // Add to legend
        legend->AddEntry(graph, Form("%d GeV", energy), "lp");
        marker_style++;
        color_index++;
    }

    // Add legend to canvas
    legend->Draw();

    // Save or display the plot
    canvas->Print("v2_vs_Centrality.png");
}
