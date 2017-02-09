    #include "Map.h"
    
    Map::Map()
    {
        tileWidth = -1;
        tileHeight = -1;
        width = -1;
        height = -1;
        layer1 = layer2 = layer3 = solidLayer = NULL;
    }
    
    Map::~Map()
    {

    }
    
    bool Map::load(char mapName[], char imageName[], Graphics* g)
    {
        layer1 = layer2 = layer3 = solidLayer = NULL;
        
        ifstream in(mapName);
        
        if(!in.good())
            return false;
            
        string buffer;
        
        loadDimensions(&in);
        loadSolidTiles(&in);
        
        layer1 = new int[width * height];
        layer2 = new int[width * height];
        layer3 = new int[width * height];
        solidLayer = new int[width * height];
        
        loadLayer(layer1, &in);
        loadLayer(layer2, &in);
        loadLayer(layer3, &in);
        generateSolidLayer();
        
        in.close();
        
        if(!tiles.load(imageName, tileWidth, tileHeight, g))
            return false;
            
        return true;
    }
    
    void Map::loadDimensions(ifstream *in)
    {
        string buffer;
        
        getline(*in, buffer, ' ');
        getline(*in, buffer, '\n');
        width = atoi(buffer.c_str());

        getline(*in, buffer, ' ');
        getline(*in, buffer, '\n');
        height = atoi(buffer.c_str());

        getline(*in, buffer, ' ');
        getline(*in, buffer, '\n');
        tileWidth = atoi(buffer.c_str());

        getline(*in, buffer, ' ');
        getline(*in, buffer, '\n');
        tileHeight = atoi(buffer.c_str());
    }
    
    void Map::loadSolidTiles(ifstream *in)
    {
        string buffer;
        
        getline(*in, buffer, '\n');
        getline(*in, buffer, '\n');
        char* string = new char[buffer.size() +
        1 ];
        strcpy(string, buffer.c_str());
        
        char* token = strtok(string, " ,");
        
        while(token != NULL)
        {
            solidTiles.push_back(atoi(token));
            token = strtok(NULL, " ,");
        }
        
        delete[] string;
    }
    
    void Map::loadLayer(int* layer, ifstream *in)
    {
        string buffer;
        
        getline(*in, buffer, '\n');
        
        int i = 0;
        
        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                char delim = ',';
                
                if(x == width-1)
                    delim = '\n';
                    
                getline(*in, buffer, delim);
                layer[i] = atoi(buffer.c_str());
                i++;
            }
        }
    }
    
    void Map::generateSolidLayer()
    {
        for(int i = 0; i < width*height; i++)
        {
            if(isTileSolid(layer1[i]) ||
            isTileSolid(layer2[i]) ||
            isTileSolid(layer3[i]))
                solidLayer[i] = 1;
            else
                solidLayer[i] = 0;
        }
    }
    
    void Map::draw(int layer, int xOffset, int yOffset, Graphics* g)
    {
        int* drawLayer = NULL;
        
        if(layer == 0)
            drawLayer = solidLayer;
        if(layer == 1 )
            drawLayer = layer1;
        if(layer == 2)
            drawLayer = layer2;
        if(layer == 3)
            drawLayer = layer3;
        else
            return;
            
        int startY = yOffset/tileHeight;
        int startX = xOffset/tileWidth;
        
        int rows = (g->getHeight()/tileHeight) + 2;
        int columns = (g->getWidth()/tileWidth) + 2;
        
        for(int y = startY; y < startY+rows; y++)
            for(int x = startX; x < startX+columns; x++)
            {
                int frame = -1;
                
                if(x >= 0 && y >= 0 && x < width && y < height)
                    frame = drawLayer[y*width+x]-1;
                    
                if(frame >= 0)
                {
                    if(layer == 0)
                    {
                        g->drawRect(x*tileWidth - xOffset, y*tileHeight-yOffset, tileWidth, tileHeight, 255, 0, 0);
                    }
                    else
                    {
                            tiles.draw(x*tileWidth - xOffset, y*tileHeight - yOffset, frame, g);
                    }
                }
            }
    }

    void Map::free()
    {
        if(layer1 != NULL)
        {
            delete[] layer1;
            layer1 = NULL;
        }
        
        if(layer2 != NULL)
        {
            delete[] layer2;
            layer2 = NULL;
        }
        
        if(layer3 != NULL)
        {
            delete[] layer3;
            layer3 = NULL;
        }
        
        if(solidLayer != NULL)
        {
            delete[] solidLayer;
            solidLayer = NULL;
        }
        
        solidTiles.clear();
        tiles.free();
    }
    

    bool Map::isTileSolid(int tile)
    {
        for(unsigned int i = 0; i < solidTiles.size(); i++)
        {
            if(solidTiles[i] == tile)
                return true;
        }
        
        return false;
    }

    bool Map::checkSolid(int x, int y)
    {
        if(x < 0 || x > width || y < 0 || y > height)
            return false;
        
        return solidLayer[y*width + x];
    }