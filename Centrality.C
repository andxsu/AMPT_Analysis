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

const int Nbin = 2000;
void Centrality(int job=100) {

TH1D* Hist_b = new TH1D("Hist_b","Hist_b",Nbin,0,20);
TH1D* Hist_refmult = new TH1D("Hist_refmult","Hist_refmult",Nbin,0,500);
TH1D* Hist_np = new TH1D("Hist_np","Hist_np",Nbin,0,300);

TChain *chain = new TChain("tree");
char fname[200],fname2[200];
//sprintf(fname,"/star/data01/pwg/zhiwanxu/AMPT_run/7GeV-minbias/output/7GeV/data*.root");
//sprintf(fname,"/star/data01/pwg/zhiwanxu/AMPT_run/19GeV-minbias/output/19GeV/data*.root");
// chain->Add("/star/data01/pwg/zhiwanxu/AMPT_run/14GeV-minbias/output/14GeV/data*.root");
// chain->Add("/star/data01/pwg/zhiwanxu/AMPT_run/clean/200GeV-cleaned/200GeV-1490/data*.root");
// sprintf(fname,"/star/data01/pwg/zhiwanxu/AMPT_run/clean/27GeV-cleaned/27GeV-500events/data*.root");
// chain->Add("/star/data01/pwg/zhiwanxu/AMPT_run/clean/27GeV-cleaned/27GeV-500events/data*.root");
//chain->Add("/star/data01/pwg/zhiwanxu/AMPT_run/clean/27GeV-cleaned/27GeV-5kevents-1/data*.root");
//chain->Add("/star/data01/pwg/zhiwanxu/AMPT_run/clean/27GeV-cleaned/27GeV-5kevents-2/data*.root");
//chain->Add("/star/data01/pwg/zhiwanxu/AMPT_run/clean/27GeV-cleaned/27GeV-data10M/data*.root");

// sprintf(fname, "/media/Disk_YIN/AMPT-7GeV/7GeV-8k/data*.root");
// chain->Add("/media/Disk_YIN/AMPT-7GeV/7GeV-8k/data_12*.root");

// sprintf(fname, "/media/Disk_YIN/AMPT-19GeV/19GeV-19M/data*.root");
// chain->Add("/media/Disk_YIN/AMPT-19GeV/19GeV-19M/data_12*.root");

// sprintf(fname, "/media/Disk_YIN/AMPT-27GeV/27GeV-10M-2/data*.root");
// chain->Add("/media/Disk_YIN/AMPT-27GeV/27GeV-10M-2/data_12*.root");
sprintf(fname, "/media/Disk_YIN/AMPT-200GeV/200GeV-cleaned/200GeV-data2055/data*.root");
chain->Add("/media/Disk_YIN/AMPT-200GeV/200GeV-cleaned/200GeV-data2055/data_1*.root");

// sprintf(fname, "/media/Disk_YIN/AMPT-14GeV/14GeV-cleaned/14GeV-8k/data*.root");
// chain->Add("/media/Disk_YIN/AMPT-14GeV/14GeV-cleaned/14GeV-8k/data_12*.root");
int Nevent = chain->GetEntries();

        for(int i=0;i<Nevent;i++) {
                // if((i+1)%1000==0) cout<<"Processing entry == "<< i+1 <<" == out of "<<Nevent<<".\n"; //if piping to stdout
                chain->GetEntry(i);
                TLeaf* leaf_b   = chain->GetLeaf("imp");
		TLeaf* leaf_Np_p= chain->GetLeaf("npp");
                TLeaf* leaf_Np_t= chain->GetLeaf("npt");
		TLeaf* leaf_Mult= chain->GetLeaf("refmult");
                float b = leaf_b->GetValue(0);
		int Np = leaf_Np_p->GetValue(0) + leaf_Np_t->GetValue(0);
		if(Np<3) continue;
		int Mult = leaf_Mult->GetValue(0);
                Hist_b->Fill(b);
		Hist_refmult->Fill(Mult);
		Hist_np->Fill(Np);
/*
		TLeaf* leaf_PID   = chain->GetLeaf("PID");
		int netCharge = 0;
          	for(int trk = 0; trk < Mult; trk++) {
                	int PID   = leaf_PID->GetValue(trk);
			if(fabs(PID)==531||fabs(PID)==533||fabs(PID)==513||fabs(PID)==511||fabs(PID)==4312||fabs(PID)==4332||fabs(PID)==441||fabs(PID)==4132||fabs(PID)==4112||fabs(PID)==421||fabs(PID)==423||fabs(PID)==3322||fabs(PID)==313||fabs(PID)==423||fabs(PID)==2112||fabs(PID)==111||fabs(PID)==22||fabs(PID)==311||fabs(PID)==3122) continue;
			if(PID==211) netCharge += 1;   //pion+
			else if(PID==-211) netCharge += -1;
			else if(PID==321) netCharge += 1;   //kaon+
                        else if(PID==-321) netCharge += -1;   
                        else if(PID==2212) netCharge += 1;   //proton
                        else if(PID==-2212) netCharge += -1;
                        else if(PID==11||PID==13) netCharge += -1;  //e- ,mu- 
                        else if(PID==-11||PID==-13) netCharge += 1;  
                        else if(PID==42) netCharge += 1;  //deuteron
                        else if(PID==-42) netCharge += -1;
			else if(PID==-3222||PID==3112||PID==3114||PID==3312||PID==3314||PID==3334) netCharge += -1;
                        else if(PID==3222||PID==-3112||PID==-3114||PID==-3312||PID==-3314||PID==-3334) netCharge += 1;
			else if(PID==411||PID==413||PID==431 ||PID==433) netCharge += 1;  //D+
                        else if(PID==-411||PID==-413||PID==-431 ||PID==-433) netCharge += -1;
                        else if(PID==4122||PID==4232||PID==4212||PID==4322||PID==5222||PID==-5132||PID==4412) netCharge += 1;  //Lambda_c+
                        else if(PID==-4122||PID==-4232||PID==-4212||PID==-4322||PID==-5222||PID==5132||PID==-4412) netCharge +=-1;  
                        else if(PID==4222||PID==4422) netCharge += 2; //cascade_c++
                        else if(PID==-4222||PID==-4422) netCharge +=- 2;
                        else if(PID==521||PID==523) netCharge += 1; //B0
                        else if(PID==-521||PID==-523) netCharge +=-1;
			else cout<<"PID = "<<PID<<",";
		}
		if(netCharge != 2*79) {cout<<endl;cout<<"net charge = "<<netCharge - 2*79<<endl;}
*/
        }

        float sum = (float)Hist_b->Integral();
        int j=1;
        for(int i=0;i<Nbin;i++) {
                float add = (float)Hist_b->Integral(1,i+1);
                float center = Hist_b->GetBinCenter(i+1);
                center += float(20)/Nbin/2;
                if(add > sum*0.05*j) {cout<<0.05*j<<"  "<<center<<"  "<<(add-sum*0.05*j)/(sum*0.05*j)<<endl;j++;}


        }


char foname[200];
sprintf(foname,"cen0.200GeV.root");
TFile *fout = new TFile(foname,"recreate");
Hist_b->Write();
Hist_refmult->Write();
Hist_np->Write();
fout->Close();
}
