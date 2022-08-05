#include "../tools/tools.h"

class Graphics{

private:

    private:

        static constexpr float SFMLScreenWidth = 800.0 ,SFMLScreenHeight = 600.0;
        //size data of SFML visualization screen

        double maxTimeLimit;
        //number of rows in a saved input .txt file

        static unsigned char *newimg;
        void createNeatImage();
        //the image buffer and the function used to size it according to screen size and default initialize it as a black canvas

        std::thread visualThread;

        const int *winScreenSize = getScreenSize();
        //pointer to width of screen ,adjecent location being the pointer to height of screen

        std::unordered_map<double ,int*> timeToPixelMap;
        //map from time stamp to pointer of x location of associated pixel ,adjecent location being pointing to y value

        void manageTakenOrder(std::string takenOrder ,int index);
        //takes a string of two tupled numbers split by an empty space ,modifies the timeToPixelMap ,needed for other methods
        //putting in a more relevant place could cause memory leaks due to the nature of unordered_map

        unsigned int transformCoordinateToBufferPosition(int x ,int y);
        //takes pixel coordinate ,upper left corner being the origin and yields image coordinate ,format of the image is different

        void markPixel(FILE *f ,int x ,int y);
        //mark the pixel (x,y) of the image f as a black cross
        void markPixelsOnRange(double upperBound ,FILE *img);
        //mark all the pixels in the img up to upperBound according to timeToPixelMap

    public:

        bool pause;
        //visual screen pause status

        bool visualizationThreadIsWorking;
        //for potential bugs in command management
        bool shouldStop;
        //for indicating the ending of visual thread

        bool sizingTimeToPixelMap(std::string filePath);
        //initializing the timeToPixelMap in accordance to row number of given txt file

        static float timeIntervalOfVisualOutput;
        //speed modifier of visualization process ,must be between 1 and 10

        float timeJumpModifier = -1;
        //for changing the time during visualization process ,-1 does nothing while x in [0,100] takes us to a different time

        void renderSFML();
        //starting point of visualization thread

        void startVisualThread();

        bool initializeGraphicalAssets(std::string fileNameTemp);
        //creates the neat ,empty white ,image file and uploads the .txt data to timeToPixelMap

        Graphics();

        ~Graphics();

};

