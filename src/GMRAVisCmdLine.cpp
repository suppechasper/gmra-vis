
#include "Data.h"
#include "Display.h"
#include "GMRAVis.h"


#include "DenseVector.h"
#include "DenseMatrix.h"
#include "LinalgIO.h"
#include "IPCATree.h"

#include "CmdLine.h"

#include "GlutStrokeFont.h"



#define MAKE_STRING_(x) #x
#define MAKE_STRING(x) MAKE_STRING_(x)


GMRAVis<double> *display;

void display1(void){
  display->display();
};


void idle1(void){
  display->idle();
};

void mouse1(int button, int state, int x, int y){
  display->mouse(button, state, x, y);
}

void motion1(int x, int y){
  display->motion(x, y);
}

void passive1(int x, int y){
  display->passive(x, y);
}

void keyboard1(unsigned char key, int x, int y){
  display->keyboard(key, x, y);
}

void special1(int key, int x, int y){
  display->special(key, x, y);
}

void reshape1(int w, int h){
  display->reshape(w, h);
}



void visible(int vis)
{
    if (vis == GLUT_VISIBLE)
        glutIdleFunc(idle1);
    else
        glutIdleFunc(NULL);
}

void printHelp(){
  //std::cout << display->title() << " Window" << std::endl << std::endl;
  display->printHelp();	
  //std::cout <<  std::endl << std::endl;
}





int main(int argc, char **argv){	
        using namespace FortranLinalg;
   //Command line parsing
  TCLAP::CmdLine cmd("GMRA Wasserstein distance visualization", ' ', "1");

  TCLAP::ValueArg<std::string> tArg("t","tree", "Tree data file",  true, "",
      "tree data file");
  cmd.add(tArg);

  TCLAP::ValueArg<std::string> lArg("l","labels", "labels identifying orig vs copy data",  true, "",
      "vector header file");
  cmd.add(lArg);

  TCLAP::ValueArg<std::string> xArg("x","data1", "Data set 1",  true, "",
      "data matrix header file");
  cmd.add(xArg);


  try{
    cmd.parse( argc, argv );
  } 
  catch (TCLAP::ArgException &e){ 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
    return -1;
  }


  GlutStrokeFont font;
     
  std::string treeFileName = tArg.getValue();
  std::string lFileName = lArg.getValue();
  std::string xFileName = xArg.getValue();

  DenseMatrix<double> X = LinalgIO<double>::readMatrix(xFileName);
  DenseVector<int> labels = LinalgIO<int>::readVector(lFileName);

  IPCATree<double> ipcaTree;

  std::ifstream treeFile(treeFileName.c_str());
  ipcaTree.unflatten(treeFile);
  treeFile.close();


  Data<double> data(ipcaTree, labels, X);


  //GL stuff
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);


  //Windows
  display = new GMRAVis<double>(font, data);
         


  
  glutInitWindowSize(900, 900); 
  int mainWindow = glutCreateWindow(display->title().c_str());
  glutDisplayFunc(display1);
  glutVisibilityFunc(visible);
  glutReshapeFunc(reshape1);
  glutMouseFunc(mouse1);
  glutMotionFunc(motion1);
  glutPassiveMotionFunc(passive1);
  glutKeyboardFunc(keyboard1);
  glutSpecialFunc(special1);
  display->init();


  printHelp();


  glutMainLoop();

  return 0;
}


