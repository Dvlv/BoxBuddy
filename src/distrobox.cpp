#include "distrobox.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

std::string runCmdAndGetOutput(const char *cmd) {
    // https://stackoverflow.com/questions/52164723/how-to-execute-a-command-and-get-return-code-stdout-and-stderr-of-command-in-c
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }

    return result;
}

std::vector<std::string> explodeString(const std::string &str, const char &ch) {
    // splits string by provided char
    // https://stackoverflow.com/questions/13172158/c-split-string-by-line
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{str};

    for (std::string line; std::getline(ss, line, ch);)
        result.push_back(line);

    return result;
}

static inline void ltrim(std::string &s) {
    // https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

std::string Distrobox::tryParseDistroFromImageUrl(const std::string &imageUrl) {
    std::vector<std::string> distros = {
        "ubuntu", "debian",    "centos", "oracle", "fedora",   "arch",
        "alma",   "slackware", "gentoo", "kali",   "alpine",   "clearlinux",
        "void",   "amazon",    "rocky",  "redhat", "opensuse", "mageia",
    };

    // first try and get it from the last part of the url
    std::string imageName = imageUrl.substr(imageUrl.find_last_of("/") + 1);

    // this z is a daft hack to get the unknown distros to the bottom of the
    // select
    std::string distroName = "zunknown";

    for (auto dn : distros) {
        if (imageName.find(dn) != std::string::npos) {
            distroName = dn;
            break;
        }
    }

    if (distroName != "zunknown") {
        return distroName;
    }

    // if not found, just search in the whole url
    // this is possibly less accurate, but eh
    for (auto dn : distros) {
        if (imageUrl.find(dn) != std::string::npos) {
            distroName = dn;
            break;
        }
    }

    return distroName;
}

std::string Distrobox::runCmdInBox(std::string cmd, std::string boxName) {
    std::string cmdStr = "distrobox enter " + boxName + " -- " + cmd;

    return runCmdAndGetOutput(cmdStr.c_str());
}

std::vector<Distrobox::DBox> Distrobox::getAllBoxes() {
    std::vector<Distrobox::DBox> boxes;

    std::string dbListOut = runCmdAndGetOutput("distrobox list --no-color");

    std::vector<std::string> dbListOutLines = explodeString(dbListOut, '\n');

    for (int i = 1; i < dbListOutLines.size(); ++i) {
        std::vector<std::string> fields = explodeString(dbListOutLines[i], '|');

        for (auto &field : fields) {
            trim(field);
        }

        Distrobox::DBox box = {.name = fields[1],
                               .distro = fields[3],
                               .imageUrl = fields[3],
                               .ID = fields[0],
                               .status = fields[2]};

        box.distro = tryParseDistroFromImageUrl(box.imageUrl);

        boxes.push_back(box);
    }

    return boxes;
}

std::vector<Distrobox::LocalApp>
Distrobox::getLocalApplications(std::string name) {
    std::string appsOutput =
        Distrobox::runCmdInBox("./src/list-local-apps.sh", name);

    std::vector<std::string> desktopFiles = explodeString(appsOutput, '\n');
    for (auto &desktopFile : desktopFiles) {
        desktopFile = desktopFile.substr(0, desktopFile.size() - 1);
    }

    if (desktopFiles.size() == 1 &&
        desktopFiles[0].find("No such file or directory") !=
            std::string::npos) {
        // box has nothing installed, return blank
        return {};
    }

    std::vector<Distrobox::LocalApp> apps;

    for (auto desktopFile : desktopFiles) {
        std::string appExec = Distrobox::runCmdInBox(
            "bash ./src/get-app-exec.sh " + desktopFile, name);

        // some desktop files have multiple execs
        // we only want the first one
        std::vector<std::string> appExecs = explodeString(appExec, '\n');
        if (appExecs.size() > 0) {
            appExec = appExecs[0];
        }

        if (appExec.size() > 5) {
            appExec = appExec.substr(5, appExec.size());
        }

        std::string appIcon = Distrobox::runCmdInBox(
            "bash ./src/get-app-icon.sh " + desktopFile, name);

        if (appIcon.size() > 5) {
            appIcon = appIcon.substr(5, appIcon.size());
        }

        // substr gets rid of "exec=" and "icon="
        auto app = Distrobox::LocalApp{
            .name = desktopFile,
            .execName = appExec.substr(0, appExec.size() - 1),
            .icon = appIcon.substr(0, appIcon.size() - 2),
        };

        apps.push_back(app);
    }

    return apps;
}

bool Distrobox::exportApplication(std::string boxName, std::string app) {
    std::string cmd = "distrobox-export -a " + app;

    std::string output = Distrobox::runCmdInBox(cmd.c_str(), boxName);
    printf("%s, %s\n", app.c_str(), output.c_str());

    return output.find("Error:") == std::string::npos;
}

bool Distrobox::exportService(std::string boxName, std::string service) {
    std::string cmd = "distrobox-export -s " + service;

    std::string output = Distrobox::runCmdInBox(cmd.c_str(), boxName);

    return output.find("Error:") == std::string::npos;
}

bool Distrobox::addToMenu(std::string boxName) {
    std::string cmd = "distrobox generate-entry " + boxName;

    std::string output = runCmdAndGetOutput(cmd.c_str());

    return output.find("Error:") == std::string::npos;
}

bool Distrobox::upgradeBox(std::string boxName) {
    std::string terminalCmd = "konsole -e";
    std::string cmd = terminalCmd + " distrobox upgrade " + boxName;

    std::system(cmd.c_str());

    return true;
}

void Distrobox::openTerminal(std::string boxName) {
    std::string terminalCmd = "setsid konsole -e";
    std::string cmd = terminalCmd + " distrobox enter " + boxName + " &";

    std::system(cmd.c_str());
}

bool Distrobox::deleteBox(std::string boxName) {
    std::string cmd = "distrobox rm " + boxName + " -f";

    std::system(cmd.c_str());

    return true; // TODO
}

std::string Distrobox::createNewBox(const std::string boxName,
                                    const std::string image) {
    std::string cmd = "distrobox create " + boxName + " -i " + image + " -Y";

    cmd += " 2>&1";

    std::string output = runCmdAndGetOutput(cmd.c_str());
    // TODO this drops into an interactive prompt if image not found.
    // not sure if a way around that, though it shouldnt happen as
    // I'm using a dropdown for selecting the image

    return output;
}

std::vector<std::string> Distrobox::getAvailableImages() {
    std::string imagesOutput = runCmdAndGetOutput("distrobox create -C");

    std::vector<std::string> imagesOutputLines =
        explodeString(imagesOutput, '\n');

    // some kind of bug in distrobox itself
    std::erase_if(imagesOutputLines,
                  [](std::string &s) { return s == "Images"; });

    std::sort(imagesOutputLines.begin(), imagesOutputLines.end(),
              [](auto &a, auto &b) {
                  return tryParseDistroFromImageUrl(a) <
                         tryParseDistroFromImageUrl(b);
              });

    return imagesOutputLines;
}

void Distrobox::initNewBox(std::string boxName) {
    std::string cmd = "setsid distrobox enter " + boxName + " -- ls";

    std::system(cmd.c_str());
}
