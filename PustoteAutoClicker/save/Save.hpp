#include "../variables/variables.h"
#include <ShlObj.h>
#include <string>
#include <fstream>

namespace Data {
    inline bool Write() {
        char appDataPath[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath))) {
            std::string folder = std::string(appDataPath) + "\\PustoteAC";
            std::string dataPath;
            CreateDirectoryA(folder.c_str(), NULL);
            dataPath = folder + "\\Data.dat";
            std::ofstream data(dataPath, std::ios::binary);
            data.write(reinterpret_cast<char*>(&vars::main::g_bindBtn), sizeof(vars::main::g_bindBtn));
            data.write(reinterpret_cast<char*>(&vars::main::g_key), sizeof(vars::main::g_key));
            data.write(reinterpret_cast<char*>(&vars::main::g_holdDuration), sizeof(vars::main::g_holdDuration));
            data.write(reinterpret_cast<char*>(&vars::main::g_clickInterval), sizeof(vars::main::g_clickInterval));
            data.write(reinterpret_cast<char*>(&vars::main::g_bindMode), sizeof(vars::main::g_bindMode));
            data.write(reinterpret_cast<char*>(&vars::main::g_lang), sizeof(vars::main::g_lang));
            data.close();
            return true;
        }
        return false;
    }

    inline bool Load() {
        char appDataPath[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath))) {
            std::string folder = std::string(appDataPath) + "\\PustoteAC";
            std::string dataPath = folder + "\\Data.dat";
            std::ifstream getdata(dataPath, std::ios::binary);
            if (!getdata.is_open()) return false;
            getdata.read(reinterpret_cast<char*>(&vars::main::g_bindBtn), sizeof(vars::main::g_bindBtn));
            getdata.read(reinterpret_cast<char*>(&vars::main::g_key), sizeof(vars::main::g_key));
            getdata.read(reinterpret_cast<char*>(&vars::main::g_holdDuration), sizeof(vars::main::g_holdDuration));
            getdata.read(reinterpret_cast<char*>(&vars::main::g_clickInterval), sizeof(vars::main::g_clickInterval));
            getdata.read(reinterpret_cast<char*>(&vars::main::g_bindMode), sizeof(vars::main::g_bindMode));
            getdata.read(reinterpret_cast<char*>(&vars::main::g_lang), sizeof(vars::main::g_lang));
            getdata.close();
            return true;
        }
        return false;
    }
}
