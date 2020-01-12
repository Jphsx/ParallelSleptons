#ifndef HISTS
#define HISTS


#include "TH1D.h"
#include "TH2D.h"
#include "ROOT/TThreadedObject.hxx"
#include "TLorentzVector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "myselector.C"


typedef  ROOT::TThreadedObject<TH1D> hist;

class histset{
	
	public:
	   histset(std::string tag);	
	   void init(); 

	   void AnalyzeEntry(myselector& s); 
	 //  void AnalyzeEntry(mytreevalues& s);

	   //bookeeping enumeration: (if we do this we dont need to worry about hist ptr copies and merging)
//	   enum th1d_index{ind_METHist, ind_cat0NjetSHist, ind_cat1NjetSHist, numTH1Hist};
//	   enum th2d_index{ind_cat0PtcmPtisrDphiCMIHist, ind_cat1PtcmPtisrDphiCMIHist, numTH2Hist};
	 enum th1d_index{ind_METHist,ind_genselLPt,  ind_gensmuLPt,ind_genselLEta, ind_gensmuLEta, ind_genselLPhi, ind_gensmuLPhi, ind_genselRPt, ind_gensmuRPt, ind_genselREta, ind_gensmuREta, ind_genselRPhi, ind_gensmuRPhi, ind_genLSPPt, ind_genLSPEta, ind_genLSPPhi, ind_genElePt, ind_genEleEta, ind_genElePhi, ind_genMuPt, ind_genMuEta, ind_genMuPhi, ind_Ele1Pt, ind_Ele2Pt, ind_EleM, ind_Mu1Pt, ind_Mu2Pt, ind_MuM, ind_Njet_ISR_C0, ind_Njet_ISR_C1, ind_Njet_S_C0, ind_Njet_S_C1, ind_Nlep_S_C0, ind_Nlep_S_C1, ind_Njet_a_C0, ind_Njet_a_C1, ind_Njet_b_C0, ind_Njet_b_C1, ind_Nlep_a_C0, ind_Nlep_a_C1, ind_Nlep_b_C0, ind_Nlep_b_C1, ind_PTCM_C0, ind_PTCM_C1, ind_MS_C0, ind_MS_C1, ind_PS_C0, ind_PS_C1, ind_PTISR_C0, ind_PTISR_C1, ind_RISR_C0, ind_RISR_C1, ind_MISR_C0, ind_MISR_C1 ,numTH1Hist};
	 enum th2d_index{numTH2Hist};
	

	   // make a big vector and load enumerated histograms onto the vector
	 std::vector<ROOT::TThreadedObject<TH1D>* >  TH1Manager{};
	 std::vector<ROOT::TThreadedObject<TH2D>* >  TH2Manager{};


	  //locate the histogram and perform ptr copying 
	  void FillTH1(int index, double x, double w);
	  void FillTH2(int index, double x, double y);
	
	  void WriteHist(std::string outputfilename, std::string TFileOption);

	//Tag for compiling multiple datasets into same file which share the same plots
	std::string _tag{}; 
	//this tag will automatically appended to the variable name in each histogram on write
	
	//cut sequence and event selection variables
	void initCounts();
	void processCutFile(std::string cutfile);
	void printSelectionTables(std::ofstream& f );

	std::vector<std::string> _cutsequence{"nocut"}; //cut names read from cut list
	std::vector<double> _npass{};//raw
	std::vector<double> _npassw{};//weighted
	double _weight{};
	std::vector<double> _cutval{1.}; //cut values read from cut list

	//for now add absolute path of cut file
	std::string cutlist = "/home/j342a201/slepton/ParallelSleptons/sleptonAnalysis/cutfile.list";

	
	

};

histset::histset(std::string tag = ""){
	_tag = tag; 
	
	 std::vector<ROOT::TThreadedObject<TH1D>*>  Manager1(numTH1Hist);
	TH1Manager=Manager1;

	 std::vector<ROOT::TThreadedObject<TH2D>*>  Manager2(numTH2Hist);
	TH2Manager=Manager2;

	init();
	processCutFile(cutlist);
	initCounts();

}
bool nocut(double& count, double weight){
	count = count + weight;
	return true;
	}
