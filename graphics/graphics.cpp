#include "graphics.h"

float Graphics::timeIntervalOfVisualOutput = 1;

unsigned char *Graphics::newimg = nullptr;

bool Graphics::initializeGraphicalAssets(std::string fileNameTemp){

    if(!sizingTimeToPixelMap(fileNameTemp)){

        return false;

    }

    newimg = (unsigned char *)malloc(3);

    newimg[0] = (unsigned char)(0); //blue
    newimg[1] = (unsigned char)(0);  //green
    newimg[2] = (unsigned char)(0);  //red

    maxTimeLimit = lineCountOfStream(fileNameTemp + ".txt");

    createNeatImage();

    return true;

}

void Graphics::manageTakenOrder(std::string takenOrder,int index){

    std::vector<std::string> tempVector;

    tokenize(takenOrder,tempVector);

    int tempX = stringToInt(tempVector[0]);

    int *tempXPointer = new int[2];
    //creates the pointer mapped by unordered_map

    *tempXPointer = tempX;
    //make the assignment through reference

    *(tempXPointer+1) = stringToInt(tempVector[1]);
    //assign next value to the adjacent pointer

    timeToPixelMap[index] = tempXPointer;
    //assign the original pointer to index

}

bool Graphics::sizingTimeToPixelMap(std::string fileName){

  //  double lineNumber = lineCountOfStream(fileName+".txt");

    std::string tempStr;

    std::ifstream file(fileName + ".txt");

    if(!file.is_open()){

        return false;
    }

    for(int i = 0 ;std::getline(file,tempStr) ;i++){

        manageTakenOrder(tempStr,i);

    }
    //takes the lines up to the end of file

    return true;

}





