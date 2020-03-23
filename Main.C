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

	disk6024("/home/mrc1104/Documents/DrMeiselLab/AlThick/dsk6024/xy.dir");
	TimeCalibration();
	DifferentialTimeCalibration();
	
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
	//TCanvas *cDisk6024_Det1=new TCanvas("cDisk6024_Det1","dsk6024_Det1");
	//graph(cDisk6024_Det1, hDet1,xTitle,yTitle);
	Double_t Det1GammaPeak = GetGammaPeak(hDet1);
	AdjustTC("Disk6024_Det1",7.0,Det1GammaPeak);
/*
	//Detector 2 -- dsk6024
	TH1F *hDet2 =new TH1F("hDet2", "Disk6024 Det2",4096,0,4095);
	det(path_to_dir + "/tag0024.xy", hDet2);
	TCanvas *cDisk6024_Det2=new TCanvas("cDisk6024_Det2","dsk6024_Det2");
	graph(cDisk6024_Det2, hDet2,xTitle,yTitle);
	//Detector 3 -- dsk6024
	TH1F *hDet3 =new TH1F("hDet3", "Disk6024 Det3",4096,0,4095);
	det(path_to_dir + "/tag0034.xy", hDet3);
	TCanvas *cDisk6024_Det3=new TCanvas("cDisk6024_Det3","dsk6024_Det3");
	graph(cDisk6024_Det3, hDet3,xTitle,yTitle);
	*/

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
	read_in_data("/home/mrc1104/Documents/DrMeiselLab/AlThick/PriorCodes/tag0010.xy",hTLC);
	TCanvas *cTLC=new TCanvas("TLC", "Linear Time Calibration");
	graph(cTLC,hTLC, xTitle,yTitle);
	
	//There exists four files for the random sprectrum: tag0200,tag0201,tag0202, tag0203. They need to be added to create the Total random (hRandom)
	
	TH1F *hRandom1=new TH1F("hRandom1","Random Source1",4096,0,4095);
	read_in_data("/home/mrc1104/Documents/DrMeiselLab/AlThick/PriorCodes/tag0200.xy",hRandom1);
	TH1F *hRandom2=new TH1F("hRandom2","Random Source2",4096,0,4095);
	read_in_data("/home/mrc1104/Documents/DrMeiselLab/AlThick/PriorCodes/tag0201.xy",hRandom2);
	TH1F *hRandom3=new TH1F("hRandom3","Random Source3",4096,0,4095);
	read_in_data("/home/mrc1104/Documents/DrMeiselLab/AlThick/PriorCodes/tag0202.xy",hRandom3);
	TH1F *hRandom4=new TH1F("hRandom4","Random Source4",4096,0,4095);
	read_in_data("/home/mrc1104/Documents/DrMeiselLab/AlThick/PriorCodes/tag0203.xy",hRandom4);

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
	fDiffSpect.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/DiffSpect.txt");
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
	read_in_data("/home/mrc1104/Documents/DrMeiselLab/AlThick/DiffSpect.txt",hDiffSpect);
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
 * Function: GetGammaPeak()
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
 *****************************************************/
void AdjustTC(string Run_Det, Double_t distance_to_detector, Double_t GammaPeakBin){

	//Differential Time Calibration (unshifted)
	TH1F *hDiffSpect=new TH1F("hDiffSpect","hDiffSpect",4096,0,4095);
	read_in_data("/home/mrc1104/Documents/DrMeiselLab/AlThick/DiffSpect.txt",hDiffSpect);
	//Adjust for the constant offset
	Double_t offset = hDiffSpect->GetBinContent(3500); //this is how high above zero the graph is
	//cout <<  " hDiffSpect->GetBinContent(3500) " << hDiffSpect->GetBinContent(3500) << endl;
	/*for(Int_t i=2791; i<4096;i++){ // shift hDiffSpect upwards/downwards to remove offset
	
		Double_t originalValue = hDiffSpect->GetBinContent(i);
		Double_t newValue = originalValue - offset;
		if(newValue >= 0){ // negative time makes no sense
			hDiffSpect->SetBinContent(i,newValue);
		}
		else{
			hDiffSpect->SetBinContent(i,0);
		}
	}
	*/
	 DTLC_Residual(hDiffSpect);


	/*
	//Calculate the distance needed to shift
	const Double_t speed_of_light = 3.33564; // speed of light in m/(ns)
	Double_t t0 = (distance_to_detector / speed_of_light); //time for gamma ray to reach detector
	Double_t ti = hDiffSpect->GetBinContent(GammaPeakBin);
	cout <<  " hDiffSpect->GetBinContent(3500) " << hDiffSpect->GetBinContent(3500) << endl;

	Double_t DelT = t0-ti; cout << GammaPeakBin << " " << DelT << endl;

	//Shift the Differential Spectrum
	for(Int_t i=0; i<=hDiffSpect->GetNbinsX();i++){
		Double_t histValue = hDiffSpect->GetBinContent(i);
	//	if(histValue>0){
			hDiffSpect->SetBinContent(i,histValue+DelT);
		//}
	}
	//Check to see if the rebinning worked
	Double_t tcheck = hDiffSpect->GetBinContent(GammaPeakBin);
	//cout << "Check" << " " << tcheck << endl;
	

	
	
	 Ok, so this is going to be handwavy but near the end, there is an apparent constant offset 
	 * of about 26. Subtracting DelT leaves about 2 (stored in tcheck).
	 * This is apparent after 3500 in the diff spect.
	 * I am just going to loop through the for loop again and subtract this offset to get it equal
	 * to 0 at the bin max. 
	 
	//Shift the Differential Spectrum again
	for(Int_t i=0; i<=hDiffSpect->GetNbinsX();i++){
		Double_t histValue = hDiffSpect->GetBinContent(i);
		//if(histValue>0){
			hDiffSpect->SetBinContent(i,histValue-tcheck);
		//}
	}
	tcheck =  hDiffSpect->GetBinContent(GammaPeakBin);
	cout << "Check" << " " << tcheck << endl;

	const char* xTitle = "Chan"; // const char* is for naming the x-axis; if you wish to change the 
	const char* yTitle = "Counts";
	TCanvas *cAdjustDS=new TCanvas("cAdjustDS","cAdjustDS");
	graph(cAdjustDS,hDiffSpect,xTitle,yTitle);

	//store the Run-Dependant adjusted Diff Spect in a file
	ofstream AdjustedDiffSpect;
	AdjustedDiffSpect.open("/home/mrc1104/Documents/DrMeiselLab/AlThick/" + Run_Det + "_AdjustedTC.txt");
	for(Int_t i=0;i<4096;i++){
		AdjustedDiffSpect << i << " " << hDiffSpect->GetBinContent(i) << endl;
	}
	AdjustedDiffSpect.close();
	*/
	return;
} //AdjustTC

/******************************************************
 * Function: DTLC_Residual()
 * Brief: Creates a residual plot of the Differential Time Calibration and the Linear Calibration 
 *
 * SubFunctions:
 *			graph()
 *
 * Parameters:
 *			distance_to_detector
 *			DTLC_Residual();
 * Note: The distance to decide T0 (distance to detector) might be different so I left it as an 
 * input value in the form of distance to detector
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
		Double_t diff = hDiffSpect->GetBinContent(i)-g1->Eval(i);
		hResidual->Fill(i,diff);
	}
	TCanvas *cResidual=new TCanvas("cResidual","cResidual");
	graph(cResidual,hResidual,xTitle,yTitle);
	

	return;


}
