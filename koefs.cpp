#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <sstream>

int main() {
    int n = 6;
    std::ifstream fin{};
    std::string filename;

    std::cout << "file: ";
    std::cin >> filename;

    fin.open(filename);

    if (fin.is_open()) {
        std::unordered_set<std::string> dnf;
        std::string str;

        while (fin >> str) {
            dnf.insert(str);
            str = "";
        }

        std::vector<std::unordered_set<int>> set_of_koef{};
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                for (int k = j; k < n; ++k) {
                    for (int l = k; l < n; ++l) {
                        for (int m = l; m < n; ++m) {
                            for (int p = m; p < n; ++p) {
                                std::unordered_set<int> set_of_number{};
                                set_of_number.insert(i);
                                set_of_number.insert(j);
                                set_of_number.insert(k);
                                set_of_number.insert(l);
                                set_of_number.insert(m);
                                set_of_number.insert(p);

                                set_of_koef.push_back(set_of_number);
                            }
                        }
                    }
                }
            }
        }

        std::vector<std::unordered_set<int>> unique_set_of_koef{};
        std::vector<std::vector<int>> vec_of_koef{};
        unique_set_of_koef.push_back(set_of_koef[0]);
        vec_of_koef.push_back({0});
        for (int i = 0; i < set_of_koef.size(); ++i) {
            bool is_add = true;
            for (int j = 0; j < unique_set_of_koef.size(); ++j) {
                if (set_of_koef[i] == unique_set_of_koef[j]) {
                    is_add = false;
                    break;
                }
            }
            if (is_add) {
                unique_set_of_koef.push_back(set_of_koef[i]);
                std::vector<int> vec;
                for (auto& j : set_of_koef[i]) {
                    vec.push_back(j);
                }

                vec_of_koef.push_back(vec);
            }
        }

        std::vector<std::bitset<6>> integer_function_values;
        for (size_t i = 0; i < 64; ++i) {
            integer_function_values.emplace_back(std::bitset<6>{i});
        }

        std::unordered_set<std::string> set_of_bad_monoms;

        for (auto& function_value : integer_function_values) {
            if (dnf.find(function_value.to_string()) != dnf.end()) {
                continue;
            }
            for (auto& koef : vec_of_koef) {
                std::string str_function_value = function_value.to_string();
                std::ostringstream out;

                out << '{';
                for (int i = 0; i < koef.size(); ++i) {
                    int j = koef[i];
                    if (str_function_value[5 - j] == '0') {
                        out << '-' << koef[i];
                    }
                    else {
                        out << '+' << koef[i];
                    }
                    out << ",";
                }
                out << "}";

                set_of_bad_monoms.insert(out.str());
            }
        }

        for (auto& function_value : integer_function_values) {
            if (dnf.find(function_value.to_string()) == dnf.end()) {
                continue;
            }
            std::string str_function_value = function_value.to_string();
            for (auto& koef : vec_of_koef) {
                std::ostringstream out;
                out << '{';
                for (int i = 0; i < koef.size(); ++i) {
                    int j = koef[i];
                    if (str_function_value[5 - j] == '0') {
                        out << '-' << koef[i];
                    }
                    else {
                        out << '+' << koef[i];
                    }
                    out << ",";
                }
                out << "}";

                if (set_of_bad_monoms.find(out.str()) == set_of_bad_monoms.end()) {
                    std::cout << out.str() << " + ";
                }
            }

            std::cout << " = " << str_function_value << std::endl;
        }

        fin.close();
    }
}
