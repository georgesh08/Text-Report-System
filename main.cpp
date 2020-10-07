#include <iostream>
#include <vector>
#include <fstream>

struct Pr {
    std::string param_name;
    std::string param_value;
};

int main(int argc, char *argv[]) {

    std::string parameters;
    char *tmp = new char[10000];
    int colon_pos;

    std::ifstream file;
    file.open("params.json");

    std::vector<Pr> params;
    Pr temp;
    while (file.getline(tmp, 10000)) {
        parameters = std::string(tmp);
        if (parameters[0] == '{' || parameters[0] == '}')
            continue;
        else {
            colon_pos = parameters.find(':');
            temp.param_name = parameters.substr(3, colon_pos - 4);
            if(parameters[parameters.length() - 1] == ',')
                temp.param_value = parameters.substr(colon_pos + 3, parameters.length() - 5 - colon_pos);
            else
                temp.param_value = parameters.substr(colon_pos + 3, parameters.length() - 4 - colon_pos);
            params.push_back(temp);
        }
    }

    std::string report, new_str;
    file.close();
    int param_begin, param_end, i;
    std::string name;
    bool isInParams;
    file.open("report_in.txt");
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
            new_str += report + "\n";
        }
    }
    file.close();
    std::string command = argv[1];
    if(command == "-f") {
        std::ofstream out;
        out.open("report_out.txt");
        out << new_str;
        out.close();
    }
    else
        std::cout << new_str;

    delete[] tmp;
    return 0;
}
