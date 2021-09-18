#ifndef GENERATEBS_HPP
#define GENERATEBS_HPP
#include "BsplineSurface.hpp"
#include <time.h>
using namespace std;
class ControlPoint {
public:
    ControlPoint() = default;
    ControlPoint(const ControlPoint& rhs)
        : controlpoint(rhs.controlpoint)
    {
    }
    ControlPoint(vector<vector<glm::vec3>>& _cp)
        : controlpoint(_cp)
    {
    }
    ~ControlPoint() = default;
    inline int readBinary(const char* filePath)
    {
        char filename[500];
        snprintf(filename, 500, "%s", filePath);
        FILE* fp = fopen(filename, "rb");
        int nsize, msize;
        fread(&nsize, sizeof(int), 1, fp);
        fread(&msize, sizeof(int), 1, fp);
        controlpoint.resize(nsize);
        for (int i = 0; i < nsize; i++) {
            controlpoint[i].resize(msize);
        }
        for (int y = 0; y < nsize; y++) {
            for (int x = 0; x < msize; x++) {
                fread(&controlpoint[y][x], sizeof(glm::vec3), 1, fp);
            }
        }
        fclose(fp);
        return 0;
    }
    //getter
    vector<vector<glm::vec3>> getControlPoint() const { return this->controlpoint; }

private:
    vector<vector<glm::vec3>> controlpoint;
    //you can add anything to the skelton code, but remember to construct it in the constructor function and destructor function
};

#endif