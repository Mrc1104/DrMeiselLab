/********************************************************
 * File: Main.C
 * Brief: Runs AlThick data through analysis code; Time Calibration an Time to Energy conversion code
 * 
 * Name: Ryan Conaway
 * Email: mc321015@ohio.edu
 * Date: 3/9/2020
 * Last Modified: 3/10/2020
 * Discription: Runs ALthick data through the code required for detector efficiency calculations
 *
 * Notes:
 *		Graph DiffSpect to verify code works
 ******************************************************/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void det(string path_to_file, TH1F* histo);
void read_in_data(string _path_to_file, TH1F* histo);
void disk6024(string path_to_dir);
Double_t GetGammaPeak(TH1F* histo);
void graph(TCanvas* canvas, TH1F* histo, const char* xTitle, const char* yTitle);

void TimeCalibration();
void DifferentialTimeCalibration();
void CreateDifferentialCalibration(TH1F* hRandom);
void AdjustTC(string Run_Det, Double_t distance_to_detector, Double_t GammaPeakBin);
void DTLC_Residual(TH1F* hDiffSpect);

void CountsVsTime(TH1F* histo, string Run_Det, string path_to_shiftedTC);
void DougCode(string Run_Det, string path_to_CountsVsTime);


/******************************************************
 * Function: Main()
 * Brief: Main function of the program. It calls other functions on a run and detector basis
 *
 * SubFunctions:
 *			disk6024()
 * Parameters:
 *
 *****************************************************/
void Main(){

	disk6024("/home/mrc/Documents/DrMeiselLab/AlThick/dsk6024/xy.dir");
	//TimeCalibration();
	//DifferentialTimeCalibration();
	
	return EXIT_SUCCESS;
} //Main()


/******************************************************
 * Function: disk6024()
 *ubsfunction of Mian(). Segments each disk as a seperate function for data management
 * SubFunctions:
 *			det()
 *			GetGammaPeak()
 * Parameters:
 *			string path_to_dir -- the path to the data directory for disk6024
 *****************************************************/

void disk6024(string path_to_dir){

	const char* xTitle = "Chan"; // const char* is for naming the x-axis; if you wish to change the 
	const char* yTitle = "Counts";

	//Detector 1 -- dsk6024
	TH1F *hDet1 =new TH1F("hDet1", "Disk6024 Det1",4096,0,4095);
	det(path_to_dir + "/tag0014.xy", hDet1);
	TCanvas *cDisk6024_Det1=new TCanvas("cDisk6024_Det1","dsk6024_Det1");
	graph(cDisk6024_Det1, hDet1,xTitle,yTitle);
	//Double_t Det1GammaPeak = GetGammaPeak(hDet1);
	//AdjustTC("Disk6024_Det1",6.93,Det1GammaPeak);
	CountsVsTime(hDet1,"Disk6024_Det1","/home/mrc/Documents/DrMeiselLab/AlThick/Disk6024_Det1_AdjustedTC.txt");
	DougCode("Disk6024_Det1", "/home/mrc/Documents/DrMeiselLab/AlThick/Disk6024_Det1_TimevsChannel.txt"); 

	
	//Detector 2 -- dsk6024
	TH1F *hDet2 =new TH1F("hDet2", "Disk6024 Det2",4096,0,4095);
	det(path_to_dir + "/tag0024.xy", hDet2);
	TCanvas *cDisk6024_Det2=new TCanvas("cDisk6024_Det2","dsk6024_Det2");
	graph(cDisk6024_Det2, hDet2,xTitle,yTitle);
	//Double_t Det2GammaPeak = GetGammaPeak(hDet2);
	//AdjustTC("Disk6024_Det2",6.93,Det2GammaPeak);
	CountsVsTime(hDet2,"Disk6024_Det2","/home/mrc/Documents/DrMeiselLab/AlThick/Disk6024_Det2_AdjustedTC.txt");
	DougCode("Disk6024_Det2", "/home/mrc/Documents/DrMeiselLab/AlThick/Disk6024_Det2_TimevsChannel.txt"); 
	
	//Detector 3 -- dsk6024
	TH1F *hDet3 =new TH1F("hDet3", "Disk6024 Det3",4096,0,4095);
	det(path_to_dir + "/tag0034.xy", hDet3);
	TCanvas *cDisk6024_Det3=new TCanvas("cDisk6024_Det3","dsk6024_Det3");
	graph(cDisk6024_Det3, hDet3,xTitle,yTitle);
	//Double_t Det3GammaPeak = GetGammaPeak(hDet3);
	//AdjustTC("Disk6024_Det3",6.93,Det3GammaPeak);
	CountsVsTime(hDet3,"Disk6024_Det3","/home/mrc/Documents/DrMeiselLab/AlThick/Disk6024_Det3_AdjustedTC.txt");
	DougCode("Disk6024_Det3", "/home/mrc/Documents/DrMeiselLab/AlThick/Disk6024_Det3_TimevsChannel.txt"); 

	return;
} //disk6024

