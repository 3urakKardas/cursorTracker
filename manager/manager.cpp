#include "manager.h"

POINT Manager::tempCursorPosition = {0};

short Manager::timeIntervalOfMouseInput = 10;

void writeMouseClickedData(Manager *tempManager){

        bool keyDownCounter = true;

        if ((GetKeyState(VK_LBUTTON) & 0x80) != 0){

            if(keyDownCounter){

            GetCursorPos(&Manager::tempCursorPosition);

            tempManager->dataFile << std::to_string(Manager::tempCursorPosition.x) + " " + std::to_string(Manager::tempCursorPosition.y) + "\n";

            keyDownCounter = false;

            }

        }else{

            keyDownCounter = true;

        }

}

void writeMouseMovedData(Manager *tempManager){

            GetCursorPos(&Manager::tempCursorPosition);

            tempManager->dataFile << std::to_string(Manager::tempCursorPosition.x) + " " + std::to_string(Manager::tempCursorPosition.y) + "\n";

}

void Manager::cursorDataManagement(){

    bool onceInputMessage = true;
    //is used for displaying thread started massage once

    while(dataThreadIsWorking){

        if(clickOrMove){

            if(onceInputMessage){

                std::cout << "The thread started taking input from mouse clicks!" << std::endl;

                onceInputMessage = false;
            }

            writeMouseClickedData(this);

        }else{

            if(onceInputMessage){

                std::cout << "The thread started taking input from mouse move!" << std::endl;

                onceInputMessage = false;
            }

            writeMouseMovedData(this);

        }

        Sleep(timeIntervalOfMouseInput);
    }
}

inline void Manager::openFile(std::string fileName){

    dataFile.open(fileName + ".txt",std::fstream::out);

    if(!dataFile){

        std::cout << "DATA FILE DID NOT GET OPENED!!!" << std::endl;

    }

}

inline void Manager::startThread(){

    dataThreadIsWorking = true;

    cursorDataThread = std::thread(&cursorDataManagement ,this);

}

void Manager::processCommand(std::string tempCommand){

    std::vector<std::string> tempCommandVector;
    //A vector to store the command taken in form of strings parsed by empty space

    tokenize(tempCommand, tempCommandVector);
    //takes the command ,stores it in tempCommandVector

    if(tempCommandVector[0] == "input" && tempCommandVector.size() == 3  && !associatedGraphics.visualizationThreadIsWorking){

        if(tempCommandVector[1] == "by_move"){

            clickOrMove = false;

            openFile(tempCommandVector[2]);

            startThread();

        }else if(tempCommandVector[1] == "by_click"){

            clickOrMove = true;

            openFile(tempCommandVector[2]);

            startThread();

        }

        Sleep(5);
        //waiting for input thread to start ,could have been done via an intermediary boolean

    }else if(tempCommandVector[0] == "end_input" && dataThreadIsWorking == true){

        dataFile.close();

        dataThreadIsWorking = false;

        cursorDataThread.detach();

        std::cout << "Input process ended!" << std::endl;

    }else if(tempCommandVector[0] == "visual" && tempCommandVector.size() == 2 && !associatedGraphics.visualizationThreadIsWorking){

        if(associatedGraphics.initializeGraphicalAssets(tempCommandVector[1])){

            associatedGraphics.pause = false;

            associatedGraphics.visualizationThreadIsWorking = true;

            associatedGraphics.shouldStop = false;

            associatedGraphics.startVisualThread();

        }else{

            std::cout << "Such a file does not exist!" << std::endl;

        }

    }else if(tempCommandVector[0] == "pause" && tempCommandVector.size() == 1 && associatedGraphics.visualizationThreadIsWorking){

        associatedGraphics.pause ^= true;

    }else if(tempCommandVector[0] == "end_visual" && associatedGraphics.visualizationThreadIsWorking){

        associatedGraphics.visualizationThreadIsWorking = false;

        associatedGraphics.shouldStop = true;

    }else if(tempCommandVector[0] == "time_input" && !associatedGraphics.visualizationThreadIsWorking){

        int tempInput = stringToInt(tempCommandVector[1]);

        if(tempInput == -99){

            std::cout << "Second argument must be a number!" << std::endl;

        }else{

            if(tempInput > 10 || tempInput < 0){

                std::cout << "The range of adjusting is 1 to 10 yours is: " << tempInput << "!" << std::endl;

            }else{

                timeIntervalOfMouseInput /= tempInput;

            }
        }

    }else if(tempCommandVector[0] == "time_visual"  && associatedGraphics.visualizationThreadIsWorking){

        int tempInput = stringToInt(tempCommandVector[1]);

            if(tempInput == -99){

                std::cout << "Second argument must be a number!" << std::endl;

            }else{

                if(tempInput > 10 || tempInput < 0){

                std::cout << "The range of adjusting is 1 to 10 yours is: " << tempInput << "!" << std::endl;

                }else{

                Graphics::timeIntervalOfVisualOutput /= tempInput;

                }
            }

    }else if(tempCommandVector[0] == "jump" && tempCommandVector.size() == 2 && associatedGraphics.visualizationThreadIsWorking){

        int tempInput = stringToInt(tempCommandVector[1]);

        if(tempInput == -99){

            std::cout << "Second argument must be a number!" << std::endl;

        }else{

            if(tempInput < 0 || tempInput > 100){

                std::cout << "Second value must be between 0 and 100!" << std::endl;
            }else{

                associatedGraphics.timeJumpModifier = tempInput;

            }

        }

    }else if(tempCommandVector[0] == "close" && !dataThreadIsWorking && !associatedGraphics.visualizationThreadIsWorking){

        programShouldClose = true;

    }else if(tempCommandVector[0] == "info" && tempCommandVector.size() == 1){

        extern const char *infoText;

        std::cout << infoText << std::endl;

    }else if(tempCommandVector[0] == "how" && tempCommandVector.size() == 1){

        extern const char *howText;

        std::cout << howText << std::endl;


    }else{

        std::cout << "This command is not viable!" << std::endl;

    }
}

void Manager::run(){

    extern const char *entryText;

    std::cout << entryText << std::endl;

    while(!programShouldClose){

        std::cout << "Enter a command: ";

        std::string inputCommand;

        getline(std::cin, inputCommand);

        processCommand(inputCommand);

    }

}

Manager::Manager(){

}

Manager::~Manager(){

}


