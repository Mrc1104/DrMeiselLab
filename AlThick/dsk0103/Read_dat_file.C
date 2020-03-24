#include <fstream>

//#include "Riostream.h"


TH1F *  Read_dat_file(char *filename, char *histname){

  Int_t bin, data;
  ifstream infile;
  char dummy[205];

  Int_t minimum=kMaxInt;
  Int_t maximum=kMinInt;

  infile.open(filename);
  infile.getline(dummy,200);
  bin=-1;

  // cout << "Before First Loop dummy=" << dummy  << endl;

  dummy[8] = '\0';
  Int_t i = 0;
  while(!infile.eof()){
    //   cout<< "Bin has been incremented bin=" << bin << endl;
    char blah = infile.get();
    if (isdigit (blah)) {
      dummy[i] = blah;
      i++;
    }
    if (i==8){
      dummy[8] = '\0';
      bin++;
      data = atol(dummy);
      i=0;

      //     cout<< "Bin has been incremented bin=" << bin << ", data=" << data << ", dummy=" << dummy << endl;
      if(bin>maximum)
	maximum=bin;
      if(bin<minimum)
	minimum=bin;
    }
  }
  
  //  infile.close();

  //cout << "after first loop"<< endl;

  if(minimum>maximum){
    cerr << "Minimum is greater than Maximum" << minimum << maximum
	 << endl;
    return;
  }
  //cout << "after test on minimum>maximum"<< endl;


  TH1F *myhist = new TH1F(histname,histname,maximum-minimum+1, minimum-.5, maximum+.5);

  //cout << "after making the hist"<< endl;

  infile.clear();
  infile.seekg(0, ios::beg);


  //  infile.open(filename);
  i=0;
  bin=-1;
  infile.getline(dummy,200);
  while (!infile.eof()) {
    char blah = infile.get();
    if (isdigit (blah)) {
      dummy[i] = blah;
      i++;
    }
    if (i==8){
      dummy[8] = '\0';
      bin++;
      data = atol(dummy);
      i=0;

      //if (bin%100==0)
      //	cout << "\t" << bin
      //	     << "\t" << data << endl;
       for (size_t j=0; j<data; j++){
	myhist->Fill(bin);
	  }
    }
  }
  

  cout << "Run Completed." << endl;
  infile.close();

  //  cout << myhist << endl;
  //mylocalhist = myhist;
  return myhist;

};


void mydat2xy(char * inputfile,char * prefix = "")
{


  TH1F * h2 = Read_dat_file(inputfile,"dumhist");
  TString mfname = inputfile;
  mfname = prefix + mfname;
  mfname = mfname + ".xy";
  ofstream mfout(mfname.Data());
  for (int i = 0; i < h2->GetNbinsX(); i++) { mfout << i << "\t" << h2->GetBinContent(i+1) << endl; }  


}