/******************************************************
 * Function: TimeCalibration()
 * Brief: Creates the time calibration using the linear and differential time calibration (dsk0166 and dsk0315 respectively)
 *
 * SubFunctions:
		DifferentialCalibration();
 * Parameters:
 ***************************************************void**/

void TimeCalibration(){
	const char* xTitle = "Chan"; // const char* is for naming the x-axis; if you wish to change the 
	const char* yTitle = "Counts";

	TH1F *hTLC=new TH1F("hTLC","Linear Time Calibration",4096,0,4095);
	read_in_data("/home/mrc/Documents/DrMeiselLab/AlThick/PriorCodes/tag0010.xy",hTLC);
	TCanvas *cTLC=new TCanvas("TLC", "Linear Time Calibration");
	graph(cTLC,hTLC, xTitle,yTitle);
	
	//There exists four files for the random sprectrum: tag0200,tag0201,tag0202, tag0203. They need to be added to create the Total random (hRandom)
	
	TH1F *hRandom1=new TH1F("hRandom1","Random Source1",4096,0,4095);
	read_in_data("/home/mrc/Documents/DrMeiselLab/AlThick/PriorCodes/tag0200.xy",hRandom1);
	TH1F *hRandom2=new TH1F("hRandom2","Random Source2",4096,0,4095);
	read_in_data("/home/mrc/Documents/DrMeiselLab/AlThick/PriorCodes/tag0201.xy",hRandom2);
	TH1F *hRandom3=new TH1F("hRandom3","Random Source3",4096,0,4095);
	read_in_data("/home/mrc/Documents/DrMeiselLab/AlThick/PriorCodes/tag0202.xy",hRandom3);
	TH1F *hRandom4=new TH1F("hRandom4","Random Source4",4096,0,4095);
	read_in_data("/home/mrc/Documents/DrMeiselLab/AlThick/PriorCodes/tag0203.xy",hRandom4);

	//Total Random Spectrum
	TH1F *hRandom = new TH1F("hRandom", "hRandom",4096,0,4095);
	hRandom->Add(hRandom1);
	hRandom->Add(hRandom2);
	hRandom->Add(hRandom3);
	hRandom->Add(hRandom4);
	TCanvas *cR=new TCanvas("cR", "Random");
	graph(cR,hRandom, xTitle,yTitle);
	
	CreateDifferentialCalibration(hRandom);


	return;
} //TimeCalibration

/******************************************************
 * Function: CreateDifferentialCalibration()
 * Brief: Fills in the Linear Time Calibration with the Random Source runs to create a differential 
 * time calibration. 
 *
 * SubFunctions:
 *			
 * Parameters:
 * 			TH1F* hRandom -- random spectrum

 *****************************************************/
