#include <iostream>
#include <string>

using namespace std;

//AuxFunctions:
void Read_in_Data(string path_to_file,TH1F* Histo);
void DrawHisto(TCanvas* Canvas,TH1F* Histo);


void Code(){


/*---------------------------READ IN TOF1N for dsk6024-----------------------------------------*/
	TH1F *htag0014_dsk6024=new TH1F("htag0014_dsk6024","htag0014_dsk6024",4096,0,4095);
	Read_in_Data("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk6024/xy.dir/tag0014.xy",htag0014_dsk6024);
	TCanvas *ctag0014_dsk6024=new TCanvas("ctag0014_dsk6024","ctag0014_dsk6024");
	DrawHisto(ctag0014_dsk6024,htag0014_dsk6024);

/*---------------------------READ IN TOF2N for dsk6024-----------------------------------------*/
	TH1F *htag0024_dsk6024=new TH1F("htag0024_dsk6024","htag0024_dsk6024",4096,0,4095);
	Read_in_Data("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk6024/xy.dir/tag0024.xy",htag0024_dsk6024);
	TCanvas *ctag0024_dsk6024=new TCanvas("ctag0024_dsk6024","ctag0024_dsk6024");
	DrawHisto(ctag0024_dsk6024,htag0024_dsk6024);

/*---------------------------READ IN TOF3N for dsk6024-----------------------------------------*/
	TH1F *htag0034_dsk6024=new TH1F("htag0034_dsk6024","htag0034_dsk6024",4096,0,4095);
	Read_in_Data("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk6024/xy.dir/tag0034.xy",htag0034_dsk6024);
	TCanvas *ctag0034_dsk6024=new TCanvas("ctag0034_dsk6024","ctag0034_dsk6024");
	DrawHisto(ctag0034_dsk6024,htag0034_dsk6024);

/*---------------------------READ IN PSD for dsk6024-----------------------------------------*/
	TH1F *htag0054_dsk6024=new TH1F("htag0054_dsk6024","htag0054_dsk6024",4096,0,4095);
	Read_in_Data("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk6024/xy.dir/tag0054.xy",htag0054_dsk6024);
	TCanvas *ctag0054_dsk6024=new TCanvas("ctag0054_dsk6024","ctag0054_dsk6024");
	DrawHisto(ctag0054_dsk6024,htag0054_dsk6024);





/*----------------------------------BAD--DATA------------------------------------------------*/
/*---------------------------READ IN TOF1N for dsk103-----------------------------------------*/
/*	TH1F *htag0014_tof1n=new TH1F("htag0014_tof1n","htag0014_tof1n",4096,0,4095);
	//Read_in_Data("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0103/xy.dir/tag0014.xy",htag0014_tof1n);
	Read_in_Data("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0103/tag0014.xy",htag0014_tof1n);
	TCanvas *cTOF1N_0103=new TCanvas("cTOF1N_0103","cTOF1N_0103");
	DrawHisto(cTOF1N_0103,htag0014_tof1n);
*/


/*----------------------------------BAD--DATA------------------------------------------------*/
/*---------------------------READ IN TOF1N for dsk103-----------------------------------------*/
/*	TH1F *htag0014_dsk6030=new TH1F("htag0014_dsk6030","htag0014_dsk6030",4096,0,4095);
	Read_in_Data("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk6030/xy.dir/tag0014.xy",htag0014_dsk6030);
	TCanvas *ctag0014_dsk6030=new TCanvas("ctag0014_dsk6030","ctag0014_dsk6030");
	DrawHisto(ctag0014_dsk6030,htag0014_dsk6030);
*/

/*----------------------------------BAD--DATA------------------------------------------------*/
/*--------------------------READ IN TOF1N for dsk6050----------------------------------------*/
/*	TH1F *htag0014_dsk6050=new TH1F("htag0014_dsk6050","htag0014_dsk6050",4096,0,4095);
	Read_in_Data("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk6050/xy.dir/tag0014.xy",htag0014_dsk6050);
	TCanvas *ctag0014_dsk6050=new TCanvas("ctag0014_dsk6050","ctag0014_dsk6050");
	DrawHisto(ctag0014_dsk6050,htag0014_dsk6050);
*/










}






/*-----------------------------Auxillory Functions -----------------------------------------*/






void Read_in_Data(string path_to_file,TH1F* Histo){

	Int_t chan;
	Double_t count;
	ifstream fData;
	fData.open(path_to_file);
	if(fData.good())
		cout << path_to_file << " exists" << endl;
	else
		cout << path_to_file << " does not exist" << endl;
	while(fData.good()){
		fData >> chan;
		if(!fData.good()){
			break;
		}
		fData >> count;
		Histo->Fill(chan,count);
	}

}

void DrawHisto(TCanvas* Canvas,TH1F* Histo){
	Canvas->cd();
	Histo->Draw("histo");
}
	