void Graphics::createNeatImage(){

    int w = *(winScreenSize);
    int h = *(winScreenSize+1);
    //w(width) and h(height) are determined by the visual hardware of system ,any hindrance in it would yield error

    FILE *f;
    //this will be the img that we will use as a texture to display the data

    unsigned char *img = (unsigned char *)malloc(3*w*h);
    memset(img,255,3*w*h);
    //3 bytes of rgb for every pixel requires 3*w*h bytes

    int filesize = 54 + 3*w*h;
    //54 bytes is the standard header size of texture files ,bmp,png...

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    f = fopen("img.bmp","wb");

    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for(int i=0; i<h; i++)
    {
        fwrite(img+(w*(h-i-1)*3),3,w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }

    free(img);
    fclose(f);

}

void Graphics::startVisualThread(){

    visualThread = std::thread(&renderSFML,this);

}

unsigned int Graphics::transformCoordinateToBufferPosition(int x ,int y){

    if(x >= *winScreenSize){

        x = *winScreenSize-1;

    }else if(x < 0){

        x = 0;

    }

    if(y > *(winScreenSize+1)){

        y = *(winScreenSize+1);

    }else if(y < 1){

        y = 1;

    }

    unsigned int position = 54+((y-1)*(*winScreenSize)*3)+((y-1)*2)+(x*3);
    //the need of these translations are caused by incompatible origins of SFML ,system window ,texture files

    return position;

}

void Graphics::markPixel(FILE *f ,int x ,int y){

        fseek(f,transformCoordinateToBufferPosition(x,y),SEEK_SET);
        //brings the buffer cursor to wanted position
        fwrite(newimg,3,1,f);
        //draws pixel at the wanted position

        fseek(f,transformCoordinateToBufferPosition(x+1,y),SEEK_SET);
        fwrite(newimg,3,1,f);

        fseek(f,transformCoordinateToBufferPosition(x,y+1),SEEK_SET);
        fwrite(newimg,3,1,f);

        fseek(f,transformCoordinateToBufferPosition(x-1,y),SEEK_SET);
        fwrite(newimg,3,1,f);

        fseek(f,transformCoordinateToBufferPosition(x,y-1),SEEK_SET);
        fwrite(newimg,3,1,f);

        //this function draws a cross centered at (x,y)
        //five seek and draw for five pixels around and on (x,y)

}

void Graphics::markPixelsOnRange(double upperBound ,FILE *f){

    int *temp = nullptr;

    fseek(f,54,SEEK_SET);
    //goes to byte 54 of f

    const unsigned char emptyA[1] = {255};

    const double last = (3*(*winScreenSize)*(*(winScreenSize+1)))+(2*(*(winScreenSize+1)));

    for(int i = 0; i < last ;i++){

        fwrite(emptyA,1,1,f);
    }
    //turns every pixel of img to white

    for(double i = 0 ;i < upperBound ;i++){

        temp = timeToPixelMap[i];

        markPixel(f,*(temp),*(temp+1));

    }
    //marks every pixel in timeToPixelMap up to upperBound
    //in reality every time you move the time stamp ,the image is turned to white and pixels up to a point are marked all over again

}

void Graphics::renderSFML(){

    sf::RenderWindow SFMLWindow(sf::VideoMode(800, 600),"cursorTracker");
    //SFML window is created

    const int rectVerticalPos = SFMLScreenHeight*0.965;
    //y pos of timeStamp rectangle ,not to be changed

    float rectHorizontalPos = SFMLScreenWidth*0.125;
    //starting pos of timeStamp rectangle ,is going to be moved as process goes on

    float horizontalPositionOfTimeStampRect = 0;
    //starting pos of rectangle activated when timeStamp rectangle is clicked ,we will henceforth call it !!!tempRectangle!!!

    bool timeStampRectIsActivated = false;
    bool timeStampOnlyOnce = false;
    //first decides whether tempRectangle should be drawn or not ,second when mouse clicked event related s triggered only once

    sf::RectangleShape rect(sf::Vector2f(10,16));
    rect.setFillColor(sf::Color::Green);
    //size and color of timeStamp rectangle

    sf::Vector2f pos = {rectVerticalPos,rectVerticalPos};
    rect.setPosition(pos);
    //initialize and assign pos of timeStamp rectangle

    sf::RectangleShape rectStamp(sf::Vector2f(10,16));
    rectStamp.setFillColor(sf::Color::Yellow);
    //size and color of tempRectangle

    double timeSignOfRendering = 0;
    //the number that is being visualized in a given time ,time stamp

    int *temp;
    temp = (int*)malloc(2*sizeof(int));
    //place to hold the pixel x ,y values of given time sign

    sf::Image myImg;
    sf::Texture texture;
    sf::Sprite sprite;
    //image ,texture and sprite that are used in drawing of img ,containing a visual representation of the screen up to the timeSign

    sprite.setOrigin(0,770);
    sf::Transform t(1.f, 0.f, 0.f,
                     0.f, -1.f, 0.f,
                     0.f, 0.f, 1.f);
    //translations needed due to problems related to SFML ,window and img file

    sprite.setScale(sf::Vector2f(SFMLScreenWidth/(*(winScreenSize)),(SFMLScreenHeight-20)/(*(winScreenSize+1))));
    //sizing the sprite whose texture is img file

    sf::Event event;
    //event type for keeping track of actions occurring on SFML screen

    FILE *nf;
    nf = fopen("img.bmp","r+");
    //setting and opening img file

    bool pauseTriggered = false;
    //not for pause itself but for triggering of pause change only once

    sf::Image pauseOffImage ,pauseOnImage;
    sf::Texture pauseOffTexture ,pauseOnTexture;
    sf::Sprite pauseOffSprite ,pauseOnSprite;

    pauseOffImage.loadFromFile("assets/pause_off_button.png");
    pauseOnImage.loadFromFile("assets/pause_on_button.png");

    pauseOffTexture.loadFromImage(pauseOffImage);
    pauseOnTexture.loadFromImage(pauseOnImage);

    pauseOffSprite.setTexture(pauseOffTexture,true);
    pauseOnSprite.setTexture(pauseOnTexture,true);

    pauseOffSprite.setPosition(sf::Vector2f(SFMLScreenWidth*0.01,-0.05*SFMLScreenHeight));
    pauseOnSprite.setPosition(sf::Vector2f(SFMLScreenWidth*0.01,-0.05*SFMLScreenHeight));
    //all of these are used to display two states of paused buttons

    while (SFMLWindow.isOpen())
    {

        float widthFactor = SFMLWindow.getSize().x/SFMLScreenWidth;
        float heightFactor = SFMLWindow.getSize().y/SFMLScreenHeight;
        //used for resized window

        temp = timeToPixelMap[timeSignOfRendering];
        //sets the value of appropriate pixel in accordance to timeSign

        if(timeSignOfRendering < maxTimeLimit-1){//triggers when we haven't reached the end of process

            markPixel(nf,*(temp),*(temp+1));
            myImg.loadFromFile("img.bmp");
            texture.loadFromImage(myImg);
            sprite.setTexture(texture, true);
            //mark pixel on file ,load new image ,set it as texture

            rectHorizontalPos = 20+((SFMLScreenWidth-40)*(timeSignOfRendering/maxTimeLimit));
            //set new position of timeStamp rectangle

            sf::Vector2f tempPosV = {rectHorizontalPos,rectVerticalPos};
            //new pos tuple of timeStamp rectangle
            rect.setPosition(tempPosV);
            //and sets it

            if(!pause){//if paused the image file will remain the same ,post of the processes will halt

                timeSignOfRendering += 1;

            }

            if(timeJumpModifier != -1){//if not -1 then it will use the value to change the time sign

                double newSignOfRendering = floor(maxTimeLimit*(timeJumpModifier/100));
                //new time sign according to timeJumpModifier

                markPixelsOnRange(newSignOfRendering,nf);
                //draws everything up to the point

                timeSignOfRendering = (newSignOfRendering == maxTimeLimit) ? newSignOfRendering-2 : newSignOfRendering;
                //checks whether it is the last value ,if not it causes trouble

                timeJumpModifier = -1;
            }



        }

        if(timeJumpModifier != -1){

            timeSignOfRendering = 0;
        }
        //in case time sign reaches the end it wont evaluate the time jump this condition helps us


        while (SFMLWindow.pollEvent(event))
        {

            sf::Vector2i tempV = sf::Mouse::getPosition(SFMLWindow);

            if (event.type == sf::Event::Closed){

                visualizationThreadIsWorking = false;
                shouldStop = true;

            }else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){

                if(isInTheBox((rectHorizontalPos-5)*widthFactor,(rectVerticalPos-5)*heightFactor,20*widthFactor,26*heightFactor,tempV.x,tempV.y)){

                    horizontalPositionOfTimeStampRect = rectHorizontalPos;

                    rectStamp.setPosition(horizontalPositionOfTimeStampRect,rectVerticalPos);

                    timeStampRectIsActivated = true;

                    timeStampOnlyOnce = true;

                    pauseTriggered = true;

                }else if(!pauseTriggered){

                    pause ^= true;

                    pauseTriggered = true;
                }

            }else{

                timeStampRectIsActivated = false;

                pauseTriggered = false;

                if(timeStampOnlyOnce){

                    timeJumpModifier = floor(((float)tempV.x/SFMLWindow.getSize().x)*100);

                    timeStampOnlyOnce = false;

                }

            }

            if(event.type == sf::Event::MouseMoved && timeStampRectIsActivated){

                if(tempV.x >= 10 && tempV.x <= SFMLWindow.getSize().x-20){

                    horizontalPositionOfTimeStampRect = ((float)tempV.x/SFMLWindow.getSize().x)*SFMLScreenWidth;

                    rectStamp.setPosition(horizontalPositionOfTimeStampRect,rectVerticalPos);

                }

            }

        }
        //upper loop manages key and mouse events triggered on SFML screen

        SFMLWindow.clear();
        //clears the window in every loop

        SFMLWindow.draw(sprite,t);
        //draws img file itself
        SFMLWindow.draw(rect);
        //draws timeStamp rectangle

        if(pause == true){

            SFMLWindow.draw(pauseOnSprite,t);

        }else{

            SFMLWindow.draw(pauseOffSprite,t);
        }
        //draws pause off/on sprite

        if(timeStampRectIsActivated){

            SFMLWindow.draw(rectStamp);
        }
        //draws tempRectangle if activated

        SFMLWindow.display();
        //displays everything drawn ,needed for SFML

        if(shouldStop){

                SFMLWindow.close();

                fclose(nf);

                free(temp);


        }
        //if visualisation should stop ,it is activated ,closing the window and as a result ending the loop
    }

    visualThread.detach();
    //if loop ended then thread is detached



}

Graphics::Graphics(){

}

Graphics::~Graphics(){

    std::cout << "The program has been successfully closed" << std::endl;

}