void CreateDifferentialCalibration(TH1F* hRandom){
	ofstream fDiffSpect;
	fDiffSpect.open("/home/mrc/Documents/DrMeiselLab/AlThick/DiffSpect.txt");
	//Calculations
	Double_t S;
	Double_t T = 200; //ns
	Double_t R;
	Double_t Sum;

	/* MeanValues are located in /PriorCodes/Mean_Values.txt */
	/* They are the bins for each TLC Peak w/ Mean1          */
	/* corresponding to 800ns                                */
	Double_t Mean1 = 350.186,
			 Mean2 = 1163.57,
			 Mean3 = 1981.21,
			 Mean4 = 2791.87;
	Double_t R_12 = T/hRandom->Integral(Mean1+1,Mean2);  
	Double_t R_23 = T/hRandom->Integral(Mean2+1,Mean3);  
	Double_t R_34 = T/hRandom->Integral(Mean3+1,Mean4);  
	//Double_t R_4 = T/hRandom->Integral(Mean4+1,3500);  
	Double_t R_4 = T/hRandom->Integral(Mean4+1,3500);  

	/* The Differential Time Calibration is created by reading through the random source spectrum */
	/* and integrting between the Linear Time Calibration peaks (a la Mean1 - Mean4). At each TLC */
	/* peak, the time is known (ie 800ns at Mean1, ..., 200ns at mean4) so the random source      */
	/* essentially fills in the gaps.                                                             */
	for(Int_t i=350; i<=4095;i++){

		if(i == 350){
			Sum = 800;		/*Channel 350.186 = 800ns, not chan 350 */
			//cout << i << " " << Sum << " " << S << endl;
					fDiffSpect << i << " " << Sum << endl;
		}
		if(i>Mean1 & i<=Mean2-1){
			S = hRandom->Integral(i+1,Mean2); 
			Sum =  3*T + S*R_12;
			fDiffSpect << i << " " << Sum << endl;
		}
		if(i == 1163){Sum = 600; /*Channel 1163.57 = 600ns, not chan 1163 */
			fDiffSpect << i << " " << Sum << endl;
		}
		if(i>Mean2 & i<=Mean3-1){
			S = hRandom->Integral(i+1,Mean3);
			Sum =  2*T + S*R_23;
			fDiffSpect << i << " " << Sum << endl;
		}
		if(i == 1981){Sum = 400; /*Channel 1981.21 = 400ns, not chan 1981 */
			fDiffSpect << i << " " << Sum << endl;
		}
		if(i>Mean3 & i<=Mean4-1){
			S = hRandom->Integral(i+1,Mean4);
			Sum =  1*T + S*R_34;
			fDiffSpect << i << " " << Sum << endl;
		}
		if(i == 2791){Sum = 200; /*Channel 2791.87 = 800ns, not chan 2791 */
			fDiffSpect << i << " " << Sum << endl;
		}
		/*if(i > 2791 ){// This needs fixed
			S = hRandom->Integral(Mean4+1,i);
			Sum = 200 - (S*R_34); 
			fDiffSpect << i << " " << Sum << endl;
			//cout << i << " " << Sum << " " << S << endl;
			}*/
			if((i > 2791) && (i<3501) ){// This needs fixed
			S = hRandom->Integral(Mean4+1,i);
			Sum = 200 - (S*R_12); //looks like the other ones except for the end; possible offset apparent. This one looks the best imo
			//Sum = 200 - (S*R_23); //Looks like the other ones except the tail is awful
			//Sum = 200 - (S*R_34);  // looks decemt except for the ned
			//Sum = 200 - (S*R_4); //Residual plot looks atrocious
			fDiffSpect << i << " " << Sum << endl;
			//cout << i << " " << Sum << " " << S << endl;
			}
	}
	fDiffSpect.close();
	

	return;
} //CreateDifferentialCalibration()


/******************************************************
 * Function: DifferentialCalibration()
 * Brief: Draws the Differential Time Calibration and creates residual to check accuracy 
 *
 * SubFunctions:
 *			
 * Parameters:
 * 			TH1F* hRandom -- random spectrum *
 * Note:
 * 			The Residual can only be accurately obtained after shifting the Differential
 * Time Calibration so that the b-intercept aligns with the run-specific gamma peak!
 *****************************************************/
