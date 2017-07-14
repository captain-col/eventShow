#include <memory>

// Include to get Event Loop.
#include <eventLoop.hxx>

#include <TEventContext.hxx>
#include <TDrawPlane.hxx>
#include <TDrawPlane.cxx>
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


	int minSampleX=-9999;
  int maxSampleX=-9999;
  int minWireX=-9999;
  int maxWireX=-9999;
  int minSampleU=-9999;
  int maxSampleU=-9999;
  int minWireU=-9999;
  int maxWireU=-9999;
  int minSampleV=-9999;
  int maxSampleV=-9999;
  int minWireV=-9999;
  int maxWireV=-9999;

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

		int plane = 0;
	
	int wireCount = CP::TGeometryInfo::Get().GetWireCount(plane);
              int center = (signalEnd+signalStart)/2;
        int spread = 0.5*(signalEnd-signalStart);
        
	if(maxSampleX==-9999)maxSampleX=center+spread;
	if(minSampleX==-9999)minSampleX=center-spread;
	if(minWireX==-9999)minWireX=0;
	if(maxWireX==-9999)maxWireX=wireCount;

	std::cout<<"minS="<<minSampleX<<"; maxS="<<maxSampleX<<"; minWire="<<minWireX<<"; maxW="<<maxWireX<<std::endl;
        
        std::cout << signalStart<< " " << signalEnd<< " " << center<< " " << spread<< std::endl;


	std::string planeNameX = "Xplane_event#_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	std::string planeNameU = "Uplane_event#_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	std::string planeNameV = "Vplane_event#_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	DrawPlane(drift,0,minSampleX,maxSampleX,minWireX,maxWireX,planeNameX);

		 wireCount = CP::TGeometryInfo::Get().GetWireCount(1);
             
        
	if(maxSampleU==-9999)maxSampleU=center+spread;
	if(minSampleU==-9999)minSampleU=center-spread;
	if(minWireU==-9999)minWireU=0;
	if(maxWireU==-9999)maxWireU=wireCount;
	DrawPlane(drift,1,minSampleU,maxSampleU,minWireU,maxWireU,planeNameU);

	       wireCount = CP::TGeometryInfo::Get().GetWireCount(2);
             
        
	if(maxSampleV==-9999)maxSampleV=center+spread;
	if(minSampleV==-9999)minSampleV=center-spread;
	if(minWireV==-9999)minWireV=0;
	if(maxWireV==-9999)maxWireV=wireCount;
	DrawPlane(drift,2,minSampleV,maxSampleV,minWireV,maxWireV,planeNameV);


	
	return true;
  }
  
  void Finalize(CP::TRootOutput * const output) {
  }

 /* bool SetOption(std::string option, std::string value="")
  {
    if(option=="minSampleX" && value!="")
      {
	minSampleX=atoi(value.c_str());
      }//else minSample=-9999;
    if(option=="maxSampleX" && value!="")
      {
	maxSampleX=atoi(value.c_str());
      }//else maxSample=-9999;
    if(option=="minWireX" && value!="")
      {
	minWireX=atoi(value.c_str());
      }//else minWire=-9999;
    if(option=="maxWireX" && value!="")
      {
	maxWireX=atoi(value.c_str());
      }//else maxWire=-9999;
     return true;
  }*/
private:
 /* int minSampleX=-9999;
  int maxSampleX=-9999;
  int minWireX=-9999;
  int maxWireX=-9999;
  int minSampleU=-9999;
  int maxSampleU=-9999;
  int minWireU=-9999;
  int maxWireU=-9999;
  int minSampleV=-9999;
  int maxSampleV=-9999;
  int minWireV=-9999;
  int maxWireV=-9999;*/
};

int main(int argc, char **argv) {
    TEventShow userCode;
    CP::eventLoop(argc,argv,userCode);
}
