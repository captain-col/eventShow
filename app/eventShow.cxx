#include <memory>

// Include to get Event Loop.
#include <eventLoop.hxx>

#include <TEventContext.hxx>

#include <TEvent.hxx>
#include <TDigitContainer.hxx>
#include <TPulseDigit.hxx>
#include <THandle.hxx>
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
}

class TEventShow: public CP::TEventLoopFunction {
public:
    TEventShow() {}
    virtual ~TEventShow() {}
  void Initialize() {
    }
  bool operator () (CP::TEvent& event) {
    
    int eventN = event.GetEventId();
    int runN = event.GetRunId();
      
    CP::TChannelInfo::Get().SetContext(event.GetContext());
    
    CP::THandle<CP::TDigitContainer> drift= event.Get<CP::TDigitContainer>("~/digits/drift");
    
        if (!drift)
	  {
            CaptLog("No drift signals for this event " << event.GetContext());
            return false;
	  }


	
	int signalEnd = -1E+6;
        for (CP::TDigitContainer::const_iterator d = drift->begin(); d != drift->end(); ++d)
	  {
            const CP::TPulseDigit* pulse = dynamic_cast<const CP::TPulseDigit*>(*d);
            if (!pulse) continue;
	    int s = pulse->GetSampleCount();
            signalEnd = std::max(signalEnd, pulse->GetFirstSample()+s);
	  }
	
	int signalStart = 1E+6;
        for (CP::TDigitContainer::const_iterator d = drift->begin(); d != drift->end(); ++d)
	  {
            const CP::TPulseDigit* pulse = dynamic_cast<const CP::TPulseDigit*>(*d);
            if (!pulse) continue;
            signalStart = std::min(signalStart, pulse->GetFirstSample());
	  }
	
        int center = (signalEnd+signalStart)/2;
        int spread = 0.66*(signalEnd-signalStart);
        
        std::cout << signalStart<< " " << signalEnd<< " " << center<< " " << spread<< std::endl;
        
        gStyle->SetOptStat(false);
        std::string drawOption("COLZ");
	
	int plane = 0;
	
	int wireCount = CP::TGeometryInfo::Get().GetWireCount(plane);
	
	TH2F* xPlane = new TH2F("xPlane", "Charge on the X wires",wireCount, 0, wireCount,2*spread,center-spread,center+spread);
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
	     
            for (std::size_t i = 0; i < pulse->GetSampleCount(); ++i)
	      {
		int tbin = pulse->GetFirstSample() + i;
		xPlane->Fill(wire,tbin+0.5,pulse->GetSample(i)-2000.0);
	      }
	  }
        xPlane->Draw(drawOption.c_str());
	std::string planeNameX = "xplane_event#_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	gPad->Print(planeNameX.c_str());
	// gPad->Print("plane.pdf(");
	
	
	wireCount = CP::TGeometryInfo::Get().GetWireCount(1);

	plane = 1;
	
	TH2F* uPlane = new TH2F("uPlane", "Charge on the U wires", wireCount, 0, wireCount,2*spread,center-spread,center+spread);
	uPlane->GetXaxis()->SetTitle("Wire");
	uPlane->GetYaxis()->SetTitle("Sample");
        
        for (CP::TDigitContainer::const_iterator d = drift->begin(); d != drift->end(); ++d)
	  {
            const CP::TPulseDigit* pulse = dynamic_cast<const CP::TPulseDigit*>(*d);
            if (!pulse) continue;
	    CP::TGeometryId id  = CP::TChannelInfo::Get().GetGeometry(pulse->GetChannelId());
 
	    double wire = CP::GeomId::Captain::GetWireNumber(id) + 0.5;
	    
	     if (CP::GeomId::Captain::GetWirePlane(id) != plane) continue;
	     
            for (std::size_t i = 0; i < pulse->GetSampleCount(); ++i)
	      {
                int tbin = pulse->GetFirstSample() + i;
                uPlane->Fill(wire,tbin+0.5,pulse->GetSample(i)-2000.0);
	      }
	  }
        uPlane->Draw(drawOption.c_str());
	// gPad->Print("plane.pdf");
	std::string planeNameU = "uplane_event_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	gPad->Print(planeNameU.c_str());
	
	wireCount = CP::TGeometryInfo::Get().GetWireCount(2);

	plane = 2;
	
	TH2F* vPlane = new TH2F("vPlane", "Charge on the V wires", wireCount, 0, wireCount,2*spread,center-spread,center+spread);
	
	vPlane->GetXaxis()->SetTitle("Wire");
	vPlane->GetYaxis()->SetTitle("Sample");
        for (CP::TDigitContainer::const_iterator d = drift->begin();d != drift->end(); ++d)
	  {
	    const CP::TPulseDigit* pulse = dynamic_cast<const CP::TPulseDigit*>(*d);
	    if (!pulse) continue;
	    
	    CP::TGeometryId id  = CP::TChannelInfo::Get().GetGeometry(pulse->GetChannelId());
 
	    double wire = CP::GeomId::Captain::GetWireNumber(id) + 0.5;

	     if (CP::GeomId::Captain::GetWirePlane(id) != plane) continue;
	    
            for (std::size_t i = 0; i < pulse->GetSampleCount(); ++i)
	      {
                int tbin = pulse->GetFirstSample() + i;
                vPlane->Fill(wire,tbin+0.5,pulse->GetSample(i)-2000.0);
	      }
	  }
        vPlane->Draw(drawOption.c_str());
        //gPad->Print("plane.pdf)");
	std::string planeNameV = "vplane_event_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	gPad->Print(planeNameV.c_str());
	
	
	return true;
  }
  
  void Finalize(CP::TRootOutput * const output) {
  }
private:
  
};

int main(int argc, char **argv) {
    TEventShow userCode;
    CP::eventLoop(argc,argv,userCode);
}
