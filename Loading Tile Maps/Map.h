    #ifndef MAP_H
    #define MAP_H
    
    #include "Core/Graphicsh"
    #include "Core/Image.h"
    #include <fstream>
    #include <cstdlib>
    #include <vector>
    
    using namespace std;
    
    class Map
    {
    private:
        Image tiles;
        int tileWidth;
        int tileHeight;
        int width;
        int height;
        int* layer1, *layer2, *layer3, *solidLayer;
        vector<int> solidTiles;
        
        void loadDimensions(ifstream *in);
        void loadSolidTiles(ifstream *in);
        void loadLayer(int* layer, ifstream *in);
        void generateSolidLayer();
        Map();
        ~Map();
        
        bool load(char mapName[], char imageName[], Graphics* g);
        void draw(int layer, int xOffset, int yOffset, Graphics* g);
        void free();
        
        bool isTileSolid(int tile);
        bool checkSolid(int x, int y);
    }
    
    #endif