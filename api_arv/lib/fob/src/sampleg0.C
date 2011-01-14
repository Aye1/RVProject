
// test the flock mode with a graphic display.
// 2 flock


#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream.h>

#include <fstream.h>

#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include <Performer/pf.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>
#include <Performer/pfui.h>
#include <Performer/pfui/pfiXformer.h>

#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfGeode.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pr/pfFont.h>
#include <Performer/pr/pfString.h>
#include <Performer/pf/pfText.h>

#include "FobFlockPerf.h"

// variable 
void       *arena;
pfPipe *p;
pfPipeWindow *pw;
pfScene *scene;
pfChannel *channel;

pfuMouse mouse;
pfuEventStream events;
int exitFlag;
pfiXformer *trackFormer;
pfCoord   *view;
pfDCS *transfo;
int idGUI;

char *dataPtr;
int c;
int xsize;
int ysize;
long win;
int frameSize;

//program class
FobFlockPerf* fob;
pfDCS* transfoHeadFob;
pfDCS* transfoHandFob;

struct SharedData {
 	bool enableDrawStat;
};
static SharedData* Shared;


void
pfdNewReference(pfGroup* parent,void* arena)
{

    pfGroup *CoordSystObject=new pfGroup;
    parent->addChild(CoordSystObject);
    
    pfMatrix *mat=new pfMatrix;
    mat->makeEuler(0.,0.,90.);
    pfSCS *XTransfo=new pfSCS(*mat);
    CoordSystObject->addChild(XTransfo);
    
    mat->makeEuler(0,-90.,0.);
    pfSCS *YTransfo=new pfSCS(*mat);
    CoordSystObject->addChild(YTransfo);
    
    pfGeode *bluearrowObjet=new pfGeode;
    XTransfo->addChild(bluearrowObjet);
    pfGeode *greenarrowObjet=new pfGeode;
    YTransfo->addChild(greenarrowObjet);
    pfGeode *redarrowObjet=new pfGeode;
    CoordSystObject->addChild(redarrowObjet);
    
    pfGeoSet *bluearrowGeometry=pfdNewArrow(10.,arena);
    pfVec4*coul=new pfVec4();
    coul->set(0.,0.,1.,1.);
    bluearrowGeometry->setAttr(PFGS_COLOR4,PFGS_OVERALL,coul,NULL);
    bluearrowObjet->addGSet(bluearrowGeometry);
    
    pfGeoSet *greenarrowGeometry=pfdNewArrow(10.,arena); 
    coul=new pfVec4();
    coul->set(0.,1.,0.,1.);
    greenarrowGeometry->setAttr(PFGS_COLOR4,PFGS_OVERALL,coul,NULL);
    greenarrowObjet->addGSet(greenarrowGeometry);
    
    pfGeoSet *redarrowGeometry=pfdNewArrow(10.,arena);
    coul=new pfVec4();
    coul->set(1.,0.,0.,1.);
    redarrowGeometry->setAttr(PFGS_COLOR4,PFGS_OVERALL,coul,NULL);
    redarrowObjet->addGSet(redarrowGeometry);
}


void 
InitSetup()
{
//init generale
    pfInit();
    pfMultiprocess(PFMP_APP_CULL_DRAW);
    arena=pfGetSharedArena();
	Shared = (SharedData*)pfCalloc(1, sizeof(SharedData), pfGetSharedArena());
	Shared->enableDrawStat=false;
	
    if (arena==NULL)
    {
        pfNotify(PFNFY_FATAL,PFNFY_RESOURCE, 
		 "erreur d'allocation memoire");
        exit(1);
    }
//init generale etendu
    pfFilePath(
	"."
	":./data"
	":../data"
	":../../data"
	":/usr/share/Performer/data"
	);
    pfuInitUtil();//keyboard & mouse
    pfiInit();//utilitie librairie
//supported 3d format
    pfdInitConverter("toto.obj"); 
    pfdInitConverter("toto.3ds");
    pfdInitConverter("toto.wrl");
    pfConfig();
}

void 
WindowSetup(const char *title)
{
//Window & Pipe Init
    p = pfGetPipe(0);
    pw=new pfPipeWindow(p);
    pw->setOriginSize(0,0,768,576);
    pw->setName(title);
    pw->setWinType(PFWIN_TYPE_X);
    pfuInitInput(pw,PFUINPUT_X);
    pw->addChan(channel);
    pw->open();
}

void 
SceneSetup()
{

    scene = new pfScene;
    pfGeoState *scene_state = new pfGeoState;
    scene_state->setMode(PFSTATE_ENLIGHTING,PF_ON);
//scene_state->setMode(PFSTATE_ENTEXTURE ,PF_OFF);  
//scene_state->setMode(PFSTATE_TRANSPARENCY ,PF_OFF); 
    scene->setGState(scene_state);
    
    scene->addChild(new pfLightSource);
    
    transfo=new pfDCS;
    scene->addChild(transfo);
    transfoHeadFob=new pfDCS;
    transfo->addChild(transfoHeadFob);
    transfoHandFob=new pfDCS;
    transfo->addChild(transfoHandFob);
//    pfFilePath(".:/usr/share/Performer/data");
//    pfNode *root = pfdLoadFile("cow.obj");
//    transfoFob->addChild(root);
    
     pfdNewReference(transfo,pfGetSharedArena());
     pfdNewReference(transfoHeadFob,pfGetSharedArena());
     pfdNewReference(transfoHandFob,pfGetSharedArena());
}

void 
drawFunc(pfChannel *chan, void *data)
{	
    chan->clear();
    pfDraw();
    if (Shared->enableDrawStat)
	chan->drawStats();
}


