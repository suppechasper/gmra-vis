
#include "Data.h"
#include "Display.h"
#include "GMRAVis.h"


#include "DenseVector.h"
#include "DenseMatrix.h"
#include "LinalgIO.h"
#include "IPCATree.h"

#include "CmdLine.h"
#include <StdOutput.h>
#include "GlutStrokeFont.h"

#include <iostream>

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
  
  // Command line arguments
  TCLAP::CmdLine cmd("GMRA Wasserstein distance visualization", ' ', "1");
  
  // Read in a config file
  TCLAP::ValueArg<std::string> cArg("c", "config", "Config file", false,  "", "Config file");
  
  // OR tree data, labels data and the data matrix files
  TCLAP::ValueArg<std::string> tArg("t","tree", "Tree data file",  true, "",
      "tree data file");
  TCLAP::ValueArg<std::string> lArg("l","labels", "labels identifying orig vs copy data",  false, "",
      "vector header file");
  TCLAP::ValueArg<std::string> xArg("x","data1", "Data set 1",  false, "",
      "data matrix header file");

  // Add the args to the command line
  std::vector<TCLAP::Arg *>  xorlist;
  xorlist.push_back(&cArg);
  xorlist.push_back(&tArg);
  cmd.xorAdd(xorlist);
  cmd.add(lArg);
  cmd.add(xArg);

  // Parse the command line
  try{
    cmd.parse( argc, argv );
  } 
  catch (TCLAP::ArgException &e){ 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
    return -1;
  }
  
  // Get the config file OR the tree, labels, and matrix files
  std::string configFileName;
  std::string treeFileName;
  std::string lFileName;
  std::string xFileName;
  if(cArg.isSet()){
    configFileName = cArg.getValue();
    std::ifstream infile(configFileName.c_str());
    std::string line;
    while (std::getline(infile, line))
    {
      std::istringstream iss(line);
      std::string flag, fileName;

      // Exit on an error
      if (!(iss >> flag >> fileName)) {
        std::cout << "ERROR Reading config file" << std::endl; 
        exit(0);
      } 

      if(flag == "-t"){
        treeFileName = fileName;
      }
      else if(flag == "-l"){
        lFileName = fileName;
      }
      else if(flag == "-x"){
        xFileName = fileName;
      }
      else{
        std::cout << "ERROR Parsing config file" << std::endl;
        exit(0);
      }
    }
  }
  else{
    if(!tArg.isSet() || !lArg.isSet() || ! xArg.isSet()){
      std::cout << "Argument Error: tree (-t), label (-l), and matrix (-x) files required!" << std::endl;
      exit(0);
    }
    treeFileName = tArg.getValue();
    lFileName = lArg.getValue();
    xFileName = xArg.getValue();
  }
     
  GlutStrokeFont font;


  DenseMatrix<double> X = LinalgIO<double>::readMatrix(xFileName);
  DenseVector<int> labels = LinalgIO<int>::readVector(lFileName);

  IPCATree<double> ipcaTree;

  std::ifstream treeFile(treeFileName.c_str());
  ipcaTree.unflatten(treeFile);
  treeFile.close();

  // Create the data structure
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

  glutMainLoop();
  
  return 0;

}


