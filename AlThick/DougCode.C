///This is a modified verson of Doug's TOF to Energy code. This file converts Time of Flight of Al27 to Energy
//Doug's code makes Counts vs Time to Counts vs Energy
#include "iostream"
#include "fstream"
#include <stdio.h>      /* printf */
#include <math.h>       /* fabs */


/////////////////NEXT//////////////////
///////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!Figure out how to plot the energy vs counts !!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//

void DougCode(Int_t Run, Int_t Detector){
//Inputs: -- 
//	Run -- The evtXXX trial that I wish to run through the code
//	Detector -- the detector that is wish to use

		cout << "Recall that Detector  " << Detector << "  refers to " << Detector+1 << endl;  
//Ouptuts:--
//	A plot of Counts vs Time
//	.Root file of Counts vs Time plot

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////FILES///////////////////////////

//HARDCODED
//Files:
//fTC is the adjusted Differiential Time Calibration
//fevt is the aligned run
//
TFile *fTC = TFile::Open(Form("/home/conaway/Documents/dsk0166/Adjusted_Time_Cal.dir/AdjustedTimeCal_evt%dDetector0.ROOT",Run)); 
fTC->ls();
	//Call the file where the Adjusted (T0=~23) Diff Time Calibration; only Det0 is called for each evt because all other Detector is aligned wrts Detector0
TFile *fevt = TFile::Open(Form("/home/conaway/Documents/Al_Analysis/evt_Aligned.dir/evt%d_Detector%d",Run,Detector+1));
fevt->ls();
	//Calls the file where the aligned (wrts det0 for each run) evt run per detector resides. 
TFile *fTvC = TFile::Open(Form("/home/conaway/Documents/dsk0166/ChannelvsTime.dir/TimeVsChannel_evt%d_Detector%d.ROOT",Run,Detector));
fTvC->ls();
	

//////////////////////////READ IN FILES///////////////////////////////////
//HARDCODED
//
//hevt
TH1F *hevt;
	if(Detector == 0){
	hevt =(TH1F*)fevt->Get("hDET_1s");
	//cout << "hevt =(TH1F*)fevt->Get(hDET_1s);" << endl;
	}
	if(Detector == 1){
	hevt =(TH1F*)fevt->Get("hDET_2s");
	//cout << "hevt =(TH1F*)fevt->Get(hDET_2s);" << endl;
	}
	if(Detector == 2){
	hevt =(TH1F*)fevt->Get("hDET_3s");
	//cout << "hevt =(TH1F*)fevt->Get(hDET_1s);" << endl;
	}
					//Detector4 is ignored for all runs
//fTC
//hTC is redundent because hTcalib1_120Al is equal to hTC and is used in code
TH1F *hTC=(TH1F*)fTC->Get("hTC");
TH1F *hTvC=(TH1F*)fTvC->Get("hTVsC");
					////////////////////VISUAL CHECK///////////////
						
						//Draw hevt and hTC
						TCanvas *cVCheck1 =new TCanvas("cVCheck1","hTC & hevt Visual Check");
						cVCheck1->Divide(1,2);
							cVCheck1->cd(1);
							hevt->Draw();
							cVCheck1->cd(2);
							hTvC->Draw("histo");
							
///////////////////////////HistoGrams////////////////////////////////////////
//
//Histo:
//	hEcalib1_120Al is the empty histogram bin that we are filling
//	hTcalib1_120Al is the time of flight we are filling with the the corrected time of flight 
//	hCvsE is the final Counts vs Energy plot --> Main goal of the code
//
//TH1F *hEcalib1_120Al=new TH1F("hEcalib1_120Al",Form("hEcalib1_120Al_evt%d_Detector%d",Run,Detector),3500,0,3500);
TH1F *hEcalib1_120Al=new TH1F("hEcalib1_120Al",Form("hEcalib1_120Al_evt%d_Detector%d",Run,Detector),3500,0,22);

//TH1F *hTcalib1_120Al=(TH1F*)fTC->Get("hTC");
TH1F *hTcalib1_120Al=(TH1F*)fTvC->Get("hTVsC");

TH1F *hCvsE =new TH1F("hCvsE",Form("Counts vs Energy evt%d Detector%d",Run,Detector),4096,0,3500);


/*
/////////////////Visual Check/////////////////////////////////////

//Draw hevt and hTC
TCanvas *cVCheck2 =new TCanvas("cVCheck2","hTcalib1_120Al");
cVCheck2->cd();
hTcalib1_120Al->Draw("histo");
 
*/




/////////I Need to Make variable list to intilize all variables used --->FINSIHED/////////////
////////////////////////////////////VARIABLES//////////////////////////////////////////////////////////////////////////////////////////////////////
//Since I am doing this detector by detector, I need 1 of each variable
//VhN relativistic velocties 
	Double_t Vh1;
//VlN	bin width // velocity width // uncertainty in velocity
	Double_t Vl1;
//TlN
	Double_t Tl1;
//ThN
	Double_t Th1;
//AlEBinError
	Double_t AlEBinError=0;
//Tcontent
	Double_t Tcontent;
//AlTlN
	Double_t AlTl1;
//AlThN
	Double_t AlTh1;
//Tdifference
	Double_t Tdifflow;
	Double_t Tdiffhigh;
	Double_t Tdifftot;
	Double_t Ttot;
	Double_t Tratio;

//note: Doug had 5000 bin histos but I only have I used 4096 bin histograms
//hEcalib1_120Al is an empty histo that he is filling with time vs counts; used for bookkeeping
  for(int i = 2;i<3500;i++)  
  {
      //
      // Use lower and upper energy bin edges to calculate related time
      //
     ////////// hEcalib1_120Al starts off as a blank histo/////////
      hEcalib1_120Al->SetBinContent(i,0);

							//939.55 is the m*c^2 of the neutron
      Vh1 = 3*pow(10,8)*sqrt(1-1/pow(1+((hEcalib1_120Al->GetBinLowEdge(i))/939.55),2));
      Vl1 = 3*pow(10,8)*sqrt(1-1/pow(1+(((hEcalib1_120Al->GetBinLowEdge(i))+(hEcalib1_120Al->GetBinWidth(i)))/939.55),2));
      
      ///5.0508 is the distance to the detector + half the thickness of the detector :: 0.0508 is the thickness of the detector (I wanna use half of the thickness)
      //	my detector distance is 7m so the numerator is 7.0254
      																	//Tl1 = 5.0508/Vl1*pow(10,9);
      Tl1 = 7.0254/Vl1*pow(10,9);
      																	//Th1 = 5.0508/Vh1*pow(10,9);
      Th1 = 7.0254/Vh1*pow(10,9);
     // cout << i << " " << "Vh1 :" << Vh1 << "  |  " <<  "Vl1 :" << Vl1 << "  |  " <<  "Tl1 : " << Tl1 << " | " << "Th1 : " << Th1 << endl;
      ////////////////////////////   NOTE     ///////////////////////////////
      //For Some reason, Th1 (i=0) = inf
      //Initlizing i @ 2 fixes (hotfix?)
      AlEBinError = 0;
      
      //Use time converted from edges of bins to find what TOF bin the Energy edges belong to
      //loop through those time bins
      for(int j = hTcalib1_120Al->FindBin(Tl1);j<hTcalib1_120Al->FindBin(Th1)+1;j++)//This for loop is getting the edge of each Diff TC and is equating it to the energy bin
	{
	  //
	  // 
	  //
	  Tcontent = hTcalib1_120Al->GetBinContent(j);
	  //cout <<i << " " << j << " "<<  "Tl1 : " << Tl1 << " | " << "Th1 : " << Th1 << " |  "<< "Tcontent  "<< Tcontent << endl;
	  //This is actual TOF bin edge, NOT FROM ENERGY
	  AlTl1 = hTcalib1_120Al->GetBinLowEdge(j);
	  
	  AlTh1 = AlTl1 + hTcalib1_120Al->GetBinWidth(j);
	  //
	  // if Energy bin falls within the same TOF:
	  if((hTcalib1_120Al->FindBin(Tl1)-hTcalib1_120Al->FindBin(Th1))==0)
	    {
	      //
	      // Energy bin is inside of one TOF bin, need to find % of
	      // TOF content that goes inside the energy bin
	      //
	      if((AlTl1<=Tl1)&&(AlTh1>=Th1))
		{ //the energy is confined to this bin
		  Tdifflow = fabs(Tl1-AlTl1);
		  Tdiffhigh = fabs(AlTh1-Th1);
		  Tdifftot = fabs(Tdifflow+Tdiffhigh);
		  Ttot = hTcalib1_120Al->GetBinWidth(j);     //Ttot = Th1-Tl1;
		  Tratio = 1-(Tdifftot/Ttot);
		  
		  AlEBinError =
			  sqrt(pow(AlEBinError,2)+pow(Tratio*hTcalib1_120Al->GetBinError(j),2));

		  hEcalib1_120Al->AddBinContent(i,Tratio*Tcontent);
		  hEcalib1_120Al->SetBinError(i,AlEBinError);
		  
		  //cout<<i<<"  "<<j<<"  test1     "<<Tratio<<"     "<<Tcontent<<endl;
		}
	      else
		{
		  // cout<<"I DON'T WANT TO SEE THIS ERROR!!!!"<<endl;
		}
	    }   //ends if((hTcalib1_120Al->FindBin(Tl1)-hTcalib1_120Al->FindBin(Th1))==0)

	  //
	  // Energy bin is spread across multiple TOF bins
	  else if((hTcalib1_120Al->FindBin(Th1)-hTcalib1_120Al->FindBin(Tl1))>0)
	    {//energy is distributed into several bins
	      if((AlTl1<Tl1)&&(AlTh1>Tl1)&&(AlTh1<Th1))
		{
		  Tdifflow = fabs(Tl1-AlTl1);
		  Ttot = hTcalib1_120Al->GetBinWidth(j);
		  //Ttot = Th1-Tl1;
		  Tratio = 1-(Tdifflow/Ttot);

		  AlEBinError = sqrt(pow(AlEBinError,2)+pow(Tratio*hTcalib1_120Al->GetBinError(j),2));
		  
		  hEcalib1_120Al->AddBinContent(i,Tratio*Tcontent);
		  hEcalib1_120Al->SetBinError(i,AlEBinError);

		  
		  // cout<<i<<"  "<<j<<"  test2     "<<Tratio<<"     "<<Tcontent<<endl;
		}
	
	      else if((AlTh1>Th1)&&(AlTl1<Th1)&&(AlTl1>Tl1))
		{
		  Tdiffhigh = fabs(AlTh1-Th1);
		  Ttot = hTcalib1_120Al->GetBinWidth(j);
		  //Ttot = Th1-Tl1;
		  Tratio = 1-(Tdiffhigh/Ttot);
		  
		  AlEBinError = sqrt(pow(AlEBinError,2)+pow(Tratio*hTcalib1_120Al->GetBinError(j),2));
		  
		  hEcalib1_120Al->AddBinContent(i,Tratio*Tcontent);
		  hEcalib1_120Al->SetBinError(i,AlEBinError);

		  
		 // cout<<i<<"  "<<j<<"  test3     "<<Tratio<<"     "<<Tcontent<<endl;
		}
	      //
	      // Energy bin fits entire TOF bin inside
	      else if((AlTh1<Th1)&&(AlTl1>Tl1))
		{

		  AlEBinError = sqrt(pow(AlEBinError,2)+pow(hTcalib1_120Al->GetBinError(j),2));
		  
		  hEcalib1_120Al->AddBinContent(i,Tcontent);
		  //this increments the height of each bin by 1
		  hEcalib1_120Al->SetBinError(i,AlEBinError);
		  
		  //cout<<i<<"  "<<j<<"  test4                "<<Tcontent<<endl;

		 
		}
	
	    }
	  cout <<i << " " << j << " "<<  "Tl1 : " << Tl1 << " | " << "Th1 : " << Th1 << " |  "<< "Tcontent : " << Tcontent << " | " << "Tratio : " << Tratio  << endl;
	
	}   //ends for loop for the Int_t j
    cout << " " <<endl;
//hCvsE->Fill(hEcalib1_120Al->GetBinContent(i),hevt->GetBinContent(i));

     }  //ends main for loop Int_t i

TCanvas *cCVsE =new TCanvas("cCVsE","Counts vs Energy");
cCVsE->cd();
hEcalib1_120Al->Draw("histo");
hCvsE->Draw("histo");
hEcalib1_120Al->Draw("histo");
cout << " " << endl;
cout << " " << endl;
cout << " " << endl;
cout << "                                        DONE!" << endl;
}      //ends void:w

