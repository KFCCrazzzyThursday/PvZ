#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <Windows.h>
namespace fs = std::filesystem;
using namespace std;

void clear_directory(const fs::path &dir_path)
{
    for (const auto &entry : fs::directory_iterator(dir_path))
    {
        fs::remove_all(entry.path());
    }
}

int main()
{
    ifstream configFile("config.txt");
    string dataPath;

    // 从config.txt中读取路径
    if (configFile.is_open())
    {
        getline(configFile, dataPath);
        configFile.close();
    }
    else
    {
        cout << "Unable to open config.txt" << endl;
        system("pause");
        return 1;
    }

    fs::path backupPath = fs::path(dataPath) / "Backup";
    fs::path yourdataPath = fs::path(dataPath) / "yourdata";
    fs::path planBPath = fs::path(dataPath) / "PlanB";

    // 清空PlanB文件夹
    if (fs::exists(planBPath))
    {
        clear_directory(planBPath);
    }
    else
    {
        fs::create_directory(planBPath); // 创建PlanB文件夹
    }

    // 将yourdata中的文件复制到PlanB
    for (const auto &file : fs::directory_iterator(yourdataPath))
    {
        fs::copy(file.path(), planBPath / file.path().filename(), fs::copy_options::overwrite_existing);
    }

    // 删除yourdata目录中的所有文件
    clear_directory(yourdataPath);

    // 检查Backup文件夹是否有文件
    if (!fs::is_empty(backupPath))
    {
        // 使用Backup中的文件覆盖yourdata中的文件
        try
        {
            for (const auto &file : fs::directory_iterator(backupPath))
            {
                fs::copy(file.path(), yourdataPath / file.path().filename(), fs::copy_options::overwrite_existing);
            }
            auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
            cout << "Restore successful at " << ctime(&now);
        }
        catch (const fs::filesystem_error &e)
        {
            cout << "Error: " << e.what() << endl;
            system("pause");
            return 1;
        }
    }
    else
    {
        cout << "No files found in Backup folder." << endl;
    }
    system("pause");
    return 0;
}