void DifferentialTimeCalibration(){
	const char* xTitle = "Chan"; // const char* is for naming the x-axis; if you wish to change the 
	const char* yTitle = "Counts";

	TH1F *hDiffSpect=new TH1F("hDiffSpect","hDiffSpect",4096,0,4095);
	read_in_data("/home/mrc/Documents/DrMeiselLab/AlThick/DiffSpect.txt",hDiffSpect);
	TCanvas *cDiffSpect=new TCanvas("cDiffSpect","cDiffSpect");
	graph(cDiffSpect,hDiffSpect, xTitle,yTitle);
	
	/*RESIDUAL    
	 * This method for detemrining the residual seems to work. However, I need to adjust 
	 * my hDiffSpect so that it crosses time=0 at the location of the gamma peak.	
	 * This location is going to be run specific.
	 */

	 DTLC_Residual(hDiffSpect);

	return;
} //DifferentialTimeCalibration


/******************************************************
 * Function: det()
 * Brief: Opens up and creates histograms for the data file 
 *
 * SubFunctions:	
 *			read_in_data()
 * Parameters:
 *			string path_to_file -- location of the file in current directory (wrts parent function) to open
 			TH1F* histo -- histogram to store data
 *****************************************************/
void det(string path_to_file, TH1F* histo){
	read_in_data(path_to_file, histo);
	return;
} // det

/******************************************************
 * Function: read_in_data()
 * Brief: reads in file into a histogram 
 *
 * SubFunctions:
 *			
 * Parameters:
 *			string path_to_file -- location of file to open and read in data from
 *			TH1F* histo -- histogram to store data
 *
 *****************************************************/
void read_in_data(string path_to_file, TH1F* histo){
	ifstream file;
	file.open(path_to_file);
	if(file.good()){
		cout << "File Opened" << endl;
	}
	else{
		cout << "File DNE" << endl;
	}
	Int_t chan;
	Double_t count;
	while(file.good()){
		file >> chan;
		if(!file.good()){break;}
		file >> count;
		histo->Fill(chan,count);
	}
	file.close();
	return;
} // read_in_data


/******************************************************
 * Function: graph()
 * Brief: graphs in histogram
 *
 * SubFunctions:
 *			
 * Parameters:
 * 			TCanvas* canvas -- Canvas to draw on
 * 			TH1F* histo -- histogram to be drawn
 * 			string xTitle -- Title for the x axis
 * 			string yTitle -- Title for the y axis
 *****************************************************/
void graph(TCanvas* canvas, TH1F* histo, const char* xTitle, const char* yTitle){
	canvas->cd();
	histo->GetXaxis()->SetTitle(xTitle);
	histo->GetYaxis()->SetTitle(yTitle);
	histo->Draw("histo");
} //graph()


/******************************************************
 * Function: GetGammaPeak()
 * Brief: Obtains the x-value of the gamma peak. Necessary for adjusting the Differential 
 * Time Spectrum.
 *
 * SubFunctions:
 *			graph()
 * Parameters:
 * 			TH1F* histo -- histogram tto extract the gamma peak channel from
 *****************************************************/

Double_t GetGammaPeak(TH1F* histo){
	histo->GetXaxis()->SetRange(3000,4000);
	Double_t binmaxY = histo->GetMaximumBin();
	Double_t binmaxX = histo->GetBinCenter(binmaxY);
	/* //Visual Check
	const char* xTitle = "Chan"; // const char* is for naming the x-axis; if you wish to change the 
	const char* yTitle = "Counts";
	TCanvas *cZOOM=new TCanvas("cZOOM","cZOOM");
	graph(cZOOM,histo,xTitle,yTitle);
	*/
	return binmaxX;
} // GetGammaPeak


/******************************************************
 * Function: AdjustTC()
 * Brief: Obtains the x-value of the gamma peak. Necessary for adjusting the Differential 
 * Time Spectrum.
 *
 * SubFunctions:
 *			graph()
 *
 * Parameters:
 *			distance_to_detector
 *			DTLC_Residual();
 * Note: The distance to decide T0 (distance to detector) might be different so I left it as an 
 * input value in the form of distance to detector
 *
 * Comments: So the Differential Time Calibration should go to zero, however it does not. 
 * Therefore, where the calibrations stops changing, I am setting that as the fastest possible time (time it takes for light to reahc the detector)
 *****************************************************/
