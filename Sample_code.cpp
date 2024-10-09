using namespace std;
#include "stdio.h"
#include "TFile.h"
#include <fstream>
#include <iostream>
#include <TChain.h>
#include "TLeaf.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TMath.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TObjArray.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TROOT.h"
#include <thread>
#include "TStopwatch.h"
#include <ROOT/TThreadExecutor.hxx>
#include "TLorentzVector.h"
#include <thread>
#include <vector>
#include <algorithm>
#include "TDatabasePDG.h"

const float PI = TMath::Pi();
const float pt_trig_up = 2;
const float pt_trig_lo = .2;
const float EtaCut = 1.0;
// const float cenDef27[9] = {12.86,12.02,11.13,10.17,9.1,7.89,6.44,4.56,3.23};//27 GeV Au+Au
const float cenDef200[9] = {13.42,12.55,11.66,10.27,8.47,7.42,6.28,3.64,2.59};
const float cenDef27[9] = {12.85, 12.02, 11.14, 10.17, 9.1, 7.88, 6.44, 4.56, 3.24};
const float cenDef19[9] = {12.83, 12, 11.12, 10.15, 9.08, 7.85, 6.42, 4.54, 3.2};
const float cenDef14[9] = {12.82, 11.98, 11.09, 10.13, 9.05, 7.84, 6.39, 4.52, 3.2};
const float cenDef7[9] = {12.81, 11.98, 11.1, 10.13, 9.07, 7.86, 6.41, 4.52, 3.2};

//{13.14,12.29,11.38,10.39,9.29,8.05,6.57,4.65,3.29};   // 200 GeV Au+Au
//{12.83, 12, 11.12, 10.15, 9.08, 7.85, 6.42, 4.54, 3.2} //19Gev-19M
//{12.85, 12.02, 11.14, 10.17, 9.1, 7.88, 6.44, 4.56, 3.24} //27GeV-10M-2
//{12.81, 11.98, 11.1, 10.13, 9.07, 7.86, 6.41, 4.52, 3.2} //7GeV-8k
const Int_t total_jobs = 1;//this is to control the portion of data you prefer to run, data format is  xxx[job][min]1.root
const Int_t total_min = 1;//this is to control the portion of data you prefer to run

// string data_path = "/media/Disk_YIN/AMPT-7GeV/7GeV-8k";
// /media/Disk_YIN/AMPT-27GeV/27GeV-10M-2/
// /media/Disk_YIN/AMPT-19GeV/19GeV-19M/
// /media/Disk_YIN/AMPT-7GeV/7GeV-8k/
// /media/Disk_YIN/AMPT-200GeV/200GeV-cleaned/200GeV-data2055/
// /media/Disk_YIN/AMPT-14GeV/14GeV-cleaned/14GeV-8k/

