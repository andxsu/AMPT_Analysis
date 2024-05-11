void plot_single_energy() {
    TCanvas *canvas = new TCanvas("canvas", "Mean v2 vs Centralities", 800, 600);
    TGraphErrors *graph = new TGraphErrors();
    int energy = 7;
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
    graph->SetTitle(Form("%d GeV", energy));
    graph->GetXaxis()->SetTitle("Centrality");
    graph->GetYaxis()->SetTitle("Mean v2");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.5);

    // Draw the graph
    canvas->cd();
    graph->Draw("AP");

    // Save or display the plot
    canvas->Print(Form("%dGeV_v2.png", energy));

}
