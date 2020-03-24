#include <iostream>
#include <iomanip>
#include <cmath>


using namespace std;

/******************************************************************* 
*  \file HW2
*  \brief Runs AlThick Data through all codes used on AlThin
*                                                                     
*  Author:      Ryan Conaway
*  Email:       mc321015@ohio.edu
*                                                                    
*  Lab Section: Dr. Meisel Low Energy Particle Physics
*                                                                    
*  Description: Runs through all data files for the ALThick and combines them into one histogram. All*  the AlThick were used wiht a f/16. It was determined to only use dsk0101 and dsk103. 
*                                                                    
*  Last Modified:        February 13, 2020
*                                                                    
*******************************************************************/


void CodeRunThrough(){

//Detector1
//DetA_XXXX 
//A - The Detector being called
//XXXX - The dsk it originates from

	
	Int_t Chan = 0;
	Double_t tof = 0;

	ifstream fDet1_0101;
	fDet1_0101.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0101/xy.dir/tag0012.xy");
	ifstream fDet1_0103;
	fDet1_0103.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0103/xy.dir/tag0012.xy");
	TFile *fevt0236_1=TFile::Open("/home/mrc1104/Documents/DrMeiselLab/AlThin/evt0236/Tofn/evt236_Detector1");
	fevt0236_1->ls();


	if(fDet1_0101.good() && fDet1_0103.good())
		cout << "Files Good" << endl;
	
	else
		cout << "File(s) Bad" << endl;
	TH1F *h1_0101=new TH1F("h1_0101","h1_0101",4096,0,4095);
	TH1F *h1_0103=new TH1F("h1_0103","h1_0103",4096,0,4095);
	TH1F *hDet1=new TH1F("hDet1","hDet1",4096,0,4095);
	TH1F *hevt0236_1=(TH1F*)fevt0236_1->Get("hDET_1");

	do{
		fDet1_0101 >> Chan >> tof;
	//	cout << Chan << " " << tof << endl;
		h1_0101->Fill(Chan,tof);
	}while(fDet1_0101.good());
	do{
		fDet1_0103 >> Chan >> tof;
	//	cout << Chan << " " << tof << endl;
		h1_0103->Fill(Chan,tof);
	}while(fDet1_0103.good());

	hDet1->Add(h1_0101);
	hDet1->Add(h1_0103);

//Detector2
//DetA_XXXX 
//A - The Detector being called
//XXXX - The dsk it originates from

	ifstream fDet2_0101;
	fDet2_0101.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0101/xy.dir/tag0022.xy");
	ifstream fDet2_0103;
	fDet2_0103.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0103/xy.dir/tag0022.xy");
	TFile *fevt0236_2=TFile::Open("/home/mrc1104/Documents/DrMeiselLab/AlThin/evt0236/Tofn/evt236_Detector2");
	fevt0236_2->ls();
	


	if(fDet2_0101.good() && fDet2_0103.good())
		cout << "Files Good" << endl;
	
	else
		cout << "File(s) Bad" << endl;
	TH1F *h2_0101=new TH1F("h2_0101","h2_0101",4096,0,4095);
	TH1F *h2_0103=new TH1F("h2_0103","h2_0103",4096,0,4095);
	TH1F *hDet2=new TH1F("hDet2","hDet2",4096,0,4095);
	TH1F *hevt0236_2=(TH1F*)fevt0236_2->Get("hDET_2");
	
	do{
		fDet2_0101 >> Chan >> tof;
	//	cout << Chan << " " << tof << endl;
		h2_0101->Fill(Chan,tof);
	}while(fDet2_0101.good());
	do{
		fDet2_0103 >> Chan >> tof;
	//	cout << Chan << " " << tof << endl;
		h2_0103->Fill(Chan,tof);
	}while(fDet2_0103.good());

	hDet2->Add(h2_0101);
	hDet2->Add(h2_0103);

//Detector3
//DetA_XXXX 
//A - The Detector being called
//XXXX - The dsk it originates from

	ifstream fDet3_0101;
	fDet3_0101.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0101/xy.dir/tag0032.xy");
	ifstream fDet3_0103;
	fDet3_0103.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0103/xy.dir/tag0034.xy");
	TFile *fevt0236_3=TFile::Open("/home/mrc1104/Documents/DrMeiselLab/AlThin/evt0236/Tofn/evt236_Detector3");
	fevt0236_3->ls();
	


	if(fDet3_0101.good() && fDet3_0103.good())
		cout << "Files Good" << endl;
	
	else
		cout << "File(s) Bad" << endl;
	TH1F *h3_0101=new TH1F("h3_0101","h3_0101",4096,0,4095);
	TH1F *h3_0103=new TH1F("h3_0103","h3_0103",4096,0,4095);
	TH1F *hDet3=new TH1F("hDet3","hDet3",4096,0,4095);
	TH1F *hevt0236_3=(TH1F*)fevt0236_3->Get("hDET_3");
	
	do{
		fDet3_0101 >> Chan >> tof;
		//cout << Chan << " " << tof << endl;
		h3_0101->Fill(Chan,tof);
	}while(fDet3_0101.good());
	do{
		fDet3_0103 >> Chan >> tof;
	//	cout << Chan << " " << tof << endl;
		h3_0103->Fill(Chan,tof);
	}while(fDet3_0103.good());

	hDet3->Add(h3_0101);
	hDet3->Add(h3_0103);

	TCanvas *cDetectors=new TCanvas("cDetectors","AlThick Detectors");
	cDetectors-> Divide(1,3);
	cDetectors->cd(1);
	h1_0101->Draw("histo");
	h1_0103->SetLineColor(kRed);
	h1_0103->Draw("SAME histo");
	hevt0236_1->SetLineColor(1);
	hevt0236_1->Draw("SAME histo");

	cDetectors->cd(2);
	h2_0101->Draw("histo");
	h2_0103->SetLineColor(kRed);
	h2_0103->Draw("SAME histo");
	hevt0236_2->SetLineColor(1);
	hevt0236_2->Draw("SAME histo");
	

	cDetectors->cd(3);
	h3_0101->Draw("histo");
	h3_0103->SetLineColor(kRed);
	h3_0103->Draw("SAME histo");
	hevt0236_3->SetLineColor(1);
	hevt0236_3->Draw("SAME histo");	
	
	TCanvas *c1=new TCanvas("c1","c1");
	c1->cd();
//	hevt0236_3->Draw("histo");
	h3_0103->Draw("histo");
}