void Sample_code(int e = 0, string path = "", int cen = 5, int job = 3, int min = 1){    //main_function
        TStopwatch timer; // Times the current min
        timer.Start();
        TStopwatch timer2; // Times every 10000 events
        timer2.Start();
        float cenDef[9];
        const float* cen_index;
        if (e == 200) cen_index = cenDef200;
        else if (e == 27) cen_index = cenDef27;
        else if (e == 19) cen_index = cenDef19;
        else if (e == 14) cen_index = cenDef14;
        else if (e == 7) cen_index = cenDef7;
        else std::cerr << "Invalid energy" << std::endl;
        copy(cen_index, cen_index+9, cenDef);
        cout<<job<<"\t"<<min<<endl;

        TChain* chain = new TChain("tree");
        int nfile = 0;
        char fname_in1[200];
        // Wherever your data is stored
        // sprintf(fname_in1,"/media/Disk_YIN/AMPT-27GeV/v27-9977/*%d%d1.root",job,min);//one data set
        // sprintf(fname_in1,"/media/Disk_YIN/AMPT-7GeV/7GeV-8k/*%d%d1.root",job,min);//one data set
        sprintf(fname_in1,"%s*%d%d1.root",path.c_str(), job,min); //generalized data set
        // nfile counts number of files ending in "[job][min].root"
        nfile += chain->Add(fname_in1); 
        
        cout <<"Added "<<nfile<<" files"<<endl;
        cout<<"# entries in chain: "<<chain->GetEntries()<<endl;

        char fname_out[200];
        sprintf(fname_out,"cen%d.%dGeVAMPT_job%d%02d.root",cen,e,job,min);
        TFile fout(fname_out,"NEW");
        if(fout.IsZombie() || !fout.IsOpen()) {
                std::cout<<"File already exists: "<<fname_out<<std::endl;
                delete chain;
                return;
        }

        std::vector<float> *px_vec  = nullptr;//momentum px
        std::vector<float> *py_vec  = nullptr;//momentum py
        std::vector<float> *pz_vec  = nullptr;//momentum pz
        std::vector<int> *pid_vec  = nullptr;//the particle id, refer to https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf

        chain->SetBranchAddress("px",  &px_vec);
        chain->SetBranchAddress("py",  &py_vec);
        chain->SetBranchAddress("pz",  &pz_vec);
        chain->SetBranchAddress("pid", &pid_vec);

        //defining variables
        Int_t   Centrality, NPTracks;   //event info
        Float_t psi, b, Eta, Theta, Phi, Pt;            //track info    
        Float_t Px, Py, Pz;//track info
        //1-2%
        //defining histograms
        // Anything you need add here 
        TH1D *hCentrality = new TH1D("hCentrality","hCentrality",10,0,10);
        TH1D *hRefMult = new TH1D("hRefMult","hRefMult",1000,-0.5,999.5);
        TH1D *hMult = new TH1D("hMult","hMult",1000,-0.5,999.5);
        TProfile *p_npp = new TProfile("p_npp","p_npp",10,-0.5,9.5,0,500); 
        TProfile *p_v2 = new TProfile("p_v2","p_v2",10,-0.5,9.5,-100,100);
        TH1D *hist_y = new TH1D("hist_y","hist_y", 100, -5, 5);

        Int_t nentries = chain->GetEntries();
        //loop through events, one data set usually contains 2000 events
        
        for(int i = 0; i < nentries; i++){ //loop through all events 
                
                if((i+1)%10000==0) {
                        cout<<"Processing entry == "<< i+1 <<" == out of "<<nentries<<" in job "<<job<<", min "<<min<<".";
                        timer2.Stop();
                        timer2.Print();
                        timer2.Start();
                }
                
                chain->GetEntry(i);

                TLeaf* leaf_b   = chain->GetLeaf("imp");
                TLeaf* leaf_RefMult = chain->GetLeaf("refmult");
                TLeaf* leaf_Np_p= chain->GetLeaf("npp");
                TLeaf* leaf_Np_t= chain->GetLeaf("npt");

                hRefMult->Fill(leaf_RefMult->GetValue(0));
                int Np = leaf_Np_p->GetValue(0) + leaf_Np_t->GetValue(0);
                if(Np<3) continue;

                psi = 0; //true reaction plane
                b = leaf_b->GetValue(0);//impact parameter
                NPTracks= px_vec->size();//total tracks in one event

                Centrality = 0;
                for(int j=0;j<9;j++) if(b<cenDef[j]) Centrality = j+1;

                p_npp->Fill(Centrality,Np);
                hCentrality->Fill(Centrality);
                if(cen && Centrality != cen) continue;          
                int Qcount =0;

                // float EtaCutLo; EtaCutHi;
                // if (e == 27) 
                // {
                //         EtaCutLo = 3.8; 
                //         EtaCutHi = 5.1;
                // }
                // else if (e == 19) 
                // {
                //         EtaCutLo = 3.2;
                //         EtaCutHi = 5.1;
                // }
                // else if (e == 14)
                // {
                //         EtaCutLo = 3.0; 
                //         EtaCutHi = 5.1;
                // } 
                // else if (e == 7) 
                // {
                //         EtaCutLo = 2.3;
                //         EtaCutHi = 5.1;
                // }
                TDatabasePDG *pdg = TDatabasePDG::Instance();
                //loop through matched primary tracks (the POI)
                for(int trki = 0; trki < NPTracks; trki++){
                        Px    = px_vec->at(trki);
                        Py    = py_vec->at(trki);
                        Pz    = pz_vec->at(trki);
                        float PID   = pid_vec->at(trki);

                        TParticlePDG *particle = pdg->GetParticle(PID);
                        Double_t mass;
                        if (particle)
                        {
                                mass = particle->Mass(); 
                        }
                        else if (abs(PID) != 42)
                        {
                                std::cerr<<"PID: "<<PID<<" particle not found"<<endl;
                        }
                        Pt = sqrt(Px*Px+Py*Py);
                        Phi = atan2(Py,Px);
                        Theta = atan2(Pt,Pz);
                        Eta = -log(tan(Theta/2.));

                        float E = sqrt(Pt*Pt + Pz*Pz + mass*mass);
                        float y = 0.5*log((E+Pz)/(E-Pz));

                        // if(PID!= 211 && PID!=-211 ) continue;//only pions
                        if (PID != 321 && PID != -321 && PID != 310 && PID != 311) continue; // only kaons
                        // if (abs(PID) == 211) EtaCutLo = -1*EtaCut; EtaCutHi = EtaCut;
                        // if (PID == 42) continue;
                        // if (abs(PID) != 2212 && abs(PID) != 2112) continue;
                        if(Pt > pt_trig_up || Pt < pt_trig_lo) continue;
                        if(Eta > EtaCut || Eta < -1*EtaCut) continue; // -1 < eta < 1

                //add whatever your analysis here, i.e.
                        float v2a =  cos(2.*Phi - 2.*psi)*100;
                        p_v2->Fill(1,v2a); //averages the v2a over the POI
                        hist_y->Fill(y);
                        Qcount++;
                
                }  //Track

        } // Event

        timer.Stop();
        std::cout<<"The time of min "<<min<<": ";
        timer.Print();
        fout.Write();
        delete chain;
        return;
}

