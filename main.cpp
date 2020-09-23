#include <iostream>
#include <vector>
#include <fstream>

struct Pr {
    std::string param_name;
    std::string param_value;
};

int main() {

    std::string parameters;
    char *tmp = new char[10000];
    int backspace_pos;

    std::fstream file;
    file.open("params.txt", std::ios::in);

    std::vector<Pr> params;
    Pr temp;
    while (file.getline(tmp, 10000)) {
        parameters = std::string(tmp);
        backspace_pos = parameters.find(' ');
        temp.param_name = parameters.substr(0, backspace_pos - 1);
        temp.param_value = parameters.substr(backspace_pos + 1, parameters.length() - 1);
        params.push_back(temp);
    }

    std::string report = "";
    file.close();
    int param_begin = 0, param_end = 0, i;
    std::string name;
    bool isInParams;
    file.open("report_in.txt", std::ios::in);
    while (!file.eof()) {
        file.getline(tmp, 10000);
        report = std::string(tmp);
        i = 0;
        isInParams = false;
        param_begin = report.find('{');
        if (param_begin == -1) {
            std::cout << report << std::endl;
            continue;
        } else {
            param_end = report.find('}');
            if (report[param_begin + 1] == '*')
                name = report.substr(param_begin + 2, param_end - param_begin - 2);
            else name = report.substr(param_begin + 1, param_end - param_begin - 1);
            while (i < params.size()) {
                if (params[i].param_name == name) {
                    isInParams = true;
                    break;
                }
                i++;
            }
            if (isInParams)
                report.replace(param_begin, param_end - param_begin + 1, params[i].param_value);
            else
                report.erase(param_begin, param_end - param_begin + 1);
            std::cout << report << std::endl;
        }
    }
    delete[] tmp;
    return 0;
}
