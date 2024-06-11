#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <regex>

void toLowerCase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

std::string cleanWord(const std::string& word) {
    std::string clean_word;
    for (char c : word) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_') {
            clean_word += c;
        } else if (!std::ispunct(static_cast<unsigned char>(c)) || c == '.' || c == '/' || c == ':') {
            clean_word += c;
        }

    }
    return clean_word;
}

bool isURL(const std::string &word) {
    const std::regex url_regex(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt)))");
    return std::regex_search(word, url_regex);
}

void countWords(std::map<std::string, std::pair< int, std::set<int> >> &wordCount, std::map<std::string, std::pair< int, std::set<int> >> &urlCount) {
    std::ifstream in("text.txt");
    std::string line;
    int line_number = 0;
    while (std::getline(in, line)) {
    std::istringstream iss(line);
    std::string word;
    line_number++;

    while (iss >> word) {
        word = cleanWord(word);
        if (!word.empty())
        {
            if(isURL(word))
            {
                urlCount[word].first++;
                urlCount[word].second.insert(line_number);
            }
            toLowerCase(word);
            wordCount[word].first++;
            wordCount[word].second.insert(line_number);
        }
    }
    }
}

void findURLs(const std::string& text, std::set<std::string>& urls) {
    std::regex url_regex(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt)))");
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), url_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        urls.insert(match.str());
    }
}

int main() {
    std::ifstream in("text.txt");
    std::ofstream outW("word.txt");
    std::ofstream outU("url.txt");

    if (!in.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    std::map<std::string, std::pair< int, std::set<int> >> wordCount;
    //std::map<std::string, std::set<int>> wordLines;
    std::map<std::string, std::pair< int, std::set<int> >> urlCount;
    //std::map<std::string, std::set<int>> urls;

    countWords(wordCount, urlCount);

    outW << "Words:" << std::endl;
    for (auto& count : wordCount) {
        if (count.second.first > 1)
        {
            outW << count.first << ": " << count.second.first << " Lines: ";
            for (int line : count.second.second) {
                outW << line << " ";
            }
            outW << "\n";
        }
    }
    outU << "Urls:" << std::endl;
    for (auto count : urlCount) {
        outU << count.first << ": " << count.second.first << " Lines: ";
        for (int line : count.second.second) {
            outU << line << " ";
        }
        outU << "\n";
    }

    in.close();
    outW.close();
    outU.close();

    return 0;
}
