#include "layer.h"

Layer::Layer(std::string blocksID)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            blocks[i][j] = blocksID[i*3+j];
}