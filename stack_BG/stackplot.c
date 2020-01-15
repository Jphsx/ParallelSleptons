//#include "plotTools.C"

void gethists(std::vector<TH1D*>& hists, std::vector<std::string> taglist, std::string histname, TFile* f){
        for(int i=0; i<taglist.size(); i++){
                std::string hname = taglist.at(i)+histname;
                hists.at(i) = (TH1D*) f->Get(hname.c_str());
        }
}



//make stack plots

//void stack(std::string histname ){
//void stackplot(){
void stackplot(std::string histname, std::string fileOpt ){
	TFile* f1 = TFile::Open("../batch_combine/All_BG/susyHists.root");
//std::string histname = "EvisHist";
	TCanvas* c = new TCanvas(histname.c_str());
	//2f from f1
	//collect BG
	
	std::vector<std::string> _BG{"TT","WJ","DY", "WW", "WZ", "ZZ"};
	std::vector<TH1D*> _BGH(_BG.size());
	
	//populatehists
	
	gethists(_BGH, _BG, histname, f1); 
//set fill colors 
	int color =37;
	for(int i=0; i<_BGH.size(); i++){
		_BGH.at(i)->SetFillColor(color);
		color= color+10;
	}

	//add BGs to stack
	TFile* out = new TFile("StackHists.root",fileOpt.c_str());
	THStack *stk = new THStack(histname.c_str(),"");
	auto xaxis = _BGH[0]->GetXaxis()->GetTitle();
        auto yaxis = _BGH[0]->GetYaxis()->GetTitle();
        stk->SetTitle((histname+";"+std::string(xaxis)+";"+std::string(yaxis)).c_str());

	float ymax=800.0; float xlmin=0.7; float ylmin=0.7;//was 35 58
        const float dx=0.28;
        const float dy=0.26;
        TLegend* leg = new TLegend(xlmin,ylmin,xlmin+dx,ylmin+dy);


	for(int i=0; i< _BGH.size(); i++){
		stk->Add(_BGH.at(i));	
		leg->AddEntry(_BGH.at(i), _BG.at(i).c_str(), "f");
	}
	stk->Draw("hist");
	leg->SetBorderSize(1);
	leg->Draw();
	c->Write();
	out->Save();
	
}
