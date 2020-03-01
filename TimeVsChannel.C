#include "iostream"
#include "fstream"
void TimeVsChannel(Int_t Run, Int_t Detector){
	// Creates a plot of Counts (y-axis) vs Time (x-axis)
//Inputs: --
//Run -- evtXXX that is wanted
//Detector -- Which Detector to use
cout << "Recall Detector " << Detector << " " << "Refers to " << Detector+1 << endl; 

//////////////////////////FILES////////////////////////////////
//fTC -- Opens the fuke for the Adjusted Time Calibraiton for each spectrum
//Opens the ROOT file for the shifted evt run
TFile *fTC =new TFile(Form("/home/conaway/Documents/dsk0166/Adjusted_Time_Cal.dir/AdjustedTimeCal_evt%dDetector0.ROOT",Run)); //the reason why I only use Det0 is because this is just the Differential Time calibration aligned wrts Det 0 so that T0 = ~23
TFile *fevt =new TFile(Form("/home/conaway/Documents/Al_Analysis/evt_Aligned.dir/evt%d_Detector%d",Run,Detector+1));



/////////////////////////GRAPHS///////////////////////////////////
//hevt -- gets the histo from the ROOT File (Detectors 2,3 are aligned wrts to Detector1)
//hTC -- histo for the adjusted time spectrum (adjusted based on the evt gamma spikes)
TH1F *hevt; 
	if(Detector == 0){
	hevt =(TH1F*)fevt->Get("hDET_1s");
	//cout << "Detector == 0" << endl;
	}
	else if(Detector == 1){
	hevt =(TH1F*)fevt->Get("hDET_2s");
	//cout << "Detector == 1" << endl;
	}
	else if(Detector == 2){
	hevt =(TH1F*)fevt->Get("hDET_3s");
	//cout << "Detector == 2" << endl;
	}
TH1F *hTC =(TH1F*)fTC->Get("hTC");
TH1F *hTVsC =new TH1F("hTVsC","TimeVsChannel",4096,0,4095);

hevt->Draw();
//////////////////////FILL hTVsC///////////////////////////////////
for(Int_t i=0; i<4096; i++){
Double_t Counts = hevt->GetBinContent(i);
Double_t Time = hTC->GetBinContent(i);
	hTVsC->Fill(Counts,Time);
//	hTVsC->Fill(Time,Counts);
}



//////////////////////TCANVAS//////////////////////////////////////
TCanvas *cTVsC =new TCanvas("hTVsC","Time Vs Calibration");




//////////////////////DRAW//////////////////////////////////////
cTVsC->cd();
hTVsC->GetXaxis()->SetRange(0,800);
hTVsC->GetXaxis()->SetTitle("Time (ns)");
hTVsC->GetYaxis()->SetTitle("Counts");
hTVsC->Draw("histo");



///////////////////SAVE hTVsC /////////////////////////////////////////
TFile *fTVsC=new TFile(Form("/home/conaway/Documents/dsk0166/ChannelvsTime.dir/TimeVsChannel_evt%d_Detector%d.ROOT",Run,Detector),"RECREATE");
hTVsC->Write();
fTVsC->Close();
cout << Form("TimeVsChannel_evt%d_Detector%d.ROOT",Run,Detector) << " Created" << endl;











cout << "			" << "DONE!" << endl;
}				//ends void





