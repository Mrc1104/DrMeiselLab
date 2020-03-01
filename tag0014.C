#include <iostream>


using namespace std;




void tag0014(){

	Int_t Chan;
	Double_t Count;
	TH1F *h1=new TH1F("h1","h1",4096,0,4095);

	ifstream fData;
	fData.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0103/tag0014.xy");
	if(fData.good())
		cout << "File exists" << endl;
	else
		cout << "File does not exist" << endl;

	while(fData.good()){
		fData >> Chan;
		if(!fData.good()){break;}
		fData >> Count;
		h1->Fill(Chan,Count);
	}

	h1->Draw("histo");





}
