#include "iostream"
#include "fstream"

void AdjustTC(Int_t Run, Int_t Detector){
//Run -- evtXXXX that is wanted
//Detector -- Which Detector to use (3 total; 4th is ignored)
cout << "Recall Detector " << Detector << " " << "refers to " << Detector+1 << endl; 


////////////////////////FILES///////////////////////////////////
//fData -- Opens the file for the TimeCalibrationSectrum
//fevt -- Opens the ROOT file for the shifted evt runs
ifstream fData;
fData.open(Form("/home/conaway/Documents/dsk0166/TvC.dir/Diff_Time_Calibration_Detector%d.txt",Detector));

TFile *fevt1 = TFile::Open(Form("/home/conaway/Documents/Al_Analysis/evt_Aligned.dir/evt%d_Detector1",Run)); 
TFile *fevt2 = TFile::Open(Form("/home/conaway/Documents/Al_Analysis/evt_Aligned.dir/evt%d_Detector2",Run)); 
TFile *fevt3 = TFile::Open(Form("/home/conaway/Documents/Al_Analysis/evt_Aligned.dir/evt%d_Detector3",Run)); 
				

//////////////////////GRAPHS//////////////////////////////
//hevt -- gets evt histo from ROOT file (Detector 2,3 are aligned to Detector1)
//hTC -- histo for the time calibration spectrum
TH1F *hevt1 = (TH1F*)fevt1->Get("hDET_1s");
TH1F *hevt2 = (TH1F*)fevt2->Get("hDET_2s");
TH1F *hevt3 = (TH1F*)fevt3->Get("hDET_3s");
TH1F *hevt;
TH1F *hTC = new TH1F("hTC","TimeCalibration",4096,0,4095);




////////////////////////VARIABLES//////////////////////////
//t0 -- time it takes for light to reach the detector in ns (Detector distance is 7m)
//ti -- the time that corresponds with the gamma spike from evt run
//Chan -- the channel number in Diff_Time_Calibration
//vTime -- the corresponding time in Diff_Time_Calibration
Float_t t0 = 23.3494867;
Double_t ti;
Int_t Chan;
Double_t vTime;




//////////////////////Calibration Offset//////////////////
//While loop reads in data from Diff_Time_Calibration.txt 
while(fData.good()){
fData >> Chan;
					if(!fData.good()){break;}
fData >> vTime;				//cout << Chan << " " << vTime << endl;
hTC->Fill(Chan,vTime);
}					//ends while loop



//Finds the x-value of the gamma peak in the evt run
//Allows us to shift the Diff Time Calibration over based
//on the x-value 
hevt1->GetXaxis()->SetRange(2000,3500);
Int_t binmaxY = hevt1->GetMaximumBin();
Double_t binmaxX = hevt1->GetBinCenter(binmaxY);
ti = hTC->GetBinContent(binmaxX);
cout << binmaxY << " " << binmaxX << " " << ti <<  endl;

//Find the Difference between ti and t0; DelT
//Shift bin contents of hTC over so that binmaxX coorrespond with t0 (if the value of 
//hTC > 0, this prevents negative time)

Float_t delT = t0 - ti; cout << delT << endl;
for(Int_t i=0; i<=hTC->GetNbinsX();i++){
Double_t histValue = hTC->GetBinContent(i);
		if(histValue>0){
		hTC->SetBinContent(i,histValue+delT);
					//cout << histValue << endl;
		}			//ends if statementi

		
		}			//ends for loop

//////////////////////CHECK//////////////////////////////
//Check to see if the rebinning worked
Double_t tcheck = hTC->GetBinContent(binmaxX);
					cout << "Check" << " " << tcheck << endl;




/////////////////////////TCANVAS/////////////////////////
TCanvas *cTC =new TCanvas("cTC","Adjusted Time Calibration");
TCanvas *cevt =new TCanvas("cevt","Visual Alignment Check");

//////////////////////////DRAW/////////////////////////////
cTC->cd();
hTC->Draw("histo");

cevt->Divide(1,3);
cevt->cd(1);
hevt1->Draw();
cevt->cd(2);
hevt2->Draw();
cevt->cd(3);
hevt3->Draw();


///////////////////SAVE AJUSTED hTC///////////////////////////
TFile *fTC=new TFile(Form("/home/conaway/Documents/dsk0166/Adjusted_Time_Cal.dir/AdjustedTimeCal_evt%dDetector.ROOT",Run),"Create");
hTC->Write();
fTC->Close();
					cout << Form("AdjustedTimeCal_evt%dDetector%d.ROOT file created",Run, Detector) << endl;


}           //ends macro


