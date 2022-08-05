#include "../tools/tools.h"

void tokenize(std::string str ,std::vector<std::string> &vct){

    std::stringstream temporary(str);

    std::string line;

    while(std::getline(temporary,line,' ')){

        vct.push_back(line);
    }

}

int* getScreenSize(){

    int *sizeArr;

    sizeArr = (int*)malloc(8);

    RECT screen;

    const HWND hDesktop = GetDesktopWindow();

    GetWindowRect(hDesktop, &screen);

    sizeArr[0] = screen.right;

    sizeArr[1] = screen.bottom;

    return sizeArr;

}

double lineCountOfStream(std::string filePath){

    std::ifstream file(filePath);

    int lineCount = 0;

    std::string temp;

    while(std::getline(file,temp)){

        lineCount++;
    }

    file.close();

    return lineCount;

}

int stringToInt(std::string givenString){

    int convertedNum = -99;

    std::stringstream tempS(givenString);

    tempS >> convertedNum;

    return convertedNum;

}

bool isInTheBox(int x ,int y,int w,int h,int a ,int b){

    if(a > x && a < x + w && b > y && b < y + h){

        return true;
    }

    return false;

}

const char *entryText = "Welcome to Cursor Tracker !This program allows you to save and visualize your mouse position data in a video like\n"
                        "manner ,to start saving your cursor enter 'input by_move myFirstFile' or 'input by_click myFirstFile' \n"
                        "for stopping the process enter the command 'end_input' ,to visualize the ended session enter 'visual myFirstFile'\n"
                        "for stopping the visualization process enter 'end_visual' ,closing the program is managed with the command 'close'\n"
                        "for more about commands enter 'info' for more about the logic and internal working of the program enter 'how'\n"
                        "-------------------------------------------------------------------------------------------------------------------";

const char *infoText = "'input by_click file': starts saving mouse position ,when clicked ,to a file named file.txt\n\n"
                       "'input by_move file': starts saving mouse position ,regardless whether is clicked or not ,to the file.txt\n\n"
                       "   both of these commands are creating a file.txt in the program folders ,\n   if there is another file.txt its contents will be erased\n"
                       "   this file.txt will later be used for visualization ,\n   erasing or changing this file will results in error or false data.\n\n"
                       "'end_input': ends input process ,everything up to this point is saved in file.txt\n\n"
                       "'visual file': starts the visualisation of file.txt ,the slider bar in the bottom of the window \n"
                       "   indicates the time of the session and can also be used for traversing through that time \n"
                       "   there is already a file named test.txt in the project file to see how this process work enter 'visual test'\n\n"
                       "'end_visual': ends the visualisation process and closes the window\n\n"
                       "'time_input number': number being an integer in [0,10] it changes the time frequency of input\n"
                       "   higher the number is smoother the visualization will be\n\n"
                       "'time_visual': number being an integer in [0,10] it changes the time frequency of visualization\n"
                       "   higher the number is faster is the visualization process going to be\n\n"
                       "'jump number': number being an integer in [0,100] it changes the time stamp of visualization\n"
                       "   'jump 50' takes you to the middle of the process\n\n"
                       "'pause': pauses the visualization process ,it also triggers when clicking the program windows\n"
                       "'close': closes the program\n\n"
                       "note: only one input and visualization process can be managed at a time entering commands related to one process\n"
                       "   while the other is performing will not yield any response\n"
                       "---------------------------------------------------------------------------------------------------------------------";

const char *howText = "cursorTracker makes use of the SFML library ,a copy of the appropriate version should be included in the \n"
                      "project directory .The program has two different threads that are started and ended at appropriate times .\n"
                      "The first thread is used for cursor data input and starts with the 'start' command ,it creates a .txt file in\n"
                      "the main directory .The second thread is used for .txt file visualisation ,it starts with 'visual' command ,\n"
                      "for this to start we need an already existing appropriately formated text file ,as soon as it starts the data\n"
                      "in the text file is uploaded to the ram space allocated to this program it also creates an image named img that\n"
                      "has the extension .bmp ,it is a simple bitmap file but can be opened by most of the media players too ,\n"
                      "as visualizing process continue the thread modify this img file by uploading time stamped pixels ,and allocating this\n"
                      "image to a SFML sprite as a texture ,that is to be displayed in every visual loop as a rectangle of appropriate size.\n"
                      "modifying appropriately named .txt file or .bmp file during the working of the program would cause errors!!!\n"
                      "----------------------------------------------------------------------------------------------------------------------";