void AdjustTC(string Run_Det, Double_t distance_to_detector, Double_t GammaPeakBin){
	Double_t C_ns = 0.2998; //speed of light in ns
	Double_t t0 = distance_to_detector / C_ns;   cout << t0 << endl;

	//Differential Time Calibration (unshifted)
	TH1F *hDiffSpect=new TH1F("hDiffSpect","hDiffSpect",4096,0,4095);
	read_in_data("/home/mrc/Documents/DrMeiselLab/AlThick/DiffSpect.txt",hDiffSpect);	
	
	//Vertical Shift
	Double_t Offset = hDiffSpect->GetBinContent(3500); //cout << Offset << endl;
	for(Int_t i=350; i<=3500;i++){
		Double_t Diff = Offset - t0;
		Double_t oldValue = hDiffSpect->GetBinContent(i);
		hDiffSpect->SetBinContent(i,oldValue-Diff);
	}
	//Check:: the new value should equal t0
	cout << hDiffSpect->GetBinContent(3500) << endl;

	//Now to horizontall adjust the differiental calibration so that t0 aligns with the gamma peak
	TH1F *hDiffSpectShifted=new TH1F("hDiffSpectiShifted","hDiffSpectShifted",4096,0,4095);
	Double_t distance = GammaPeakBin - 3500;
	for(Int_t i=0;i<4096;i++){
		if(i<350){
			hDiffSpectShifted->SetBinContent(i,0);
		}
		else{
			Double_t value = hDiffSpect->GetBinContent(i);
			hDiffSpectShifted->SetBinContent(i+distance,value);
		}
	}

	const char* xTitle = "Chan"; // const char* is for naming the x-axis; if you wish to change the 
	const char* yTitle = "Counts";
	TCanvas *cAdjustDS=new TCanvas("cAdjustDS","cAdjustDS");
	graph(cAdjustDS,hDiffSpectShifted,xTitle,yTitle);

	//store the Run-Dependant adjusted Diff Spect in a file
	ofstream AdjustedDiffSpect;
	AdjustedDiffSpect.open("/home/mrc/Documents/DrMeiselLab/AlThick/" + Run_Det + "_AdjustedTC.txt");
	for(Int_t i=0;i<4096;i++){
		AdjustedDiffSpect << i << " " << hDiffSpect->GetBinContent(i) << endl;
	}
	AdjustedDiffSpect.close();
	return;
} // AdjustTC


/******************************************************
 * Function: DTLC_Residual()
 * Brief: Creates a residual plot of the Differential Time Calibration and the Linear Calibration 
 *
 * SubFunctions:
 *			graph()
 *
 * Parameters:
 *		TH1F*( hDiffSpect
 * Note: 
 *****************************************************/
void DTLC_Residual(TH1F* hDiffSpect){
	const char* xTitle = "Chan"; // const char* is for naming the x-axis; if you wish to change the 
	const char* yTitle = "Counts";	
	//Values from Linear Time Calibration
	Double_t Mean1 = 350.186,
			 Mean2 = 1163.57,
			 Mean3 = 1981.21,
			 Mean4 = 2791.87;
	TGraph *g1=new TGraph();
	g1->SetPoint(0,Mean1,800);
	g1->SetPoint(1,Mean2,600);
	g1->SetPoint(2,Mean3,400);
	g1->SetPoint(3,Mean4,200);
	TCanvas *cg1=new TCanvas("cg1","cg1");
	cg1->cd();
	g1->SetMarkerStyle(8);
	g1->Draw("AP");
	TH1F *hResidual=new TH1F("hResidual","Residual",4096,0,4095);
	for(Int_t i=351;i<3501;i++){
		Double_t diff = hDiffSpect->GetBinContent(i) - g1->Eval(i); 
		hResidual->Fill(i,diff);
	}
	TCanvas *cResidual=new TCanvas("cResidual","cResidual");
	graph(cResidual,hResidual,xTitle,yTitle);
	

	return;
}