void file_looper(int e = 0, string path = "", Int_t centrality_select = 1, Int_t job_select = 0)
{
        static Int_t min_select = 0;
        // TStopwatch timer; // Times the current min
        
        // Loops through all the centralities
        // If you want a specfic centrality, change the parameter value and value in while loop condition
        // e.g. for centrality 5: make parameter = 5, change while loop to <= 5
        // for centralities 4, 5, and 6: make parameter = 4, change while loop to <= 6
        // while(centrality_select <= 9) //change to 1 for cen1.. run cen5 compare
        while(centrality_select <= 9)
        {
                // Directory where the data will be stored
                char folder_name[200];
                // sprintf(folder_name, "/media/Disk_YIN/Andrew/New_Cuts/new_cen%dresults_newCuts",centrality_select);
                sprintf(folder_name, "/media/Disk_YIN/Andrew/New_Cuts/cen%d_%dGeV",centrality_select, e);
                // Command to create the directory if it doesn't exist
                char create_folder[200];
                sprintf(create_folder, "mkdir /media/Disk_YIN/Andrew/New_Cuts/cen%d_%dGeV",centrality_select, e);
                gSystem->Exec(create_folder);
                gSystem->cd(folder_name);

                // Loop through all jobs
                while(job_select < total_jobs)//should be total_jobs
                {
                        for(int min_select = 0;min_select<total_min; min_select++){
                                Sample_code(e, path, centrality_select, job_select, min_select); 
                        };
                        job_select++;
                }
                // Start over for next centrality 
                job_select = 0;
                min_select = 0;
                gSystem->cd("/media/Disk_YIN/Andrew/");
                centrality_select++;
        }
        
        return;

}



