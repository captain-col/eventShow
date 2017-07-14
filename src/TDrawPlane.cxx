#include <TDrawPlane.hxx>

#include <memory>

#include <TPulseDigit.hxx>
#include <TCaptLog.hxx>
#include <TChannelId.hxx>
#include <TChannelInfo.hxx>
#include <TGeometryInfo.hxx>
#include <CaptGeomId.hxx>
// Includes for ROOT classes

#include <string.h>
#include <TROOT.h>
#include <TH2F.h>
#include <TPad.h>
#include <TStyle.h>
#include <TColor.h>
#include <sstream>
namespace
{
  std::string toString(int i)
  {
    std::ostringstream s;
    s << i;
    return s.str();
  }

   double GetDigitFirstTime(const CP::TDigit* d) {
        const CP::TPulseDigit* pulse 
            = dynamic_cast<const CP::TPulseDigit*>(d);
        if (pulse) return pulse->GetFirstSample();
       
        return 0.0;
    }

    double GetDigitLastTime(const CP::TDigit* d) {
        const CP::TPulseDigit* pulse 
            = dynamic_cast<const CP::TPulseDigit*>(d);
        if (pulse) return pulse->GetFirstSample()+pulse->GetSampleCount();
      
        return 0.0;
    }

    std::size_t GetDigitSampleCount(const CP::TDigit* d) {
        const CP::TPulseDigit* pulse 
            = dynamic_cast<const CP::TPulseDigit*>(d);
        if (pulse) return pulse->GetSampleCount();
      
        return 0;
    }

    double GetDigitSampleTime(const CP::TDigit* d) {
        double diff = GetDigitLastTime(d) - GetDigitFirstTime(d);
        return diff/GetDigitSampleCount(d);
    }

    double GetDigitSample(const CP::TDigit* d, int i) {
        const CP::TPulseDigit* pulse 
            = dynamic_cast<const CP::TPulseDigit*>(d);
        if (pulse) return pulse->GetSample(i);
       
        return 0;
    }
};

void DrawPlane(CP::THandle<CP::TDigitContainer> drift, int plane, int minSample, int maxSample, int minWire, int maxWire, std::string planeName){
std::vector<double> samples;
    for (CP::TDigitContainer::const_iterator d = drift->begin();
         d != drift->end(); ++d) {
        // Figure out if this is in the right plane, and get the wire
        // number.
        const CP::TDigit* digit 
            = dynamic_cast<const CP::TDigit*>(*d);
        if (!digit) continue;
        CP::TGeometryId id 
            = CP::TChannelInfo::Get().GetGeometry(digit->GetChannelId());
        if (CP::GeomId::Captain::GetWirePlane(id) != plane) continue;
        // Save the sample to find the median.
        for (std::size_t i = 0; i < GetDigitSampleCount(*d); ++i) {
            double s = GetDigitSample(*d,i);
            if (!std::isfinite(s)) continue;
            samples.push_back(s);
        }
    }
    //if (samples.empty()) return 0;   
    std::sort(samples.begin(),samples.end());
    double medianSample = samples[0.5*samples.size()];
    double maxVal = std::abs(samples[0.99*samples.size()]-medianSample);
    maxVal = std::max(maxVal,
                         std::abs(samples[0.01*samples.size()]-medianSample));
	

const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	
        gStyle->SetOptStat(false);
        std::string drawOption("COLZ");
TH2F* xPlane = NULL;
if(plane==0)
	xPlane = new TH2F("xPlane", "Charge on the X wires",maxWire-minWire,minWire,maxWire,maxSample-minSample,minSample,maxSample);
if(plane==1)
        xPlane = new TH2F("xPlane", "Charge on the U wires",maxWire-minWire,minWire,maxWire,maxSample-minSample,minSample,maxSample);
if(plane==2)
        xPlane = new TH2F("vPlane", "Charge on the V wires",maxWire-minWire,minWire,maxWire,maxSample-minSample,minSample,maxSample);

	xPlane->GetXaxis()->SetTitle("Wire");
	xPlane->GetYaxis()->SetTitle("Sample");
        for (CP::TDigitContainer::const_iterator d = drift->begin(); d != drift->end(); ++d)
	  {
            const CP::TPulseDigit* pulse = dynamic_cast<const CP::TPulseDigit*>(*d);
            if (!pulse) continue;
	    CP::TChannelId CId = pulse->GetChannelId();
	    CP::TGeometryId id  = CP::TChannelInfo::Get().GetGeometry(CId); 
	    double wire = CP::GeomId::Captain::GetWireNumber(id) + 0.5;
	    
	     if (CP::GeomId::Captain::GetWirePlane(id) != plane) continue;
	     double maxSignal = 0.0;
	     double noise = 0;
        for (std::size_t i = 0; i < GetDigitSampleCount(*d); ++i) {
            double s = GetDigitSample(*d,i);
	    if(std::abs(s-medianSample)>0.9*maxVal)noise++;
            if (!std::isfinite(s)) continue;
            maxSignal = std::max(maxSignal,
                                 std::abs(GetDigitSample(*d,i)-medianSample));
        }
	if (noise>0.9*GetDigitSampleCount(*d)) continue;
        if (maxSignal < 0.25*maxVal) continue;
            for (std::size_t i = 0; i < pulse->GetSampleCount(); ++i)
	      {
		int tbin = pulse->GetFirstSample() + i;
		xPlane->Fill(wire,tbin+0.5,pulse->GetSample(i)-medianSample);
	      }
	  }
        xPlane->Draw(drawOption.c_str());
//	std::string planeNameX = "xplane_event#_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	gPad->Print(planeName.c_str());
	// gPad->Print("plane.pdf(");
	

}
