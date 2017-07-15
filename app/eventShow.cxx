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
std::cout<<"!**********************************!"<<std::endl;
std::cout<<"-O Zoom Optin Must be set to get proper picktures!"<<std::endl;
std::cout<<"!**********************************!"<<std::endl;      
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

		int plane = 0;
	
	int wireCount = CP::TGeometryInfo::Get().GetWireCount(plane);
              int center = (signalEnd+signalStart)/2;
        int spread = 0.5*(signalEnd-signalStart);
        
	if(!maxSX)maxSampleX=center+spread;
	if(!minSX)minSampleX=center-spread;
	if(!minWX)minWireX=0;
	if(!maxWX)maxWireX=wireCount;

	std::cout<<"minSX="<<minSampleX<<"; maxSX="<<maxSampleX<<"; minWireX="<<minWireX<<"; maxWX="<<maxWireX<<std::endl;
        
        std::cout << signalStart<< " " << signalEnd<< " " << center<< " " << spread<< std::endl;


	std::string planeNameX = "Xplane_event#_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	std::string planeNameU = "Uplane_event#_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	std::string planeNameV = "Vplane_event#_"+toString(eventN)+"_run#_"+toString(runN)+".png";
	DrawPlane(drift,0,minSampleX,maxSampleX,minWireX,maxWireX,planeNameX);

		 wireCount = CP::TGeometryInfo::Get().GetWireCount(1);
             
        
	if(!maxSU)maxSampleU=center+spread;
	if(!minSU)minSampleU=center-spread;
	if(!minWU)minWireU=0;
	if(!maxWU)maxWireU=wireCount;
std::cout<<"minSU="<<minSampleU<<"; maxSU="<<maxSampleU<<"; minWireU="<<minWireU<<"; maxWU="<<maxWireU<<std::endl;
	DrawPlane(drift,1,minSampleU,maxSampleU,minWireU,maxWireU,planeNameU);

	       wireCount = CP::TGeometryInfo::Get().GetWireCount(2);
             
        
	if(!maxSV)maxSampleV=center+spread;
	if(!minSV)minSampleV=center-spread;
	if(!minWV)minWireV=0;
	if(!maxWV)maxWireV=wireCount;
std::cout<<"minV="<<minSampleV<<"; maxSV="<<maxSampleV<<"; minWireV="<<minWireV<<"; maxWV="<<maxWireV<<std::endl;
	DrawPlane(drift,2,minSampleV,maxSampleV,minWireV,maxWireV,planeNameV);


	
	return true;
  }
  
  void Finalize(CP::TRootOutput * const output) {
  }

  bool SetOption(std::string option, std::string value="")
  {
	if(option=="Zoom")
      {
	
	minSX=false;
	maxSX=false;
	minWX=false;
	maxWX=false;
	minSU=false;
	maxSU=false;
	minWU=false;
	maxWU=false;
        minSV=false;
        maxSV=false;
        minWV=false;
        maxWV=false;
      }
    else if(option=="minSampleX" && value!="")
      {
	minSX=1;
	minSampleX=atoi(value.c_str());
      }
    else if(option=="maxSampleX" && value!="")
      {
	maxSX=1;
	maxSampleX=atoi(value.c_str());
      }
    else if(option=="minWireX" && value!="")
      {
	minWX=1;
	minWireX=atoi(value.c_str());
      }
    else if(option=="maxWireX" && value!="")
      {
	maxWX=1;
	maxWireX=atoi(value.c_str());
      }
else if(option=="minSampleU" && value!="")
      {
        minSU=1;
        minSampleU=atoi(value.c_str());
      }
    else if(option=="maxSampleU" && value!="")
      {
        maxSU=1;
        maxSampleU=atoi(value.c_str());
      }
    else if(option=="minWireU" && value!="")
      {
        minWU=1;
        minWireU=atoi(value.c_str());
      }
    else if(option=="maxWireU" && value!="")
      {
        maxWU=1;
        maxWireU=atoi(value.c_str());
      }
else if(option=="minSampleV" && value!="")
      {
        minSV=1;
        minSampleV=atoi(value.c_str());
      }
    else if(option=="maxSampleV" && value!="")
      {
        maxSV=1;
        maxSampleV=atoi(value.c_str());
      }
    else if(option=="minWireV" && value!="")
      {
        minWV=1;
        minWireV=atoi(value.c_str());
      }
    else if(option=="maxWireV" && value!="")
      {
        maxWV=1;
        maxWireV=atoi(value.c_str());
      }
	else 
	{
	return false;
	}
     return true;
  }
private:
  int minSampleX;
	bool minSX; 
 int maxSampleX;
	bool maxSX;
  int minWireX;
	bool minWX;
  int maxWireX;
	bool maxWX;
  int minSampleU;
	bool minSU;
  int maxSampleU;
	bool maxSU;
  int minWireU;
	bool minWU;
  int maxWireU;
	bool maxWU;
  int minSampleV;
	bool minSV;
  int maxSampleV;
	bool maxSV;
  int minWireV;
	bool minWV;
  int maxWireV;
	bool maxWV;
};

int main(int argc, char **argv) {
    TEventShow userCode;
    CP::eventLoop(argc,argv,userCode);
}
