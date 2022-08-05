#include "../graphics/graphics.h"


class Manager{

private:

    bool programShouldClose = false;

    bool dataThreadIsWorking = false;
    //thread of mouse input status

    bool clickOrMove = false;
    //save data when click is true ,save data when moves false

    std::thread cursorDataThread;
    //for taking cursor position and saving it to file on a different thread

    void startThread();

    std::ofstream dataFile;
    //is used for creating and updating the file of cursor location data

    void openFile(std::string fileName);
    //open file fileName.txt in the project directory

    void cursorDataManagement();
    //apply writeMouseClickedData or writeMouseMovedData according to clickOrMove

    friend void writeMouseClickedData(Manager *tempManager);
    friend void writeMouseMovedData(Manager *tempManager);
    //used for uploading the cursor position data via dataFile in accordance to clickOrMove
    //might as well have been member functions but decreases memory requirement of Manager since static tempCursorPosition

    void processCommand(std::string tempCommand);

    Graphics associatedGraphics;
    //used for everything related to visualization of data

public:

    static short timeIntervalOfMouseInput;
    //modifier for data input time frequency

    static POINT tempCursorPosition;
    //needed for scoped cursor position data ,static since cursor position is the same for all Managers

    void run();

    Manager();

    ~Manager();

};
