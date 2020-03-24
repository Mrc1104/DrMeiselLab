#include "iostream"
#include "fstream"

void Random_Spectrum(){

//Dummy Variables
	Double_t Counts1;
	Double_t Channels1;

	Double_t Counts2;
	Double_t Channels2;

	Double_t Counts3;
	Double_t Channels3;

	Double_t Counts4;
	Double_t Channels4;

//Histograms
	TH1F *h1 =new TH1F("h1","Random Spectrum 1",4096,0,4095);
	TH1F *h2 =new TH1F("h2","Random Spectrum 2",4096,0,4095);
	TH1F *h3 =new TH1F("h3","Random Spectrum 3",4096,0,4095);
	TH1F *h4 =new TH1F("h4","Random Spectrum 4",4096,0,4095);
	TH1F *hTotal =new TH1F("hTotal","Total Random Spectrum",4096,0,4095);



ifstream inp1;
	inp1.open("/home/conaway/Documents/dsk0315/tag.xy/tag0200.xy");
		while(inp1.good()){
			inp1 >> Counts1;
				if(!inp1.good()){break;}
			inp1 >> Channels1;
				
				h1->Fill(Counts1,Channels1);
		}

ifstream inp2;
	inp2.open("/home/conaway/Documents/dsk0315/tag.xy/tag0201.xy");
		while(inp2.good()){
			inp2 >> Counts2;
				if(!inp2.good()){break;}
			inp2 >> Channels2;
				
				h2->Fill(Counts2,Channels2);
		}


ifstream inp3;
	inp3.open("/home/conaway/Documents/dsk0315/tag.xy/tag0202.xy");
		while(inp3.good()){
			inp3 >> Counts3;
				if(!inp3.good()){break;}
			inp3 >> Channels3;
				
				h3->Fill(Counts3,Channels3);
		}

ifstream inp4;
	inp4.open("/home/conaway/Documents/dsk0315/tag.xy/tag0203.xy");
		while(inp4.good()){
			inp4 >> Counts4;
				if(!inp4.good()){break;}
			inp4 >> Channels4;
				
				h4->Fill(Counts4,Channels4);
		}


//Draw Each individual Tag file
TCanvas *c1 =new TCanvas ("c1","Random Spectrums");
		c1->Divide(2,2);
c1->cd(1);
h1->Draw("histo");

c1->cd(2);
h2->Draw("histo");

c1->cd(3);
h3->Draw("histo");

c1->cd(4);
h4->Draw("histo");
			c1->Print("RandomSpectrum.pdf(","pdf");

TCanvas *c2 =new TCanvas("c2","Total Random Spectrum");
c2->cd();
hTotal->Add(h1,1);
hTotal->Add(h2,1);
hTotal->Add(h3,1);
hTotal->Add(h4,1);
hTotal->Draw("histo");
			c2->Print("RandomSpectrum.pdf)","pdf");


//Create a File for Total Random_Spectrum
	TFile *f0 =new TFile("/home/conaway/Documents/dsk0166/TotalRandomSpectrumFile.ROOT","New");			hTotal->Write();
		f0->Close();
			cout << "Total Random Spectrum File Created" << endl;

//Create a File for RandomSpectrum Detector1
	TFile *f1 =new TFile("/home/conaway/Documents/dsk0166/RandomSpectrumDet1.ROOT","New");
			h1->Write();
		f1->Close();
			cout << "RandomSpectrumDet1.ROOT created" << endl;
//Create a File for RandomSpectrum Detector2
	TFile *f2 =new TFile("/home/conaway/Documents/dsk0166/RandomSpectrumDet2.ROOT","New");
			h2->Write();
		f2->Close();
			cout << "RandomSpectrumDet2.ROOT created" << endl;

//Create a File for RandomSpectrum Detector3
	TFile *f3 =new TFile("/home/conaway/Documents/dsk0166/RandomSpectrumDet3.ROOT","New");
			h3->Write();
		f3->Close();
			cout << "RandomSpectrumDet3.ROOT created" << endl;
//Create a File for RandomSpectrum Detector4
	TFile *f4 =new TFile("/home/conaway/Documents/dsk0166/RandomSpectrumDet4.ROOT","New");
			h4->Write();
		f4->Close();
			cout << "RandomSpectrumDet4.ROOT created" << endl;

}//ends void