/******************************************************
 * Function: CountsVsTime()
 * Brief: Create the counts vs Time plotas
 *
 * SubFunctions:
 *			graph()
 *
 * Parameters:
 *		TH1F* histo
 *		string Run_Det
 *		string path_to_shiftedTC
 * Note: 
 *****************************************************/
void CountsVsTime(TH1F* histo, string Run_Det, string path_to_shiftedTC){
	TH1F *hTC=new TH1F("hTC","hTC",4096,0,4095);
	TH1F *hTVsC=new TH1F("hTVsC","hTVsC",4096,0,4095);
	read_in_data(path_to_shiftedTC,hTC);
	/*const char* xTitle = "Chan"; // const char* is for naming the x-axis; if you wish to change the axis-titles, change these values 
	const char* yTitle = "Time";	
	TCanvas *c100=new TCanvas("c100","c100");
	graph(c100,hTC,xTitle,yTitle);
*/
	ofstream fTimeVsChannel;
	fTimeVsChannel.open("/home/mrc/Documents/DrMeiselLab/AlThick/" + Run_Det + "_TimevsChannel.txt");
	for(Int_t i=0; i<4096; i++){
		Double_t Counts = histo->GetBinContent(i);
		Double_t Time = hTC->GetBinContent(i);
	//	hTVsC->Fill(Counts,Time);
	//	fTimeVsChannel << Counts << " " << Time << endl;
		hTVsC->Fill(Time,Counts);
		fTimeVsChannel << Time << " " << Counts << endl;
	}
	const char* xTitle = "Time"; // const char* is for naming the x-axis; if you wish to change the axis-titles, change these values 
	const char* yTitle = "Counts";
	hTVsC->GetXaxis()->SetRange(5,900);
	TCanvas *cTvC=new TCanvas("cTvC","cTvC");
	graph(cTvC,hTVsC,xTitle,yTitle);

} //CountsVsTime



/******************************************************
 * Function: DougCode()
 reates Counts vs Energy Spectrum using modified code form Doug
 *
 * SubFunctions:
 *
 * Parameters:
 *		string Run_Det
 *		string path_to_CountsVsTime
 * Note: 
 *****************************************************/