bool performcut(double& count, double weight,  double observedvalue, double cutvalue, std::function<bool(double,double)> func ){
	if( func(observedvalue, cutvalue) ){
		count = count + weight;
		return true;
	}
	return false;

}
///CUT LIST I/O
template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
void histset::processCutFile(std::string cutfile){
        ifstream file;
        file.open (cutfile);
        std::string line;
        while (getline(file, line)) {
        	std::vector<std::string> strs = split(line,  ' ');
                //push back cut names 
                _cutsequence.push_back(strs[0]);
                _cutval.push_back(std::stod(strs[1]));
         }
        for(int i=0; i< _cutsequence.size(); i++){
                std::cout<<_cutsequence.at(i)<<" "<<_cutval.at(i)<<" ";
        }
        std::cout<<std::endl;
	
        file.close();
}
//END CUT LIST I/O
void histset::initCounts(){
        std::vector<double> c(_cutsequence.size());	
	std::vector<double> cw(_cutsequence.size());

	for(unsigned int i=0; i<_cutsequence.size(); i++){
		c[i] = 0.;
		cw[i] = 0.;
	}
	_npass = c;
	_npassw = cw;
}
void histset::init(){

//init TH1D
	TH1Manager.at(ind_METHist) = new ROOT::TThreadedObject<TH1D>("METHist", "MET;GeV;Entries per 5 GeV bin", 140, 100, 800);

	//slepton gen kinematics includes LandR selectron and smuon
	TH1Manager.at(ind_genselLPt) =new hist("genselLPt", "Gen. #widetilde{e_{L}} Pt;GeV;Entries per 20 GeV bin", 50,0.,1000.);
	TH1Manager.at(ind_gensmuLPt) =new hist("gensmuLPt", "Gen. #widetilde{#mu_{L}} Pt;GeV;Entries per 20 GeV bin", 50, 0., 1000.);
	TH1Manager.at(ind_genselLEta) =new hist("genselLEta", "Gen. #widetilde{e_{L}} #eta;#eta;Entries per 0.1 bin",60, -3.,3.);
	TH1Manager.at(ind_gensmuLEta) =new hist("gensmuLEta", "Gen. #widetilde{#mu_{L}} #eta;#eta;Entries per 0.1 bin",60,-3.,3.);
	TH1Manager.at(ind_genselLPhi) =new hist("genselLPhi", "Gen. #widetilde{e_{L}} #phi;#phi;Entries per 0.1 bin",63,-3.14,3.14);
	TH1Manager.at(ind_gensmuLPhi) =new hist("gensmuLPhi", "Gen. #widetilde{#mu_{L}} #phi;#phi;Entries per 0.1 bin",63,-3.14,3.14);
	TH1Manager.at(ind_genselRPt) =new hist("genselRPt", "Gen. #widetilde{e_{R}} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000.);
	TH1Manager.at(ind_gensmuRPt) =new hist("gensmuRPt", "Gen. #widetilde{#mu_{R}} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000.);
	TH1Manager.at(ind_genselREta) =new hist("genselREta", "Gen. #widetilde{e_{R}} #eta;#eta;Entries per 0.1 bin",60,-3.,3.);
	TH1Manager.at(ind_gensmuREta) =new hist("gensmuREta", "Gen. #widetilde{#mu_{R}} #eta; #eta;Entries per 0.1 bin",60, -3.,3.);
	TH1Manager.at(ind_genselRPhi) =new hist("genselRPhi", "Gen. #widetilde{e_{R}} #phi; #phi;Entries per 0.1 bin",63,-3.14,3.14);
	TH1Manager.at(ind_gensmuRPhi) =new hist("gensmuRPhi", "Gen. #widetilde{#mu_{R}} #phi; #phi; Entries per 0.1 bin", 63, -3.14,3.14);
	
	
	TH1Manager.at(ind_genLSPPt) =new hist("genLSPPt", "Gen. #widetilde{#chi^{0}_{1}} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000.);
	TH1Manager.at(ind_genLSPEta) =new hist("genLSPEta", "Gen. #widetilde{#chi^{0}_{1}} #eta; #eta; Entries per 0.1 bin", 50, -3., 3.);
	TH1Manager.at(ind_genLSPPhi) =new hist("genLSPPhi", "Gen. #widetilde{#chi^{0}_{1}} #phi; #phi; Entries per 0.1 bin", 50, -3.14, 3.14);

	TH1Manager.at(ind_genElePt) =new hist("genElePt", "Gen e^{#pm} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000.);
	TH1Manager.at(ind_genEleEta) =new hist("genEleEta", "Gen e^{#pm} #eta; #eta, Entries per 0.1 bin",60, -3., 3.);
	TH1Manager.at(ind_genElePhi) =new hist("genElePhi", "Gen e^{#pm} #phi; #phi, Entries per 0.1 bin",63, -3.14,3.14);

	TH1Manager.at(ind_genMuPt) =new hist("genMuPt", "Gen #mu^{#pm} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000.);
	TH1Manager.at(ind_genMuEta) =new hist("genMuEta", "Gen #mu^{#pm} #eta; #eta; Entries per 0.1 bin",60, -3.,3.);
	TH1Manager.at(ind_genMuPhi) =new hist("genMuPhi", "Gen #mu^{#pm} #phi; #phi; Entries per 0.1 bin",63, -3.14,3.14);


	//reco variables pt,eta,phi of leading 2 leptons
	TH1Manager.at(ind_Ele1Pt) =new hist("Ele1Pt", "Reco Leading e^{#pm} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000); 
	TH1Manager.at(ind_Ele2Pt) =new hist("Ele2Pt", "Reco Sub-Leading e^{#pm} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000);
	TH1Manager.at(ind_EleM) =new hist("EleM", "Reco M_{ee} ; GeV; Entries per 20 GeV bin", 25, 0., 500);
	
	TH1Manager.at(ind_Mu1Pt) =new hist("Mu1Pt", "Reco Leading #mu^{#pm} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000);
        TH1Manager.at(ind_Mu2Pt) =new hist("Mu2Pt", "Reco Sub-Leading #mu^{#pm} Pt; GeV; Entries per 20 GeV bin", 50, 0., 1000);
        TH1Manager.at(ind_MuM) =new hist("MuM", "Reco M_{#mu#mu} ; GeV; Entries per 20 GeV bin", 25, 0., 500);	
	
	//susy special variables seperated between cat0 & cat1 => C0 C1 
	//cat 0
	TH1Manager.at(ind_Njet_ISR_C0) =new hist("Njet_ISR_C0", "N ISR Jets 0;N jets",11,-0.5,10.5);
	TH1Manager.at(ind_Njet_S_C0) =new hist("Njet_S_C0", "N S Jets 0; N jets",11,-0.5,10.5);
	TH1Manager.at(ind_Nlep_S_C0) = new hist("Nlep_S_C0", "N S Lep 0; N Lep",11,-0.5,10.5);
	TH1Manager.at(ind_Njet_a_C0) = new hist("Njet_a_C0", "N jets A 0; N jets",11,-0.5,10.5);
	TH1Manager.at(ind_Njet_b_C0) = new hist("Njet_b_C0", "N jets B 0; N jets",11,-0.5,10.5);
	TH1Manager.at(ind_Nlep_a_C0) = new hist("Nlep_a_C0", "N Lep A 0; N Lep",11,-0.5,10.5);
	TH1Manager.at(ind_Nlep_b_C0) = new hist("Nlep_b_C0", "N Lep B 0; N Lep",11,-0.5,10.5);
	TH1Manager.at(ind_PTCM_C0) = new hist("PTCM_C0", "PTCM 0; GeV; Entries per 10 GeV bin",10,0,100);
	TH1Manager.at(ind_MS_C0) = new hist("MS_C0", "MS 0; ;Entries per 50 bin",60,0,3000);
	TH1Manager.at(ind_PS_C0) = new hist("PS_C0", "PS 0; ;Entries per 20 bin",50,0.,1000.);
	TH1Manager.at(ind_PTISR_C0) = new hist("PTISR_C0", "PTISR 0;GeV;Entries per 20 GeV bin",50,0,1000);
	TH1Manager.at(ind_RISR_C0) = new hist("RISR_C0", "RISR 0; ;Entries per 0.5 bin",100,0,20);
	TH1Manager.at(ind_MISR_C0) = new hist("MISR_C0", "MISR 0; ;Entries per 20 bin",50,0.,1000);

	//cat1
      	TH1Manager.at(ind_Njet_ISR_C1) = new hist("Njet_ISR_C1", "N ISR Jets 1;N jets",11,-0.5,10.5);
        TH1Manager.at(ind_Njet_S_C1) = new hist("Njet_S_C1", "N S Jets 1; N jets",11,-0.5,10.5);
        TH1Manager.at(ind_Nlep_S_C1) = new hist("Nlep_S_C1", "N S Lep 1; N Lep",11,-0.5,10.5);
        TH1Manager.at(ind_Njet_a_C1) = new hist("Njet_a_C1", "N jets A 1; N jets",11,-0.5,10.5);
        TH1Manager.at(ind_Njet_b_C1) = new hist("Njet_b_C1", "N jets B 1; N jets",11,-0.5,10.5);
        TH1Manager.at(ind_Nlep_a_C1) = new hist("Nlep_a_C1", "N Lep A 1; N Lep",11,-0.5,10.5);
        TH1Manager.at(ind_Nlep_b_C1) = new hist("Nlep_b_C1", "N Lep B 1; N Lep",11,-0.5,10.5);
        TH1Manager.at(ind_PTCM_C1) = new hist("PTCM_C1", "PTCM 1; GeV; Entries per 10 GeV bin",10,0,100);
        TH1Manager.at(ind_MS_C1) = new hist("MS_C1", "MS 1; ;Entries per 50 bin",60,0,3000);
        TH1Manager.at(ind_PS_C1) = new hist("PS_C1", "PS 1; ;Entries per 20 bin",50,0.,1000.);
        TH1Manager.at(ind_PTISR_C1) = new hist("PTISR_C1", "PTISR 1;GeV;Entries per 20 GeV bin",50,0,1000);
        TH1Manager.at(ind_RISR_C1) = new hist("RISR_C1", "RISR 1; ;Entries per 0.5 bin",100,0,20);
        TH1Manager.at(ind_MISR_C1) = new hist("MISR_C1", "MISR 1; ;Entries per 20 bin",50,0.,1000);
	
	

}
template <class type>
void printvec(std::ofstream& f, std::vector<type> vec){
	for(unsigned int i=0; i< vec.size(); i++){
	f<<vec[i]<<" ";
	}
	f<<"\n";
}
void histset::printSelectionTables(std::ofstream& f){
	f<<_tag<<" "<<" "<<"no_w\n";
	printvec( f, _cutsequence );
	printvec( f, _npass );
	f<<"\n";

	f<<_tag<<" "<<" "<<"w\n";
	printvec( f, _cutsequence );
	//multiply by weight
	printvec( f, _npassw );
	f<<"\n";
}
void histset::FillTH1(int index, double x, double w=1){
	//we must make ptr copies for performance reasons when trying to fill a histogram
	auto myhist = TH1Manager.at(index)->Get();
	myhist->Fill(x,w);
}
void histset::FillTH2(int index, double x, double y){
	auto myhist = TH2Manager.at(index)->Get();
	myhist->Fill(x,y);
}
void histset::WriteHist(std::string outputfilename, std::string TFileOption){

	TFile* outfile = new TFile(outputfilename.c_str(),TFileOption.c_str());


	for(int i=0; i<numTH1Hist; i++){
		//do a check for entries, merge isnt safe on 0 entry histograms
		auto hptr = TH1Manager.at(i)->Get();		
		if(hptr->GetEntries() > 0){
			auto histmerged = TH1Manager.at(i)->Merge();
			TH1D* h = (TH1D*) histmerged->Clone();
			std::string hname(h->GetName());
			outfile->WriteObject(h, (_tag+hname).c_str() );
		}
		else{
			auto h = TH1Manager.at(i)->Get()->Clone();
			std::string hname(h->GetName());
			outfile->WriteObject(h, (_tag+hname).c_str() );
		}
	}

	for(int i=0; i<numTH2Hist; i++){
		auto hptr = TH2Manager.at(i)->Get();
		if(hptr->GetEntries() > 0){
			auto histmerged = TH2Manager.at(i)->Merge();
			TH2D* h = (TH2D*) histmerged->Clone();
			std::string hname(h->GetName());
			outfile->WriteObject(h,(_tag+hname).c_str() );
		}
		else{
			auto h = TH2Manager.at(i)->Get()->Clone();
			std:;string hname(h->GetName());
			outfile->WriteObject(h, (_tag+hname).c_str() );
		}
	}
	outfile->Close();	

}
void histset::AnalyzeEntry(myselector& s){
   	
	//always make a local copy, if its a value dereference.. if you dont do this scope/dereferencing will get really weird, clunky, and unmanageable
	//have to auto& or myreader will try to register copy of the readerarray ptr
	auto MET = *(s.MET);
	auto Nmu = *(s.Nmu);	
	auto weight = *(s.weight);

	//variables needed for gen level susy stuff
	auto genNsusy = *(s.genNsusy);
	auto& genPDGID_susy = s.genPDGID_susy;
	auto& genPT_susy = s.genPT_susy;
	auto& genEta_susy = s.genEta_susy;
	auto& genPhi_susy = s.genPhi_susy;

	//gen leptons (we dont require susy)
	auto genNlep = *(s.genNlep);
	auto& genPDGID_lep = s.genPDGID_lep;
	auto& genPT_lep = s.genPT_lep;
	auto& genEta_lep = s.genEta_lep;
	auto& genPhi_lep = s.genPhi_lep;

	//reco lep
	auto Nlep = *(s.Nlep);
	auto& PT_lep = s.PT_lep;
	auto& Eta_lep = s.Eta_lep;
	auto& Phi_lep = s.Phi_lep;
	auto& Charge_lep = s.Charge_lep;
	auto& PDGID_lep =s.PDGID_lep;
	auto& M_lep = s.M_lep;

	//susy vars
	auto& Njet_ISR = s.Njet_ISR;
	auto& Njet_S = s.Njet_S;
	auto& Nlep_S = s.Nlep_S;
	auto& Njet_a = s.Njet_a;
	auto& Njet_b = s.Njet_b;
	auto& Nlep_a = s.Nlep_a;
	auto& Nlep_b = s.Nlep_b;
	auto& PTCM = s.PTCM;
	auto& MS = s.MS;
	auto& PS = s.PS;
	auto& PTISR = s.PTISR;
	auto& RISR = s.RISR;
	auto& MISR = s.MISR;


	_weight = weight* 137.;//scale to 137 fb-1?
	double w = _weight;
	//auto& PTCM = s.PTCM;

	FillTH1(ind_METHist, MET, w);

	//if there are susy particles, fill gen susy hists
	if( genNsusy > 0 ){
		//loop over susy parts
		int pdg;
		for(int i=0; i<genPDGID_susy.GetSize(); i++){
			pdg = abs(genPDGID_susy[i]);
			if( pdg == 1000011){
				//selectron L
				FillTH1(ind_genselLPt,genPT_susy[i],w);
				FillTH1(ind_genselLEta,genEta_susy[i],w);
				FillTH1(ind_genselLPhi,genPhi_susy[i],w);
			}
			if( pdg == 1000013){
				//smuon L
				FillTH1(ind_gensmuLPt,genPT_susy[i],w);
				FillTH1(ind_gensmuLEta,genEta_susy[i],w);
				FillTH1(ind_gensmuLPhi,genPhi_susy[i],w);
			}
			if( pdg == 2000011){
				//selectron R
				FillTH1(ind_genselRPt,genPT_susy[i],w);
				FillTH1(ind_genselREta,genEta_susy[i],w);
				FillTH1(ind_genselRPhi,genPhi_susy[i],w);
			}
			if( pdg == 2000013){
				//smuon R
				FillTH1(ind_gensmuRPt, genPT_susy[i],w);
				FillTH1(ind_gensmuREta, genEta_susy[i],w);
				FillTH1(ind_gensmuRPhi, genPhi_susy[i],w);
			}
			if( pdg == 1000022){
				//LSP
				FillTH1(ind_genLSPPt, genPT_susy[i],w);
				FillTH1(ind_genLSPEta, genEta_susy[i],w);
				FillTH1(ind_genLSPPhi, genPhi_susy[i],w);
			}

		}	
	}

	//loop over all gen leps
	if( genNlep > 0){
		int pdg;
		for(int i=0; i<genPDGID_lep.GetSize(); i++){
			pdg = abs(genPDGID_lep[i]);
			if( pdg == 11 ){
				//electron
				FillTH1(ind_genElePt, genPT_lep[i],w);
				FillTH1(ind_genEleEta, genEta_lep[i],w);
				FillTH1(ind_genElePhi, genPhi_lep[i],w);			
			}
			if( pdg == 13){
				//muon
				FillTH1(ind_genMuPt, genPT_lep[i],w);
				FillTH1(ind_genMuEta, genEta_lep[i],w);
				FillTH1(ind_genMuPhi, genPhi_lep[i],w);

			}
		}	
	}

	//collect leading and sub leading reco leps require OSSF
	//highest pt lep determines flavor of event
	//leps are pre sorted by PT
	if( Nlep > 1){
		int leadpdg;
		int leadq;
		leadpdg = abs(PDGID_lep[0]);
		leadq = Charge_lep[0];
		TLorentzVector L1, L2;
		L1.SetPtEtaPhiM(PT_lep[0], Eta_lep[0], Phi_lep[0], M_lep[0]);
	
					
		//find subleading same flavor opposite sign 
		double subleading_PT = 0.0;
		for(int i=1; i<PDGID_lep.GetSize(); i++){
			if( (leadpdg == abs(PDGID_lep[i])) &&  ((leadq*Charge_lep[i]) <0) ){
				subleading_PT = PT_lep[i];
				L2.SetPtEtaPhiM(PT_lep[i], Eta_lep[i], Phi_lep[i], M_lep[i]);
				break;
			}
		}
		TLorentzVector L1L2 = L1+L2;
		if(leadpdg == 11){
			FillTH1(ind_Ele1Pt, L1.Pt(), w);
			FillTH1(ind_Ele2Pt, L2.Pt(), w);
			FillTH1(ind_EleM, L1L2.M(), w);  
		}	
		if(leadpdg == 13){
			FillTH1(ind_Mu1Pt, L1.Pt(), w);
			FillTH1(ind_Mu2Pt, L2.Pt(), w);
			FillTH1(ind_MuM, L1L2.M(), w);
		}

	}
	
	//loop over categorical variables
	for( int i=0; i<2; i++){
		if(i==0){
		//cat 0
			FillTH1(ind_Njet_ISR_C0, Njet_ISR[i], w);
			FillTH1(ind_Njet_S_C0, Njet_S[i], w);
			FillTH1(ind_Nlep_S_C0, Nlep_S[i], w);
			FillTH1(ind_Njet_a_C0, Njet_a[i], w);
			FillTH1(ind_Njet_b_C0, Njet_b[i], w);
			FillTH1(ind_Nlep_a_C0, Nlep_a[i], w);
			FillTH1(ind_Nlep_b_C0, Nlep_b[i], w);
			FillTH1(ind_PTCM_C0, PTCM[i], w);
			FillTH1(ind_MS_C0, MS[i], w);
			FillTH1(ind_PS_C0, PS[i], w);
			FillTH1(ind_PTISR_C0, PTISR[i], w);
			FillTH1(ind_RISR_C0, RISR[i], w);
			FillTH1(ind_MISR_C0, MISR[i], w);
		}
		if(i==1){
		//cat 1
			FillTH1(ind_Njet_ISR_C1, Njet_ISR[i], w);
			FillTH1(ind_Njet_S_C1, Njet_S[i], w);
			FillTH1(ind_Nlep_S_C1, Nlep_S[i], w);
			FillTH1(ind_Njet_a_C1, Njet_a[i], w);
			FillTH1(ind_Njet_b_C1, Njet_b[i], w);
			FillTH1(ind_Nlep_a_C1, Nlep_a[i], w);
			FillTH1(ind_Nlep_b_C1, Nlep_b[i], w);
			FillTH1(ind_PTCM_C1, PTCM[i], w);
			FillTH1(ind_MS_C1, MS[i], w);
			FillTH1(ind_PS_C1, PS[i], w);
			FillTH1(ind_PTISR_C1, PTISR[i], w);
			FillTH1(ind_RISR_C1, RISR[i], w);
			FillTH1(ind_MISR_C1, MISR[i], w);

		}

	}
	

	


	//event selection
	//iterate over cut sequence, apply operators and values
	bool pass;
        for(unsigned int i=0; i<_cutsequence.size(); i++){
                std::string cut = _cutsequence.at(i);
		double cutvalue = _cutval.at(i);

		if(cut.compare("nocut")==0){
			pass = nocut(_npass[i], 1);
			pass = nocut(_npassw[i], weight);
		}
		if( cut.compare("minMET")==0){
			pass = performcut( _npass[i], 1, MET, cutvalue, std::greater<double>());
			pass = performcut( _npassw[i], weight, MET, cutvalue, std::greater<double>());		
                        if(!pass) break;
                }
                if( cut.compare("minMu")==0){
               		pass = performcut( _npass[i],1, Nmu, cutvalue, std::greater_equal<double>());
                        pass = performcut( _npassw[i],weight,Nmu,cutvalue,std::greater_equal<double>());
			if(!pass) break;
		}
             
	}//end cut sequence loop	


}
#endif
