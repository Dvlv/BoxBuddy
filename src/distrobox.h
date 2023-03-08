#pragma once
#include <QMetaType>
#include <string>
#include <vector>

namespace Distrobox {
// functions for communicating with distrobox on the host

struct DBox {
    std::string name;
    std::string distro;
    std::string imageUrl;
    std::string ID;
    std::string status;
};

struct LocalApp {
    std::string name;
    std::string execName;
    std::string icon;
};

std::string runCmdInBox(std::string cmd, std::string boxName);
std::vector<LocalApp> getLocalApplications(std::string name);
std::vector<std::string> getAvailableImages();
std::string tryParseDistroFromImageUrl(const std::string &imageUrl);

std::vector<DBox> getAllBoxes();
std::string createNewBox(std::string name, std::string image);
bool deleteBox(std::string name);
void openTerminal(std::string name);
bool exportApplication(std::string boxName, std::string app);
bool exportService(std::string boxName, std::string service);
bool addToMenu(std::string boxName);
bool upgradeBox(std::string boxName);
void initNewBox(std::string boxName);

} // namespace Distrobox
Q_DECLARE_METATYPE(std::vector<Distrobox::LocalApp>);
