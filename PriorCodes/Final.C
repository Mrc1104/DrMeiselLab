#include "iostream"
#include "fstream"

void Final( Int_t Detector){
	cout << "Recall that" << " " << Detector << " " <<  "Corresponds to Detector" << Detector+1<< endl;
//Variables
	
	Int_t Counts;
	Double_t vRandom;
	Double_t Mean1;
	Double_t Mean2;
	Double_t Mean3;
	Double_t Mean4;

	Float_t D = 23.3494867; //ns (Distance to Detectors / Speed of Light)
	Double_t T = 200; //ns
	Double_t R;
	Double_t Sum;

//Files
TFile *fevt3 = TFile::Open("/home/conaway/Documents/Al_Analysis/AlData/evt118_Detector3");
TFile *fTime = TFile::Open("/home/conaway/Documents/dsk0166/TimeCalibrationFile1.ROOT");

ifstream fRandom;
	fRandom.open(Form("/home/conaway/Documents/dsk0315/tag.xy/tag020%d.xy",Detector));
ifstream fTimeMeanValues;
	fTimeMeanValues.open("/home/conaway/Documents/dsk0166/Mean_Values.txt");

//HistoGrams
TH1F *hevt3 =(TH1F*)fevt3->Get("hDET_3");

TH1F *hRandom =new TH1F("hRandom",Form("Random Spectrum Detector%d",Detector),4096,0,4095);
TH1F *hRandomEND =new TH1F("hRandomEND",Form("Random Spectrum Detector%d END",Detector),4096,0,4095);
TGraphErrors *gTime =(TGraphErrors*)fTime->Get("Graph");

//Read in Files
while(fRandom.good()){
	fRandom >> Counts;
if(!fRandom.good()){break;}
	fRandom >> vRandom; 
hRandom->Fill(Counts,vRandom);
hRandomEND->Fill(Counts,vRandom);
}
while(fTimeMeanValues.good()){
	fTimeMeanValues >> Mean1;
		if(!fTimeMeanValues.good()){break;}
	fTimeMeanValues >> Mean2 >> Mean3 >> Mean4;
}



//TCanvas
TCanvas	*cevt3 =new TCanvas("cevt3","Evt Detector3");
TCanvas *cRandom =new TCanvas("cRandom",Form("Random Spectrum Detector%d",Detector));
TCanvas *cRandomEND =new TCanvas("cRandomEND",Form("Random Spectrum Detector%d END",Detector));
TCanvas *cTime =new TCanvas("cTime","Linear Time Calibration");
//Draw
	cevt3->cd();
	hevt3->Draw();
	cRandom->cd();
	hRandom->Draw("histo");
	cTime->cd();
	gTime->Draw("APE");
	cRandomEND->cd();
	hRandomEND->GetXaxis()->SetRange(Mean4,4095);
	Int_t binmin = hRandomEND->GetMinimumBin();
	Double_t x_binmin = hRandomEND->GetBinCenter(binmin); cout << x_binmin << endl;
	hRandomEND->Draw("histo");


//File to save data
ofstream fData; ofstream fDataTail;
	fData.open(Form("/home/conaway/Documents/dsk0166/TvC.dir/Diff_Time_Calibration_Detector%d.txt",Detector));
	fDataTail.open(Form("/home/conaway/Documents/dsk0166/TvC.dir/Diff_Time_Calibration_Tail_Detector%d.txt",Detector));

//Calculations
Double_t S;

R_12 = T/hRandom->Integral(Mean1+1,Mean2);  
R_23 = T/hRandom->Integral(Mean2+1,Mean3);  
R_34 = T/hRandom->Integral(Mean3+1,Mean4);  
R_4 = T/hRandom->Integral(Mean4+1,x_binmin);  
	cout <<"R :" <<  R << " " <<  "hRandom->Integral(Mean1,Mean2); :" << hRandom->Integral(Mean1,Mean2) << endl;

for(Int_t i=350; i<=4095;i++){

	if(i == 350){
		Sum = 800;		/*Channel 350.186 = 800ns, not chan 350 */
	//	cout << i << " " << Sum << " " << S << endl;
				fData << i << " " << Sum << endl;
	}
	if(i>Mean1 & i<=Mean2-1){
		S = hRandom->Integral(i+1,Mean2); 
	//	cout << "i : " << i << " " << "S : " << S << endl;

		Sum =  3*T + S*R_12;
	//	cout << i << " " << Sum << " " << S << endl;
			
	//	Sum = D + Ty*R + 2*T + S*R; 
	//	cout << "Sum : " << Sum << endl;
				fData << i << " " << Sum << endl;
	}
	if(i == 1163){Sum = 600; /*Channel 1163.57 = 600ns, not chan 1163 */

	//	cout << i << " " << Sum << " " << S << endl;
				fData << i << " " << Sum << endl;

	}
	if(i>Mean2 & i<=Mean3-1){
		S = hRandom->Integral(i+1,Mean3);
	//	cout << "i : " << i << " " << "S : " << S << endl;
		Sum =  2*T + S*R_23;
	//	cout << i << " " << Sum << " " << S << endl;
		
	//	Sum = D + Ty*R + T + S*R;
	//	cout << "Sum : " << Sum << endl;
				fData << i << " " << Sum << endl;
		
	}
	if(i == 1981){Sum = 400; /*Channel 1981.21 = 400ns, not chan 1981 */

	//	cout << i << " " << Sum << " " << S << endl;
				fData << i << " " << Sum << endl;
	}

	if(i>Mean3 & i<=Mean4-1){
		S = hRandom->Integral(i+1,Mean4);
	//	cout << "i : " << i << " " << "S : " << S << endl;
		Sum =  1*T + S*R_34;
	//	cout << i << " " << Sum << " " << S << endl;
	
	//	Sum = D + Ty*R + S*R;
		//cout << "Sum : " << Sum << endl;
				fData << i << " " << Sum << endl;
	}
	if(i == 2791){Sum = 200; /*Channel 2791.87 = 800ns, not chan 2791 */

		//cout << i << " " << Sum << " " << S << endl;
				fData << i << " " << Sum << endl;
	}
	if(i > 2791 ){// This needs fixed
		S = hRandom->Integral(Mean4+1,i);
		Sum = 200 - (S*R_34); 
				fData << i << " " << Sum << endl;
		cout << i << " " << Sum << " " << S << endl;

	}


}//ends for

cout << Mean1 << " " << Mean2 << " " << Mean3 << " " << Mean4 << endl;




cout <<" " <<  " 				" <<  "!! Final.C Finished !!" << endl;
}//ends void
