#include "sfmlUtilities.h"

//int SFMLUtilities::Sprites::coordinateX = 5;

    const int w = 1366 ;
    const int h = 768 ;

    int red[w][h];
    int green[w][h] ;//= {1,0.7,0.1};
    int blue[w][h];

    FILE *f;
    unsigned char *img = (unsigned char *)malloc(3*w*h);
    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int

    //img = (unsigned char *)malloc(3*w*h);



void Graphics::createNeatImage(std::string nameOfImage){

     memset(img,255,3*w*h);

    //int x , y;

    //float r,g,b;

    /*for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            x=i; y=(h-1)-j;
            r = red[i][j]*255;
            g = green[i][j]*255;
            b = blue[i][j]*255;
            if (r > 255) r=255;
            if (g > 255) g=255;
            if (b > 255) b=255;
            img[(x+y*w)*3+2] = (unsigned char)(r);
            img[(x+y*w)*3+1] = (unsigned char)(g);
            img[(x+y*w)*3+0] = (unsigned char)(b);

            //img[(i+j*w)*3+2] = (unsigned char)(0);
            //img[(i+j*w)*3+1] = (unsigned char)(255);
            //img[(i+j*w)*3+0] = (unsigned char)(0);
        }
    }*/

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

void SFMLUtilities::Sprite::drawToScreen(sf::RenderWindow *win,sf::RectangleShape &rs){

    std::cout << "beforeeeeeeeeeeeeeeeeeeeeee" << std::endl;

    std::cout << tile.getPosition().x << "jjjjj " << std::endl;

    win->draw(rs);

                    /*sf::RectangleShape tile;

                        tile.setSize(sf::Vector2f(23,20));

                    tile.setPosition(sf::Vector2f(150,150));

                    tile.setFillColor(sf::Color::Yellow);

                    win->draw(tile);*/

    std::cout << "drawing" << std::endl;

}

/*void SFMLUtilities::initializeTileGrid(){

    int *temp = getScreenSize();

    int first = *temp ,second = *(temp+1);

    tileGrid = new SFMLUtilities::Sprite * [first];

    std::cout << "herexxxxxxxxxxxzzzzz: " << first << " " << second << std::endl;

    for(int i = 0; i < first ;i++){

        tileGrid[i] = new SFMLUtilities::Sprite[second];
    }

    for(int m = 0; m < 1366 ; m++){

        for(int n = 0; n < 768 ;n++){

            //std::cout << pixelToClickCountGrid[m][n] << "  " << m << " and " << n << std::endl;

            tileGrid[m][n].density;

        }
    }


}*/

//SFMLUtilities::Sprite* SFMLUtilities::tileGrid = (Sprite*)malloc(sizeof(Sprite) * SFMLScreenWidth *SFMLScreenHeight);

void SFMLUtilities::initializeTileGrid(){

    static constexpr int widthTimesHeight = SFMLScreenWidth *SFMLScreenHeight;

    for(int i = 0 ;i < SFMLScreenWidth ;i++){

            for(int j = 0 ;j < SFMLScreenHeight ;j++){

                *(tileGrid+((j*SFMLScreenWidth) + i)) = Sprite(i,j);
            }

    }

    /*for(int m = 0;m < widthTimesHeight;m++){

        std::cout << tileGrid[m].coordinateX << " " << tileGrid[m].coordinateY << std::endl;

    }*/

}

void SFMLUtilities::startVisualThread(){

    //initializeTileGrid();

    renderingShouldStop = false;

    visualThread = std::thread(&renderSFML,this);

    //std::cout << "visual thread started" << std::endl;

}

void SFMLUtilities::endVisualThread(){

    std::cout << "before ending thread" << std::endl;

    renderingShouldStop = true;

    while(true){

        if(!renderingShouldStop){

                visualThread.detach();

                break;

        }

    }

    std::cout << "after ending thread" << std::endl;

}

void SFMLUtilities::renderSFML(){

    SFMLWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML works!");
    //sf::CircleShape shape(50.f);
    sf::RectangleShape rect(sf::Vector2f(20,20));
    rect.setFillColor(sf::Color::Green);

    sf::Vector2f pos = {20,20};

    rect.setPosition(pos);

   /* FILE * fp = fopen ("file.bmp", "wb");
fwrite(bmpfile_header, sizeof(bmpfile_header), 1, fp);
fwrite(bmp_dib_v3_header, sizeof(bmp_dib_v3_header_t), 1, fp);

for (int i = 0; i < 200; i++)  {
 for (int j = 0; j < 200; j++) {
  fwrite(&image[j][i][2], 1, 1, fp);
  fwrite(&image[j][i][1], 1, 1, fp);
  fwrite(&image[j][i][0], 1, 1, fp);
 }
}

fclose(fp);*/




    while (SFMLWindow->isOpen())
    {

 /*           sf::RectangleShape rectOfImg(sf::Vector2f(50,50));
    rectOfImg.setFillColor(sf::Color::Yellow);

    sf::Vector2f posOfImg = {50,50};

    rectOfImg.setPosition(posOfImg);

    sf::Image myImg;

    myImg.loadFromFile("art.png");



    sf::Texture texture;
    texture.loadFromImage(myImg);
    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    sprite.setScale(sf::Vector2f(800.0/1366,600.0/768));*/

    /*std::cout << "jdjdj" << std::endl;

    Sleep(2000);*/

        sf::Event event;
        while (SFMLWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                SFMLWindow->close();
        }

        //SFMLWindow->draw(tileGrid[2500].tile);

        SFMLWindow->clear();



        /*for(int i = 0;i < 5000;i++){

            Sprite tempSprite = tileGrid[i];

            tempSprite.drawToScreen(SFMLWindow,tempSprite.tile);

                    //SFMLWindow->draw(rect);
        }*/


        //SFMLWindow->draw(sprite);
        SFMLWindow->draw(rect);
        SFMLWindow->display();

        //Sleep(4000);
        //std::cout << "uti" << std::endl;

        if(renderingShouldStop){

            renderingShouldStop = false;

            break;
        }
    }

}

void SFMLUtilities::sizingPixelToCursorCountGrid(){

    int *temp = getScreenSize();

    int first = *temp ,second = *(temp+1);

    pixelToClickCountGrid = new int * [first];

    std::cout << "herexxxxxxxxxxxzzzzz: " << first << " " << second << std::endl;

    for(int i = 0; i < first ;i++){

        pixelToClickCountGrid[i] = new int[second];
    }

    for(int m = 0; m < 1366 ; m++){

        for(int n = 0; n < 768 ;n++){

            //std::cout << pixelToClickCountGrid[m][n] << "  " << m << " and " << n << std::endl;

            pixelToClickCountGrid[m][n] = 0;

        }
    }



    //*(*(pixelToClickCountGrid+800)+1400) = 99;

    //pixelToClickCountGrid[1365][768] = 136; 0 dan 1365e 1366 yapar

    //std::cout << "djjdjdjdjdjd" << pixelToClickCountGrid[1365][768] << " jjd " << sizeof(pixelToClickCountGrid[0]) << std::endl;

}

SFMLUtilities::SFMLUtilities(/*Graphics *tempGraphics*/){

    //associatedGraphics = tempGraphics;

    sizingPixelToCursorCountGrid();

    std::cout << "utilities created "<< std::endl;

    std::cout << "ddddddddd:" << sizeof(Sprite) << std::endl;

    initializeTileGrid();

    createNeatImage("jhhd");

}

