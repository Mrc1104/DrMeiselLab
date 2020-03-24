





void TEST(){


	Int_t Chan;
	Double_t tof;
	TH1F *h1=new TH1F("h1","h1",4096,0,4095);


	ifstream fData;
	fData.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk0101/xy.dir/tag0012.xy");

		if(fData.good())
			cout << "FILE GOOD" << endl;
		else 
			cout << "FILE BAD" << endl;

		while(fData.good()){
			fData >> Chan;
			if(!fData.good()){
				break;
			}
			fData >> tof;
		//	cout << Chan << " " << tof << endl;
			h1->Fill(Chan,tof);
		}
	h1->Draw("histo");
	
//This runs other macros insite a macro, super cool!
	gROOT->ProcessLine(".x CodeRunThrough.C");
///////////////////////////////////////////////////////////////////////////////////////////////////



















}
