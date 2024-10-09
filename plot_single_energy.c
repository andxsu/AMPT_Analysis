void plot_single_energy() {
    TCanvas *canvas = new TCanvas("canvas", "Mean v2 vs Centralities", 800, 600);
    TGraphErrors *graph = new TGraphErrors();
    int energy = 14;
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

        double meanV2 = p_v2->GetMean(2); 
        double meanV2Error = p_v2->GetMeanError(2); 

        graph->SetPoint(centrality - 1, centrality, meanV2);
        graph->SetPointError(centrality - 1, 0, meanV2Error);

        file->Close();
    }

    graph->SetTitle(Form("%d GeV", energy));
    graph->GetXaxis()->SetTitle("Centrality");
    graph->GetYaxis()->SetTitle("Mean v2");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.5);

    canvas->cd();
    graph->Draw("AP");

    canvas->Print(Form("%dGeV_v2.png", energy));

}