void DougCode(string Run_Det, string path_to_CountsVsTime){

	
	  ///////// CHECK TO MAKE SURE DETECTOR DISTANCE IS CORRECT //////////////////////////
		cout << endl; 
		cout << endl; 
		cout << "Warning: Double Check to make sure the detector distance is correct in Doug's Code. Line " << __LINE__ << endl;
		cout << endl;  
		cout << endl; 
		///////// CHECK TO MAKE SURE DETECTOR DISTANCE IS CORRECT ///////////////////////////

	TH1F *hTcalib1_120Al=new TH1F("hTcalib1_120Al","hTcalib1_120Al",4096,0,4095);
	read_in_data(path_to_CountsVsTime,hTcalib1_120Al);
	
  //hTcalib1_120Al->Draw();
	TH1F *hEcalib1_120Al=new TH1F("hEcalib1_120Al","hEcalib1_120Al",4096,0,4095);

////////////////////////////////////VARIABLES//////////////////////////////////////////////////////////////////////////////////////////////////////
//VhN relativistic velocties 
	Double_t Vh1=0;
//VlN	bin width // velocity width // uncertainty in velocity
	Double_t Vl1=0;
//TlN
	Double_t Tl1=0;
//ThN
	Double_t Th1=0;
//AlEBinError
	Double_t AlEBinError=0;
//Tcontent
	Double_t Tcontent=0;
//AlTlN
	Double_t AlTl1=0;
//AlThN
	Double_t AlTh1=0;
//Tdifference
	Double_t Tdifflow=0;
	Double_t Tdiffhigh=0;
	Double_t Tdifftot=0;
	Double_t Ttot=0;
	Double_t Tratio=0;

//hEcalib1_120Al is an empty histo that he is filling with time vs counts; used for bookkeeping
  for(int i = 2;i<4096;i++)  { //For Some reason, Th1 (i=0) = inf
      							//Initlizing i @ 2 fixes (hotfix?)

      // Use lower and upper energy bin edges to calculate related time
     ////////// hEcalib1_120Al starts off as a blank histo/////////
      hEcalib1_120Al->SetBinContent(i,0);
		//939.55 is the m*c^2 of the neutron
      Vh1 = 3*pow(10,8)*sqrt(1-1/pow(1+((hEcalib1_120Al->GetBinLowEdge(i))/939.55),2));
      Vl1 = 3*pow(10,8)*sqrt(1-1/pow(1+(((hEcalib1_120Al->GetBinLowEdge(i))+(hEcalib1_120Al->GetBinWidth(i)))/939.55),2));
		
	 /*
	  ///////// CHECK TO MAKE SURE DETECTOR DISTANCE IS CORRECT //////////////////////////
		cout << endl; 
		cout << endl; 
		cout << "Warning: Double Check to make sure the detector distance is correct in Doug's Code. Line " << __LINE__ << endl;
		cout << endl;  
		cout << endl; 
		///////// CHECK TO MAKE SURE DETECTOR DISTANCE IS CORRECT ///////////////////////////
		*/
		//5.0508 is the distance to the detector + half the thickness of the detector :: 0.0508 is the thickness of the detector (I wanna use half of the thickness)
      //	my detector distance is 7m so the numerator is 7.0254
	  //	my detector distance is 6.93m so the numerator is 6.9554
      																	//Tl1 = 5.0508/Vl1*pow(10,9);
      //Tl1 = 7.0254/Vl1*pow(10,9);
      Tl1 = 6.955/Vl1*pow(10,9);
      																	//Th1 = 5.0508/Vh1*pow(10,9);
      //Th1 = 7.0254/Vh1*pow(10,9);
      Th1 = 6.9554/Vh1*pow(10,9);
     // cout << i << https://www.desmos.com/calculator" " << "Vh1 :" << Vh1 << "  |  " <<  "Vl1 :" << Vl1 << "  |  " <<  "Tl1 : " << Tl1 << " | " << "Th1 : " << Th1 << endl;
      ////////////////////////////   NOTE     ///////////////////////////////
      AlEBinError = 0;
      
      //Use time converted from edges of bins to find what TOF bin the Energy edges belong to
      //loop through those time bins
      for(int j = hTcalib1_120Al->FindBin(Tl1);j<hTcalib1_120Al->FindBin(Th1)+1;j++)//This for loop is getting the edge of each Diff TC and is equating it to the energy bin
	{
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
	  //cout <<i << " " << j << " "<<  "Tl1 : " << Tl1 << " | " << "Th1 : " << Th1 << " |  "<< "Tcontent : " << Tcontent << " | " << "Tratio : " << Tratio  << endl;
	
	}   //ends for loop for the Int_t j
    //cout << " " <<endl;
//hCvsE->Fill(hEcalib1_120Al->GetBinContent(i),hevt->GetBinContent(i));

     }  //ends main for loop Int_t i
	TCanvas *cCVsE =new TCanvas("cCVsE","Counts vs Energy");
	cCVsE->cd();
	//hCvsE->Draw("histo");
	hEcalib1_120Al->Draw("histo");
	

	ofstream fCountsvsEnergy;
	fCountsvsEnergy.open("/home/mrc/Documents/DrMeiselLab/AlThick/" + Run_Det + "_CountsvsEnergy.txt");
	for(Int_t k=0;k<4096;k++){
		fCountsvsEnergy << k << " " << hEcalib1_120Al->GetBinContent(k) << endl;
		//cout << k << " " <<  hEcalib1_120Al->GetBinContent(k) << endl;
	}
	fCountsvsEnergy.close();

	return;
} //Doug's Code
