void 
ChannelSetup()
{
    p = pfGetPipe(0);
//chan=new pfChannel(p);
//Camera geometric parameter 
//chan->setScene(scene);
//chan->setFOV(45.0f, 0.0f);
//chan->setNearFar(0.01f,5000.0f);

    pfEarthSky *esky=new pfEarthSky();
    esky->setColor(PFES_GRND_FAR,0.0f,0.0f,0.0f,1.0f);
    esky->setColor(PFES_GRND_NEAR,0.0f,0.0f,0.0f,1.0f);
    esky->setColor(PFES_CLEAR,0.f,0.f,0.f,1.f);
//esky->setMode( PFES_BUFFER_CLEAR ,PFES_FAST );
    esky->setColor(PFES_SKY_TOP,0.f,0.f,0.f,1.f);
    esky->setColor(PFES_SKY_BOT,0.f,0.f,0.f,1.f);
    esky->setColor(PFES_HORIZ,0.f,0.f,0.f,1.f);
    esky->setMode( PFES_BUFFER_CLEAR ,PFES_SKY_GRND );
    esky->setAttr( PFES_GRND_HT , -100.0f );
//chan->setESky(esky);

    channel=new pfChannel(p);
    channel->setScene(scene);
    channel->setTravFunc(PFTRAV_DRAW, drawFunc);
}

void 
TimeSetup()
{
    pfFrameRate(50.0f);
	pfPhase(PFPHASE_LOCK);
//pfPhase(PFPHASE_FREE_RUN); //???
    pfInitClock(0.0);
}


void 
handleEvents(void)
{
    int i,j;
    pfuGetEvents(&events);
    for(j=0;j<events.numDevs;j++) {
	int dev = events.devQ[j];
	if (events.devCount[dev]>0) {
	    switch (dev) {
	    case PFUDEV_REDRAW:
		events.devCount[dev] = 0;
		break;
	    case PFUDEV_WINQUIT:
		exitFlag = 1;
		events.devCount[dev] = 0;
		break;
	    case PFUDEV_KEYBD:
		for (i=0;i<events.numKeys;i++) {
		    int key = events.keyQ[i];
		    if (events.keyCount[key]) {
			switch (key) {
			case 27:               // ESC key
			    exitFlag = 1;
			    break;  
			case 's':Shared->enableDrawStat=!(Shared->enableDrawStat);
			case 32:               // SPACE stop motion
			    break;          
			case 114:              // r reset mouse
			    trackFormer->stop();
			    trackFormer->resetPosition();

			    break;
			}
		    }
		}
		events.devCount[dev] = 0;
		break;  
	    default:
		break;
	    }
	}
    }
    events.numKeys = 0;
    events.numDevs = 0;
}


void 
DeviceSetup()
{
    trackFormer=new pfiTDFXformer();
    trackFormer->selectModel(PFITDF_TRACKBALL);//motion model
//trackFormer->setCoord(&view);//init coord
//trackFormer->setResetCoord(&view); 
    trackFormer->setAutoInput(channel,&mouse,&events); 
//trackFormer->setAutoInput(leftChannel,&mouse,&events);//input from mouse
//pfuTravCalcBBox(scene, &bbox);
    trackFormer->setNode(scene);
    trackFormer->setAutoPosition(channel,transfo);
    

    fob=new FobFlockPerf(2);
    fob->Set(FobFlockPerf::SERIAL_SOFT,5);

    fob->Init();
    fob->SetIGlasses(0);
    fob->SetPen(1);

    fob->Start();



    pfSphere bsphere;
    (scene->getChild(1))->getBound(&bsphere);
    pfVec3 xyz(0.,-3.*bsphere.radius,0.);
    pfVec3 hpr(0.,0.,0.);
    channel->setView(xyz,hpr);
}

void
Finish()
{
 
   fob->Stop();
   fob->Close();	
    pfuExitInput();
    pfuExitUtil();
    pfExit();
}


int
main (int argc, char *argv[],char *envp[])
{
//argv[1] nom de la scene a charger

    InitSetup();
    SceneSetup();
    ChannelSetup();

  
    WindowSetup("Test Tracker Performer");
    DeviceSetup();
    TimeSetup();

    pfSphere bsphere;
    pfSync();
    pfFrame();

//pfSphere bsphere;
//(scene->getChild(1))->getBound(&bsphere);
//sur l'axe -y
//pfCoord view;
//float t;
    exitFlag=0;
    int headFob=0;
    int handFob=1; 
	
	ofstream valQuat;
	valQuat.open("mesureXHead.dat");
	int nbmeasure=0;
	float timeInit,timeLast;
	timeInit=pfGetTime();
	
    while (!exitFlag) 
    {   	
	pfuGetMouse(&mouse);
	trackFormer->update();          
    //   float      s, c; 
	pfVec3 pos;
	pfQuat quat;
	pfMatrix mat; 

	fob->GetPosQuat(pos,quat,headFob);
	//fob->InfoPosQuat(handFob);
	mat.makeQuat(quat);
	mat.postTrans(mat,pos[0],pos[1],pos[2]);
	transfoHeadFob->setMat(mat);
    valQuat<<quat[PF_Y]<<endl;
	fob->GetPosQuat(pos,quat,handFob);
	//fob->InfoPosQuat(handFob);
	mat.makeQuat(quat);
	mat.postTrans(mat,pos[0],pos[1],pos[2]);
	transfoHandFob->setMat(mat);

	pfSync();
	pfFrame();
	handleEvents();
	nbmeasure++;
    }
	valQuat.close();
	cout<<"Frequence="<<nbmeasure/(pfGetTime()-timeInit)<<endl;
    Finish();
    return 0;
}




