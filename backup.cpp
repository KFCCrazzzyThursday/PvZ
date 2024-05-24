#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <Windows.h>
namespace fs = std::filesystem;
using namespace std;

void clear_directory(const fs::path& dir_path) {
    for (const auto& entry : fs::directory_iterator(dir_path)) {
        fs::remove_all(entry.path());
    }
}

int main() {
    ifstream configFile("config.txt");
    string dataPath;

    // 从config.txt中读取路径
    if (configFile.is_open()) {
        getline(configFile, dataPath);
        configFile.close();
    } else {
        cout << "Unable to open config.txt" << endl;
        system("pause");
        return 1;
    }

    fs::path sourcePath = fs::path(dataPath) / "yourdata";
    fs::path backupPath = fs::path(dataPath) / "Backup";

    // 检查yourdata文件夹是否存在
    if (!fs::exists(sourcePath)) {
        cout << "Error: 'yourdata' folder does not exist at the provided path: " << sourcePath << endl;
        system("pause");
        return 1;
    }

    // 清空Backup文件夹
    if (fs::exists(backupPath)) {
        clear_directory(backupPath);
    } else {
        fs::create_directory(backupPath); // 创建Backup文件夹
    }

    // 拷贝文件
    try {
        for (const auto& file : fs::directory_iterator(sourcePath)) {
            fs::copy(file.path(), backupPath / file.path().filename(), fs::copy_options::overwrite_existing);
        }
        auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        cout << "Backup successful at " << ctime(&now);
    } catch (const fs::filesystem_error& e) {
        cout << "Error: " << e.what() << endl;
        system("pause");
        return 1;
    }
    system("pause");
    return 0;
}
