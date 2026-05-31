#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>
#include <regex>

const std::string input = "tekstas.md";
const std::string output_words = "zodziai.txt";
const std::string output_urls = "url.txt";

std::string stripMarkdown(const std::string& line) {
    std::string result;
    int i = 0, len = (int)line.size();
    while(i < len) {
        if(i + 1 < len && line[i] == '[' && line[i+1] == '[') {
            while(i < len && line[i] != ']') i++;
            while(i < len && line[i] == ']') i++;
            continue;
        }
        if(line[i] == '(') {
            int depth = 1; i++;
            while(i < len && depth > 0) {
                if(line[i] == '(') depth++;
                else if(line[i] == ')') depth--;
                i++;
            }
            continue;
        }
        if(line[i] == '[' || line[i] == ']') {
            result += ' '; 
            i++; 
            continue; 
        }
        result += line[i++];
    }
    return result;
}

std::string punctuation(const std::string& word) {
    std::string result;
    int i = 0, len = (int)word.size();
    while(i < len) {
        unsigned char c = word[i];
        if(c == 0xE2 && i + 2 < len && (unsigned char)word[i+1] == 0x88 &&
           (unsigned char)word[i+2] == 0x92) {
            i += 3; continue;
        }
        if(c == 0xE2 && i + 2 < len &&
           (unsigned char)word[i+1] == 0x80 &&
           ((unsigned char)word[i+2] == 0x92 ||
            (unsigned char)word[i+2] == 0x93 ||
            (unsigned char)word[i+2] == 0x94)) {
            i += 3;
            continue;
        }
        if(c > 127 || std::isalpha(c))
            result += c;
        i++;
    }
    
    
    return result;
}

std::string toLower(const std::string& word) {
    std::string result = word;
    for(char& c : result) {
        if((unsigned char)c < 128) {
            c = std::tolower((unsigned char)c);
        }
    }
    return result;
}

void print_w(std::ofstream& fout,
                const std::map<std::string, std::set<int>>& wordLines,
                const std::vector<std::pair<int,std::string>>& sorted) {

    fout << "Žodžiai pasikartojantys daugiau nei 1 kartą" << std::endl;
    int nr = 1;
    for(const auto& [cnt, word] : sorted) {
        std::string lineList;
        for(int ln : wordLines.at(word))
            lineList += (lineList.empty() ? "" : ", ") + std::to_string(ln);

        fout << nr++ << ". " << word << " (" << cnt << ")" << " - eilutės: " << lineList << std::endl;
    }
}

void print_u(std::ofstream& fout,
                const std::vector<std::pair<int,std::string>>& urlEntries) {

    fout << "URL adresai " << std::endl;
    int nr = 1;
    for(const auto& [line_num, url] : urlEntries)
        fout << nr++ << ". [eilutė " << line_num << "] " << url << std::endl;
}

// When building tests that include this translation unit, define TESTING_MODE
#ifndef TESTING_MODE
int main() {
    std::ifstream fin(input);
    std::map<std::string, int> wordcount;

    std::map<std::string, std::set<int>> wordlines;

    std::vector<std::pair<int,std::string>> url_vector;

    std::regex urlPattern(R"(https?://[^\s\[\]\,"\'<>\xC2]+)");

    std::string line;
    int line_num = 0;
    while(std::getline(fin, line)) {
        line_num++;

        auto it = std::sregex_iterator(line.begin(), line.end(), urlPattern);
        for(; it != std::sregex_iterator(); ++it) {
            std::string url = it->str();
            const std::string skrb = ".,:;!?";
            while(!url.empty()) {
                unsigned char c = url.back();
                if(skrb.find((char)c) != std::string::npos) {
                    url.pop_back();
                    continue;
                }
                if(c == ')') {
                    int open = 0, close = 0;
                    for(char ch : url) {
                        if(ch == '(') open++;
                        else if(ch == ')') close++;
                    }
                    if(close > open) {
                        url.pop_back(); 
                        continue; 
                    }
                }
                break;
            }
            url_vector.push_back({line_num, url});
        }
        std::string clean = stripMarkdown(line);
        std::istringstream ss(clean);
        std::string token;
        while(ss >> token) {
            std::string word = toLower(punctuation(token));
            if(word.empty()) continue;
            wordcount[word]++;
            wordlines[word].insert(line_num);
        }
    }
    fin.close();

    std::vector<std::pair<int,std::string>> sorted;
    for(const auto& [word, count] : wordcount) {
        if(count > 1)
            sorted.push_back({count, word});
    }
    std::sort(sorted.rbegin(), sorted.rend());

    std::ofstream w_out(output_words);
    print_w(w_out, wordlines, sorted);
    w_out.close();

    std::ofstream u_out(output_urls);
    print_u(u_out, url_vector);
    u_out.close();

    std::cout << "Zodziai issaugoti: " << output_words << std::endl;
    std::cout << "URL issaugoti: " << output_urls << std::endl;
    std::cout << "Pasikartojantys zodziai: " << sorted.size() << std::endl;
    std::cout << "URL is viso: " << url_vector.size() << std::endl;
    return 0;
}
#endif // TESTING_MODE